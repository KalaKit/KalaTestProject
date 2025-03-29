//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <Windows.h>

namespace Project
{
	class Core

	{
	public:
		static void Initialize();
		static void Update();
		static void RedrawCallback();
	private:
		static inline HDC hdc;
	};
}