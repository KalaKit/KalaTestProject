//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include "graphics/renderprogram.hpp"

using KalaTestProject::Graphics::RenderProgram;

int main()
{
	if (!RenderProgram::Initialize(
		"Test 1234",
		640,
		480,
		16,
		200))
	{
		return 0;
	}

	RenderProgram::Render();

	return 0;
}