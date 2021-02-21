#pragma once

#include <vector>

#include "FigureDraw.h"

namespace GameEngine {

	class MultiFigure3 : public FigureDraw {
	public:


	private:
		std::vector<GLuint> m_VAOs;
	};
}