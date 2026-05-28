pub struct CardinalSplineBasisFunctions {
    pub c0: Vec<[f32; 2]>,
    pub c1: Vec<[f32; 2]>,
    pub c2: Vec<[f32; 2]>,
    pub c3: Vec<[f32; 2]>,
}

impl CardinalSplineBasisFunctions {
    pub fn calculate_cardinal_spline_functions(s: f32, lod: usize) -> Self {
        let [c0, c1, c2, c3] = Self::generate_cardinal_plot_points(s, lod);
        Self { c0, c1, c2, c3 }
    }

    fn generate_cardinal_plot_points(s: f32, samples: usize) -> [Vec<[f32; 2]>; 4] {
        let mut c0_points = Vec::new();
        let mut c1_points = Vec::new();
        let mut c2_points = Vec::new();
        let mut c3_points = Vec::new();

        for i in 0..=samples {
            let t = i as f32 / samples as f32;
            let [c0, c1, c2, c3] = Self::cardinal_basis_values(t, s);
            c0_points.push([t, c0]);
            c1_points.push([t, c1]);
            c2_points.push([t, c2]);
            c3_points.push([t, c3]);
        }
        [c0_points, c1_points, c2_points, c3_points]
    }

    fn cardinal_basis_values(t: f32, s: f32) -> [f32; 4] {
        let t2 = t * t;
        let t3 = t2 * t;

        let c0 = s * (-t3 + 2.0 * t2 - t);
        let c1 = (2.0 - s) * t3 + (s - 3.0) * t2 + 1.0;
        let c2 = (s - 2.0) * t3 + (3.0 - 2.0 * s) * t2 + s * t;
        let c3 = s * (t3 - t2);

        [c0, c1, c2, c3]
    }
}

