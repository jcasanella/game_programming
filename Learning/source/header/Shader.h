#pragma once

#include <glad/glad.h>
#include <vector>

namespace GameEngine {

	class Shader {
	public:
		Shader();
		~Shader();

		GLuint CompileVertexShader(const char* shaderLocation);
		GLuint CompileFragmentShader(const char* shaderLocation);
		GLuint CompileShaderProgram() const;
		void Terminate();

	private:
		const char* ReadFile(const char*);
		GLuint CompileShader(const char* shaderLocation, const char* message, const GLenum& shaderType);

	private:
		std::vector<GLuint> m_shadersId;
		GLuint m_programId;
	};

}

