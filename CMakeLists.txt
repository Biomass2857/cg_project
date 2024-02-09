cmake_minimum_required(VERSION 3.22)
project(OpenGLProject)

find_package(OpenGL 4.6 REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW 2.2 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.hpp" "src/*.cpp")

add_executable(OpenGLProject "main.cpp" ${SOURCES})

target_include_directories(OpenGLProject PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_link_libraries(OpenGLProject PRIVATE OpenGL::GL glfw GLEW::GLEW)
