#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLuint loadShader();
GLuint prepareTriangle();
void drawTriangle(GLuint vertexbuffer);
const char* readFile(const char* fileName);

int main() {
	const int WIDTH = 800;
	const int HEIGHT = 600;
	const char* TITLE = "LearnOpenGL";

	if (!glfwInit()) {
		cout << "Error initializing glfw" << endl;
		return -1;
	}

	namespace fs = std::filesystem;
	std::cout << "Current path is " << fs::current_path() << '\n';

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

	GLuint vertexBuffer = prepareTriangle();
	GLuint programId = loadShader();

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glUseProgram(programId);

		drawTriangle(vertexBuffer);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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

GLuint loadShader()
{
	// Build and compile the vertex shader
	// Version matches with the current Open GL version
	// Define the input - in this case is aPos
	const char* vertexShaderSource = readFile("Shaders\\VertexShader.glsl");

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check if vertex shader compilation worked
	GLint success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << endl << infoLog << endl;
	}

	// Build Fragment Shader and compile it
	const char* fragmentShaderSource = readFile("Shaders\\FragmentShader.glsl");

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check if fragment shader compiation worked
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		memset(infoLog, 0, 512);
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << endl << infoLog << endl;
	}

	// Link the different shaders
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		memset(infoLog, 0, 512);
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADERPROGRAM::LINK_FAILED" << endl << infoLog << endl;
	}

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}