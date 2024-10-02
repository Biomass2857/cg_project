use std::collections::HashMap;
use kira::manager::{AudioManager, AudioManagerSettings, DefaultBackend};
use kira::sound::static_sound::StaticSoundData;

pub struct SoundDefintion {
    name: String,
    filename: String,
}

impl SoundDefintion {
    pub fn new(name: String, filename: String) -> Self {
        Self {
            name,
            filename
        }
    }
}

pub struct SoundManager {
    audio_manager: AudioManager,
    sound_map: HashMap<String, StaticSoundData>,
}

impl SoundManager {
    pub fn new(defintions: Vec<SoundDefintion>) -> Self {
        let mut sound_map = HashMap::new();

        for defintion in defintions {
            sound_map.insert(defintion.name, StaticSoundData::from_file(defintion.filename).unwrap());
        }

        Self {
            audio_manager: AudioManager::<DefaultBackend>::new(AudioManagerSettings::default()).unwrap(),
            sound_map: sound_map,
        }
    }

    pub fn play_sound(&mut self, name: &str) {
        self.audio_manager.play(self.sound_map.get(name).unwrap().clone());
    }
}