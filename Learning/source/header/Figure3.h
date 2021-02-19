#pragma once

#include <glad/glad.h>

#include "FigureDraw.h"
#include "Figure.h"

namespace GameEngine {

	class Figure3 : public FigureDraw {
	public:
		Figure3(const GLfloat* data, ULLong dataSize);
		Figure3(const GLfloat* data, ULLong dataSize, const GLuint* index, ULLong indexSize);

		virtual void Draw();

	private:
		void PrepareVAO(const GLfloat* data, ULLong dataSize);
		void BuildEBO(GLuint* index, ULLong indexSize);
		void BuildVAOAttrPointer();

	private:
		GLuint m_vao;
	};

}
