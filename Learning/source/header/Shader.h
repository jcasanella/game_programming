#pragma once

#include <glad/glad.h>


class Shader {
public:

	enum ShaderType {
		VERTEX_SHADER,
		FRAGMENT_SHADER
	};

	Shader(const char* shaderLocation, const ShaderType& shaderType);
	~Shader();

	GLuint CompileShader() const;

private:
	const char* ReadFile(const char*);

private:
	const char* VERTEX_SHADER_ERROR = "ERROR::SHADER::VERTEX::COMPILATION_FAILED";
	const char* FRAGMENT_SHADER_ERROR = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED";
};