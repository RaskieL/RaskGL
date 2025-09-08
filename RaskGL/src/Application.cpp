#include "GLTools.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <windows.h>
#include <GL/gl.h>
#include <tchar.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec2 uv;
};

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

    /* Create a windowed mode window and its OpenGL context */
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

    window = glfwCreateWindow(WINDOW_WIDTH * (int)main_scale, WINDOW_HEIGHT * (int)main_scale, "RaskGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

/* Initialize GLEW */
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW INITIALIZATION ERROR" << std::endl;
        return -1;
    }

    /* Prints the current OpenGL version in the console */
    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float s = 50.0f;
        /* Triangle vertex positions */
		std::vector<Vertex> vertices = {
			// +Z (front)
			{{-s,-s, +s}, {0.0f, 0.0f}},
			{{+s,-s, +s}, {1.0f, 0.0f}},
			{{+s,+s, +s}, {1.0f, 1.0f}},
			{{-s,+s, +s}, {0.0f, 1.0f}},

			// +X (right)
			{{+s,-s, +s}, {0.0f, 0.0f}},
			{{+s,-s, -s}, {1.0f, 0.0f}},
			{{+s,+s, -s}, {1.0f, 1.0f}},
			{{+s,+s, +s}, {0.0f, 1.0f}},

			// -Z (back)
			{{+s,-s,-s}, {0.0f, 0.0f}},
			{{-s,-s,-s}, {1.0f, 0.0f}},
			{{-s,+s,-s}, {1.0f, 1.0f}},
			{{+s,+s,-s}, {0.0f, 1.0f}},

			// -X (left)
			{{-s,-s,-s}, {0.0f, 0.0f}},
			{{-s,-s, +s}, {1.0f, 0.0f}},
			{{-s,+s, +s}, {1.0f, 1.0f}},
			{{-s,+s,-s}, {0.0f, 1.0f}},

			// +Y (top)
			{{-s,+s, +s}, {0.0f, 0.0f}},
			{{+s,+s, +s}, {1.0f, 0.0f}},
			{{+s,+s, -s}, {1.0f, 1.0f}},
			{{-s,+s, -s}, {0.0f, 1.0f}},

			// -Y (bottom)
			{{-s,-s,-s}, {0.0f, 0.0f}},
			{{+s,-s,-s}, {1.0f, 0.0f}},
			{{+s,-s, +s}, {1.0f, 1.0f}},
			{{-s,-s, +s}, {0.0f, 1.0f}},
		};

        /* Triangle vertex indices to draw a square */

        /*
        std::vector<unsigned int> indices;
        std::vector<glm::vec3>::iterator it;
        std::vector<glm::vec3> posBuffer = {};
        for(int i = 0; i < positions.size(); i++) {
            if (i % 2 != 0) continue;
            it = std::find(posBuffer.begin(), posBuffer.end(), positions[i]);
            if(it != posBuffer.end()){
                indices.push_back(it - posBuffer.begin());
            }else {
                if (i != 0) indices.push_back(i - 1);
                else indices.push_back(i);
                posBuffer.push_back(positions[i]);
            }
        }*/

		std::vector<unsigned int> indices = {
	        0,1,2, 2,3,0,       // front
	        4,5,6, 6,7,4,       // right
	        8,9,10, 10,11,8,    // back
	        12,13,14, 14,15,12, // left
	        16,17,18, 18,19,16, // top
	        20,21,22, 22,23,20  // bottom
		}; 

        /* Texture Shader Blending */
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Enable Depth testing */
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(GL_LESS));

        /* Vertex Array Object & Vertex Buffer initialization */
        VertexArray vertexArray;
        VertexBuffer vertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        vertexArray.AddBuffer(vertexBuffer, layout);

        /* Index Buffer Initialization */
        IndexBuffer indexBuffer(indices.data(), (unsigned int)indices.size());



        /* Shader initialization */
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();


        Texture texture("res/textures/seal_arrow.png");
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);

        vertexArray.Unbind();
        shader.Unbind();
        vertexBuffer.Unbind();
        indexBuffer.Unbind();

        Renderer renderer;

        /* Initialize ImGui */
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsLight();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);
        style.FontScaleDpi = main_scale;

        ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
        ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
        ImGui_ImplOpenGL3_Init(glsl_version);

        /* Increment variables */
        float increment = 0.01f;

        glm::vec3 translation(0, 0, 0);
        float scale = 1.0f;
        glm::vec3 rotation(0, 0, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();
            /* Poll for and process events */
            glfwPollEvents();

            /* Rendering matrices */
            glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH * main_scale / (float)WINDOW_HEIGHT * main_scale, 0.1f, 100.0f); // projection
            glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0, 0, 50), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // position, looking at, head is up

            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale));
            glm::mat4 rotXMatrix = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0));
            glm::mat4 rotYMatrix = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0));
            glm::mat4 rotZMatrix = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));
            glm::mat4 rotationMatrix = rotXMatrix * rotYMatrix * rotZMatrix;
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);

            glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

            glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix; // Multiplication dans l'odre PVM dans OpenGL

            /* GL Rendering */
            shader.Bind();

            {
                shader.SetUniformMat4f("u_MVP", mvpMatrix);
                renderer.Draw(vertexArray, indexBuffer, shader);
            }

            /* ImGui Rendering */
            if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
            {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Example Imgui window
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Settings");

                if (ImGui::Button("Rtx")) translation.x = 0.0f;
                ImGui::SameLine();
                ImGui::SliderFloat("X Translate", &translation.x, -WINDOW_WIDTH, WINDOW_WIDTH);
                if (ImGui::Button("Rty")) translation.y = 0.0f;
                ImGui::SameLine();
                ImGui::SliderFloat("Y Translate", &translation.y, -WINDOW_HEIGHT, WINDOW_HEIGHT);
				if (ImGui::Button("Rtz")) translation.z = 0.0f;
				ImGui::SameLine();
				ImGui::SliderFloat("Z Translate", &translation.z, -100.0f, 100.0f);

                if (ImGui::Button("RoX")) rotation.x = 0.0f;
                ImGui::SameLine();
                ImGui::SliderFloat("X Rotation", &rotation.x, 0.0f, 20.0f);

				if (ImGui::Button("RoY")) rotation.y = 0.0f;
				ImGui::SameLine();
				ImGui::SliderFloat("Y Rotation", &rotation.y, 0.0f, 20.0f);

				if (ImGui::Button("RoZ")) rotation.z = 0.0f;
				ImGui::SameLine();
				ImGui::SliderFloat("Z Rotation", &rotation.z, 0.0f, 20.0f);

                if (ImGui::Button("Rzo")) scale = 1.0f;
                ImGui::SameLine();
                ImGui::SliderFloat("Scale", &scale, 0.1f, 10.0f);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            ImGui::Render();

            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


            /* Operations */

            rotation += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }

    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

