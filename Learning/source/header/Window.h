#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

class Window 
{
public:
	int Init();
	int BuildWindow();
	void Terminate();

	void RenderLoop(const std::vector<GLuint>& programIds, GLuint VAO1, GLuint VAO2, GLuint VAO3, GLuint VAO4, GLuint VAO5);

	inline GLFWwindow* GetWindow() const { return m_pWindow;  }

	Window(int width, int height, const char* title);
	~Window();

private:
	void SetCallbacks();
	void Draw(GLuint VAO);

private:
	GLFWwindow* m_pWindow;
	int m_width, m_height;
	const char* m_pTitle;
	int m_init;
};