#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <tuple>

#include <cassert>

using namespace std;

const char* VERTEX_SHADER_LOCATION = "Shaders\\VertexShader.glsl";
const char* VERTEX_SHADER_ERROR = "ERROR::SHADER::VERTEX::COMPILATION_FAILED";

const char* FRAGMENT_SHADER_LOCATION = "Shaders\\FragmentShader.glsl";
const char* FRAGMENT_SHADER2_LOCATION = "Shaders\\FragmentShader2.glsl";
const char* FRAGMENT_SHADER_ERROR = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLuint prepareTriangle();
void drawTriangle(GLuint vertexbuffer);
const char* readFile(const char* fileName);
GLuint buildShader(const char* shaderLocation, const char* errorMessage, GLuint shaderType);
GLuint buildShaderProgram(vector<GLuint> shadersVector);
GLuint buildShaderAndProgram(vector<tuple<const char*, const char*, GLuint>> shaders);

int main() {
	const int WIDTH = 800;
	const int HEIGHT = 600;
	const char* TITLE = "LearnOpenGL";

	if (!glfwInit()) {
		cout << "Error initializing glfw" << endl;
		return -1;
	}

#if DEBUG
	namespace fs = std::filesystem;
	std::cout << "Current path is " << fs::current_path() << '\n';
#endif

	glfwWindowHint(GLFW_SAMPLES, 4);								// Antialiasing 4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// mac os  compability
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// only new open gl

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); 

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		cout << "Failed to init GLAD" << endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, process_input_callback);

	GLuint vertexBuffer = prepareTriangle();

	auto vertex = make_tuple(VERTEX_SHADER_LOCATION, VERTEX_SHADER_ERROR, GL_VERTEX_SHADER);
	auto fragment = make_tuple(FRAGMENT_SHADER_LOCATION, FRAGMENT_SHADER_ERROR, GL_FRAGMENT_SHADER);
	vector<tuple<const char*, const char*, GLuint>> shadersVector;
	shadersVector.push_back(vertex);
	shadersVector.push_back(fragment);
	GLuint programId = buildShaderAndProgram(shadersVector);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glUseProgram(programId);

		drawTriangle(vertexBuffer);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(programId);

	glfwTerminate();
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

GLuint prepareTriangle() 
{
	GLuint vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	return vertexBuffer;
}

void drawTriangle(GLuint vertexbuffer) 
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // same as location in the vertex shader location
		3,                  // size - it's a vec3 so it contains 3 values
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride, where it starts the next vertex
		(void*)0            // desfase del buffer
	);
	// Dibujar el triángulo !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo
	glDisableVertexAttribArray(0);
}

GLuint buildShader(const char* shaderLocation, const char* errorMessage, GLuint shaderType) 
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

GLuint buildShaderProgram(vector<GLuint> shadersVector)
{
	// Link the different shaders
	GLuint shaderProgram = glCreateProgram();
	assert(shaderProgram != 0);

	GLint success;
	char infoLog[512];

	for (vector<GLuint>::iterator it = shadersVector.begin(); it != shadersVector.end(); ++it) {
		glAttachShader(shaderProgram, *it);
	}


	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADERPROGRAM::LINK_FAILED" << endl << infoLog << endl;
	}

	for (vector<GLuint>::iterator it = shadersVector.begin(); it != shadersVector.end(); ++it) {
		glDetachShader(shaderProgram, *it);
		glDeleteShader(*it);
	}

	return shaderProgram;
}

GLuint buildShaderAndProgram(vector<tuple<const char*, const char*, GLuint>> shaders)
{
	vector<GLuint> shaderVector;
	for (std::vector<tuple<const char*, const char*, GLuint>>::iterator it = shaders.begin(); it != shaders.end(); ++it) {
		GLuint shaderId = buildShader(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
		shaderVector.push_back(shaderId);
	}

	//GLuint vertexShader = buildShader(std::get<0>(shaders), std::get<1>(shaders), std::get<2>(shaders));
	//GLuint fragmentShader = buildShader(FRAGMENT_SHADER_LOCATION, FRAGMENT_SHADER_ERROR, GL_FRAGMENT_SHADER);

	//vector<GLuint> shaderVector = { vertexShader, fragmentShader };

	GLuint shaderProgram = buildShaderProgram(shaderVector);

	return shaderProgram;
}

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
		cout << "Space pressed" << endl;
	}
}