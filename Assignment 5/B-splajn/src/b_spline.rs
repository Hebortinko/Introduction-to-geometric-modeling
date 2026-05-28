use core::f32;

use crate::b_spline_basis_functions::KnotVector;
use egui::Pos2;

pub struct BSpline {
    pub degree: usize,
    pub control_points: Vec<Pos2>,
    pub knots: KnotVector,
}

impl BSpline {
    pub fn new(degree: usize, control_points: Vec<Pos2>, knots: KnotVector) -> Self {
        Self {
            degree: degree,
            control_points: control_points,
            knots: knots,
        }
    }

    pub fn evaluate(&self, t: f64) -> Pos2 {
        let mut result = egui::pos2(0.0, 0.0);

        for i in 0..self.control_points.len() {
            let weight = self.knots.basis(i, t, self.degree) as f32;

            result.x += self.control_points[i].x * weight;
            result.y += self.control_points[i].y * weight;
        }
        result
    }

    pub fn sample(&self, lod: usize) -> Vec<Pos2> {
        let mut points = Vec::new();

        for i in self.degree..self.control_points.len() {
            let a = self.knots.values[i];
            let b = self.knots.values[i + 1];

            if a == b {
                continue;
            }

            for j in 0..lod {
                let t = a + (b - a) * j as f64 / lod as f64;
                points.push(self.evaluate(t));
            }
        }

        let t_end = self.knots.values[self.control_points.len()];
        points.push(self.evaluate(t_end));

        points
    }
}
