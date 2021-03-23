#pragma once

#include <glad/glad.h>
#include <vector>

#include "FigureDraw.h"

namespace GameEngine {

	class Figure6 : public FigureDraw {
	public:
		Figure6(const GLfloat* data, ULLong dataSize, const std::vector<GLuint>& programIds);

		virtual int Draw(int indexProgram);

	private:
		void PrepareVAO(const GLfloat* data, ULLong dataSize);
		void BuildVAOAttrPointer();

	private:
		GLuint m_vao;
		int m_vertexDraw;
		std::vector<GLuint> m_programIds;
	};

}