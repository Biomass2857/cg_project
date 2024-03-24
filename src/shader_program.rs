extern crate glutin;
extern crate nalgebra_glm as glm;

use std::ptr;
use gl::types::*;
use std::ffi::CString;
use std::str;

use glm::Mat4;

use crate::texture::Texture;
use crate::shader::Shader;

#[derive(Clone, Copy)]
pub struct ShaderProgram {
    id: u32,
}

impl ShaderProgram {
    pub fn new(shaders: Vec<Shader>) -> ShaderProgram {
        let id = unsafe {
            let id = gl::CreateProgram();
            for shader in shaders {
                gl::AttachShader(id, shader.id());
            }
            gl::LinkProgram(id);

            let mut success = gl::FALSE as GLint;
            let mut info_log = Vec::with_capacity(512);
            info_log.set_len(512 - 1); // subtract 1 to skip the trailing null character
            gl::GetProgramiv(id, gl::LINK_STATUS, &mut success);
            if success != gl::TRUE as GLint {
                gl::GetProgramInfoLog(id, 512, ptr::null_mut(), info_log.as_mut_ptr() as *mut GLchar);
                println!("ERROR::PROGRAM::LINKING_FAILED\n{}", str::from_utf8(&info_log).unwrap());
            }
            id
        };

        ShaderProgram { id }
    }

    pub fn use_program(&self) {
        unsafe {
            gl::UseProgram(self.id);
        }
    }

    pub fn set_float(&self, name: &str, value: f32) {
        let name_c_string = CString::new(name).unwrap();
        unsafe {
            let location = gl::GetUniformLocation(self.id, name_c_string.as_ptr());
            gl::Uniform1f(location, value);
        }
    }

    pub fn set_matrix4(&self, name: &str, matrix: &Mat4) {
        let name_c_string = CString::new(name).unwrap();
        unsafe {
            let location = gl::GetUniformLocation(self.id, name_c_string.as_ptr());
            gl::UniformMatrix4fv(location, 1, gl::FALSE, matrix.as_ptr());
        }
    }

    pub fn set_texture(&self, name: &str, texture: &Texture) {
        let name_c_string = CString::new(name).unwrap();
        unsafe {
            let location = gl::GetUniformLocation(self.id, name_c_string.as_ptr());
            gl::Uniform1i(location, (texture.slot + 1) as GLint);
        }
    }

    pub fn free(&self) {
        unsafe {
            gl::DeleteProgram(self.id);
        }
    }
}