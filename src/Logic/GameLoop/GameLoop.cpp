#include "GameLoop.hpp"

GameLoop::GameLoop() {
    running.store(false);
}

void GameLoop::start(Game::State initialState) {
    if(thread != nullptr) return;

    running.store(true);
    thread = new std::thread([this, initialState] {
        Game::World world(initialState);

        auto lastTime = std::chrono::steady_clock::now();

        float tickTimeMs = 1000.f / Game::gameFrequency;

        while(running.load()) {
            std::unique_lock<std::mutex> lock(mutex);

            std::vector<Game::Event> mergedEvents = this->getEvents();
            std::cout <<"[GameLoop] Merged events: " << mergedEvents.size() << std::endl;
            world.tick(mergedEvents);
            snapshot = world.getState();
            this->clearEvents();

            lock.unlock();

            auto now = std::chrono::steady_clock::now();
            float timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count();
            lastTime = now;

            std::cout <<"[GameLoop] Time taken: " << timeTaken << "ms"<< std::endl;

            if(timeTaken < tickTimeMs) {
                std::chrono::milliseconds sleepTime((long long) std::max(0.f, tickTimeMs - timeTaken));
                std::this_thread::sleep_for(sleepTime);
            }
        }
    });
}

void GameLoop::stop() {
    if(thread == nullptr) return;

    running.store(false);

    delete thread;
    thread = nullptr;
}

std::vector<Game::Event> GameLoop::getEvents() {
    std::vector<Game::Event> values;
    for(auto event : eventBuffer) {
        values.push_back(event.second);
    }

    return values;
}

void GameLoop::clearEvents() {
    eventBuffer.clear();
}

void GameLoop::accumulateEvents(std::vector<Game::Event> events) {
    for(auto event : events) {
        auto key = std::make_pair(event.type, event.tankId);
        eventBuffer.insert(std::make_pair(key, event));
    }
}

Game::State GameLoop::getGameSnapshot() {
    return snapshot;
}