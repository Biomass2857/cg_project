extern crate glutin;

mod shader;
mod shader_program;

use glutin::event::{Event, WindowEvent};
use glutin::event_loop::{ControlFlow, EventLoop};
use crate::shader::{Shader, ShaderType};
use crate::shader_program::ShaderProgram;
use glutin::window::WindowBuilder;
use glutin::ContextBuilder;
use gl::types::*;
use std::str;
use std::ffi::CString;
use std::ptr;

fn compile_shader(src: &str, ty: u32) -> u32 {
    let shader;
    unsafe {
        shader = gl::CreateShader(ty);
        // attempt to compile the shader
        let c_str = CString::new(src.as_bytes()).unwrap();
        gl::ShaderSource(shader, 1, &c_str.as_ptr(), ptr::null());
        gl::CompileShader(shader);

        // get compile status
        let mut status = gl::FALSE as GLint;
        gl::GetShaderiv(shader, gl::COMPILE_STATUS, &mut status);

        // fail on error
        if status != (gl::TRUE as GLint) {
            let mut len = 0;
            gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &mut len);
            let mut buf = Vec::with_capacity(len as usize);
            buf.set_len((len as usize) - 1); // subtract 1 to skip the trailing null character
            gl::GetShaderInfoLog(shader, len, ptr::null_mut(), buf.as_mut_ptr() as *mut GLchar);
            panic!("{}", str::from_utf8(&buf).ok().expect("ShaderInfoLog not valid utf8"));
        }
    }
    shader
}

fn link_program(vs: GLuint, fs: GLuint) -> GLuint {
    unsafe {
        let program = gl::CreateProgram();
        gl::AttachShader(program, vs);
        gl::AttachShader(program, fs);
        gl::LinkProgram(program);

        let mut status = gl::FALSE as GLint;
        gl::GetProgramiv(program, gl::LINK_STATUS, &mut status);

        if status != (gl::TRUE as GLint) {
            let mut len: GLint = 0;
            gl::GetProgramiv(program, gl::INFO_LOG_LENGTH, &mut len);

            let mut buf = Vec::with_capacity(len as usize);
            buf.set_len((len as usize) - 1);

            gl::GetProgramInfoLog(program, len, ptr::null_mut(), buf.as_mut_ptr() as *mut GLchar);
            panic!("{}", str::from_utf8(&buf).ok().expect("ProgramInfoLog not valid utf8"));
        }

        gl::DetachShader(program, vs);
        gl::DetachShader(program, fs);

        program
    }
}

fn create_vbo(vertices: &[f32]) -> GLuint {
    unsafe {
        let mut vbo: GLuint = 0;
        gl::GenBuffers(1, &mut vbo);
        gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
        gl::BufferData(
            gl::ARRAY_BUFFER,
            (vertices.len() * std::mem::size_of::<GLfloat>()) as GLsizeiptr,
            vertices.as_ptr() as *const GLvoid,
            gl::STATIC_DRAW,
        );
        vbo
    }
}

fn create_vao(vbo: GLuint) -> GLuint {
    unsafe {
        let mut vao: GLuint = 0;
        gl::GenVertexArrays(1, &mut vao);
        gl::BindVertexArray(vao);

        gl::BindBuffer(gl::ARRAY_BUFFER, vbo);

        gl::EnableVertexAttribArray(0);
        gl::VertexAttribPointer(
            0,
            2,
            gl::FLOAT,
            gl::FALSE as GLboolean,
            0,
            ptr::null()
        );

        vao
    }
}

fn main() {
    let event_loop = EventLoop::new();
    let window_builder = WindowBuilder::new()
        .with_title("Triangle Window")
        .with_inner_size(glutin::dpi::LogicalSize::new(1024.0, 768.0));
    let context = ContextBuilder::new().build_windowed(window_builder, &event_loop).unwrap();

    let context = unsafe { context.make_current().unwrap() };

    gl::load_with(|symbol| context.get_proc_address(symbol) as *const _);

    let vertex_shader_src = r#"
        #version 140

        in vec2 position;

        void main() {
            gl_Position = vec4(position, 0.0, 1.0);
        }
    "#;

    let fragment_shader_src = r#"
        #version 140

        out vec4 color;

        void main() {
            color = vec4(1.0, 0.5, 0.0, 1.0);
        }
    "#;

    let default_vertex_shader = Shader::new("../../shader/default/default", ShaderType::Vertex);
    let default_fragment_shader = Shader::new("../../shader/default/default", ShaderType::Fragment);
    let mut default_shader_program = ShaderProgram::new(vec![default_vertex_shader, default_fragment_shader]);

    let vertices: Vec<f32> = vec![
        -0.5, -0.5,
         0.5, -0.5,
         0.0,  0.5
    ];

    let vbo = create_vbo(&vertices);
    create_vao(vbo);

    event_loop.run(move |event, _, control_flow| {
        *control_flow = ControlFlow::Wait;

        match event {
            Event::WindowEvent { event, .. } => match event {
                WindowEvent::CloseRequested => *control_flow = ControlFlow::Exit,
                _ => (),
            },
            Event::RedrawRequested(_) => {
                context.swap_buffers().unwrap();
            },
            _ => (),
        }
    });
}