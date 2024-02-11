#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#elif __linux__
#include <GL/gl.h>
#elif _WIN32
#include <GL/gl.h>
#endif

#include "Shader/Shader.hpp"
#include "ShaderProgram/ShaderProgram.hpp"
#include "Object/Object.hpp"
#include "Camera/Camera.hpp"
#include "Texture/TextureAtlas.hpp"
#include "Texture/Texture.hpp"
#include "GameObjects/Box/Box.hpp"
#include "GameObjects/GameMap/GameMap.hpp"

const unsigned int WINDOW_WIDTH = 600;
const unsigned int WINDOW_HEIGHT = 600;

void errorCallback(int iError, const char* pcDescription) {
    std::cerr << "GLFW Error: " + std::to_string(iError) + " " + std::string(pcDescription) << std::endl;
}

int main() {
    if(!glfwInit()) {
        return -1;
    }

    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    GLFWwindow* window = glfwCreateWindow(::WINDOW_WIDTH, ::WINDOW_HEIGHT, "OpenGL Project", nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK) {
        glfwTerminate();
        return -1;
    }

    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL version supported by this platform: " << version << std::endl;

    std::vector<struct TextureConfiguration> environmentTextureConfigurations = {
        TextureConfiguration("floor_dark", 5, 276, 1024, 512),
        TextureConfiguration("floor_light", 5, 276 + 1024 + 5, 1024, 512),
        TextureConfiguration("box_texture_light", 902, 207, 64, 64)
    };

    TextureAtlas textureAtlas("../assets/wii/tanks_environment_texture_atlas.png", environmentTextureConfigurations);

    Shader defaultVertexShader = Shader("../shader/default/default", ShaderType::VERTEX);
    Shader defaultFragmentShader = Shader("../shader/default/default", ShaderType::FRAGMENT);
    ShaderProgram defaultShaderProgram = ShaderProgram({ defaultVertexShader, defaultFragmentShader });
    defaultShaderProgram.use();

    Shader textureVertexShader = Shader("../shader/texture/texture", ShaderType::VERTEX);
    Shader textureFragmentShader = Shader("../shader/texture/texture", ShaderType::FRAGMENT);
    ShaderProgram textureShaderProgram = ShaderProgram({ textureVertexShader, textureFragmentShader });

    glEnable(GL_DEPTH_TEST);

    float u_time = 0.0f;
    auto start = std::chrono::steady_clock::now();

    Camera camera(::WINDOW_WIDTH, ::WINDOW_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f), 45.f, 0.1f, 100.0f);

    GameMap gameMap(textureAtlas, defaultShaderProgram, textureShaderProgram);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float newTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0f;
        float deltaTime = newTime - u_time;
        u_time = newTime;

        defaultShaderProgram.setFloat("u_time", u_time);
        textureShaderProgram.setFloat("u_time", u_time);

        camera.getKeyInput(window, deltaTime);

        gameMap.render(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    defaultShaderProgram.free();
    defaultVertexShader.free();
    defaultFragmentShader.free();

    gameMap.free();

    textureAtlas.free();

    glfwTerminate();

    return 0;
}