//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>

//external
#include "window.hpp"
#include "input.hpp"
#include "crashHandler.hpp"
#include "opengl.hpp"
#include "opengl_loader.hpp"

//project
#include "core.hpp"
#include "triangle.hpp"

using std::cout;
using std::cin;

using KalaKit::KalaWindow;
using KalaKit::KalaInput;
using KalaKit::KalaCrashHandler;
using KalaKit::DebugType;
using KalaKit::OpenGL;
using KalaKit::OpenGLLoader;
using Graphics::Triangle;

namespace Project
{
	void Core::Initialize()
	{
		KalaCrashHandler::Initialize();

		KalaWindow::Initialize("window", 800, 600);

		OpenGL::Initialize();
		OpenGLLoader::LoadAllFunctions();

		OpenGLLoader::glViewportPtr(0, 0, 800, 600);
		OpenGLLoader::glDisablePtr(GL_DEPTH_TEST);

		KalaInput::Initialize();

		Triangle::Initialize();

		//KalaWindow::SetDebugType(DebugType::DEBUG_WINDOW_CORNER_EDGE);
	}
		
	void Core::Update()
	{
		while (!KalaWindow::ShouldClose())
		{
			KalaWindow::Update();

			Triangle::Render();
		}
	}
}