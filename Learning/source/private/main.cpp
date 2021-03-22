#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <filesystem>
#include <vector>

#include "Window.h"
#include "Shader.h"
#include "Vertexs.h"
#include "Resources.h"
#include "FigureDraw.h"
#include "Figure3.h"
#include "MultiFigure3.h"

#include <cassert>

using namespace std;
using namespace GameEngine;

GLuint buildShaderProgram(const char* vertex_shader, const char* fragment_shader);

int main() {
	const int WIDTH = 800;
	const int HEIGHT = 600;
	const char* TITLE = "LearnOpenGL";

	Window* pWindow = new Window(WIDTH, HEIGHT, TITLE);
	pWindow->Init();
	pWindow->BuildWindow();

//#if DEBUG
	namespace fs = std::filesystem;
	std::cout << "Current path is " << fs::current_path() << '\n';
//#endif

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		cout << "Failed to init GLAD" << endl;
		return -1;
	}

	// a VAO that stores our vertex attribute configurationand which VBO to use. Usually when you have multiple
	// objects you want to draw, you first generate / configure all the VAOs(and thus the required VBO and
	// attribute pointers) and store those for later use.The moment we want to draw one of our objects, we
	// take the corresponding VAO, bind it, then draw the object and unbind the VAO again.
	/*
	Figure attribs6 = BuildFigure(&vertex_buffer_with_colors[0], sizeof(vertex_buffer_with_colors), STRIDE_6);*/

	/*const GLfloat vertex_buffer_with_colors[] = {
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,		// bottom right and color red
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	// bottom left and color green
		0.0f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f		// top and color blue
	};*/

	vector<GLuint> programIds;
	for (int idx = 0; idx < NUM_SHADERS; idx++) {
		GLuint programId = buildShaderProgram(SHADERS[idx][0], SHADERS[idx][1]);
		programIds.push_back(programId);
	}

	// This programId has only one shader - used for VAO with 6 index
	vector<GLuint> programIds2;
	GLuint programId = buildShaderProgram(VERTEX_SHADER_COLOR, FRAGMENT_SHADER_COLOR);
	programIds2.push_back(programId);

	FigureDraw* pFd1 = new Figure3(&vertex_buffer_data[0], sizeof(vertex_buffer_data), programIds);	// triangle
	FigureDraw* pFd2 = new Figure3(&vertex_buffer_data2[0], sizeof(vertex_buffer_data2), &indexes_data[0], sizeof(indexes_data), programIds);	// rectangle
	FigureDraw* pFd3 = new Figure3(&vertex_buffer_data3[0], sizeof(vertex_buffer_data3), programIds);	// double triangle
	
	// Object to draw multiple figures with different VAOs
	MultiFigure3* pMultiple = new MultiFigure3(&vertex_buffer_data4[0], sizeof(vertex_buffer_data4), programIds);
	pMultiple->AddFigureToDraw(&vertex_buffer_data5[0], sizeof(vertex_buffer_data5));
	FigureDraw* pFd4 = dynamic_cast<FigureDraw*>(pMultiple);

	FigureDraw* pFd5 = new Figure3(&vertex_buffer_with_colors[0], sizeof(vertex_buffer_with_colors), programIds2);

	std::vector<FigureDraw*> pFds = { pFd1, pFd2, pFd3, pFd4, pFd5 };
	pWindow->RenderLoop(pFds);

	delete pFd5;
	delete pMultiple;
	delete pFd3;
	delete pFd2;
	delete pFd1;
	delete pWindow;
	return 0;
}

GLuint buildShaderProgram(const char* vertex_shader, const char* fragment_shader)
{
	Shader* pShader = new Shader();
	pShader->CompileVertexShader(vertex_shader);
	pShader->CompileFragmentShader(fragment_shader);
	GLuint programId = pShader->CompileShaderProgram();
	delete pShader;

	return programId;
}

