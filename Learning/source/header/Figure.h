#pragma once

#include <glad/glad.h>

namespace GameEngine {

	typedef unsigned long long ULLong;

	class Figure {
	public:
		Figure(const GLfloat* data, ULLong dataSize, const GLuint* indexes = nullptr, ULLong indexesSize = 0);

		GLuint Build();

	private:
		const GLfloat* m_data;
		const GLuint* m_index;
		ULLong m_dataSize;
		ULLong m_indexesSize;
	};
}
