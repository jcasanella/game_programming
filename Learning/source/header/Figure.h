#pragma once

#include <glad/glad.h>
#include <vector>

namespace GameEngine {

	typedef unsigned long long ULLong;

	typedef struct {
		GLuint vao;
		std::vector<int> vertextAttrPointer;
	} Figure;

	Figure BuildFigure(const GLfloat* data, ULLong dataSize, const GLuint* indexes = nullptr, ULLong indexesSize = 0);
}
