#pragma once

#include <glad/glad.h>
#include <vector>

namespace GameEngine {

	class Shader {
	public:
		Shader();
		~Shader();

		GLuint CompileVertexShader(const char* shaderLocation) const;
		GLuint CompileFragmentShader(const char* shaderLocation) const;
		GLuint CompileShaderProgram() const;

	private:
		const char* ReadFile(const char*) const;
		GLuint CompileShader(const char* shaderLocation, const char* message, const GLenum& shaderType) const;

	private:
		std::vector<GLuint> m_shadersId;
		std::vector<GLuint> shaderVector2;
	};

}

