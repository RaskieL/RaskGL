#include "CameraController.h"

CameraController::CameraController(Camera* camera, GLFWwindow* window)
	: camera(camera), window(window) {
	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);
	lastX = (float)width / 2.0f;
	lastY = (float)height / 2.0f;
}

void CameraController::ProcessInput(float deltaTime) {
	// Camera controls
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->MoveZ(movementSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->MoveZ(-movementSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->MoveX(-movementSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->MoveX(movementSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->MoveY(movementSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera->MoveY(-movementSpeed * deltaTime);

	// Exit on ESC
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Speed boost with Shift
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT == GLFW_PRESS))
		setMovementSpeed(30.0f);
	else
		setMovementSpeed(10.0f);
}

void CameraController::MouseCallback(double xpos, double ypos) {
	if (firstMouse) {
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = ((float)xpos - lastX) * mouseSensitivity;
	float yoffset = (lastY - (float)ypos) * mouseSensitivity;
	lastX = (float)xpos;
	lastY = (float)ypos;

	camera->Rotate(xoffset, yoffset);
}
