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
	UserInterface(GLFWwindow* window, float main_scale);

	void InitUI(float *fov);
	void RenderUI();

	ImGuiIO& getIO() { return *io; };
};