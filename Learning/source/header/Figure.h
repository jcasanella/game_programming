#pragma once

#include <glad/glad.h>

namespace GameEngine {

	class Figure {
	public:
		Figure(const GLfloat* data, const GLuint* indexes);

		GLuint Build();

	private:
		const GLfloat* m_data;
		const GLuint* m_index;
	};
}
