#include "GameMap.hpp"

GameMap::GameMap(TextureAtlas &atlas, ShaderProgram& colorShader, ShaderProgram& textureShader):
    floor({
            0.0f, 0.f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 10.f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            20.f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            20.f, 10.f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
        }, {
            0, 1, 2,
            1, 2, 3
        }, { VertexFeature::Position, VertexFeature::Normal, VertexFeature::UV }
    ),
    atlas(&atlas),
    colorShader(&colorShader),
    textureShader(&textureShader)
{
    Texture environmentTexture = this->atlas->getTexture("floor_dark");
    floor.setTexture(environmentTexture);

    floor.setShader(colorShader);

    generateWall();

    std::vector<Game::Tank> gameTanks = generateTanks();
    Game::State initialState(Game::State { gameTanks });
    gameLoop.start(initialState);
}

void GameMap::render(Camera& camera) {
    preprareShowState(gameLoop.getGameSnapshot());

    floor.render(camera);

    for(Object& box : boxes) {
        box.render(camera);
    }

    for(Tank& tank : tanks) {
        tank.render(camera);
    }

    for(auto& shell : shells) {
        shell.second.render(camera);
    }
}

void GameMap::free() {
    floor.free();
}

void GameMap::generateWall() {
    Texture boxTexture = this->atlas->getTexture("box_texture_light");

    unsigned int blockCountX = static_cast<unsigned int>(size.x / boxWidth);
    unsigned int blockCountY = static_cast<unsigned int>(size.y / boxWidth);

    for(unsigned int i = 0; i < blockCountX; i++) {
        for(unsigned int j = 0; j < blockCountY; j++) {
            if(i == 0 || i == blockCountX - 1 || j == 0 || j == blockCountY - 1) {
                Box box(boxWidth / 2, boxTexture);
                box.translate(glm::vec3(i * boxWidth, j * boxWidth, 0.0));
                box.scale(0.9f);
                box.setShader(*textureShader);
                boxes.push_back(box);
            }
        }
    }
}

void GameMap::preprareShowState(Game::State state) {
    for(int i = 0; i < state.tanks.size(); i++) {
        Game::Tank& stateTank = state.tanks[i];

        glm::vec2 relativePos = state.tanks[i].pos / Game::mapSize;
        tanks[i].setTranslation(glm::vec3(relativePos * size, 0.0f));

        float rotationAngle = glm::atan(stateTank.wheelDirection.y, stateTank.wheelDirection.x) - 3.0 * glm::pi<float>() / 2.0f;
        tanks[i].setRotation(rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));

        for(Game::Bullet bullet : stateTank.bullets) {
            bool shellExists = shells.find(bullet.id) != shells.end();

            if(!shellExists) {
                Shell shell;
                shell.setRotation(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                shell.setShader(*colorShader);
                shells[bullet.id] = shell;
            }

            Shell& shell = shells[bullet.id];
            glm::vec2 relativePos = bullet.pos / Game::mapSize;
            shell.setTranslation(glm::vec3(relativePos * size, 0.0f));

            float rotationAngle = glm::atan(bullet.direction.y, bullet.direction.x) - 3.0 * glm::pi<float>() / 2.0f;
            shell.setRotation(rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        }
    }
}

std::vector<struct Game::Tank> GameMap::generateTanks() {
    std::vector<struct Game::Tank> gameTanks;

    for(int i = 0; i < 4; i++) {
        glm::vec2 relativePos = glm::vec2(0.25f + 0.5f * (i / 2), 0.25f + 0.5f * (i % 2));
        struct Game::Tank tank(i, Game::mapSize * relativePos);
        gameTanks.push_back(tank);

        Tank tankObject;
        tankObject.setShader(*colorShader);
        tankObject.scale(0.5f);
        tanks.push_back(tankObject);
    }

    return gameTanks;
}

void GameMap::getInput(GLFWwindow* window) {
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Game::Event event;
        event.tankId = 0;
        event.type = Game::EventType::FORWARD;
        gameLoop.accumulateEvents({ event });
	}

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Game::Event event;
        event.tankId = 0;
        event.type = Game::EventType::BACKWARD;
        gameLoop.accumulateEvents({ event });
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Game::Event event;
        event.tankId = 0;
        event.type = Game::EventType::ROTATE_WHEELS_CCLOCKWISE;
        gameLoop.accumulateEvents({ event });
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Game::Event event;
        event.tankId = 0;
        event.type = Game::EventType::ROTATE_WHEELS_CLOCKWISE;
        gameLoop.accumulateEvents({ event });
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Game::Event event;
        event.tankId = 0;
        event.type = Game::EventType::SHOOT;
        gameLoop.accumulateEvents({ event });
    }
}