#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#elif __linux__
#include <GL/gl.h>
#elif _WIN32
#include <GL/gl.h>
#endif

std::string readFileToString(const std::string& filename) {
    std::ifstream file;
    std::stringstream ss;

    file.open(filename);
    if(file.is_open()) {
        ss << file.rdbuf();
        file.close();
    } else {
        std::cerr <<"Unable to open file: "<< filename << std::endl;
    }
    
    return ss.str();
}

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

    float len = 0.5f;
    float vertices[] = {
        -len, -len, len, 1.0f, 0.0f, 0.0f,
        len, -len, len, 1.0f, 0.0f, 0.0f,
        -len, len, len, 1.0f, 0.0f, 0.0f,
        -len, len, len, 0.0f, 1.0f, 0.0f,
        len, len, len, 0.0f, 1.0f, 0.0f,
        len, -len, len, 0.0f, 1.0f, 0.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // read positions from buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // read colors from buffer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // enable vertex attribute number 1 (we use as color)
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::string vertexShaderSourceString = readFileToString("../shader/shader.vert");
    std::string fragmentShaderSourceString = readFileToString("../shader/shader.frag");

    const char* const vertexShaderSource = vertexShaderSourceString.c_str();
    const char* const fragmentShaderSource = fragmentShaderSourceString.c_str();

    unsigned int vertexShader, fragmentShader, shaderProgram;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float theta = 0.5f;
    glUseProgram(shaderProgram);
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        theta += 0.01f;
        float rotMatrix[16] = {
            cos(theta), -sin(theta), 0.0f, 0.0f,
            sin(theta), cos(theta), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transformationMatrix"), 1, GL_FALSE, rotMatrix);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}