#pragma once

#include <list>

#include "FigureDraw.h"

namespace GameEngine {

	typedef struct {
		GLuint vao;
		int vertexDraw;
	} Attributes;

	class MultiFigure3 : public FigureDraw {
	public:
		MultiFigure3(const GLfloat* data, ULLong dataSize);

		void AddFigureToDraw(const GLfloat* data, ULLong dataSize);
		virtual void Draw();

	private:
		void DrawFigure(const Attributes& attr);
		GLuint PrepareVAO(const GLfloat* data, ULLong dataSize);
		void BuildVAOAttrPointer();

	private:
		std::list<Attributes> m_VAOs;
	};
}