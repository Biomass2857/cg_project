use std::collections::HashMap;

use glutin::event::ElementState;
use glutin::event::MouseButton;
use glutin::event::VirtualKeyCode;

pub struct InputState {
    keyboard_state: HashMap<VirtualKeyCode, ElementState>,
    mouse_state: HashMap<MouseButton, ElementState>,
}

impl InputState {
    pub fn new() -> Self {
        Self {
            keyboard_state: HashMap::new(),
            mouse_state: HashMap::new(),
        }
    }

    pub fn is_key_pressed(&self, key: &VirtualKeyCode) -> bool {
        self.keyboard_state.get(key).map(|&s| s == ElementState::Pressed).unwrap_or(false)
    }

    pub fn is_mousebutton_pressed(&self, button: &MouseButton) -> bool {
        self.mouse_state.get(button).map(|&s| s == ElementState::Pressed).unwrap_or(false)
    }

    pub fn process_event_key(&mut self, code: VirtualKeyCode, state: ElementState) {
        match state {
            ElementState::Pressed => {
                self.keyboard_state.insert(code, state);
            },
            ElementState::Released => {
                self.keyboard_state.remove(&code);
            }
        }
    }

    pub fn process_event_mouse_button(&mut self, mouse_button: MouseButton, state: ElementState) {
        match state {
            ElementState::Pressed => {
                self.mouse_state.insert(mouse_button, state);
            },
            ElementState::Released => {
                self.mouse_state.remove(&mouse_button);
            }
        }
    }
}