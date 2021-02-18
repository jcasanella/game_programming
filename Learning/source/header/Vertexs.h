#pragma once

#include <glad/glad.h>

namespace GameEngine {

	// Triangle
	const GLfloat vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	// Rectangle sharing indexes
	const GLfloat vertex_buffer_data2[] = {
		0.5f, 0.5f, 0.0f,	// Top Right
		0.5f, -0.5f, 0.0f,	// Bottom Right
		-0.5f, -0.5f, 0.0f,	// Bottom Left
		-0.5f, 0.5f, 0.0f	// Top Left
	};

	const GLuint indexes_data[] = {
		0, 1, 3,	// First triangle
		1, 2, 3		// Second triangle
	};

	// Double triangle
	const GLfloat vertex_buffer_data3[] = {
	-1.0f, -1.0f, 0.0f,	// Left Bottom Triangle1
	-1.0f, 1.0f, 0.0f,	// Left Up Triangle1 
	0.0f,  0.0f, 0.0f,	// Middle Triangle1

	0.0f, -1.0f, 0.0f,	// Left Bottom Triangle2
	0.0f, 1.0f, 0.0f,	// Left Up Triangle2
	1.0f, 0.0f, 0.0f,	// Middle Triangle2
	};

	// Triangle
	const GLfloat vertex_buffer_data4[] = {
		-1.0f, -1.0f, 0.0f,	// Left Bottom Triangle1
		-1.0f, 1.0f, 0.0f,	// Left Up Triangle1 
		0.0f,  0.0f, 0.0f,	// Middle Triangle1
	};

	// Triangle
	const GLfloat vertex_buffer_data5[] = {
		0.0f, -1.0f, 0.0f,	// Left Bottom Triangle2
		0.0f, 1.0f, 0.0f,	// Left Up Triangle2
		1.0f, 0.0f, 0.0f,	// Middle Triangle2
	};

	// Triangle with color
	const GLfloat vertex_buffer_with_colors[] = {
	0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,		// bottom right and color red
	-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	// bottom left and color green
	0.0f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f		// top and color blue
	};
}