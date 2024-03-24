use nalgebra_glm::Vec3;

use crate::{camera::Camera, shader_program::ShaderProgram, texture::Texture};

pub trait Render {
    fn set_texture(&mut self, texture: Texture);

    fn set_shader(&mut self, shader: ShaderProgram);

    fn set_rotation(&mut self, angle: f32, axis: Vec3);

    fn rotate(&mut self, angle: f32, axis: Vec3);

    fn set_translation(&mut self, translation: Vec3);

    fn translate(&mut self, translation: Vec3);

    fn scale(&mut self, factor: f32);

    fn render(&self, camera: &Camera);
}