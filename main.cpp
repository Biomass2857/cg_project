#include <GL/glew.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef __linux__
#include <GL/gl.h>
#elif _WIN32
#include <GL/gl.h>  // You might need to adjust this depending on your setup
#endif

int main() {
    if(!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Project", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // GLEW initialization
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}