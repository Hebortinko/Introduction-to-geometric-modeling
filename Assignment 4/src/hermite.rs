pub struct HermiteBasis {
    pub h00: f32,
    pub h10: f32,
    pub h01: f32,
    pub h11: f32,
}

impl HermiteBasis {
    pub fn calculate_hermite_basis(t: f32) -> Self {
        Self {
            h00: (1.0 + 2.0 * t) * (1.0 - t) * (1.0 - t),
            h10: t * (1.0 - t) * (1.0 - t),
            h01: t * t * (3.0 - 2.0 * t),
            h11: t * t * (t - 1.0),
        }
    }
}
