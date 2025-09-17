
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
#include "InstanceRenderer.h"
#include "Chunk.h"
#include "Block.h"

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
		float worldSize = 2.0f * CHUNK_SIZE * BLOCK_SCALE;
        /* Triangle vertex positions */
		std::vector<Vertex> vertices = {
			// +Z (front)
			{{-BLOCK_SCALE,-BLOCK_SCALE, +BLOCK_SCALE}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{+BLOCK_SCALE,-BLOCK_SCALE, +BLOCK_SCALE}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{+BLOCK_SCALE,+BLOCK_SCALE, +BLOCK_SCALE}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
			{{-BLOCK_SCALE,+BLOCK_SCALE, +BLOCK_SCALE}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},

			// +X (right)
			{{+BLOCK_SCALE,-BLOCK_SCALE, +BLOCK_SCALE}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
			{{+BLOCK_SCALE,-BLOCK_SCALE, -BLOCK_SCALE}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
			{{+BLOCK_SCALE,+BLOCK_SCALE, -BLOCK_SCALE}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
			{{+BLOCK_SCALE,+BLOCK_SCALE, +BLOCK_SCALE}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},

			// -Z (back)
			{{+BLOCK_SCALE,-BLOCK_SCALE,-BLOCK_SCALE}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{-BLOCK_SCALE,-BLOCK_SCALE,-BLOCK_SCALE}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{-BLOCK_SCALE,+BLOCK_SCALE,-BLOCK_SCALE}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
			{{+BLOCK_SCALE,+BLOCK_SCALE,-BLOCK_SCALE}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},

			// -X (left)
			{{-BLOCK_SCALE,-BLOCK_SCALE,-BLOCK_SCALE}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{-BLOCK_SCALE,-BLOCK_SCALE, +BLOCK_SCALE}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{-BLOCK_SCALE,+BLOCK_SCALE, +BLOCK_SCALE}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
			{{-BLOCK_SCALE,+BLOCK_SCALE,-BLOCK_SCALE}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},

			// +Y (top)
			{{-BLOCK_SCALE,+BLOCK_SCALE, +BLOCK_SCALE}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{+BLOCK_SCALE,+BLOCK_SCALE, +BLOCK_SCALE}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{+BLOCK_SCALE,+BLOCK_SCALE, -BLOCK_SCALE}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
			{{-BLOCK_SCALE,+BLOCK_SCALE, -BLOCK_SCALE}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},

			// -Y (bottom)
			{{-BLOCK_SCALE,-BLOCK_SCALE,-BLOCK_SCALE}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
			{{+BLOCK_SCALE,-BLOCK_SCALE,-BLOCK_SCALE}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
			{{+BLOCK_SCALE,-BLOCK_SCALE, +BLOCK_SCALE}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
			{{-BLOCK_SCALE,-BLOCK_SCALE, +BLOCK_SCALE}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
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

		auto cubeMesh = std::make_shared<Mesh>(vertices, indices);
		auto shader = std::make_shared<Shader>("res/shaders/Basic.glsl");
		auto texture = std::make_shared<Texture>("res/textures/seal_arrow.png");
		auto material = std::make_shared<Material>(shader, texture);


		std::vector<Chunk> chunks;
		for (int cz = 0; cz < 2; cz++) {
			for (int cy = 0; cy < 2; cy++) {
				for (int cx = 0; cx < 2; cx++) {
					std::vector<Block> blocks;
					for (int z = 0; z < CHUNK_SIZE; z++) {
						for (int y = 0; y < CHUNK_SIZE; y++) {
							for (int x = 0; x < CHUNK_SIZE; x++) {
								glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x * 2.0f * BLOCK_SCALE - CHUNK_SIZE * cx * 2, y * 2.0f * BLOCK_SCALE - CHUNK_SIZE * cy * 2, z * 2.0f * BLOCK_SCALE - CHUNK_SIZE * cz * 2));
								blocks.push_back({ 1, glm::vec3(x, 0, z), model });
							}
						}
					}
					chunks.push_back({ glm::vec3(cx, cy, cz), blocks });
				}
			}
		}
		

		InstanceRenderer renderer(cubeMesh, material);
        Camera camera(glm::vec3(0, 0, 50), glm::vec3(0.0f), glm::vec3(0, 1, 0), 103.0f, (float)WINDOW_WIDTH * main_scale / (float)WINDOW_HEIGHT * main_scale, 0.01f, 100.0f);
		CameraController controller(&camera, window);

        GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED));
		GLCall(glfwSetWindowUserPointer(window, &controller));
		GLCall(glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
			auto controller = static_cast<CameraController*>(glfwGetWindowUserPointer(win));
			controller->MouseCallback(x, y);
		}));
        renderer.Init();

        

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
			std::vector<glm::mat4> visibleTransforms; // A OPTIMISER
			for(auto& chunk : chunks) {
				glm::vec3 center = chunk.position + glm::vec3(worldSize / 4.0f, worldSize / 4.0f, worldSize / 4.0f);
				float radius = (glm::sqrt(3.0f) * (worldSize / 4.0f));


				if (!SphereInFrustum(frustum, center, radius)) {
					std::cout << "Culled chunk at x:" << chunk.position.x << " y:" << chunk.position.y << " z:" << chunk.position.z << std::endl;
					continue;
				}
				for (auto& block : chunk.blocks) {
					visibleTransforms.push_back(block.transform);
				}
			}

			renderer.setTransforms(visibleTransforms);
			renderer.UpdateInstances();
			renderer.Draw(camera);
            

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
