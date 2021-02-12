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

#include <cassert>

using namespace std;
using namespace GameEngine;

GLuint prepareImage(const GLfloat* data, ULLong sizeData, const GLuint* indexes=nullptr, ULLong sizeIndexes = 0);

int main() {
	const int WIDTH = 800;
	const int HEIGHT = 600;
	const char* TITLE = "LearnOpenGL";

	Window* pWindow = new Window(WIDTH, HEIGHT, TITLE);
	pWindow->Init();
	pWindow->BuildWindow();

#if DEBUG
	namespace fs = std::filesystem;
	std::cout << "Current path is " << fs::current_path() << '\n';
#endif

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		cout << "Failed to init GLAD" << endl;
		return -1;
	}

	// a VAO that stores our vertex attribute configurationand which VBO to use. Usually when you have multiple
	// objects you want to draw, you first generate / configure all the VAOs(and thus the required VBO and
	// attribute pointers) and store those for later use.The moment we want to draw one of our objects, we
	// take the corresponding VAO, bind it, then draw the object and unbind the VAO again.

	GLuint VAO1 = prepareImage(&vertex_buffer_data[0], sizeof(vertex_buffer_data));

	GLuint VAO2 = prepareImage(&vertex_buffer_data2[0], sizeof(vertex_buffer_data2), &indexes_data[0], sizeof(indexes_data));

	GLuint VAO3 = prepareImage(&vertex_buffer_data3[0], sizeof(vertex_buffer_data3));	

	GLuint VAO4 = prepareImage(&vertex_buffer_data4[0], sizeof(vertex_buffer_data4));

	GLuint VAO5 = prepareImage(&vertex_buffer_data5[0], sizeof(vertex_buffer_data5));

	const GLfloat vertex_buffer_with_colors[] = {
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,		// bottom right and color red
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	// bottom left and color green
		0.0f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f		// top and color blue
	};

	vector<GLuint> programIds;
	for (int idx = 0; idx < NUM_SHADERS; idx++) {
		Shader* pShader = new Shader();
		pShader->CompileVertexShader(SHADERS[idx][0]);
		pShader->CompileFragmentShader(SHADERS[idx][1]);
		GLuint programId = pShader->CompileShaderProgram();
		programIds.push_back(programId);
		delete pShader;	// 	NOTE: Deleting the Object Shader will delete the program either
	}

	const vector<GLuint> vaos = { VAO1, VAO2, VAO3, VAO4, VAO5 };
	pWindow->RenderLoop(programIds, vaos);

	delete pWindow;
	return 0;
}

GLuint prepareImage(const GLfloat* vertex_data, ULLong sizeData, const GLuint* index_data, ULLong sizeIndex)
{
	Figure* pTriangle = new Figure(vertex_data, sizeData, index_data, sizeIndex);
	GLuint VAO = pTriangle->Build();
	delete pTriangle;

	return VAO;
}
