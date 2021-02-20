#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <filesystem>
#include <vector>

#include "Window.h"
#include "Shader.h"
#include "Figure.h"
#include "Vertexs.h"
#include "Resources.h"
#include "FigureDraw.h"
#include "Figure3.h"

#include <cassert>

using namespace std;
using namespace GameEngine;

//GLuint prepareImage(const GLfloat* data, ULLong sizeData, const GLuint* indexes=nullptr, ULLong sizeIndexes = 0);

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
	/*Figure attribs1 = BuildFigure(&vertex_buffer_data[0], sizeof(vertex_buffer_data), STRIDE_3);

	Figure attribs2 = BuildFigure(&vertex_buffer_data2[0], sizeof(vertex_buffer_data2), STRIDE_3, &indexes_data[0], sizeof(indexes_data));

	Figure attribs3 = BuildFigure(&vertex_buffer_data3[0], sizeof(vertex_buffer_data3), STRIDE_3);	

	Figure attribs4 = BuildFigure(&vertex_buffer_data4[0], sizeof(vertex_buffer_data4), STRIDE_3);

	Figure attribs5 = BuildFigure(&vertex_buffer_data5[0], sizeof(vertex_buffer_data5), STRIDE_3);

	Figure attribs6 = BuildFigure(&vertex_buffer_with_colors[0], sizeof(vertex_buffer_with_colors), STRIDE_6);*/

	/*const GLfloat vertex_buffer_with_colors[] = {
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,		// bottom right and color red
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	// bottom left and color green
		0.0f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f		// top and color blue
	};*/

	vector<GLuint> programIds;
	for (int idx = 0; idx < NUM_SHADERS; idx++) {
		Shader* pShader = new Shader();
		pShader->CompileVertexShader(SHADERS[idx][0]);
		pShader->CompileFragmentShader(SHADERS[idx][1]);
		GLuint programId = pShader->CompileShaderProgram();
		programIds.push_back(programId);
		delete pShader;	// 	NOTE: Deleting the Object Shader will delete the program either
	}

	//const vector<Figure> vaos = { attribs1, attribs2, attribs3, attribs4, attribs5 };
	FigureDraw* pFd1 = new Figure3(&vertex_buffer_data[0], sizeof(vertex_buffer_data));	// triangle
	FigureDraw* pFd2 = new Figure3(&vertex_buffer_data2[0], sizeof(vertex_buffer_data2), &indexes_data[0], sizeof(indexes_data));	// rectangle
	FigureDraw* pFd3 = new Figure3(&vertex_buffer_data3[0], sizeof(vertex_buffer_data3));	// double triangle
	std::vector<FigureDraw*> pFds = { pFd1, pFd2, pFd3 };
	pWindow->RenderLoop(programIds, pFds);

	delete pFd3;
	delete pFd2;
	delete pFd1;
	delete pWindow;
	return 0;
}

/*
GLuint prepareImage(const GLfloat* vertex_data, ULLong sizeData, const GLuint* index_data, ULLong sizeIndex)
{
	Figure* pTriangle = new Figure(vertex_data, sizeData, index_data, sizeIndex);
	GLuint VAO = pTriangle->Build();
	delete pTriangle;

	return VAO;
}*/
