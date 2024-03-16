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
use std::ptr;

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
            3,
            gl::FLOAT,
            gl::FALSE as GLboolean,
            6 * std::mem::size_of::<GLfloat>() as GLsizei,
            ptr::null(),
        );

        let pointer = (3 * std::mem::size_of::<GLfloat>()) as *const GLvoid;
        gl::EnableVertexAttribArray(1);
        gl::VertexAttribPointer(
            1,
            3,
            gl::FLOAT,
            gl::FALSE as GLboolean,
            6 * std::mem::size_of::<GLfloat>() as GLsizei,
            pointer,
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

    let default_vertex_shader = Shader::new("../../shader/color/color", ShaderType::Vertex);
    let default_fragment_shader = Shader::new("../../shader/color/color", ShaderType::Fragment);
    let default_shader_program = ShaderProgram::new(vec![default_vertex_shader, default_fragment_shader]);

    default_shader_program.use_program();

    let vertices: Vec<f32> = vec![
        -0.5, -0.5, 0.0, 1.0, 1.0, 1.0,
         0.5, -0.5, 0.0, 1.0, 1.0, 1.0,
         0.0, 0.5, 0.0, 1.0, 1.0, 1.0,
    ];

    let vbo = create_vbo(&vertices);
    let vao = create_vao(vbo);

    event_loop.run(move |event, _, control_flow| {
        *control_flow = ControlFlow::Wait;

        match event {
            Event::WindowEvent { event, .. } => match event {
                WindowEvent::CloseRequested => *control_flow = ControlFlow::Exit,
                _ => (),
            },
            Event::RedrawRequested(_) => {
                unsafe {
                    gl::ClearColor(0.2, 0.0, 0.3, 1.0);
                    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

                    gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
                    gl::BindVertexArray(vao);
                    gl::DrawArrays(gl::TRIANGLES, 0, 3);
                }

                context.swap_buffers().unwrap();
            },
            _ => (),
        }
    });
}