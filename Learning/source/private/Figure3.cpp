#include "Figure3.h"

#include <GLFW/glfw3.h>

namespace GameEngine {

	Figure3::Figure3(const GLfloat* data, ULLong dataSize, const std::vector<GLuint>& programIds): m_withIndex(false), m_programIds(programIds) {
		PrepareVAO(data, dataSize);
		BuildVAOAttrPointer();

		m_vertexDraw = GetVertexDraw(dataSize);
	}

	Figure3::Figure3(const GLfloat* data, ULLong dataSize, const GLuint* index, ULLong indexSize, const std::vector<GLuint>& programIds): m_withIndex(true), m_programIds(programIds)
	{
		PrepareVAO(data, dataSize);
		BuildEBO(index, indexSize);
		BuildVAOAttrPointer();

		m_indexDraw = GetIndexDraw(indexSize);
	}

	void Figure3::PrepareVAO(const GLfloat* data, ULLong dataSize)
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

	void Figure3::BuildVAOAttrPointer()
	{
		glVertexAttribPointer(
			0,                  // same as location in the vertex shader location
			3,                  // size - it's a vec3 so it contains 3 values
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			3 * sizeof(float),  // stride, where it starts the next vertex - valid values 3 and 6
			(void*)0            // desfase del buffer
		);

		glBindVertexArray(0);	// unbind VAO
	}

	void Figure3::BuildEBO(const GLuint* index, ULLong indexSize)
	{
		GLuint EBO;
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, index, GL_STATIC_DRAW);
	}

	int Figure3::Draw(int indexProgram)
	{
		indexProgram %= m_programIds.size();
		glUseProgram(m_programIds[indexProgram]);
		if (indexProgram == m_programIds.size() - 1) {
			float timeValue = glfwGetTime();
			float greenValue = sin(timeValue) / 2.0f + 0.5f;
			int vertexColorLocation = glGetUniformLocation(m_programIds[indexProgram], "ourColor");
			glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		}

		glBindVertexArray(m_vao);
		glEnableVertexAttribArray(0);	// same as location in the vertex shader
		if (!m_withIndex)
			glDrawArrays(GL_TRIANGLES, 0, m_vertexDraw);
		else
			glDrawElements(GL_TRIANGLES, m_indexDraw, GL_UNSIGNED_INT, 0);
		
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);			// unbind

		return indexProgram;
	}

}
