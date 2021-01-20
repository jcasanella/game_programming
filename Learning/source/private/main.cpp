#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <tuple>

#include "Window.h"

#include <cassert>

using namespace std;

const char* VERTEX_SHADER_LOCATION = "Shaders\\VertexShader.glsl";
const char* VERTEX_SHADER_A_LOCATION = "Shaders\\VertexShaderA.glsl";
const char* VERTEX_SHADER_ERROR = "ERROR::SHADER::VERTEX::COMPILATION_FAILED";

const char* FRAGMENT_SHADER_LOCATION = "Shaders\\FragmentShader.glsl";
const char* FRAGMENT_SHADER_A_LOCATION = "Shaders\\FragmentShaderA.glsl";
const char* FRAGMENT_SHADER2_LOCATION = "Shaders\\FragmentShader2.glsl";
const char* FRAGMENT_SHADER_UNIFORM_LOCATION = "Shaders\\FragmentShaderUniform.glsl";
const char* FRAGMENT_SHADER_ERROR = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLuint prepareRectangle();
GLuint prepareTriangle();
GLuint prepareDoubleTriangle();
GLuint prepareTriangle1();
GLuint prepareTriangle2();
const char* readFile(const char*);
GLuint compileShader(const char* shaderLocation, const char* errorMessage, GLuint shaderType);
GLuint compileShaderProgram(const vector<GLuint>& shadersVector);
GLuint buildShaderAndProgram(const vector<tuple<const char*, const char*, GLuint>>& shaders);

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
	GLuint VAO1 = prepareTriangle();
	GLuint VAO2 = prepareRectangle();
	GLuint VAO3 = prepareDoubleTriangle();

	GLuint VAO4 = prepareTriangle1();
	GLuint VAO5 = prepareTriangle2();

	// First Shader program
	auto vertex = make_tuple(VERTEX_SHADER_A_LOCATION, VERTEX_SHADER_ERROR, GL_VERTEX_SHADER);
	auto fragment = make_tuple(FRAGMENT_SHADER_A_LOCATION, FRAGMENT_SHADER_ERROR, GL_FRAGMENT_SHADER);
	vector<tuple<const char*, const char*, GLuint>> shadersVector = { vertex, fragment };
	GLuint programId = buildShaderAndProgram(shadersVector);

	// Second Shader program
	auto vertex2 = make_tuple(VERTEX_SHADER_LOCATION, VERTEX_SHADER_ERROR, GL_VERTEX_SHADER);
	auto fragment2 = make_tuple(FRAGMENT_SHADER2_LOCATION, FRAGMENT_SHADER_ERROR, GL_FRAGMENT_SHADER);
	vector<tuple<const char*, const char*, GLuint>> shadersVector2 = { vertex2, fragment2 };
	GLuint programId2 = buildShaderAndProgram(shadersVector2);

	auto vertex3 = make_tuple(VERTEX_SHADER_LOCATION, VERTEX_SHADER_ERROR, GL_VERTEX_SHADER);
	auto fragment3 = make_tuple(FRAGMENT_SHADER_UNIFORM_LOCATION, FRAGMENT_SHADER_ERROR, GL_FRAGMENT_SHADER);
	vector<tuple<const char*, const char*, GLuint>> shadersVector3 = { vertex3, fragment3 };
	GLuint programId3 = buildShaderAndProgram(shadersVector3);

	const vector<GLuint> programIds = { programId, programId2, programId3 };
	pWindow->RenderLoop(programIds, VAO1, VAO2, VAO3, VAO4, VAO5);

	for(vector<GLuint>::const_iterator iter = programIds.begin(); iter != programIds.end(); ++iter)
		glDeleteProgram(*iter);

	delete pWindow;
	return 0;
}

const char* readFile(const char* fileName)
{
	char* content = NULL;
	ifstream inData(fileName);
	if (inData.is_open()) {
		inData.seekg(0, ios::end);
		size_t fileSize = inData.tellg();
		inData.seekg(0);
		content = new char[fileSize + 1];
		inData.read(content, fileSize);
		content[fileSize] = '\0';
	}

	inData.close();

	return content;
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
}

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

GLuint compileShader(const char* shaderLocation, const char* errorMessage, GLuint shaderType) 
{
	const char* shaderSource = readFile(shaderLocation);
	assert(shaderSource != NULL);

	GLuint shaderId = glCreateShader(shaderType);
	assert(shaderId != 0);

	glShaderSource(shaderId, 1, &shaderSource, NULL);
	glCompileShader(shaderId);

	// Check if vertex shader compilation worked
	GLint success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		cout << errorMessage << endl << infoLog << endl;
	}

	delete[] shaderSource;

	return shaderId;
}

GLuint compileShaderProgram(const vector<GLuint>& shadersVector)
{
	// Link the different shaders
	GLuint shaderProgram = glCreateProgram();
	assert(shaderProgram != 0);

	GLint success;
	char infoLog[512];

	for (vector<GLuint>::const_iterator it = shadersVector.begin(); it != shadersVector.end(); ++it) {
		glAttachShader(shaderProgram, *it);
	}


	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADERPROGRAM::LINK_FAILED" << endl << infoLog << endl;
	}

	for (vector<GLuint>::const_iterator it = shadersVector.begin(); it != shadersVector.end(); ++it) {
		glDetachShader(shaderProgram, *it);
		glDeleteShader(*it);
	}

	return shaderProgram;
}

GLuint buildShaderAndProgram(const vector<tuple<const char*, const char*, GLuint>>& shaders)
{
	vector<GLuint> shaderVector;
	for (std::vector<tuple<const char*, const char*, GLuint>>::const_iterator it = shaders.begin(); it != shaders.end(); ++it) {
		GLuint shaderId = compileShader(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
		shaderVector.push_back(shaderId);
	}

	GLuint shaderProgram = compileShaderProgram(shaderVector);

	return shaderProgram;
}
