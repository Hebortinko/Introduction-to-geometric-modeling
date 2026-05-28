use crate::hermite::HermiteBasis;
use core::f32;
use eframe::egui::{Pos2, Vec2};
use std::usize;

#[derive(Clone, Copy, PartialEq, Eq)]
pub enum EndMode {
    FreeEnds,
    InterpolateEnds,
}
pub struct CardinalSpline<'a> {
    points: &'a [Pos2],
    shape_parameter: f32,
    lod: usize,
    mode: EndMode,
}

impl<'a> CardinalSpline<'a> {
    pub fn new(ref_points: &'a [Pos2], shape_parameter: f32, lod: usize, mode: EndMode) -> Self {
        Self {
            points: ref_points,
            shape_parameter,
            lod,
            mode,
        }
    }

    pub fn calculate(&self) -> Vec<Pos2> {
        let working_points = self.handle_endmode();

        if working_points.len() < 4 {
            return Vec::new();
        }

        self.sample_curve(&working_points)
    }

    fn handle_endmode(&self) -> Vec<Pos2> {
        match self.mode {
            EndMode::FreeEnds => self.points.to_vec(),
            EndMode::InterpolateEnds => {
                let mut points = Vec::with_capacity(self.points.len() + 2);

                if let Some(first) = self.points.first().copied() {
                    points.push(first);
                }

                points.extend(self.points.iter().copied());

                if let Some(last) = self.points.last().copied() {
                    points.push(last);
                }
                points
            }
        }
    }

    fn sample_curve(&self, points: &[Pos2]) -> Vec<Pos2> {
        let mut result = Vec::new();

        for i in 0..(points.len() - 3) {
            let p0 = points[i];
            let p1 = points[i + 1];
            let p2 = points[i + 2];
            let p3 = points[i + 3];

            let segment_points = self.sample_segment(p0, p1, p2, p3);

            if i == 0 {
                result.extend(segment_points);
            } else {
                result.extend(segment_points.into_iter().skip(1));
            }
        }
        result
    }

    fn sample_segment(&self, p0: Pos2, p1: Pos2, p2: Pos2, p3: Pos2) -> Vec<Pos2> {
        let mut result = Vec::with_capacity(self.lod + 1);

        for step in 0..=self.lod {
            let t = step as f32 / self.lod as f32;
            result.push(self.evaluate_segment(p0, p1, p2, p3, t));
        }

        result
    }

    fn evaluate_segment(&self, p0: Pos2, p1: Pos2, p2: Pos2, p3: Pos2, t: f32) -> Pos2 {
        let start_tangent: Vec2 = self.shape_parameter * (p2 - p0);
        let end_tangent: Vec2 = self.shape_parameter * (p3 - p1);
        let basis = HermiteBasis::calculate_hermite_basis(t);

        let segment_point_x = (basis.h00 * p1.x)
            + (basis.h10 * start_tangent.x)
            + (basis.h01 * p2.x)
            + (basis.h11 * end_tangent.x);
        let segment_point_y = (basis.h00 * p1.y)
            + (basis.h10 * start_tangent.y)
            + (basis.h01 * p2.y)
            + (basis.h11 * end_tangent.y);

        Pos2::new(segment_point_x, segment_point_y)
    }
}
