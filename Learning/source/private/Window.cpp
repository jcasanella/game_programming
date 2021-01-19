#include "Window.h"

#include <iostream>

enum DrawType { TRIANGLE, RECTANGLE, DOUBLE_TRIANGLE, MULTIPLE_VAO };
DrawType g_type = TRIANGLE;
bool g_isClicked = false;

// Define callbacks
void framebuffer_size_callback(GLFWwindow*, int width, int height);
void process_input_callback(GLFWwindow*, int key, int scancode, int action, int mods);

// Class starts here
Window::Window(int width, int height, const char* title)
{
	m_width = width;
	m_height = height;
	m_pTitle = title;
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

	return 0;
}

int Window::BuildWindow()
{
	glfwWindowHint(GLFW_SAMPLES, 4);								// Antialiasing 4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// mac os  compability
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// only new open gl

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

void Window::SetCallbacks()
{
	glfwSetFramebufferSizeCallback(m_pWindow, framebuffer_size_callback);
	glfwSetKeyCallback(m_pWindow, process_input_callback);
}

void Window::RenderLoop(GLuint programId, GLuint programId2, GLuint VAO1, GLuint VAO2, GLuint VAO3, GLuint VAO4, GLuint VAO5)
{
	while (!glfwWindowShouldClose(m_pWindow)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader program before to render
		glUseProgram(!g_isClicked ? programId : programId2);

		// Draw the object
		if (g_type == TRIANGLE) {
			Draw(VAO1);
		}
		else if (g_type == RECTANGLE) {
			Draw(VAO2);
		}
		else if (g_type == DOUBLE_TRIANGLE) {
			Draw(VAO3);
		}
		else {
			Draw(VAO4);
			Draw(VAO5);
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
		g_isClicked = !g_isClicked;
		std::cout << "Space pressed, changing color" << std::endl;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		if (g_type == TRIANGLE) {
			g_type = DOUBLE_TRIANGLE;
			std::cout << "A pressed, drawing a Double Triangle" << std::endl;
		}
		else if (g_type == DOUBLE_TRIANGLE) {
			g_type = RECTANGLE;
			std::cout << "A pressed, drawing a Rectangle" << std::endl;
		}
		else if (g_type == RECTANGLE) {
			g_type = MULTIPLE_VAO;
			std::cout << "A pressed, drawing a Double Triangle with multiples VAO" << std::endl;
		}
		else {
			g_type = TRIANGLE;
			std::cout << "A pressed, drawing a Triangle" << std::endl;
		}
	}
}

