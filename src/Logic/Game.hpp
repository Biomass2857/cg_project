#ifndef GAME_HPP
#define GAME_HPP

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Game {
    typedef int TankID;

    struct Bullet;

    const float gameFrequency = 300.0f;
    const float tankSpeed = 200.0f / gameFrequency / 8.f;
    const float bulletSpeed = 2.5f * tankSpeed;
    const float tankRotationSpeed = 2.0f * glm::pi<float>() / gameFrequency / 3.f;
    const unsigned short maxBullets = 5;
    const glm::vec2 mapSize = glm::vec2(200.0f, 100.0f);

    struct Tank {
        TankID id;
        glm::vec2 pos;
        glm::vec2 size;
        glm::vec2 gunDirection;
        glm::vec2 wheelDirection;
        float speed;

        bool isAlive;

        std::vector<Bullet> bullets;

        Tank(int id, glm::vec2 pos);
    };

    struct Bullet {
        glm::vec2 pos;
        glm::vec2 direction;
        float speed = bulletSpeed;
        unsigned short richochets = 0;
        unsigned short maxRichochets = 1;

        Bullet(glm::vec2 pos, glm::vec2 direction);
    };

    struct State {
        std::vector<struct Tank> tanks;
    };

    enum class EventType {
        FORWARD = 0,
        BACKWARD,
        SHOOT,
        ROTATE_WHEELS_CLOCKWISE,
        ROTATE_WHEELS_CCLOCKWISE,
        GUN_DIRECTION
    };

    struct Event {
        int tankId;
        EventType type;

        float value;
    };

    class World {
        public:
            World(State initialState);

            void tick(std::vector<Event> events);

            State getState() const;
            State getNextState(State state, std::vector<Event> events) const;
        private:
            State state;
    };
}

#endif