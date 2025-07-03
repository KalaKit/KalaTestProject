//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <string>

namespace KalaTestProject::Graphics
{
	using std::string;

	class RenderProgram
	{
	public:
		static bool Initialize(
			const string& title,
			unsigned int width,
			unsigned int height,
			unsigned int newActiveSleep,
			unsigned int newIdleSleep);
		static void Render();
	private:
		static inline bool isInitialized = false;
		static inline unsigned int activeSleep{};
		static inline unsigned int idleSleep{};

		static void Redraw();
		static void SleepFor(unsigned int ms);
	};
}