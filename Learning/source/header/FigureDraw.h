#pragma once

#include "Figure.h"

namespace GameEngine {

	class FigureDraw {
	public:
		virtual void Draw() = 0;
		inline int GetVertexDraw(ULLong dataSize) { return dataSize / sizeof(GLfloat); }
		inline int GetIndexDraw(ULLong dataSize) { return dataSize / sizeof(GLuint); }
	};

}
