use crate::b_spline::BSpline;
use crate::b_spline_basis_functions::{KnotVector, KnotVectorError};

pub struct SplineApp {
    pub points: Vec<egui::Pos2>,
    pub knots: Vec<f64>,
    pub text: String,
    pub circle_radius: f32,
    pub degree: usize,
    pub lod: usize,

    pub points_color: egui::Color32,
    pub segment_color: egui::Color32,
}
impl Default for SplineApp {
    fn default() -> Self {
        Self {
            points: Vec::new(),
            knots: vec![1.0, 2.0, 3.0, 4.0],
            circle_radius: 10.0,
            degree: 3,
            text: String::from("1, 2, 3, 4"),
            lod: 42,
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

    pub fn expected_knot_count(&self) -> usize {
        self.points.len() + self.degree + 1
    }

    pub fn knot_count(&self) -> usize {
        self.text
            .split(',')
            .map(str::trim)
            .filter(|value| !value.is_empty())
            .count()
    }

    pub fn parse_knots(&self) -> Result<Vec<f64>, String> {
        let tokens: Vec<&str> = self.text.split(',').map(str::trim).collect();

        if tokens.len() == 1 && tokens[0].is_empty() {
            return Err(String::from("Knot vector is empty."));
        }

        let mut knots = Vec::with_capacity(tokens.len());

        for (index, token) in tokens.iter().enumerate() {
            if token.is_empty() {
                return Err(format!("Missing knot value at position {}.", index + 1));
            }

            let knot = token.parse::<f64>().map_err(|_| {
                format!("Invalid knot value at position {}: `{}`.", index + 1, token)
            })?;

            knots.push(knot);
        }

        Ok(knots)
    }

    pub fn try_build_spline(&self) -> Result<BSpline, String> {
        if self.points.len() < self.degree + 1 {
            return Err(format!(
                "Need at least {} control points for degree {}.",
                self.degree + 1,
                self.degree
            ));
        }

        let knots = self.parse_knots()?;
        let expected_knot_count = self.expected_knot_count();

        if knots.len() != expected_knot_count {
            return Err(format!(
                "Expected {} knots for {} control points and degree {}, got {}.",
                expected_knot_count,
                self.points.len(),
                self.degree,
                knots.len()
            ));
        }

        let knot_vector = KnotVector::new(knots, self.degree).map_err(|error| match error {
            KnotVectorError::Decreasing => String::from("Knot vector must be non-decreasing."),
            KnotVectorError::TooFewKnots => String::from("Knot vector has too few values."),
            KnotVectorError::InvalidDegree => String::from("Degree must be greater than 0."),
        })?;

        Ok(BSpline::new(
            self.degree,
            self.points.clone(),
            knot_vector,
        ))
    }
}
