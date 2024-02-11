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

    float len = 0.5f;
    std::vector<float> vertices = {
        // front face
        -len, -len, len, -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        len, -len, len, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        -len, len, len, -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        len, len, len, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        // right face
        len, -len, len, 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        len, len, len, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        len, -len, -len, 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        len, len, -len, 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

        // back face
        -len, -len, -len, -0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.5f,
        len, -len, -len, 0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.5f,
        -len, len, -len, -0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.5f,
        len, len, -len, 0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.5f,

        // left face
        -len, -len, len, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -len, -len, -len, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -len, len, len, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -len, len, -len, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,

        // top face
        -len, len, len, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        len, len, len, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        -len, len, -len, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        len, len, -len, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,

        // bottom face
        -len, -len, len, -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        len, -len, len, 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -len, -len, -len, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        len, -len, -len, 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f
    };

    std::vector<unsigned int> indices = {
        // front face
        0, 1, 2,
        1, 2, 3,

        // right face
        4, 5, 6,
        5, 6, 7,

        // back face
        8, 9, 10,
        9, 10, 11,

        // left face
        12, 13, 14,
        13, 14, 15,

        // top face
        16, 17, 18,
        17, 18, 19,

        // bottom face
        20, 21, 22,
        21, 22, 23
    };

    Object cube = Object(vertices, indices, { VertexFeature::Position, VertexFeature::Normal, VertexFeature::Color });
    Object tank = Object("../assets/Sketch_Tank/tank_1.obj", { VertexFeature::Position, VertexFeature::Normal, VertexFeature::Color });
    tank.scale(0.05f);

    std::vector<struct TextureConfiguration> environmentTextureConfigurations = {
        TextureConfiguration("background_dark", 5, 276, 1024, 512)
        // TextureConfiguration("background_light", 5, 1029, 1024, 512)
    };

    TextureAtlas textureAtlas("../assets/wii/tanks_environment_texture_atlas.png", environmentTextureConfigurations);
    Texture environmentTexture = textureAtlas.getTexture("background_dark");

    std::vector<float> floor_vertices = {
        0.0f, 0.f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 5.f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        10.f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        10.f, 5.f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    std::vector<unsigned int> floor_indices = {
        0, 1, 2,
        1, 2, 3
    };

    Object floor = Object(floor_vertices, floor_indices, { VertexFeature::Position, VertexFeature::Normal, VertexFeature::UV });

    floor.setTexture(environmentTexture);

    Shader defaultVertexShader = Shader("../shader/default/default", ShaderType::VERTEX);
    Shader defaultFragmentShader = Shader("../shader/default/default", ShaderType::FRAGMENT);
    ShaderProgram defaultShaderProgram = ShaderProgram({ defaultVertexShader, defaultFragmentShader });
    defaultShaderProgram.use();

    Shader textureVertexShader = Shader("../shader/texture/texture", ShaderType::VERTEX);
    Shader textureFragmentShader = Shader("../shader/texture/texture", ShaderType::FRAGMENT);
    ShaderProgram textureShaderProgram = ShaderProgram({ textureVertexShader, textureFragmentShader });

    cube.setShader(defaultShaderProgram);
    tank.setShader(defaultShaderProgram);
    floor.setShader(textureShaderProgram);

    glEnable(GL_DEPTH_TEST);

    float u_time = 0.0f;
    auto start = std::chrono::steady_clock::now();

    Camera camera(::WINDOW_WIDTH, ::WINDOW_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
    camera.addShader(defaultShaderProgram);
    camera.addShader(textureShaderProgram);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        u_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0f;

        defaultShaderProgram.setFloat("u_time", u_time);
        textureShaderProgram.setFloat("u_time", u_time);

        camera.getKeyInput(window);
        
        // cube.render();
        tank.render();
        camera.updateCameraMatrix(45.f, 0.1f, 100.0f, "camMatrix");

        floor.render();
        camera.updateCameraMatrix(45.f, 0.1f, 100.0f, "camMatrix");

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    defaultShaderProgram.free();
    defaultVertexShader.free();
    defaultFragmentShader.free();

    cube.free();
    tank.free();
    floor.free();

    textureAtlas.free();

    glfwTerminate();

    return 0;
}