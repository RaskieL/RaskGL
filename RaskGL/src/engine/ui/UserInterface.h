#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glm/glm.hpp"

#include <GLFW/glfw3.h>

class UserInterface {
private:
	GLFWwindow* window;
	ImGuiIO* io;
public:
	UserInterface(GLFWwindow* window, int main_scale);

	void InitUI(glm::vec3 *translation, glm::vec3 *rotation, glm::vec3 *scale);
	void RenderUI();

	ImGuiIO& getIO() { return *io; };
};