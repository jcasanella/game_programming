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

const char* FRAGMENT_SHADER_LOCATION = "Shaders\\FragmentShader.glsl";
const char* FRAGMENT_SHADER_A_LOCATION = "Shaders\\FragmentShaderA.glsl";
const char* FRAGMENT_SHADER2_LOCATION = "Shaders\\FragmentShader2.glsl";
const char* FRAGMENT_SHADER_UNIFORM_LOCATION = "Shaders\\FragmentShaderUniform.glsl";

GLuint prepareRectangle();
//GLuint prepareTriangle();
GLuint prepareDoubleTriangle();
GLuint prepareTriangle1();
GLuint prepareTriangle2();

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
	Figure* pTriangle1 = new Figure(&vertex_buffer_data[0], nullptr);
	GLuint VAO1 = pTriangle1->Build();
	delete pTriangle1;

//	GLuint VAO1 = prepareTriangle();
	GLuint VAO2 = prepareRectangle();
	GLuint VAO3 = prepareDoubleTriangle();

	GLuint VAO4 = prepareTriangle1();
	GLuint VAO5 = prepareTriangle2();

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

GLuint prepareRectangle()
{
	const GLfloat vertex_buffer_data[] = {
		0.5f, 0.5f, 0.0f,	// Top Right
		0.5f, -0.5f, 0.0f,	// Bottom Right
		-0.5f, -0.5f, 0.0f,	// Bottom Left
		-0.5f, 0.5f, 0.0f	// Top Left
	};

	const GLuint indexes_data[] = {
		0, 1, 3,	// First triangle
		1, 2, 3		// Second triangle
	};

	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information 
	// for a complete rendered object. Can contain multiples VBO
	// Contains the following info:
	// - Calls to glEnableVertexAttribArray or glDisableVertexAttrib Array
	// - Vertex attribute configurations via glVertexAttribPointer and objects associated via glVertexAttribPointer
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// A Vertex Buffer Object(VBO) is a memory buffer in the GPU designed to hold information about vertices. VBOs can also store information such as normals, 
	// texcoords, indices, etc.
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Copy our vertices array into a buffer to be used by OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes_data), indexes_data, GL_STATIC_DRAW);


	// Set the vertex attribute pointers
	glVertexAttribPointer(
		0,                  // same as location in the vertex shader location
		3,                  // size - it's a vec3 so it contains 3 values
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride, where it starts the next vertex
		(void*)0            // desfase del buffer
	);

	glBindVertexArray(0);	// unbind VAO

	return VAO;
}

/*
GLuint prepareTriangle() 
{
	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information 
	// for a complete rendered object. Can contain multiples VBO
	// Contains the following info:
	// - Calls to glEnableVertexAttribArray or glDisableVertexAttrib Array
	// - Vertex attribute configurations via glVertexAttribPointer and objects associated via glVertexAttribPointer
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	const GLfloat vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	// A Vertex Buffer Object(VBO) is a memory buffer in the GPU designed to hold information about vertices. VBOs can also store information such as normals, 
	// texcoords, indices, etc.
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Copy our vertices array into a buffer to be used by OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	glVertexAttribPointer(
		0,                  // same as location in the vertex shader location
		3,                  // size - it's a vec3 so it contains 3 values
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride, where it starts the next vertex
		(void*)0            // desfase del buffer
	);

	glBindVertexArray(0);	// unbind VAO

	return VAO;
}*/

GLuint prepareDoubleTriangle()
{
	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information 
	// for a complete rendered object. Can contain multiples VBO
	// Contains the following info:
	// - Calls to glEnableVertexAttribArray or glDisableVertexAttrib Array
	// - Vertex attribute configurations via glVertexAttribPointer and objects associated via glVertexAttribPointer
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	const GLfloat vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,	// Left Bottom Triangle1
		-1.0f, 1.0f, 0.0f,	// Left Up Triangle1 
		0.0f,  0.0f, 0.0f,	// Middle Triangle1

		0.0f, -1.0f, 0.0f,	// Left Bottom Triangle2
		0.0f, 1.0f, 0.0f,	// Left Up Triangle2
		1.0f, 0.0f, 0.0f,	// Middle Triangle2
	};

	// A Vertex Buffer Object(VBO) is a memory buffer in the GPU designed to hold information about vertices. VBOs can also store information such as normals, 
	// texcoords, indices, etc.
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Copy our vertices array into a buffer to be used by OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	glVertexAttribPointer(
		0,                  // same as location in the vertex shader location
		3,                  // size - it's a vec3 so it contains 3 values
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride, where it starts the next vertex
		(void*)0            // desfase del buffer
	);

	glBindVertexArray(0);	// unbind VAO

	return VAO;
}

GLuint prepareTriangle1()
{
	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information 
	// for a complete rendered object. Can contain multiples VBO
	// Contains the following info:
	// - Calls to glEnableVertexAttribArray or glDisableVertexAttrib Array
	// - Vertex attribute configurations via glVertexAttribPointer and objects associated via glVertexAttribPointer
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	const GLfloat vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,	// Left Bottom Triangle1
		-1.0f, 1.0f, 0.0f,	// Left Up Triangle1 
		0.0f,  0.0f, 0.0f,	// Middle Triangle1
	};

	// A Vertex Buffer Object(VBO) is a memory buffer in the GPU designed to hold information about vertices. VBOs can also store information such as normals, 
	// texcoords, indices, etc.
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Copy our vertices array into a buffer to be used by OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	glVertexAttribPointer(
		0,                  // same as location in the vertex shader location
		3,                  // size - it's a vec3 so it contains 3 values
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride, where it starts the next vertex
		(void*)0            // desfase del buffer
	);

	glBindVertexArray(0);	// unbind VAO

	return VAO;
}

GLuint prepareTriangle2()
{
	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information 
	// for a complete rendered object. Can contain multiples VBO
	// Contains the following info:
	// - Calls to glEnableVertexAttribArray or glDisableVertexAttrib Array
	// - Vertex attribute configurations via glVertexAttribPointer and objects associated via glVertexAttribPointer
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	const GLfloat vertex_buffer_data[] = {
		0.0f, -1.0f, 0.0f,	// Left Bottom Triangle2
		0.0f, 1.0f, 0.0f,	// Left Up Triangle2
		1.0f, 0.0f, 0.0f,	// Middle Triangle2
	};

	// A Vertex Buffer Object(VBO) is a memory buffer in the GPU designed to hold information about vertices. VBOs can also store information such as normals, 
	// texcoords, indices, etc.
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Copy our vertices array into a buffer to be used by OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	glVertexAttribPointer(
		0,                  // same as location in the vertex shader location
		3,                  // size - it's a vec3 so it contains 3 values
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride, where it starts the next vertex
		(void*)0            // desfase del buffer
	);

	glBindVertexArray(0);	// unbind VAO

	return VAO;
}
