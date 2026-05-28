use crate::cardinal_spline::{CardinalSpline, EndMode};
use crate::cardinal_spline_basis_functions::CardinalSplineBasisFunctions;
use crate::spline_app::SplineApp;
use eframe::egui;
use egui::Color32;

pub fn run() -> eframe::Result<()> {
    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default()
            .with_inner_size([1500.0, 1000.0])
            .with_title("Cardinal Spline"),
        ..Default::default()
    };
    eframe::run_native(
        "Cardinal Spline",
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
                egui::Slider::new(
                    &mut self.shape_parameter,
                    self.slider_values.min_value..=self.slider_values.max_value,
                )
                .text("Shape Parameter")
                .step_by(0.05),
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
            ui.add_space(20.0);
            ui.checkbox(&mut self.box_checked, "Interpolate Ends");
            ui.add_space(20.0);
            ui.label(format!("Number of control points: {}", self.points.len()));
            ui.add_space(20.0);
            ui.label("Change Color of Points");
            ui.color_edit_button_srgba(&mut self.points_color);
            ui.label("Change Color of Segment");
            ui.color_edit_button_srgba(&mut self.segment_color);

            self.mode = if self.box_checked {
                EndMode::InterpolateEnds
            } else {
                EndMode::FreeEnds
            };
        });
        egui::Panel::right("Cardinal Basis Functions").show_inside(ui, |ui| {
            ui.heading("Cadinal Basis Functions");
            ui.label(format!("s: {:.3}", self.shape_parameter));
            let available_size = ui.available_size();
            let plot_height = available_size.y.min(400.0);
            let plot_width = available_size.x.max(300.0);
            let (rect, _response) =
                ui.allocate_exact_size(egui::vec2(plot_width, plot_height), egui::Sense::hover());

            let painter = ui.painter_at(rect);
            let basis = CardinalSplineBasisFunctions::calculate_cardinal_spline_functions(
                self.shape_parameter,
                self.lod,
            );

            let y_min = -0.3;
            let y_max = 1.1;

            painter.rect_stroke(
                rect,
                0.0,
                egui::Stroke::new(1.0, Color32::GRAY),
                egui::StrokeKind::Inside,
            );
            Self::draw_basis_curve(&painter, rect, &basis.c0, egui::Color32::RED, y_min, y_max);

            Self::draw_basis_curve(
                &painter,
                rect,
                &basis.c1,
                egui::Color32::GREEN,
                y_min,
                y_max,
            );
            Self::draw_basis_curve(&painter, rect, &basis.c2, egui::Color32::BLUE, y_min, y_max);
            Self::draw_basis_curve(
                &painter,
                rect,
                &basis.c3,
                egui::Color32::YELLOW,
                y_min,
                y_max,
            );

            ui.separator();
            ui.label("Legend:");

            ui.colored_label(egui::Color32::RED, "C0(t) = s(-t³ + 2t² - t)");
            ui.colored_label(egui::Color32::GREEN, "C1(t) = (2-s)t³ + (s-3)t² + 1");
            ui.colored_label(egui::Color32::BLUE, "C2(t) = (s-2)t³ + (3-2s)t² + st");
            ui.colored_label(egui::Color32::YELLOW, "C3(t) = s(t³ - t²)");
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
                        egui::Stroke::new(2.0, egui::Color32::YELLOW),
                    );
                }

                if point_response.dragged() {
                    *point += point_response.drag_delta();

                    point.x = point.x.clamp(canvas.left(), canvas.right());
                    point.y = point.y.clamp(canvas.top(), canvas.bottom());
                }
            }

            let spline =
                CardinalSpline::new(&self.points, self.shape_parameter, self.lod, self.mode);
            let sampled_points = spline.calculate();

            for pair in sampled_points.windows(2) {
                painter.line_segment(
                    [pair[0], pair[1]],
                    egui::Stroke::new(2.0, self.segment_color),
                );
            }

            if response.clicked() {
                if let Some(pos) = response.interact_pointer_pos() {
                    let local = egui::Pos2 { x: pos.x, y: pos.y };
                    self.points.push(local);
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
