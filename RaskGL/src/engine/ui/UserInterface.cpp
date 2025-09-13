#include "UserInterface.h"

UserInterface::UserInterface(GLFWwindow* wdw, float main_scale){
	UserInterface::window = wdw;
	/* Initialize ImGui */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsLight();

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	UserInterface::io = &ImGui::GetIO(); (void)io;
	UserInterface::getIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void UserInterface::InitUI(float *fov) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Settings");
	if (ImGui::Button("Rfv")) *fov = 103.0f;
	ImGui::SameLine();
	ImGui::SliderFloat("FOV", fov, 10.0f, 130.0f);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / UserInterface::getIO().Framerate, UserInterface::getIO().Framerate);
	ImGui::End();
}

void UserInterface::RenderUI(){
	ImGui::Render();

	int display_w;
	int display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}