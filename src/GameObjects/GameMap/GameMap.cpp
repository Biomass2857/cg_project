#include "GameMap.hpp"

GameMap::GameMap(TextureAtlas &atlas, ShaderProgram& colorShader, ShaderProgram& textureShader):
    floor({
            0.0f, 0.f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 5.f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            10.f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            10.f, 5.f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
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

    // Texture boxTexture = this->atlas->getTexture("box_texture_light");

    Tank tank;
    Object shell = Object("../assets/Shell/shell.obj", { VertexFeature::Position, VertexFeature::Normal, VertexFeature::Color });
    shell.scale(0.05f);

    floor.setShader(textureShader);
    tank.setShader(colorShader);
    shell.setShader(colorShader);

    tanks.push_back(tank);
    shells.push_back(shell);
}

void GameMap::render(Camera& camera) {
    floor.render(camera);

    for(Object& box : boxes) {
        box.render(camera);
    }

    for(Tank& tank : tanks) {
        tank.render(camera);
    }

    for(Object& shell : shells) {
        shell.render(camera);
    }
}

void GameMap::free() {
    floor.free();
}

void GameMap::generateWall() {
    Texture boxTexture = this->atlas->getTexture("box_texture_light");

    unsigned int blockCountX = mapWidth / boxWidth;
    unsigned int blockCountY = mapHeight / boxWidth;

    for(unsigned int i = 0; i < blockCountX; i++) {
        for(unsigned int j = 0; j < blockCountY; j++) {
            if(i == 0 || i == blockCountX - 1 || j == 0 || j == blockCountY - 1) {
                Box box(0.5f, boxTexture);
                box.translate(glm::vec3(i * boxWidth, 0.0f, j * boxWidth));
                boxes.push_back(box);
            }
        }
    }
}