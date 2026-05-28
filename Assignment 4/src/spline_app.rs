use crate::cardinal_spline::EndMode;

#[derive(Clone, Copy, PartialEq)]
pub struct SliderValues {
    pub min_value: f32,
    pub max_value: f32,
}

pub struct SplineApp {
    pub points: Vec<egui::Pos2>,
    pub circle_radius: f32,
    pub shape_parameter: f32,
    pub lod: usize,
    pub mode: EndMode,
    pub box_checked: bool,
    pub slider_values: SliderValues,
    pub points_color: egui::Color32,
    pub segment_color: egui::Color32,
}
impl Default for SplineApp {
    fn default() -> Self {
        Self {
            points: Vec::new(),
            circle_radius: 10.0,
            shape_parameter: 0.5,
            lod: 42,
            mode: EndMode::FreeEnds,
            box_checked: false,
            slider_values: SliderValues {
                min_value: -5.0,
                max_value: 5.0,
            },
            points_color: egui::Color32::RED,
            segment_color: egui::Color32::LIGHT_BLUE,
        }
    }
}

impl SplineApp {
    pub fn new(_cc: &eframe::CreationContext<'_>) -> Self {
        Self::default()
    }

    pub fn reset_to_default(&mut self) {
        *self = Self::default();
    }
    pub fn draw_basis_curve(
        painter: &egui::Painter,
        rect: egui::Rect,
        points: &Vec<[f32; 2]>,
        color: egui::Color32,
        y_min: f32,
        y_max: f32,
    ) {
        for pair in points.windows(2) {
            let point_a = pair[0];
            let point_b = pair[1];

            let t_a = point_a[0];
            let y_a = point_a[1];

            let t_b = point_b[0];
            let y_b = point_b[1];

            let x_a = rect.left() + t_a * rect.width();
            let normalized_y_a = (y_a - y_min) / (y_max - y_min);
            let screen_y_a = rect.bottom() - normalized_y_a * rect.height();

            let x_b = rect.left() + t_b * rect.width();
            let normalized_y_b = (y_b - y_min) / (y_max - y_min);
            let screen_y_b = rect.bottom() - normalized_y_b * rect.height();

            let screen_a = egui::pos2(x_a, screen_y_a);
            let screen_b = egui::pos2(x_b, screen_y_b);

            painter.line_segment([screen_a, screen_b], egui::Stroke::new(2.0, color));
        }
    }
}
