#pragma once

#include "Camera.h"
#include <GLFW/glfw3.h>

class CameraController
{
public:
	CameraController(Camera* camera, GLFWwindow* window);

	void ProcessInput(float deltaTime);
	void MouseCallback(double xpos, double ypos);

	void setMovementSpeed(float speed) { movementSpeed = speed; }

private:
	Camera* camera;
	GLFWwindow* window;
	float lastX = 0.0f;
	float lastY = 0.0f;
	bool firstMouse = true;
	float movementSpeed = 10.0f;
	float mouseSensitivity = 0.1f;
};

