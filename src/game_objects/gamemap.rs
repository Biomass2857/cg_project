extern crate glutin;
extern crate nalgebra_glm as glm;

use std::collections::HashMap;

use crate::camera::Camera;
use crate::game;
use crate::gameloop::GameLoop;
use crate::input_state::InputState;
use crate::object::Object;
use crate::render::Render;
use crate::shader_program::ShaderProgram;
use crate::texture_atlas::TextureAtlas;
use crate::vertex_feature::VertexFeature;

use super::cube::Cube;
use super::shell::Shell;
use super::tank::Tank;

pub const CUBE_WIDTH: f32 = 1.0;

pub struct GameMap {
    size: glm::Vec2,
    color_shader: ShaderProgram,
    texture_shader: ShaderProgram,
    atlas: TextureAtlas,
    floor: Object,
    tanks: HashMap<game::TankID, Tank>,
    shells: HashMap<i32, Shell>,
    cubes: Vec<Cube>,
    game_loop: GameLoop,
    space_pressed: bool,
}

impl GameMap {
    pub fn new(
        atlas: TextureAtlas,
        color_shader: ShaderProgram,
        texture_shader: ShaderProgram,
    ) -> Self {
        let mut floor = Object::new(
            vec![
                0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0,
                20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 20.0, 10.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,
            ],
            vec![0, 1, 2, 1, 2, 3],
            vec![
                VertexFeature::Position,
                VertexFeature::Normal,
                VertexFeature::UV,
            ],
        );

        let environment_texture = atlas.get_texture("floor_dark").unwrap();
        floor.set_texture(environment_texture);
        floor.set_shader(texture_shader);

        let (game_tanks, graphic_tanks) = Self::generate_tanks(color_shader);
        let initial_state = game::State { tanks: game_tanks };
        let mut game_loop = GameLoop::new();

        // game_loop.start(initial_state);

        let mut new_game_map = Self {
            floor,
            atlas,
            color_shader,
            texture_shader,
            game_loop,
            size: glm::vec2(20.0, 20.0),
            tanks: graphic_tanks,
            shells: HashMap::new(),
            cubes: Vec::new(),
            space_pressed: false,
        };

        new_game_map.generate_wall();

        new_game_map
    }

    pub fn get_input(&mut self, input_state: &InputState) {
        let mut event = game::Event {
            tank_id: 0,
            event_type: game::EventType::Forward,
            value: 0.0,
        };

        if input_state.is_key_pressed(&glutin::event::VirtualKeyCode::W) {
            event.event_type = game::EventType::Forward;
            self.game_loop.accumulate_events(vec![event.clone()]);
        }

        if input_state.is_key_pressed(&glutin::event::VirtualKeyCode::S) {
            event.event_type = game::EventType::Backward;
            self.game_loop.accumulate_events(vec![event.clone()]);
        }

        if input_state.is_key_pressed(&glutin::event::VirtualKeyCode::A) {
            event.event_type = game::EventType::RotateWheelsCClockwise;
            self.game_loop.accumulate_events(vec![event.clone()]);
        }

        if input_state.is_key_pressed(&glutin::event::VirtualKeyCode::D) {
            event.event_type = game::EventType::RotateWheelsClockwise;
            self.game_loop.accumulate_events(vec![event.clone()]);
        }

        if input_state.is_key_pressed(&glutin::event::VirtualKeyCode::Space) {
            if !self.space_pressed {
                event.event_type = game::EventType::Shoot;
                self.game_loop.accumulate_events(vec![event.clone()]);
                self.space_pressed = true;
            }
        } else {
            self.space_pressed = false;
        }
    }

    pub fn render(&mut self, camera: &Camera) {
        // self.preprare_show_state(self.game_loop.get_game_snapshot());

        self.floor.render(camera);

        for cube in &self.cubes {
            cube.render(camera);
        }

        for (_, tank) in &self.tanks {
            tank.render(camera);
        }

        for (_, shell) in &self.shells {
            shell.render(camera);
        }
    }

    fn divide_component_wise(a: glm::Vec2, b: glm::Vec2) -> glm::Vec2 {
        glm::Vec2::new(a.x / b.x, a.y / b.y)
    }

    fn mul_comp_wise(a: glm::Vec2, b: glm::Vec2) -> glm::Vec2 {
        glm::Vec2::new(a.x * b.x, a.y * b.y)
    }

    fn preprare_show_state(&mut self, state: game::State) {
        for tank_pair in &state.tanks {
            let state_tank_id = tank_pair.0;
            let state_tank = tank_pair.1;

            if let Some(tank) = self.tanks.get_mut(&state_tank_id) {
                let relative_pos = Self::divide_component_wise(state_tank.pos, game::map_size());
                let pos_in_map = Self::mul_comp_wise(relative_pos, self.size);
                tank.set_translation(glm::Vec3::new(pos_in_map.x, pos_in_map.y, 0.0));

                let rotation_angle = state_tank
                    .wheel_direction
                    .y
                    .atan2(state_tank.wheel_direction.x)
                    - 3.0 * glm::pi::<f32>() / 2.0;
                tank.set_rotation(rotation_angle, glm::vec3(0.0, 0.0, 1.0));
            } else {
                panic!("Tank with id {} not found", state_tank_id);
            }

            for bullet in &state_tank.bullets {
                let shell_exists = self.shells.contains_key(&bullet.id);

                if !shell_exists {
                    let mut shell = Shell::new();
                    shell.set_rotation(90.0_f32.to_radians(), glm::vec3(1.0, 0.0, 0.0));
                    shell.set_shader(self.color_shader);
                    self.shells.insert(bullet.id, shell);
                }

                let shell = self.shells.get_mut(&bullet.id).unwrap();
                let relative_pos = Self::divide_component_wise(bullet.pos, game::map_size());
                let pos_in_map = Self::mul_comp_wise(relative_pos, self.size);
                shell.set_translation(glm::vec3(pos_in_map.x, pos_in_map.y, 0.0));

                let rotation_angle =
                    bullet.direction.y.atan2(bullet.direction.x) - 3.0 * glm::pi::<f32>() / 2.0;
                shell.set_rotation(rotation_angle, glm::vec3(0.0, 0.0, 1.0));
            }
        }

        let mut shells_to_remove = Vec::new();
        for (shell_id, _) in &self.shells {
            let mut shell_exists = false;

            for (_, tank) in &state.tanks {
                for bullet in &tank.bullets {
                    if *shell_id == bullet.id {
                        shell_exists = true;
                        break;
                    }
                }
            }

            if !shell_exists {
                shells_to_remove.push(*shell_id);
            }
        }

        for shell_id in shells_to_remove {
            self.shells.remove(&shell_id);
        }
    }

    fn generate_wall(&mut self) {
        let cube_texture = self.atlas.get_texture("box_texture_light").unwrap();

        let block_count_x = (self.size.x / CUBE_WIDTH) as u32;
        let block_count_y = (self.size.y / CUBE_WIDTH) as u32;

        for i in 0..block_count_x {
            for j in 0..block_count_y {
                if i == 0 || i == block_count_x - 1 || j == 0 || j == block_count_y - 1 {
                    let mut cube = Cube::new(CUBE_WIDTH / 2.0, cube_texture);
                    cube.translate(glm::vec3(i as f32 * CUBE_WIDTH, j as f32 * CUBE_WIDTH, 0.0));
                    cube.scale(0.9);
                    cube.set_shader(self.texture_shader);
                    self.cubes.push(cube);
                }
            }
        }
    }

    fn generate_tanks(
        color_shader: ShaderProgram,
    ) -> (
        HashMap<game::TankID, game::Tank>,
        HashMap<game::TankID, Tank>,
    ) {
        let mut game_tanks = HashMap::new();
        let mut tanks = HashMap::new();

        for i in 0..4 {
            let relative_pos =
                glm::vec2(0.25 + 0.5 * (i as f32 / 2.0), 0.25 + 0.5 * (i as f32 % 2.0));
            let tank = game::Tank::new(i, Self::mul_comp_wise(game::map_size(), relative_pos));
            game_tanks.insert(i, tank);

            let mut tank_object = Tank::new();
            tank_object.set_shader(color_shader);
            tank_object.scale(0.5);
            tanks.insert(i, tank_object);
        }

        (game_tanks, tanks)
    }
}
