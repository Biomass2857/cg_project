extern crate nalgebra_glm as glm;

use glm::{pi, rotate_vec2, Vec2, Vec3};
use std::collections::HashMap;
use std::sync::atomic::{AtomicUsize, Ordering};

pub type TankID = i32;
pub type BulletID = i32;

pub const GAME_FREQUENCY: f32 = 300.0;
pub fn tank_size() -> Vec2 {
    Vec2::new(10.0, 10.0)
}

pub const TANK_SPEED: f32 = 200.0 / GAME_FREQUENCY / 4.0;
pub const BULLET_SPEED: f32 = 1.5 * TANK_SPEED;
pub fn tank_rotation_speed() -> f32 {
    2.0 * pi::<f32>() / GAME_FREQUENCY / 1.5
}
pub const MAX_BULLETS: u16 = 5;
pub fn map_size() -> Vec2 {
    Vec2::new(200.0, 100.0)
}

#[derive(Clone)]
pub struct Bullet {
    pub id: BulletID,
    pub pos: Vec2,
    pub direction: Vec2,
    pub speed: f32,
    pub ricochets: u16,
    pub max_ricochets: u16,
}

impl Bullet {
    pub fn new(id: BulletID, pos: Vec2, direction: Vec2) -> Self {
        Self {
            id,
            pos,
            direction,
            speed: BULLET_SPEED,
            ricochets: 0,
            max_ricochets: 1,
        }
    }

    pub fn generate_bullet_id() -> BulletID {
        static ID: AtomicUsize = AtomicUsize::new(0);
        ID.fetch_add(1, Ordering::SeqCst) as BulletID
    }
}

#[derive(Clone)]
pub struct Tank {
    pub id: TankID,
    pub pos: Vec2,
    pub size: Vec2,
    pub gun_direction: Vec2,
    pub wheel_direction: Vec2,
    pub speed: f32,
    pub is_alive: bool,
    pub bullets: Vec<Bullet>,
}

impl Tank {
    pub fn new(id: TankID, pos: Vec2) -> Self {
        Self {
            id,
            pos,
            size: tank_size(),
            gun_direction: Vec2::new(0.0, -1.0),
            wheel_direction: Vec2::new(0.0, -1.0),
            speed: TANK_SPEED,
            is_alive: true,
            bullets: Vec::new(),
        }
    }
}

#[derive(Clone)]
pub struct State {
    pub tanks: HashMap<TankID, Tank>,
}

#[derive(Copy, Clone, Hash, PartialEq)]
pub enum EventType {
    Forward,
    Backward,
    Shoot,
    RotateWheelsClockwise,
    RotateWheelsCClockwise,
    // GunDirection,
}

#[derive(Clone)]
pub struct Event {
    pub tank_id: i32,
    pub event_type: EventType,

    pub value: f32,
}

pub struct World {
    state: State,
}

impl World {
    pub fn new(initial_state: State) -> Self {
        Self {
            state: initial_state,
        }
    }

    pub fn tick(&mut self, events: Vec<Event>) {
        self.state = self.get_next_state(self.state.clone(), events);
    }

    pub fn get_state(&self) -> &State {
        &self.state
    }
}

impl World {
    fn bullet_collides_map_vertical(bullet: &Bullet) -> bool {
        bullet.pos.y <= 0.0 || bullet.pos.y >= map_size().y
    }

    fn bullet_collides_map_horizontal(bullet: &Bullet) -> bool {
        bullet.pos.x <= 0.0 || bullet.pos.x >= map_size().x
    }

    fn intersects(tank: &Tank, bullet: &Bullet) -> bool {
        tank.pos.x < bullet.pos.x
            && bullet.pos.x < tank.pos.x + tank.size.x
            && tank.pos.y < bullet.pos.y
            && bullet.pos.y < tank.pos.y + tank.size.y
    }

    pub fn clamp3(v: Vec3, min: Vec3, max: Vec3) -> Vec3 {
        Vec3::new(
            v.x.max(min.x).min(max.x),
            v.y.max(min.y).min(max.y),
            v.z.max(min.z).min(max.z),
        )
    }

    pub fn clamp2(v: Vec2, min: Vec2, max: Vec2) -> Vec2 {
        Vec2::new(v.x.max(min.x).min(max.x), v.y.max(min.y).min(max.y))
    }

    pub fn get_next_state(&self, mut state: State, events: Vec<Event>) -> State {
        for event in events {
            if let Some(tank) = state.tanks.get_mut(&event.tank_id) {
                match event.event_type {
                    EventType::Forward => {
                        tank.pos += tank.wheel_direction * tank.speed;
                        tank.pos =
                            Self::clamp2(tank.pos, Vec2::new(0.0, 0.0), map_size() - tank.size);
                    }
                    EventType::Backward => {
                        tank.pos -= tank.wheel_direction * tank.speed;
                        tank.pos =
                            Self::clamp2(tank.pos, Vec2::new(0.0, 0.0), map_size() - tank.size);
                    }
                    EventType::Shoot => {
                        if tank.bullets.len() < MAX_BULLETS as usize {
                            tank.bullets.push(Bullet::new(
                                Bullet::generate_bullet_id(),
                                tank.pos + tank.gun_direction * 2.0,
                                tank.gun_direction,
                            ));
                        }
                    }
                    EventType::RotateWheelsClockwise => {
                        tank.wheel_direction =
                            rotate_vec2(&tank.wheel_direction, -tank_rotation_speed());
                        tank.gun_direction = tank.wheel_direction;
                    }
                    EventType::RotateWheelsCClockwise => {
                        tank.wheel_direction =
                            rotate_vec2(&tank.wheel_direction, tank_rotation_speed());
                        tank.gun_direction = tank.wheel_direction;
                    } 
                    /* EventType::GunDirection => {
                        tank.gun_direction = Vec2::new(event.value.cos(), event.value.sin());
                    } */
                }
            }
        }

        for (_, tank) in &mut state.tanks {
            for i in 0..tank.bullets.len() {
                let bullet = &mut tank.bullets[i];
                bullet.pos += bullet.direction * bullet.speed;
                bullet.pos = Self::clamp2(bullet.pos, Vec2::new(0.0, 0.0), map_size());

                if Self::bullet_collides_map_vertical(bullet) {
                    bullet.direction.y *= -1.0;
                    bullet.ricochets += 1;
                }

                if Self::bullet_collides_map_horizontal(bullet) {
                    bullet.direction.x *= -1.0;
                    bullet.ricochets += 1;
                }
            }
        }

        let mut dead_tanks = Vec::new();
        let mut dead_bullets: Vec<(i32, usize)> = Vec::new();
        for (tank_id, tank) in &state.tanks {
            for i in (0..tank.bullets.len()).rev() {
                let bullet = &tank.bullets[i];

                if bullet.ricochets > bullet.max_ricochets {
                    dead_bullets.push((*tank_id, i));
                    continue;
                }

                for (other_tank_id, other_tank) in &state.tanks {
                    if Self::intersects(&other_tank, bullet) {
                        dead_bullets.push((*tank_id, i));
                        dead_tanks.push(*other_tank_id);
                        break;
                    }
                }
            }
        }

        for (tank_id, bullet_id) in dead_bullets {
            state
                .tanks
                .get_mut(&tank_id)
                .unwrap()
                .bullets
                .remove(bullet_id);
        }

        for dead_tank in dead_tanks {
            state.tanks.remove(&dead_tank);
        }

        state
    }
}
