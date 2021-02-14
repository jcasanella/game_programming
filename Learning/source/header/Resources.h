#pragma once

namespace GameEngine {

	const char* VERTEX_SHADER_LOCATION = "Shaders/VertexShader.glsl";
	const char* VERTEX_SHADER_A_LOCATION = "Shaders/VertexShaderA.glsl";
	const char* VERTEX_SHADER_COLOR = "Shaders/VertexShader3.glsl";

	const char* FRAGMENT_SHADER_LOCATION = "Shaders/FragmentShader.glsl";
	const char* FRAGMENT_SHADER_A_LOCATION = "Shaders/FragmentShaderA.glsl";
	const char* FRAGMENT_SHADER2_LOCATION = "Shaders/FragmentShader2.glsl";
	const char* FRAGMENT_SHADER_UNIFORM_LOCATION = "Shaders/FragmentShaderUniform.glsl";
	const char* FRAGMENT_SHADER_COLOR = "Shaders/FragmentShader3.glsl";

	const int NUM_SHADERS = 3;
	const char* SHADERS[3][2] = {
		{ VERTEX_SHADER_A_LOCATION, FRAGMENT_SHADER_A_LOCATION },		// Red
		{ VERTEX_SHADER_LOCATION, FRAGMENT_SHADER2_LOCATION },			// Blue
		{ VERTEX_SHADER_LOCATION, FRAGMENT_SHADER_UNIFORM_LOCATION }	// Changing color
	};

}