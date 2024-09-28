use std::ops::{Deref, DerefMut};

use crate::object::Object;
use crate::object_template::ObjectTemplate;
use crate::vertex_feature::VertexFeature;

pub struct Shell {
    object: Object,
}

impl Shell {
    pub fn new() -> Self {
        let shell_template = ObjectTemplate::new(
            "./assets/shell/shell.obj",
            vec![
                VertexFeature::Position,
                VertexFeature::Normal,
                VertexFeature::Color,
            ],
            0.05,
        );

        Self {
            object: Object::from_template(&shell_template),
        }
    }
}

impl Deref for Shell {
    type Target = Object;

    fn deref(&self) -> &Self::Target {
        &self.object
    }
}

impl DerefMut for Shell {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.object
    }
}
