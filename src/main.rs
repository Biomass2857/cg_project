extern crate glutin;
extern crate nalgebra_glm as glm;

use glutin::dpi::LogicalSize;
use glutin::event::{Event, WindowEvent};
use glutin::event_loop::{ControlFlow, EventLoop};
use glutin::window::WindowBuilder;
use glutin::ContextBuilder;
use std::time::Instant;

mod camera;
mod game;
mod game_objects;
mod gameloop;
mod object;
mod object_loader;
mod object_template;
mod render;
mod shader;
mod shader_program;
mod texture;
mod texture_atlas;
mod vertex_feature;

use crate::camera::Camera;
use crate::shader::Shader;
use crate::shader::ShaderType;
use crate::shader_program::ShaderProgram;
use crate::texture_atlas::TextureAtlas;
use crate::texture_atlas::TextureConfiguration;

use crate::game_objects::gamemap::GameMap;

const WINDOW_WIDTH: i32 = 600;
const WINDOW_HEIGHT: i32 = 600;

fn main() -> Result<(), std::io::Error> {
    let el = EventLoop::new();
    let wb = WindowBuilder::new()
        .with_title("Wii Tanks!")
        .with_inner_size(LogicalSize::new(WINDOW_WIDTH, WINDOW_HEIGHT));

    let windowed_context = ContextBuilder::new().build_windowed(wb, &el).unwrap();
    let windowed_context = unsafe { windowed_context.make_current().unwrap() };

    gl::load_with(|symbol| windowed_context.get_proc_address(symbol) as *const _);

    let mut u_time = 0.0;
    let start = Instant::now();

    let texture_configurations = vec![
        TextureConfiguration::new("floor_dark".to_string(), 5, 276, 1024, 512),
        TextureConfiguration::new("floor_light".to_string(), 5, 276 + 1024 + 5, 1024, 512),
        TextureConfiguration::new("box_texture_light".to_string(), 902, 207, 64, 64),
    ];

    let texture_atlas = TextureAtlas::new(
        "./assets/wii/tanks_environment_texture_atlas.png".to_string(),
        texture_configurations,
    )?;
    let default_vertex_shader = Shader::new("./shader/default/default", ShaderType::Vertex);
    let default_fragment_shader = Shader::new("./shader/default/default", ShaderType::Fragment);
    let default_shader_program =
        ShaderProgram::new(vec![default_vertex_shader, default_fragment_shader]);
    default_shader_program.use_program();

    let texture_vertex_shader = Shader::new("./shader/texture/texture", ShaderType::Vertex);
    let texture_fragment_shader = Shader::new("./shader/texture/texture", ShaderType::Fragment);
    let texture_shader_program =
        ShaderProgram::new(vec![texture_vertex_shader, texture_fragment_shader]);

    let mut game_map = GameMap::new(
        texture_atlas,
        default_shader_program,
        texture_shader_program,
    );
    let mut camera = Camera::new(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        glm::Vec3::new(0.0, 0.0, 2.0),
        45.0,
        0.1,
        100.0,
    );

    let mut left_mouse_key_pressed = false;
    let mut delta_time = 0.0;

    el.run(move |event, _, control_flow| {
        *control_flow = ControlFlow::Wait;

        match event {
            Event::WindowEvent { event, .. } => match event {
                WindowEvent::CloseRequested => *control_flow = ControlFlow::Exit,
                WindowEvent::MouseInput { state, button, .. } => match button {
                    glutin::event::MouseButton::Left => {
                        if state == glutin::event::ElementState::Pressed {
                            left_mouse_key_pressed = true;
                        } else {
                            left_mouse_key_pressed = false;
                        }
                    }
                    _ => (),
                },
                WindowEvent::KeyboardInput { .. } => {
                    if left_mouse_key_pressed {
                        let _ = windowed_context.window().set_cursor_grab(true);
                        windowed_context.window().set_cursor_visible(false);

                        camera.get_key_input(windowed_context.window(), &event, delta_time);
                    } else {
                        let _ = windowed_context.window().set_cursor_grab(false);
                        windowed_context.window().set_cursor_visible(true);

                        game_map.get_input(&event);
                    }
                }
                _ => (),
            },
            Event::MainEventsCleared => {
                windowed_context.window().request_redraw();
            }
            Event::RedrawRequested(_) => {
                unsafe {
                    gl::ClearColor(0.2, 0.0, 0.3, 1.0);
                    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
                }

                let new_time = start.elapsed().as_secs_f32();
                delta_time = new_time - u_time;
                u_time = new_time;

                default_shader_program.set_float("u_time", u_time);
                texture_shader_program.set_float("u_time", u_time);
                game_map.render(&camera);

                windowed_context.swap_buffers().unwrap();
            }
            _ => (),
        }
    });
}
