#include "Game.hpp"

namespace Game {
    Tank::Tank(int id, glm::vec2 pos) {
        this->id = id;
        this->pos = pos;
        
        size = glm::vec2(1.0f, 1.0f);
        gunDirection = glm::vec2(0.0f, -1.0f);
        wheelDirection = glm::vec2(0.0f, -1.0f);
        speed = tankSpeed;

        isAlive = true;
    }

    Bullet::Bullet(BulletID id, glm::vec2 pos, glm::vec2 direction) {
        this->id = id;
        this->pos = pos;
        this->direction = direction;
    }

    BulletID Bullet::generateBulletID() {
        static BulletID id = 0;
        return id++;
    }

    World::World(State initialState) {
        state = initialState;
    }

    void World::tick(std::vector<Event> events) {
        state = getNextState(state, events);
    }

    State World::getState() const {
        return state;
    }

    bool intersects(Tank tank, Bullet bullet) {
        return tank.pos.x < bullet.pos.x && bullet.pos.x < tank.pos.x + tank.size.x &&
               tank.pos.y < bullet.pos.y && bullet.pos.y < tank.pos.y + tank.size.y;
    }

    bool bulletCollidesMapVertical(Bullet bullet) {
        return bullet.pos.y <= 0 || bullet.pos.y >= mapSize.y;
    }

    bool bulletCollidesMapHorizontal(Bullet bullet) {
        return bullet.pos.x <= 0 || bullet.pos.x >= mapSize.x;
    }

    State World::getNextState(State state, std::vector<Event> events) const {
        for(Event event : events) {
            Tank& tank = state.tanks[event.tankId];

            switch(event.type) {
                case EventType::FORWARD:
                    tank.pos += tank.wheelDirection * tank.speed;
                    tank.pos = glm::clamp(tank.pos, glm::vec2(0.0f, 0.0f), mapSize - tank.size);
                    break;
                case EventType::BACKWARD:
                    tank.pos -= tank.wheelDirection * tank.speed;
                    tank.pos = glm::clamp(tank.pos, glm::vec2(0.0f, 0.0f), mapSize - tank.size);
                    break;
                case EventType::SHOOT:
                    if(tank.bullets.size() >= maxBullets) { break; }
                    tank.bullets.push_back(Bullet(Bullet::generateBulletID(), tank.pos + tank.gunDirection * 2.f, tank.gunDirection));
                    break;
                case EventType::ROTATE_WHEELS_CLOCKWISE:
                    tank.wheelDirection = glm::rotate(tank.wheelDirection, -tankRotationSpeed);

                    // todo: remove this when gun is not tied to wheels
                    tank.gunDirection = tank.wheelDirection;
                    break;
                case EventType::ROTATE_WHEELS_CCLOCKWISE:
                    tank.wheelDirection = glm::rotate(tank.wheelDirection, tankRotationSpeed);

                    // todo: remove this when gun is not tied to wheels
                    tank.gunDirection = tank.wheelDirection;
                    break;
                case EventType::GUN_DIRECTION:
                    tank.gunDirection = glm::vec2(cos(event.value), sin(event.value));
                    break;
            }
        }

        for(Tank& tank : state.tanks) {
            for(int i = 0; i < tank.bullets.size(); i++) {
                Bullet& bullet = tank.bullets[i];
                bullet.pos += bullet.direction * bullet.speed;
                bullet.pos = glm::clamp(bullet.pos, glm::vec2(0.0f, 0.0f), mapSize);

                if(bulletCollidesMapVertical(bullet)) {
                    bullet.direction.y *= -1;
                    bullet.richochets++;
                }

                if(bulletCollidesMapHorizontal(bullet)) {
                    bullet.direction.x *= -1;
                    bullet.richochets++;
                }

                if(bullet.richochets >= bullet.maxRichochets) {
                    tank.bullets.erase(tank.bullets.begin() + i--);
                    continue;
                }

                for(Tank& otherTank : state.tanks) {
                    if(intersects(otherTank, bullet)) {
                        tank.bullets.erase(tank.bullets.begin() + i--);
                        otherTank.isAlive = false;
                        break;
                    }
                }
            }
        }

        return state;
    }
}