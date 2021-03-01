#pragma once

#include <glad/glad.h>

#include "Figure.h"

namespace GameEngine {

	typedef unsigned long long ULLong;

	class FigureDraw {
	public:
		virtual void Draw() = 0;
		inline int GetVertexDraw(ULLong dataSize) { return dataSize / sizeof(GLfloat); }
		inline int GetIndexDraw(ULLong dataSize) { return dataSize / sizeof(GLuint); }
	};

}
