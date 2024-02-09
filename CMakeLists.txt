cmake_minimum_required(VERSION 3.22)
project(OpenGLProject)

find_package(OpenGL 4.6 REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW 2.2 REQUIRED)

file(GLOB SOURCES
    "*.hpp"
    "*.cpp"
)

add_executable(OpenGLProject ${SOURCES})

target_include_directories(OpenGLProject PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_link_libraries(OpenGLProject PRIVATE OpenGL::GL glfw GLEW::GLEW)
