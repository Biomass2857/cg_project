extern crate glutin;

use std::fs::File;
use std::io::prelude::*;
use std::ffi::CString;
use std::str;
use std::ptr;
use gl::types::*;

pub enum ShaderType {
    Vertex,
    Fragment,
}

#[derive(Clone, Copy)]
pub struct Shader {
    id: u32,
}

impl Shader {
    pub fn new(path: &str, shader_type: ShaderType) -> Shader {
        let path = format!("{}{}", path, Self::get_extension(&shader_type));
        // println!("Loading shader from: {}", path);
        let mut file = File::open(path).expect("Unable to open the file");
        let mut source = String::new();
        file.read_to_string(&mut source).expect("Unable to read the file");

        let id = unsafe {
            let id = gl::CreateShader(Self::get_gl_shader_type(&shader_type));
            let c_str = CString::new(source.as_bytes()).unwrap();
            gl::ShaderSource(id, 1, &c_str.as_ptr(), ptr::null());
            gl::CompileShader(id);

            let mut success = gl::FALSE as GLint;
            let mut info_log = Vec::with_capacity(512);
            info_log.set_len(512 - 1); // subtract 1 to skip the trailing null character
            gl::GetShaderiv(id, gl::COMPILE_STATUS, &mut success);

            if success != gl::TRUE as GLint {
                gl::GetShaderInfoLog(id, 512, ptr::null_mut(), info_log.as_mut_ptr() as *mut GLchar);
                println!("ERROR::SHADER::COMPILATION_FAILED\n{}", str::from_utf8(&info_log).unwrap());
            }
            
            id
        };

        Shader { id }
    }

    fn get_gl_shader_type(shader_type: &ShaderType) -> u32 {
        match shader_type {
            ShaderType::Vertex => gl::VERTEX_SHADER,
            ShaderType::Fragment => gl::FRAGMENT_SHADER,
        }
    }

    fn get_extension(shader_type: &ShaderType) -> &'static str {
        match shader_type {
            ShaderType::Vertex => ".vert",
            ShaderType::Fragment => ".frag",
        }
    }

    pub fn id(&self) -> u32 {
        self.id
    }

    pub fn free(&self) {
        unsafe {
            gl::DeleteShader(self.id);
        }
    }
}