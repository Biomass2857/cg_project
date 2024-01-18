cmake_minimum_required(VERSION 3.27)
project(OpenGLProject)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW 2.2 REQUIRED)

add_executable(OpenGLProject main.cpp)

target_link_libraries(OpenGLProject PRIVATE OpenGL::GL glfw)
