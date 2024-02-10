#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>

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
#include "camera/Camera.hpp"

const unsigned int width = 800;
const unsigned int height = 800;

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
    
    GLFWwindow* window = glfwCreateWindow(600, 600, "OpenGL Project", nullptr, nullptr);
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


    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));



    float len = 0.5f;
    std::vector<float> vertices = {
        // front face
        -len, -len, len, 1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.5f,
        len, -len, len, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f,
        -len, len, len, 1.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f,
        len, len, len, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f,

        // right face
        len, -len, len, 0.0f, 1.0f, 0.0f, 0.5f, -0.5f, 0.5f,
        len, len, len, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f,
        len, -len, -len, 0.0f, 1.0f, 0.0f, 0.5f, -0.5f, -0.5f,
        len, len, -len, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f,

        // back face
        -len, -len, -len, 0.5f, 0.0f, 0.5f, -0.5f, -0.5f, -0.5f,
        len, -len, -len, 0.5f, 0.0f, 0.5f, 0.5f, -0.5f, -0.5f,
        -len, len, -len, 0.5f, 0.0f, 0.5f, -0.5f, 0.5f, -0.5f,
        len, len, -len, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, -0.5f,

        // left face
        -len, -len, len, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,
        -len, -len, -len, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f,
        -len, len, len, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.5f,
        -len, len, -len, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, -0.5f,

        // top face
        -len, len, len, 1.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f,
        len, len, len, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f,
        -len, len, -len, 1.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f,
        len, len, -len, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f,

        // bottom face
        -len, -len, len, 0.0f, 1.0f, 1.0f, -0.5f, -0.5f, 0.5f,
        len, -len, len, 0.0f, 1.0f, 1.0f, 0.5f, -0.5f, 0.5f,
        -len, -len, -len, 0.0f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f,
        len, -len, -len, 0.0f, 1.0f, 1.0f , 0.5f, -0.5f, -0.5f
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

    Object cube = Object(vertices, indices);

    Object tank = Object("../assets/Sketch_Tank/tank_1.obj");
    tank.scale(0.05f);

    Shader vertexShader = Shader("../shader/default/default", ShaderType::VERTEX);
    Shader fragmentShader = Shader("../shader/default/default", ShaderType::FRAGMENT);
    std::vector<Shader> shaders;
    shaders.push_back(vertexShader);
    shaders.push_back(fragmentShader);
    ShaderProgram shaderProgram = ShaderProgram(shaders);
    shaderProgram.use();

    tank.setShader(shaderProgram);

    glEnable(GL_DEPTH_TEST);

    float start_time = ((float) clock()) / CLOCKS_PER_SEC;
    float u_time = 0.0f;

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        u_time = ((float) clock()) / CLOCKS_PER_SEC - start_time;

        shaderProgram.setFloat("u_time", u_time);

        // Updates and exports the camera matrix to the Vertex Shader
        camera.Inputs(window);
        camera.Matrix(45.f, 0.1f, 100.0f, "camMatrix");
        
        // cube.render();
        tank.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shaderProgram.free();
    for(Shader shader : shaders) {
        shader.free();
    }

    cube.free();
    tank.free();

    glfwTerminate();

    return 0;
}