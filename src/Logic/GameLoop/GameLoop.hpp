#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include <thread>
#include <unordered_map>
#include <atomic>
#include <chrono>
#include <mutex>
#include <iostream>
#include <algorithm>

#include "../Game.hpp"

struct PairHash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

class GameLoop {
    public:
        GameLoop();

        void start(Game::State initialState);
        void stop();

        void accumulateEvents(std::vector<Game::Event> events);
        std::vector<Game::Event> getEvents();
        void clearEvents();

        Game::State getGameSnapshot();
    private:
        std::mutex mutex;
        std::atomic<bool> running;
        std::thread* thread;
        Game::State snapshot;
        std::unordered_map<std::pair<Game::EventType, Game::TankID>, Game::Event, PairHash> eventBuffer;
};

#endif