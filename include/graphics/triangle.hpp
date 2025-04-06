//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <memory>

#include "shader.hpp"

namespace Graphics
{
	using std::unique_ptr;

	using KalaKit::Shader;

	class Triangle
	{
	public:
		static void Initialize();
		static void Render();
	private:
		static inline GLuint vao;
		static inline GLuint vbo;
		static inline unique_ptr<Shader> shader;
	};
}