#include "Window.h"

#include <iostream>
#include <vector>

namespace GameEngine {

	int g_indexProgram = 0;
	int g_indexVAO = 0;
	int g_totalVAO = 0;

	// Define callbacks
	void framebuffer_size_callback(GLFWwindow*, int width, int height);
	void process_input_callback(GLFWwindow*, int key, int scancode, int action, int mods);

	// Class starts here
	Window::Window(int width, int height, const char* title) : m_init(-1), m_width(width), m_height(height), m_pTitle(title)
	{
		m_pWindow = NULL;
	}

	Window::~Window()
	{
		Terminate();
	}

	void Window::Terminate()
	{
		glfwTerminate();
	}

	int Window::Init()
	{
		if (!glfwInit()) {
			std::cout << "Error initializing glfw" << std::endl;
			return -1;
		}

		m_init = 0;
		return m_init;
	}

	int Window::BuildWindow()
	{
		if (m_init == 0)
		{
			glfwWindowHint(GLFW_SAMPLES, 4);								// Antialiasing 4
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// OpenGL 3.3
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// macOS  compability
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// only new OpenGL

			m_pWindow = glfwCreateWindow(m_width, m_height, m_pTitle, NULL, NULL);
			if (m_pWindow == NULL) {
				std::cout << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
				return -1;
			}

			glfwMakeContextCurrent(m_pWindow);

			SetCallbacks();
			return 0;
		}
		else
		{
			std::cout << "Cannot BuildWindow without Initialize" << std::endl;
			return -1;
		}
	}

	void Window::SetCallbacks()
	{
		glfwSetFramebufferSizeCallback(m_pWindow, framebuffer_size_callback);
		glfwSetKeyCallback(m_pWindow, process_input_callback);
	}

	void Window::RenderLoop(const std::vector<FigureDraw*>& fds)
	{
		//g_totalSize = programIds.size();
		g_totalVAO = fds.size();

		while (!glfwWindowShouldClose(m_pWindow)) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Draw the Object
			FigureDraw* pFd = fds.at(g_indexVAO);
			g_indexProgram = pFd->Draw(g_indexProgram);

			glfwSwapBuffers(m_pWindow);
			glfwPollEvents();
		}
	}

	// Not class methods
	// Callbacks
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void process_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			++g_indexProgram;
			std::cout << "Space pressed, changing color " << g_indexProgram << std::endl;
		}

		if (key == GLFW_KEY_A && action == GLFW_PRESS) {
			++g_indexVAO;
			g_indexVAO = g_indexVAO % g_totalVAO;
		}
	}

}

