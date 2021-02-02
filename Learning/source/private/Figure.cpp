#include "Figure.h"

namespace GameEngine {

	Figure::Figure(const GLfloat* data, ULLong sizeData, const GLuint* indexes, ULLong sizeIndexes) : m_data(data), m_index(indexes), m_dataSize(sizeData), m_indexesSize(sizeIndexes)
	{

	}

	GLuint Figure::Build() 
	{
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
		glBufferData(GL_ARRAY_BUFFER, m_dataSize, m_data, GL_STATIC_DRAW);		// note: work around sizeof(m_data)

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
}
