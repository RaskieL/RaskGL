
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

// Pipeline
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

// Renderer
#include "GameObject.h"
#include "Transformation.h"
#include "Camera.h"
#include "CameraController.h"
#include "Chunk.h"

// UI
#include "UserInterface.h"

float computeDeltaTime() {
	static float lastFrame = 0.0f;
	float currentFrame = (float)glfwGetTime();
	float deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	return deltaTime;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int WINDOW_WIDTH = 1280;
    int WINDOW_HEIGHT = 720;

    /* Create a windowed mode window and its OpenGL context */
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

    window = glfwCreateWindow(WINDOW_WIDTH * (int)main_scale, WINDOW_HEIGHT * (int)main_scale, "RaskGL", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);


    /* Initialize GLEW */
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW INITIALIZATION ERROR" << std::endl;
        return -1;
    }

    UserInterface UI(window, main_scale);

    /* Prints the current OpenGL version in the console */
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float s = 1.0f;
        /* Triangle vertex positions */
		std::vector<Vertex> vertices = {
			// +Z (front)
			{{-s,-s, +s}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{+s,-s, +s}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{+s,+s, +s}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
			{{-s,+s, +s}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},

			// +X (right)
			{{+s,-s, +s}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
			{{+s,-s, -s}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
			{{+s,+s, -s}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
			{{+s,+s, +s}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},

			// -Z (back)
			{{+s,-s,-s}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{-s,-s,-s}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{-s,+s,-s}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
			{{+s,+s,-s}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},

			// -X (left)
			{{-s,-s,-s}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{-s,-s, +s}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{-s,+s, +s}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
			{{-s,+s,-s}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},

			// +Y (top)
			{{-s,+s, +s}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{+s,+s, +s}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{+s,+s, -s}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
			{{-s,+s, -s}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},

			// -Y (bottom)
			{{-s,-s,-s}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
			{{+s,-s,-s}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
			{{+s,-s, +s}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
			{{-s,-s, +s}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
		};


        /* Triangle vertex indices to draw a square */
		std::vector<unsigned int> indices = {
	        0,1,2, 2,3,0,       // front
	        4,5,6, 6,7,4,       // right
	        8,9,10, 10,11,8,    // back
	        12,13,14, 14,15,12, // left
	        16,17,18, 18,19,16, // top
	        20,21,22, 22,23,20  // bottom
		}; 

        Renderer renderer;
        Camera camera(glm::vec3(0, 0, 50), glm::vec3(0.0f), glm::vec3(0, 1, 0), 103.0f, (float)WINDOW_WIDTH * main_scale / (float)WINDOW_HEIGHT * main_scale, 0.01f, 100.0f);
		CameraController controller(&camera, window);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetWindowUserPointer(window, &controller);
		glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
			auto controller = static_cast<CameraController*>(glfwGetWindowUserPointer(win));
			controller->MouseCallback(x, y);
		});
        renderer.Init();

        auto cubeMesh = std::make_shared<Mesh>(vertices, indices);
        auto shader =  std::make_shared<Shader>("res/shaders/Basic.glsl");
        auto texture = std::make_shared<Texture>("res/textures/seal_arrow.png");
        auto material = std::make_shared<Material>(shader, texture);

		std::vector<glm::mat4> cubeTransforms;
        const int GRID_SIZE = 256;

		for (int z = 0; z < GRID_SIZE; z++) {
			for (int x = 0; x < GRID_SIZE; x++) {
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x * 2.0f * s - GRID_SIZE, 0.0f, z * 2.0f * s - GRID_SIZE));
				cubeTransforms.push_back(model);
			}
		}

        Chunk cubeChunk(cubeMesh, material, cubeTransforms, GRID_SIZE, glm::vec3(0.0f));

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
			float deltaTime = computeDeltaTime();
            /* Clear render space */
            renderer.Clear();
            /* Poll for and process events */
            glfwPollEvents();

            controller.ProcessInput(deltaTime);

			glm::mat4 vp = camera.getProjectionMatrix() * camera.getViewMatrix();
			Frustum frustum = ExtractFrustum(vp);

            /* GL Rendering */
			for (int i = 0; i < cubeChunk.getTransforms().size(); i++) {
				cubeChunk.getTransforms()[i] = glm::translate(glm::mat4(1.0f),
					glm::vec3(cubeChunk.getTransforms()[i][3])); // keep original position or modify
			}
			cubeChunk.UpdateInstances();

			// Draw all cubes in one instanced draw
			cubeChunk.Draw(camera, frustum);
            

            /* ImGui Rendering */
            UI.InitUI(&camera.getFOV());
            UI.RenderUI();

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
