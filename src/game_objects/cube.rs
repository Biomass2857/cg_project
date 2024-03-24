extern crate nalgebra_glm as glm;

use std::ops::{Deref, DerefMut};

use crate::object::Object;
use crate::texture::Texture;
use crate::render::Render;
use crate::vertex_feature::VertexFeature;

pub struct Cube {
    object: Object,
}

impl Cube {
    pub fn new(len: f32, texture: Texture) -> Self {
        let mut object = Object::new(
            vec![
                // front face
                -len, -len, len, -0.5, -0.5, 0.5, 0.0, 0.0,
                len, -len, len, 0.5, -0.5, 0.5, 1.0, 0.0,
                -len, len, len, -0.5, 0.5, 0.5, 0.0, 1.0,
                len, len, len, 0.5, 0.5, 0.5, 1.0, 1.0,

                // right face
                len, -len, len, 0.5, -0.5, 0.5, 0.0, 1.0,
                len, len, len, 0.5, 0.5, 0.5, 1.0, 1.0,
                len, -len, -len, 0.5, -0.5, -0.5, 0.0, 0.0,
                len, len, -len, 0.5, 0.5, -0.5, 1.0, 0.0,

                // back face
                -len, -len, -len, -0.5, -0.5, -0.5, 0.0, 0.0,
                len, -len, -len, 0.5, -0.5, -0.5, 1.0, 0.0,
                -len, len, -len, -0.5, 0.5, -0.5, 0.0, 1.0,
                len, len, -len, 0.5, 0.5, -0.5, 1.0, 1.0,

                // left face
                -len, -len, len, -0.5, -0.5, 0.5, 0.0, 1.0,
                -len, -len, -len, -0.5, -0.5, -0.5, 0.0, 0.0,
                -len, len, len, -0.5, 0.5, 0.5, 1.0, 1.0,
                -len, len, -len, -0.5, 0.5, -0.5, 1.0, 0.0,

                // top face
                -len, len, len, -0.5, 0.5, 0.5, 0.0, 1.0,
                len, len, len, 0.5, 0.5, 0.5, 1.0, 1.0,
                -len, len, -len, -0.5, 0.5, -0.5, 0.0, 0.0,
                len, len, -len, 0.5, 0.5, -0.5, 1.0, 0.0,

                // bottom face
                -len, -len, len, -0.5, -0.5, 0.5, 0.0, 1.0,
                len, -len, len, 0.5, -0.5, 0.5, 1.0, 1.0,
                -len, -len, -len, -0.5, -0.5, -0.5, 0.0, 0.0,
                len, -len, -len, 0.5, -0.5, -0.5, 1.0, 0.0
            ],
            vec![
                // front face
                0, 1, 2,
                1, 2, 3,

                // right face
                4, 5, 6,
                5, 6, 7,

                // back face
                8, 9, 10,
                9, 10, 11,

                // left face
                12, 13, 14,
                13, 14, 15,

                // top face
                16, 17, 18,
                17, 18, 19,

                // bottom face
                20, 21, 22,
                21, 22, 23
            ],
            vec![VertexFeature::Position, VertexFeature::Normal, VertexFeature::UV]
        );

        object.set_texture(texture);
        object.translate(glm::vec3(len, len, len));

        Cube { object }
    }
}

impl Deref for Cube {
    type Target = Object;
    
    fn deref(&self) -> &Self::Target {
        &self.object
    }
}

impl DerefMut for Cube {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.object
    }
}