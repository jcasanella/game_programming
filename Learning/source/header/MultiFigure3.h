#pragma once

#include <list>
#include<vector>

#include "FigureDraw.h"

namespace GameEngine {

	typedef struct {
		GLuint vao;
		int vertexDraw;
	} Attributes;

	class MultiFigure3 : public FigureDraw {
	public:
		MultiFigure3(const GLfloat* data, ULLong dataSize, const std::vector<GLuint>& programIds);

		void AddFigureToDraw(const GLfloat* data, ULLong dataSize);
		virtual void Draw();

	private:
		void DrawFigure(const Attributes& attr);
		GLuint PrepareVAO(const GLfloat* data, ULLong dataSize);
		void BuildVAOAttrPointer();

	private:
		std::list<Attributes> m_VAOs;
		std::vector<GLuint> m_programIds;
	};
}