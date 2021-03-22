#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "FigureDraw.h"

namespace GameEngine {

	class Window
	{
	public:
		int Init();
		int BuildWindow();
		void Terminate();

		void RenderLoop(const std::vector<FigureDraw*>& fds);

		inline GLFWwindow* GetWindow() const { return m_pWindow; }

		Window(int width, int height, const char* title);
		~Window();

	private:
		void SetCallbacks();

	private:
		GLFWwindow* m_pWindow;
		int m_width, m_height;
		const char* m_pTitle;
		int m_init;
	};
}
