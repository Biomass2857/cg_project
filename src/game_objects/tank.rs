use std::ops::{Deref, DerefMut};

use crate::object::Object;
use crate::object_template::ObjectTemplate;
use crate::vertex_feature::VertexFeature;

pub struct Tank {
    object: Object,
}

impl Tank {
    pub fn new() -> Self {
        let tank_template = ObjectTemplate::new(
            "./assets/tank/tank_1.obj",
            vec![
                VertexFeature::Position,
                VertexFeature::Normal,
                VertexFeature::Color,
            ],
            0.05,
        );

        Self {
            object: Object::from_template(&tank_template),
        }
    }
}

impl Deref for Tank {
    type Target = Object;

    fn deref(&self) -> &Self::Target {
        &self.object
    }
}

impl DerefMut for Tank {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.object
    }
}
