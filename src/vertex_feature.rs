#[derive(Copy, Clone, PartialEq)]
pub enum VertexFeature {
    Position = 0,
    Normal = 1,
    Color = 2,
    UV = 3,
}

impl VertexFeature {
    pub fn size(&self) -> u16 {
        match self {
            VertexFeature::Position => 3,
            VertexFeature::Normal => 3,
            VertexFeature::Color => 3,
            VertexFeature::UV => 2,
        }
    }
}