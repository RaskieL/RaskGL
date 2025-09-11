
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
#include "Transformation.h"

// UI
#include "UserInterface.h"

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

    int WINDOW_WIDTH = 1280;
    int WINDOW_HEIGHT = 720;

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


    /* Initialize GLEW */
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW INITIALIZATION ERROR" << std::endl;
        return -1;
    }

    UserInterface UI(window, main_scale);

    /* Prints the current OpenGL version in the console */
    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float s = 5.0f;
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
        VertexBuffer vertexBuffer(vertices.data(), (unsigned int)vertices.size() * (unsigned int)sizeof(Vertex));

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
        Transformation transform;

        glm::vec3 translation(0, 0, 0);
        glm::vec3 scale(1.0f);
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

            transform.setTranslation(translation);
            transform.setRotation(rotation);
            transform.setScale(scale);

            glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * transform.getModelMatrix(); // Multiplication dans l'odre PVM dans OpenGL

            /* GL Rendering */
            shader.Bind();
			shader.SetUniformMat4f("u_MVP", mvpMatrix);
			renderer.Draw(vertexArray, indexBuffer, shader);

            /* ImGui Rendering */
            UI.InitUI(&translation, &rotation, &scale);
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

