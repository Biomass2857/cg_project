#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include <vector>
#include "../../Object/Object.hpp"
#include "../Box/Box.hpp"
#include "../Tank/Tank.hpp"
#include "../../Texture/TextureAtlas.hpp"
#include "../../ShaderProgram/ShaderProgram.hpp"
#include "../../Texture/Texture.hpp"
#include "../../Camera/Camera.hpp"

class GameMap {
    public:
        GameMap(TextureAtlas &atlas, ShaderProgram& colorShader, ShaderProgram& textureShader);

        void render(Camera& camera);
        void free();
    private:
        void generateWall();

        float mapWidth = 20.f;
        float mapHeight = 10.f;
        float boxWidth = 1.0f;

        ShaderProgram* colorShader;
        ShaderProgram* textureShader;
        TextureAtlas* atlas;
        Object floor;
        std::vector<Tank> tanks;
        std::vector<Object> shells;
        std::vector<Box> boxes;
};

#endif