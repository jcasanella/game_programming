#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <filesystem>
#include <vector>

#include "Window.h"
#include "Shader.h"
#include "Figure.h"

#include <cassert>

using namespace std;
using namespace GameEngine;

const char* VERTEX_SHADER_LOCATION = "Shaders\\VertexShader.glsl";
const char* VERTEX_SHADER_A_LOCATION = "Shaders\\VertexShaderA.glsl";
const char* VERTEX_SHADER_COLOR = "Shaders\\VertexShader3.glsl";

const char* FRAGMENT_SHADER_LOCATION = "Shaders\\FragmentShader.glsl";
const char* FRAGMENT_SHADER_A_LOCATION = "Shaders\\FragmentShaderA.glsl";
const char* FRAGMENT_SHADER2_LOCATION = "Shaders\\FragmentShader2.glsl";
const char* FRAGMENT_SHADER_UNIFORM_LOCATION = "Shaders\\FragmentShaderUniform.glsl";
const char* FRAGMENT_SHADER_COLOR = "Shaders\\FragmentShader3.glsl";

string test[3][2] = {
	{ VERTEX_SHADER_A_LOCATION, VERTEX_SHADER_A_LOCATION },
	{ VERTEX_SHADER_LOCATION, FRAGMENT_SHADER2_LOCATION },
	{ VERTEX_SHADER_LOCATION, FRAGMENT_SHADER_UNIFORM_LOCATION }
}; 

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

	const GLfloat vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};
	GLuint VAO1 = prepareImage(&vertex_buffer_data[0], sizeof(vertex_buffer_data));

	const GLfloat vertex_buffer_data2[] = {
		0.5f, 0.5f, 0.0f,	// Top Right
		0.5f, -0.5f, 0.0f,	// Bottom Right
		-0.5f, -0.5f, 0.0f,	// Bottom Left
		-0.5f, 0.5f, 0.0f	// Top Left
	};

	const GLuint indexes_data[] = {
		0, 1, 3,	// First triangle
		1, 2, 3		// Second triangle
	};
	GLuint VAO2 = prepareImage(&vertex_buffer_data2[0], sizeof(vertex_buffer_data2), &indexes_data[0], sizeof(indexes_data));

	const GLfloat vertex_buffer_data3[] = {
		-1.0f, -1.0f, 0.0f,	// Left Bottom Triangle1
		-1.0f, 1.0f, 0.0f,	// Left Up Triangle1 
		0.0f,  0.0f, 0.0f,	// Middle Triangle1

		0.0f, -1.0f, 0.0f,	// Left Bottom Triangle2
		0.0f, 1.0f, 0.0f,	// Left Up Triangle2
		1.0f, 0.0f, 0.0f,	// Middle Triangle2
	};
	GLuint VAO3 = prepareImage(&vertex_buffer_data3[0], sizeof(vertex_buffer_data3));	

	const GLfloat vertex_buffer_data4[] = {
		-1.0f, -1.0f, 0.0f,	// Left Bottom Triangle1
		-1.0f, 1.0f, 0.0f,	// Left Up Triangle1 
		0.0f,  0.0f, 0.0f,	// Middle Triangle1
	};
	GLuint VAO4 = prepareImage(&vertex_buffer_data4[0], sizeof(vertex_buffer_data4));

	const GLfloat vertex_buffer_data5[] = {
		0.0f, -1.0f, 0.0f,	// Left Bottom Triangle2
		0.0f, 1.0f, 0.0f,	// Left Up Triangle2
		1.0f, 0.0f, 0.0f,	// Middle Triangle2
	};
	GLuint VAO5 = prepareImage(&vertex_buffer_data5[0], sizeof(vertex_buffer_data5));

	const GLfloat vertex_buffer_with_colors[] = {
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,		// bottom right and color red
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	// bottom left and color green
		0.0f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f		// top and color blue
	};

	//const char

	Shader* pShader = new Shader();
	pShader->CompileVertexShader(VERTEX_SHADER_A_LOCATION);
	pShader->CompileFragmentShader(FRAGMENT_SHADER_A_LOCATION);
	GLuint programId = pShader->CompileShaderProgram();

	Shader* pShader2 = new Shader();
	pShader2->CompileVertexShader(VERTEX_SHADER_LOCATION);
	pShader2->CompileFragmentShader(FRAGMENT_SHADER2_LOCATION);
	GLuint programId2 = pShader2->CompileShaderProgram();

	Shader* pShader3 = new Shader();
	pShader3->CompileVertexShader(VERTEX_SHADER_LOCATION);
	pShader3->CompileFragmentShader(FRAGMENT_SHADER_UNIFORM_LOCATION);
	GLuint programId3 = pShader3->CompileShaderProgram();



	const vector<GLuint> programIds = { programId, programId2, programId3 };
	const vector<GLuint> vaos = { VAO1, VAO2, VAO3, VAO4, VAO5 };
	pWindow->RenderLoop(programIds, vaos);

	// NOTE: Deleting the Object Shader will delete the program either
	delete pShader;
	delete pShader2;
	delete pShader3;

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
