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

#include "src/Shader.hpp"
#include "src/ShaderProgram.hpp"

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

    float len = 0.5f;
    float vertices[] = {
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

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // read positions from buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // read colors from buffer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    // enable vertex attribute number 1 (we use as color)
    glEnableVertexAttribArray(1);

    // normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned short indices[] = {
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

    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    Shader vertexShader = Shader("../shader/default/default", VERTEX);
    Shader fragmentShader = Shader("../shader/default/default", FRAGMENT);
    std::vector<Shader> shaders;
    shaders.push_back(vertexShader);
    shaders.push_back(fragmentShader);
    ShaderProgram shaderProgram = ShaderProgram(shaders);
    shaderProgram.use();

    glEnable(GL_DEPTH_TEST);

    float start_time = ((float) clock()) / CLOCKS_PER_SEC;
    float u_time = 0.0f;

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        u_time = ((float) clock()) / CLOCKS_PER_SEC - start_time;

        shaderProgram.setFloat("u_time", u_time);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned short), GL_UNSIGNED_SHORT, nullptr);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &IBO);
    glDeleteBuffers(1, &VBO);

    shaderProgram.free();
    for(Shader shader : shaders) {
        shader.free();
    }

    glfwTerminate();

    return 0;
}