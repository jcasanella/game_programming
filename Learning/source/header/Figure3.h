#pragma once

#include <glad/glad.h>
#include <vector>

#include "FigureDraw.h"
#include "Figure.h"

namespace GameEngine {

	class Figure3 : public FigureDraw {
	public:
		Figure3(const GLfloat* data, ULLong dataSize, const std::vector<GLuint>& programIds);
		Figure3(const GLfloat* data, ULLong dataSize, const GLuint* index, ULLong indexSize, const std::vector<GLuint>& programIds);

		virtual void Draw();

	private:
		void PrepareVAO(const GLfloat* data, ULLong dataSize);
		void BuildEBO(const GLuint* index, ULLong indexSize);
		void BuildVAOAttrPointer();

	private:
		GLuint m_vao;
		bool m_withIndex;
		int m_vertexDraw;
		int m_indexDraw;
		std::vector<GLuint> m_programIds;
	};

}
