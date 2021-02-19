#include "Figure3.h"

namespace GameEngine {

	Figure3::Figure3(const GLfloat* data, ULLong dataSize) {
		PrepareVAO(data, dataSize);
		BuildVAOAttrPointer();
	}

	Figure3::Figure3(const GLfloat* data, ULLong dataSize, const GLuint* index, ULLong indexSize)
	{
		PrepareVAO(data, dataSize);
		BuildEBO(index, indexSize);
		BuildVAOAttrPointer();
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


	/*
	void Figure3::LoadVAO(const GLfloat* data, ULLong dataSize)
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

	/*	if (indexes) {
			GLuint EBO;
			glGenBuffers(1, &EBO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexesSize, indexes, GL_STATIC_DRAW);
		}*/

		// Set the vertex attribute pointers
	//	attributes.vertextAttrPointer.push_back(0);	// same as location in the vertex shader location
	/*	glVertexAttribPointer(
			0,                  // same as location in the vertex shader location
			3,                  // size - it's a vec3 so it contains 3 values
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			3 * sizeof(float),  // stride, where it starts the next vertex - valid values 3 and 6
			(void*)0            // desfase del buffer
		);

		/*if (strideSize == STRIDE_6) {
			attributes.vertextAttrPointer.push_back(1);
			// 1rst argument - location in vertex shader
			// last argument offset in the buffer
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strideSize * sizeof(float), (void*)(3 * sizeof(float)));
		}*/

	/*	glBindVertexArray(0);	// unbind VAO
	}*/

	void Figure3::Draw()
	{
		glBindVertexArray(m_vao);
		glEnableVertexAttribArray(0);	// same as location in the vertex shader

		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);			// unbind
	}

}
