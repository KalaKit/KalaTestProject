//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>

//external
#include "window.hpp"
#include "input.hpp"
#include "crashHandler.hpp"

//project
#include "core.hpp"

using std::cout;
using std::cin;

using KalaKit::KalaWindow;
using KalaKit::KalaInput;
using KalaKit::KalaCrashHandler;
using KalaKit::DebugType;

namespace Project
{
	void Core::Initialize()
	{
		KalaCrashHandler::Initialize();
		KalaWindow::Initialize("window", 800, 600);
		KalaInput::Initialize();

		KalaWindow::SetDebugType(DebugType::DEBUG_WINDOW_CORNER_EDGE);
	}
		
	void Core::Update()
	{
		cout << "Reached render loop!\n";

		while (!KalaWindow::ShouldClose())
		{
			KalaWindow::Update();
		}
	}
}