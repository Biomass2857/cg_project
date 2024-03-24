extern crate gl;

use std::sync::atomic::{AtomicUsize, Ordering};
use gl::types::*;

#[derive(Copy, Clone)]
pub struct Texture {
    id: GLuint,
    pub slot: i32,
}

impl Texture {
    pub fn new(data: Vec<u8>, width: u32, height: u32, channels: u16) -> Self {
        let slot = next_slot();
        let mut id: GLuint = 0;

        unsafe {
            gl::GenTextures(1, &mut id);
            gl::ActiveTexture(gl::TEXTURE0 + slot as GLuint);
            gl::BindTexture(gl::TEXTURE_2D, id);

            gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST as i32);
            gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST as i32);

            gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::REPEAT as i32);
            gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::REPEAT as i32);

            gl::TexImage2D(
                gl::TEXTURE_2D,
                0,
                gl::RGBA as i32,
                width as i32,
                height as i32,
                0,
                gl::RGBA,
                gl::UNSIGNED_BYTE,
                data.as_ptr() as *const GLvoid,
            );
            gl::GenerateMipmap(gl::TEXTURE_2D);

            gl::BindTexture(gl::TEXTURE_2D, 0);
        }

        Self {
            id,
            slot,
        }
    }

    pub fn bind(&self) {
        unsafe {
            gl::ActiveTexture(gl::TEXTURE0 + self.slot as GLuint);
            gl::BindTexture(gl::TEXTURE_2D, self.id);
        }
    }

    pub fn free(&self) {
        unsafe {
            gl::DeleteTextures(1, &self.id);
        }
    }
}

fn next_slot() -> GLint {
    static USED_SLOTS: AtomicUsize = AtomicUsize::new(0);
    USED_SLOTS.fetch_add(1, Ordering::SeqCst) as GLint
}
