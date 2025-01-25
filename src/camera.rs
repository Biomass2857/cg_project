extern crate glutin;
extern crate nalgebra_glm as glm;

use glm::{Mat4, Vec3};
use glutin::event::MouseButton;
use glutin::dpi::LogicalPosition;

use crate::input_state::InputState;

#[derive(Clone, Copy)]
pub struct Camera {
    pub position: Vec3,
    pub orientation: Vec3,
    pub up: Vec3,
    pub first_click: bool,
    pub width: i32,
    pub height: i32,
    pub slow_move_speed: f32,
    pub fast_move_speed: f32,
    pub current_speed: f32,
    pub sensitivity: f32,
    pub fov_deg: f32,
    pub near_plane: f32,
    pub far_plane: f32,
}

impl Camera {
    pub fn new(
        width: i32,
        height: i32,
        position: Vec3,
        fov_deg: f32,
        near_plane: f32,
        far_plane: f32,
    ) -> Camera {
        Camera {
            position,
            orientation: Vec3::new(0.0, 0.0, -1.0),
            up: Vec3::new(0.0, 1.0, 0.0),
            first_click: true,
            width,
            height,
            slow_move_speed: 10.0,
            fast_move_speed: 40.0,
            current_speed: 10.0,
            sensitivity: 10000.0,
            fov_deg,
            near_plane,
            far_plane,
        }
    }

    pub fn get_matrix(&self) -> Mat4 {
        let view = glm::look_at(
            &self.position,
            &(self.position + self.orientation),
            &self.up,
        );
        let projection = glm::perspective(
            self.fov_deg.to_radians(),
            self.width as f32 / self.height as f32,
            self.near_plane,
            self.far_plane,
        );

        projection * view
    }

    pub fn get_key_input(
        &mut self,
        window: &glutin::window::Window,
        input_state: &InputState,
        cursor_pos: &LogicalPosition<f32>,
        time_step: f32,
    ) {
        if input_state.is_key_pressed(&glutin::event::VirtualKeyCode::W) {
            self.position += self.current_speed * time_step * self.orientation;
        }

        if input_state.is_key_pressed(&glutin::event::VirtualKeyCode::A) {
            self.position += self.current_speed
                * time_step
                * -glm::normalize(&glm::cross::<f32, glm::U3>(&self.orientation, &self.up));
        }

        if input_state.is_key_pressed(&glutin::event::VirtualKeyCode::S) {
            self.position += self.current_speed * time_step * -self.orientation;
        }

        if input_state.is_key_pressed(&glutin::event::VirtualKeyCode::D) {
            self.position += self.current_speed
                * time_step
                * glm::normalize(&glm::cross::<f32, glm::U3>(&self.orientation, &self.up));
        }

        if input_state.is_key_pressed(&glutin::event::VirtualKeyCode::Space) {
            self.position += self.current_speed * time_step * self.up;
        }

        if input_state.is_key_pressed(&glutin::event::VirtualKeyCode::LControl) {
            self.position += self.current_speed * time_step * -self.up;
        }

        if input_state.is_key_pressed(&glutin::event::VirtualKeyCode::LShift) {
            self.current_speed = self.fast_move_speed;
        } else {
            self.current_speed = self.slow_move_speed;
        }

        if input_state.is_mousebutton_pressed(&MouseButton::Left) {
            if self.first_click {
                window
                    .set_cursor_position(LogicalPosition::new(
                        self.width / 2,
                        self.height / 2,
                    ))
                    .unwrap();

                self.first_click = false;
            }

            let rot_x = time_step * self.sensitivity * (cursor_pos.y - self.height as f32 / 2.0)
                / self.height as f32;
            let rot_y = time_step * self.sensitivity * (cursor_pos.x - self.width as f32 / 2.0)
                / self.width as f32;

            let rot_matrix = glm::rotate(
                &Mat4::identity(),
                -rot_x.to_radians(),
                &glm::normalize(&glm::cross::<f32, glm::U3>(&self.orientation, &self.up)),
            );

            let new_orientation = glm::mat4_to_mat3(&rot_matrix) * self.orientation;

            let degress: f32 = 90.0;
            let orientation_threshhold_degrees: f32 = 85.0;
            if (glm::angle(&new_orientation, &self.up) - degress.to_radians()).abs()
                <= orientation_threshhold_degrees.to_radians()
            {
                self.orientation = new_orientation;
            }

            let rot_matrix = glm::rotate(&Mat4::identity(), -rot_y.to_radians(), &self.up);
            self.orientation = glm::mat4_to_mat3(&rot_matrix) * self.orientation;

            window
                .set_cursor_position(LogicalPosition::new(
                    self.width / 2,
                    self.height / 2,
                ))
                .unwrap();
        } else {
            self.first_click = true;
        }
    }
}
