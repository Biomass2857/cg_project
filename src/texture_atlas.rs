use std::collections::HashMap;
use std::path::Path;
use std::fs::File;
use std::io::Read;
use std::io::Error;
use image::{ImageBuffer, Rgba};

use crate::texture::Texture;

pub struct TextureConfiguration {
    pub name: String,
    pub start_x: u32,
    pub start_y: u32,
    pub width: u32,
    pub height: u32,
}

impl TextureConfiguration {
    pub fn new(name: String, start_x: u32, start_y: u32, width: u32, height: u32) -> Self {
        Self {
            name,
            start_x,
            start_y,
            width,
            height,
        }
    }
}

pub struct TextureAtlas {
    textures: HashMap<String, Texture>,
}

impl TextureAtlas {
    pub fn new(path: String, configuration: Vec<TextureConfiguration>) -> Result<Self, Error> {
        let mut file = File::open(Path::new(&path))?;
        let mut buffer = Vec::new();
        file.read_to_end(&mut buffer)?;

        let img = image::load_from_memory(&buffer).unwrap().to_rgba8();

        let mut textures = HashMap::new();

        for config in configuration {
            let sub_image = image::imageops::crop_imm(&img, config.start_x, config.start_y, config.width, config.height).to_image();
            textures.insert(config.name, Texture::new(sub_image.to_vec(), config.width, config.height, 4));
        }

        Ok(Self { textures })
    }

    pub fn get_texture(&self, name: &str) -> Option<Texture> {
        self.textures.get(name).copied()
    }

    pub fn free(&mut self) {
        self.textures.clear();
    }
}