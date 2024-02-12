#include"Camera.hpp"

Camera::Camera(int width, int height, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane): 
    width(width),
    height(height),
    Position(position),
    FOVdeg(FOVdeg),
    nearPlane(nearPlane),
    farPlane(farPlane)
{}

glm::mat4 Camera::getMatrix() const {
    glm::mat4 view = glm::lookAt(Position, Position + Orientation, Up);
    glm::mat4 projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
    return projection * view;
}

void Camera::getKeyInput(GLFWwindow* window, float timestep) {
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            Position += currentSpeed * timestep * Orientation;
        }

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            Position += currentSpeed * timestep * -glm::normalize(glm::cross(Orientation, Up));
        }

        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            Position += currentSpeed * timestep * -Orientation;
        }

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            Position += currentSpeed * timestep * glm::normalize(glm::cross(Orientation, Up));
        }

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            Position += currentSpeed * timestep * Up;
        }

        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            Position += currentSpeed * timestep * -Up;
        }

        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            currentSpeed = fastMoveSpeed;
        } else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
            currentSpeed = slowMoveSpeed;
        }

		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if(firstClick) {
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = timestep * sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = timestep * sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if(abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	} else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}