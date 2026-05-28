use crate::spline_app::SplineApp;
use eframe::egui;
use egui::Color32;

pub fn run() -> eframe::Result<()> {
    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default()
            .with_inner_size([1500.0, 1000.0])
            .with_title("B-Spline Editor"),
        ..Default::default()
    };
    eframe::run_native(
        "B-Spline Editor",
        options,
        Box::new(|cc| Ok(Box::new(SplineApp::new(cc)))),
    )
}
impl eframe::App for SplineApp {
    fn ui(&mut self, ui: &mut egui::Ui, _frame: &mut eframe::Frame) {
        egui::Panel::left("left_panel").show_inside(ui, |ui| {
            ui.heading("Control Panel");
            ui.add_space(20.0);
            ui.add(
                egui::Slider::new(&mut self.degree, 1..=10)
                    .text("Degree")
                    .step_by(1.0),
            );
            ui.add_space(20.0);
            ui.add(
                egui::Slider::new(&mut self.lod, 2..=100)
                    .text("Level Of Detail")
                    .step_by(1.0),
            );
            ui.add_space(20.0);
            if ui.add(egui::Button::new("Reset")).clicked() {
                self.reset_to_default();
            }

            if ui.add(egui::Button::new("Debug")).clicked() {
                for (i, _) in self.points.iter().enumerate() {
                    println!("{}", i)
                }
            }
            ui.add_space(20.0);

            ui.label("Knots (comma separated)");
            if ui.text_edit_singleline(&mut self.text).changed() {
                if let Ok(knots) = self.parse_knots() {
                    self.knots = knots;
                }
            }

            ui.add_space(20.0);
            ui.label(format!("Number of control points: {}", self.points.len()));
            ui.label(format!("Current knot count: {}", self.knot_count()));
            ui.label(format!(
                "Expected knot count: {}",
                self.expected_knot_count()
            ));

            if let Err(message) = self.try_build_spline() {
                ui.add_space(10.0);
                ui.colored_label(Color32::RED, message);
            }

            ui.add_space(20.0);
            ui.label("Change Color of Points");
            ui.color_edit_button_srgba(&mut self.points_color);
            ui.label("Change Color of Segment");
            ui.color_edit_button_srgba(&mut self.segment_color);
        });
        egui::CentralPanel::default().show_inside(ui, |ui| {
            ui.heading("Canvas");
            let size = ui.available_size();
            let (canvas, response) = ui.allocate_exact_size(size, egui::Sense::click_and_drag());

            let painter = ui.painter_at(canvas);

            for (i, point) in self.points.iter_mut().enumerate() {
                let point_rect = egui::Rect::from_center_size(
                    *point,
                    egui::Vec2::splat(2.0 * self.circle_radius),
                );

                let point_id = response.id.with(i);
                let point_response = ui.interact(point_rect, point_id, egui::Sense::drag());

                if point_response.hovered() {
                    painter.circle_stroke(
                        *point,
                        self.circle_radius + 2.0,
                        egui::Stroke::new(2.0, egui::Color32::PURPLE),
                    );
                }

                if point_response.dragged() {
                    *point += point_response.drag_delta();

                    point.x = point.x.clamp(canvas.left(), canvas.right());
                    point.y = point.y.clamp(canvas.top(), canvas.bottom());
                }
            }

            if response.clicked() {
                if let Some(pos) = response.interact_pointer_pos() {
                    let local = egui::Pos2 { x: pos.x, y: pos.y };
                    self.points.push(local);
                }
            }

            for pair in self.points.windows(2) {
                painter.line_segment(
                    [pair[0], pair[1]],
                    egui::Stroke::new(2.0, egui::Color32::PURPLE),
                );
            }

            if let Ok(spline) = self.try_build_spline() {
                let sampled_points = spline.sample(self.lod);

                for pair in sampled_points.windows(2) {
                    painter.line_segment(
                        [pair[0], pair[1]],
                        egui::Stroke::new(2.0, self.segment_color),
                    );
                }
            }

            if !self.points.is_empty() {
                for (i, point) in self.points.iter().enumerate() {
                    painter.circle_filled(*point, self.circle_radius, self.points_color);

                    painter.text(
                        egui::pos2(point.x, point.y - 15.0),
                        egui::Align2::CENTER_BOTTOM,
                        format!("{}", { i }),
                        egui::FontId::proportional(14.0),
                        egui::Color32::RED,
                    );
                }
            }
        });
    }
}
