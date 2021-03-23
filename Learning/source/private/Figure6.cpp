#include "Figure6.h"

#include <GLFW/glfw3.h>

namespace GameEngine {

	Figure6::Figure6(const GLfloat* data, ULLong dataSize, const std::vector<GLuint>& programIds) : m_programIds(programIds) {
		PrepareVAO(data, dataSize);
		BuildVAOAttrPointer();

		m_vertexDraw = GetVertexDraw(dataSize);
	}

	void Figure6::PrepareVAO(const GLfloat* data, ULLong dataSize)
	{
		// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information 
		// for a complete rendered object. Can contain multiples VBO
		// Contains the following info:
		// - Calls to glEnableVertexAttribArray or glDisableVertexAttrib Array
		// - Vertex attribute configurations via glVertexAttribPointer and objects associated via glVertexAttribPointer
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		// A Vertex Buffer Object(VBO) is a memory buffer in the GPU designed to hold information about vertices. VBOs can also store information such as normals, 
		// texcoords, indices, etc.
		GLuint VBO;
		glGenBuffers(1, &VBO);

		// Copy our vertices array into a buffer to be used by OpenGL
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);		// note: work around sizeof(m_data)
	}

	void Figure6::BuildVAOAttrPointer()
	{
		glVertexAttribPointer(
			0,                  // same as location in the vertex shader location
			3,                  // size - it's a vec3 so it contains 3 values
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			6 * sizeof(float),  // stride, where it starts the next vertex - valid values 3 and 6
			(void*)0            // buffer offset
		);

		glVertexAttribPointer(
			1,					// same as location in the vertex shader location
			3,					// size - it's a vec3 so it contains 3 values
			GL_FLOAT,			// type
			GL_FALSE,			// normalized
			6 * sizeof(float),	// stride, where it starts the next vertex - valid values 3 and 6
			(void*)(3 * sizeof(float))	// buffer offset
		);

		glBindVertexArray(0);	// unbind VAO
	}

	int Figure6::Draw(int indexProgram)
	{
		indexProgram %= m_programIds.size();
		glUseProgram(m_programIds[indexProgram]);

		glBindVertexArray(m_vao);
		glEnableVertexAttribArray(0);	// same as location in the vertex shader
		glEnableVertexAttribArray(1);	// same as location in the vertex shader
		
		glDrawArrays(GL_TRIANGLES, 0, m_vertexDraw);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);			// unbind VAO

		return indexProgram;
	}

}