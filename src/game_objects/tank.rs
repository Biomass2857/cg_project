use std::ops::{Deref, DerefMut};

use crate::object::Object;
use crate::object_loader::LoadOptions;
use crate::object_template::ObjectTemplate;
use crate::vertex_feature::VertexFeature;

pub struct Tank {
    object: Object,
}

impl Tank {
    pub fn new(template: &ObjectTemplate) -> Self {
        Self {
            object: Object::from_template(&template),
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
