extern crate gl;
extern crate nalgebra as na;
extern crate nalgebra_glm as glm;

use glm::{Mat4, Vec3};
use gl::types::{GLuint, GLfloat, GLsizeiptr, GLvoid};
use crate::render::Render;
use crate::shader_program::ShaderProgram;
use crate::texture::Texture;
use crate::camera::Camera;
use crate::vertex_feature::VertexFeature;
use crate::object_template::ObjectTemplate;

pub struct Object {
    scaling: Mat4,
    rotation: Mat4,
    translation: Mat4,
    texture_enabled: bool,
    texture: Option<Texture>,
    shader: Option<ShaderProgram>,
    index_count: usize,
    vao: GLuint,
    vbo: GLuint,
    ibo: GLuint,
}

impl Object {
    pub fn new(vertices: Vec<f32>, indices: Vec<u32>, features: Vec<VertexFeature>) -> Self {
        let mut object = Object {
            scaling: Mat4::identity(),
            rotation: Mat4::identity(),
            translation: Mat4::identity(),
            texture_enabled: false,
            texture: None,
            shader: None,
            index_count: 0,
            vao: 0,
            vbo: 0,
            ibo: 0,
        };
        object.init(vertices, indices, features);
        object
    }

    pub fn from_template(object_template: &ObjectTemplate) -> Self {
        Self::new(
            object_template.vertices.clone(),
            object_template.indices.clone(),
            object_template.features.clone()
        )
    }

    fn init(&mut self, vertices: Vec<f32>, indices: Vec<u32>, features: Vec<VertexFeature>) {
        unsafe {
            gl::GenVertexArrays(1, &mut self.vao);
            gl::BindVertexArray(self.vao);

            gl::GenBuffers(1, &mut self.vbo);
            gl::BindBuffer(gl::ARRAY_BUFFER, self.vbo);
            gl::BufferData(gl::ARRAY_BUFFER, (vertices.len() * std::mem::size_of::<GLfloat>()) as GLsizeiptr, vertices.as_ptr() as *const GLvoid, gl::STATIC_DRAW);

            let stride: u16 = features.iter().map(|feature| feature.size()).sum();

            let mut initial_offset = 0;
            for feature in &features {
                let size = feature.size();
                gl::EnableVertexAttribArray(*feature as GLuint);
                gl::VertexAttribPointer(*feature as GLuint, size as i32, gl::FLOAT, gl::FALSE, (stride * std::mem::size_of::<GLfloat>() as u16) as i32, (initial_offset * std::mem::size_of::<GLfloat>() as u16) as *const GLvoid);
                initial_offset += size;
            }

            self.index_count = indices.len();
            gl::GenBuffers(1, &mut self.ibo);
            gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, self.ibo);
            gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, (self.index_count * std::mem::size_of::<u32>()) as GLsizeiptr, indices.as_ptr() as *const GLvoid, gl::STATIC_DRAW);

            self.texture_enabled = features.contains(&VertexFeature::UV);
        }
    }
}

impl Render for Object {
    fn set_texture(&mut self, texture: Texture) {
        self.texture = Some(texture);
    }

    fn set_shader(&mut self, shader: ShaderProgram) {
        self.shader = Some(shader);
    }

    fn set_rotation(&mut self, angle: f32, axis: Vec3) {
        self.rotation = glm::rotate(&Mat4::identity(), angle, &axis);
    }

    fn rotate(&mut self, angle: f32, axis: Vec3) {
        self.rotation = glm::rotate(&self.rotation, angle, &axis);
    }

    fn set_translation(&mut self, translation: Vec3) {
        self.translation = glm::translate(&Mat4::identity(), &translation);
    }

    fn translate(&mut self, translation: Vec3) {
        self.translation = glm::translate(&self.translation, &translation);
    }

    fn scale(&mut self, factor: f32) {
        self.scaling = glm::scale(&self.scaling, &Vec3::new(factor, factor, factor));
    }

    fn render(&self, camera: &Camera) {
        unsafe {
            gl::BindBuffer(gl::ARRAY_BUFFER, self.vbo);
            gl::BindVertexArray(self.vao);
            gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, self.ibo);

            if let Some(shader) = &self.shader {
                shader.use_program();

                if self.texture_enabled {
                    if let Some(texture) = &self.texture {
                        texture.bind();
                        shader.set_texture("tex0", texture);
                    }
                }

                shader.set_matrix4("camMatrix", &camera.get_matrix());
                shader.set_matrix4("modelview", &(self.translation * self.rotation * self.scaling));

                gl::DrawElements(gl::TRIANGLES, self.index_count as i32, gl::UNSIGNED_INT, std::ptr::null());
            }

            gl::BindVertexArray(0);
            gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0);
            gl::BindBuffer(gl::ARRAY_BUFFER, 0);
        }
    }
}

impl Drop for Object {
    fn drop(&mut self) {
        unsafe {
            gl::DeleteVertexArrays(1, &mut self.vao);
            gl::DeleteBuffers(1, &mut self.vbo);
            gl::DeleteBuffers(1, &mut self.ibo);
        }
    }
}