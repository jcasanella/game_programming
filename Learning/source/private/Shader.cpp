#include "Shader.h"

#include <iostream>
#include <fstream>
#include <cassert>

namespace GameEngine {

	Shader::Shader()
	{
	}

	GLuint Shader::CompileVertexShader(const char* shaderLocation) const
	{
		const char* VERTEX_SHADER_ERROR = "ERROR::SHADER::VERTEX::COMPILATION_FAILED";
		return CompileShader(shaderLocation, VERTEX_SHADER_ERROR, GL_VERTEX_SHADER);
	}

	GLuint Shader::CompileFragmentShader(const char* shaderLocation) const
	{
		const char* FRAGMENT_SHADER_ERROR = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED";
		return CompileShader(shaderLocation, FRAGMENT_SHADER_ERROR, GL_FRAGMENT_SHADER);
	}

	GLuint Shader::CompileShaderProgram() const
	{
		// Link the different shaders
		GLuint shaderProgram = glCreateProgram();
		assert(shaderProgram != 0);

		GLint success;
		char infoLog[512];

		for (std::vector<GLuint>::const_iterator it = m_shadersId.begin(); it != m_shadersId.end(); ++it) {
			glAttachShader(shaderProgram, *it);
		}


		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADERPROGRAM::LINK_FAILED" << std::endl << infoLog << std::endl;
		}

		for (std::vector<GLuint>::const_iterator it = m_shadersId.begin(); it != m_shadersId.end(); ++it) {
			glDetachShader(shaderProgram, *it);
			glDeleteShader(*it);
		}

		return shaderProgram;
	}

	GLuint Shader::CompileShader(const char* shaderLocation, const char* errorMessage, const GLenum& shaderType) const
	{
		const char* shaderSource = ReadFile(shaderLocation);
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
			std::cout << errorMessage << std::endl << infoLog << std::endl;
		}

		delete[] shaderSource;

		std::vector<GLuint> shaderVector;
		shaderVector.push_back(shaderId);
	//	this->shaderVector2.push_back(shaderId);


		return shaderId;
	}

	const char* Shader::ReadFile(const char* fileName) const
	{
		char* content = NULL;
		std::ifstream inData(fileName);
		if (inData.is_open()) {
			inData.seekg(0, std::ios::end);
			size_t fileSize = inData.tellg();
			inData.seekg(0);
			content = new char[fileSize + 1];
			inData.read(content, fileSize);
			content[fileSize] = '\0';
		}

		inData.close();

		return content;
	}

}