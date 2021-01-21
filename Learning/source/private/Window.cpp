#include "Window.h"

#include <iostream>

enum DrawType { 
	TRIANGLE, 
	RECTANGLE, 
	DOUBLE_TRIANGLE, 
	MULTIPLE_VAO,

	SIZE_DRAW_TYPE
};
DrawType g_type = TRIANGLE;

int g_indexProgram = 0;
int g_totalSize = 0;

int g_indexVAO = 0;

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

void Window::RenderLoop(const std::vector<GLuint>& programIds, const std::vector<GLuint>& vaoIds)
{
	g_totalSize = programIds.size();

	while (!glfwWindowShouldClose(m_pWindow)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader program before to render
		glUseProgram(programIds[g_indexProgram]);
		if (g_indexProgram == g_totalSize - 1) {
			float timeValue = glfwGetTime();
			float greenValue = sin(timeValue) / 2.0f + 0.5f;
			int vertexColorLocation = glGetUniformLocation(programIds[g_indexProgram], "ourColor");
			glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		}

		// Draw the Object
		Draw(vaoIds[g_indexVAO]);
		if (g_type == MULTIPLE_VAO)
		{
			Draw(vaoIds[g_indexVAO + 1]);
		}

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}
}

void Window::Draw(GLuint VAO)
{
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);	// same as location in the vertex shader

	if (g_type == TRIANGLE || g_type == MULTIPLE_VAO) {
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	else if (g_type == DOUBLE_TRIANGLE) {
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	else {
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);	// same as location in the vertex shader
	glBindVertexArray(0);			// unbinds
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
		g_indexProgram = ++g_indexProgram % g_totalSize;
		std::cout << "Space pressed, changing color " << g_indexProgram <<  std::endl;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		g_type = static_cast<DrawType>((static_cast<int>(g_type) + 1) % static_cast<int>(DrawType::SIZE_DRAW_TYPE));
		g_indexVAO = static_cast<int>(g_type);
		std::cout << "A pressed, changing image " << g_type << std::endl;
	}
}

