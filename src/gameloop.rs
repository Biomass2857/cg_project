use std::collections::HashMap;
use std::sync::{Arc, Mutex};
use std::thread;
use std::time::{Duration, Instant};

use crate::game::{self, Event, EventType, State, TankID, World};

#[derive(Clone, Hash, PartialEq)]
struct EventKey {
    event_type: EventType,
    tank_id: TankID,
}

impl Eq for EventKey {}

pub struct GameLoop {
    mutex: Arc<Mutex<()>>,
    running: Arc<Mutex<bool>>,
    thread: Option<thread::JoinHandle<()>>,
    snapshot: Arc<Mutex<State>>,
    event_buffer: Arc<Mutex<HashMap<EventKey, Event>>>,
}

impl GameLoop {
    pub fn new() -> Self {
        Self {
            mutex: Arc::new(Mutex::new(())),
            running: Arc::new(Mutex::new(false)),
            thread: None,
            snapshot: Arc::new(Mutex::new(State {
                tanks: HashMap::new(),
            })),
            event_buffer: Arc::new(Mutex::new(HashMap::default())),
        }
    }

    pub fn start(&mut self, initial_state: State) {
        if self.thread.is_some() {
            return;
        }

        let mutex = Arc::clone(&self.mutex);
        let running = Arc::clone(&self.running);
        let snapshot = Arc::clone(&self.snapshot);
        let event_buffer = Arc::clone(&self.event_buffer);

        *running.lock().unwrap() = true;

        self.thread = Some(thread::spawn(move || {
            let mut world = World::new(initial_state);
            let mut last_time = Instant::now();
            let tick_time = Duration::from_secs_f32(1.0 / game::GAME_FREQUENCY);

            while *running.lock().unwrap() {
                let _lock = mutex.lock().unwrap();

                let merged_events = Self::get_events(&event_buffer);
                world.tick(merged_events);
                *snapshot.lock().unwrap() = world.get_state().clone();
                Self::clear_events(&event_buffer);

                drop(_lock);

                let now = Instant::now();
                let time_taken = now.duration_since(last_time);
                last_time = now;

                if time_taken > tick_time {
                    println!(
                        "[GameLoop] Overload! gameloop tick time taken: {}ms",
                        time_taken.as_millis()
                    );
                } else {
                    thread::sleep(tick_time - time_taken);
                }
            }
        }));
    }

    pub fn stop(&mut self) {
        if let Some(thread) = self.thread.take() {
            *self.running.lock().unwrap() = false;
            thread.join().unwrap();
        }
    }

    fn get_events(event_buffer: &Arc<Mutex<HashMap<EventKey, Event>>>) -> Vec<Event> {
        event_buffer.lock().unwrap().values().cloned().collect()
    }

    fn clear_events(event_buffer: &Arc<Mutex<HashMap<EventKey, Event>>>) {
        event_buffer.lock().unwrap().clear();
    }

    pub fn accumulate_events(&self, events: Vec<Event>) {
        let _lock = self.mutex.lock().unwrap();

        for event in events {
            let key = EventKey {
                event_type: event.event_type,
                tank_id: event.tank_id,
            };
            self.event_buffer.lock().unwrap().insert(key, event);
        }
    }

    pub fn get_game_snapshot(&self) -> State {
        self.snapshot.lock().unwrap().clone()
    }
}

impl Drop for GameLoop {
    fn drop(&mut self) {
        self.stop();
    }
}
