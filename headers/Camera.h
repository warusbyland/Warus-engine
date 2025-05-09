#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"ManageShader.h"

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

class Camera 
{
public:
    // Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	float lastX, lastY; // Mouse Pos start

	// Adjust the speed of the camera and it's sensitivity when looking around
	float defaultSpeed = 20.0f;
	float sensitivity = 100.0f;

	// Makes sure the next time the camera looks around it doesn't jump
	bool firstClick = true;

    Camera(glm::vec3 aPosition) {
        Position = aPosition;
    }

    void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform) {
	    glm::mat4 view = glm::mat4(1.0f);
	    glm::mat4 projection = glm::mat4(1.0f);

	    view = glm::lookAt(Position, Position + Orientation, Up);
        projection = glm::perspective(glm::radians(FOVdeg), (float)SCR_WIDTH / SCR_HEIGHT, nearPlane, farPlane);

        // Export matrix
        shader.setMat4(uniform, projection * view);
    }

	void processInput(GLFWwindow *window, float deltaTime)
	{
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

		float cameraSpeed = defaultSpeed * deltaTime;
    	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    	    Position += cameraSpeed * Orientation;
    	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			Position -= cameraSpeed * Orientation;
    	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			Position -= glm::normalize(glm::cross(Orientation, Up)) * cameraSpeed;
    	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			Position += glm::normalize(glm::cross(Orientation, Up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			Position += cameraSpeed * Up;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			Position -= cameraSpeed * Up;

		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) 
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Handles mouse inputs
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); //hides cursor
			// Prevents camera from jumping on the first click
			if (firstClick) {
				glfwSetCursorPos(window, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));
				firstClick = false;
			}
			double mouseX;
			double mouseY;
			// Fetches the coordinates of the cursor
			glfwGetCursorPos(window, &mouseX, &mouseY);

			// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
			// and then "transforms" them into degrees 
			float rotX = sensitivity * (float)(mouseY - (SCR_HEIGHT / 2)) / SCR_HEIGHT;
			float rotY = sensitivity * (float)(mouseX - (SCR_WIDTH / 2)) / SCR_WIDTH;

			// Calculates upcoming vertical change in the Orientation
			glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
			// Decide wether the calculated pitch is legal
			if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
				Orientation = newOrientation;
			}
			// Rotates left and right
			Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

			// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
			glfwSetCursorPos(window, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));
		} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstClick = true;
		}
	}
};