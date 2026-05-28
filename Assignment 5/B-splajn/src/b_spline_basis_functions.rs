#[derive(Debug)]
pub enum KnotVectorError {
    Decreasing,
    TooFewKnots,
    InvalidDegree,
}

pub struct KnotVector {
    pub values: Vec<f64>,
}

impl KnotVector {
    pub fn new(values: Vec<f64>, degree: usize) -> Result<Self, KnotVectorError> {
        if degree == 0 {
            return Err(KnotVectorError::InvalidDegree);
        }

        if values.len() < degree + 2 {
            return Err(KnotVectorError::TooFewKnots);
        }

        if !Self::is_non_decreasing(&values) {
            return Err(KnotVectorError::Decreasing);
        }

        Ok(Self { values })
    }

    pub fn basis(&self, i: usize, t: f64, degree: usize) -> f64 {
        if degree == 0 {
            if self.values[i] <= t && t < self.values[i + 1] {
                return 1.0;
            }
            return 0.0;
        }
        let first_term = self.calculate_first_term(t, i, degree);
        let second_term = self.calculate_second_term(t, i, degree);

        first_term * self.basis(i, t, degree - 1) + second_term * self.basis(i + 1, t, degree - 1)
    }
    fn calculate_first_term(&self, t: f64, i: usize, degree: usize) -> f64 {
        let denominator = self.values[i + degree] - self.values[i];

        if denominator == 0.0 {
            return 0.0;
        }
        let numerator = t - self.values[i];

        numerator / denominator
    }
    fn calculate_second_term(&self, t: f64, i: usize, degree: usize) -> f64 {
        let denominator = self.values[i + degree + 1] - self.values[i + 1];

        if denominator == 0.0 {
            return 0.0;
        }

        let numerator = self.values[i + degree + 1] - t;

        numerator / denominator
    }

    pub fn is_non_decreasing(values: &[f64]) -> bool {
        values.windows(2).all(|pair| pair[0] <= pair[1])
    }
}
