#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#elif __linux__
#include <GL/gl.h>
#elif _WIN32
#include <GL/gl.h>
#endif

#include <vector>
#include <unordered_map>

#include "../../Object/Object.hpp"
#include "../Box/Box.hpp"
#include "../Tank/Tank.hpp"
#include "../../Texture/TextureAtlas.hpp"
#include "../../ShaderProgram/ShaderProgram.hpp"
#include "../../Texture/Texture.hpp"
#include "../../Camera/Camera.hpp"
#include "../Shell/Shell.hpp"
#include "../../Logic/Game.hpp"
#include "../../Logic/GameLoop/GameLoop.hpp"

class GameMap {
    public:
        GameMap(TextureAtlas &atlas, ShaderProgram& colorShader, ShaderProgram& textureShader);

        void getInput(GLFWwindow* window);

        void render(Camera& camera);
        void free();
    private:
        void preprareShowState(Game::State state);
        void generateWall();
        std::vector<struct Game::Tank> generateTanks();

        glm::vec2 size = glm::vec2(20.0f, 10.0f);
        float boxWidth = 1.0f;

        ShaderProgram* colorShader;
        ShaderProgram* textureShader;
        TextureAtlas* atlas;
        Object floor;
        std::vector<Tank> tanks;
        std::unordered_map<int, Shell> shells;
        std::vector<Box> boxes;

        GameLoop gameLoop;
};

#endif