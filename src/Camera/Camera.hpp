#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../ShaderProgram/ShaderProgram.hpp"

class Camera {
    public:
        Camera(int width, int height, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane);

        // Stores the main vectors of the camera
        glm::vec3 Position;
        glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

        // Prevents the camera from jumping around when first clicking left click
        bool firstClick = true;

        // Stores the width and height of the window
        int width;
        int height;

        // Adjust the speed of the camera and it's sensitivity when looking around
        const float slowMoveSpeed = 10.f;
        const float fastMoveSpeed = 40.f;
        float currentSpeed = 10.f;
        const float sensitivity = 10000.0f;

        float FOVdeg, nearPlane, farPlane;

        void getKeyInput(GLFWwindow* window, float timestep);
        
        glm::mat4 getMatrix() const;
};

#endif