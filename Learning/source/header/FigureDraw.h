#pragma once

#include <glad/glad.h>

namespace GameEngine {

	typedef unsigned long long ULLong;

	class FigureDraw {
	public:
		virtual int Draw(int indexProgram) = 0;
		inline int GetVertexDraw(ULLong dataSize) { return dataSize / sizeof(GLfloat); }
		inline int GetIndexDraw(ULLong dataSize) { return dataSize / sizeof(GLuint); }
	};

}
