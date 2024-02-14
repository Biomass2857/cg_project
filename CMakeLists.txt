cmake_minimum_required(VERSION 3.22)
project(OpenGLProject)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL 4.6 REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW 2.2 REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.hpp" "src/*.cpp")

add_executable(OpenGLProject "src/main.cpp" ${SOURCES})

target_include_directories(OpenGLProject PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
target_link_libraries(OpenGLProject PRIVATE OpenGL::GL glfw GLEW::GLEW glm::glm)