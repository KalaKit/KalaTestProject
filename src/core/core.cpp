//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>

//external
#include "window.hpp"
#include "opengl_loader.hpp"

//project
#include "core.hpp"
#include "triangle.hpp"

using std::cout;
using std::cin;

using KalaKit::KalaWindow;
using KalaKit::OpenGLLoader;
using KalaKit::DebugType;
using Graphics::Triangle;

namespace Project
{
	void Core::Initialize()
	{
		bool initialized = KalaWindow::Initialize("window", 800, 600);
		if (!initialized)
		{
			cout << "Error: Failed to initialize KalaWindow!\n";
			return;
		}

#ifdef KALAKIT_WINDOWS
		hdc = GetDC(KalaWindow::window);
#endif

		OpenGLLoader::glDisablePtr(GL_BLEND);      //no transparency
		OpenGLLoader::glDisablePtr(GL_CULL_FACE);  //don't discard faces
		OpenGLLoader::glDisablePtr(GL_DEPTH_TEST); //no depth test

		Triangle::Initialize();

		KalaWindow::SetRedrawCallback(RedrawCallback);
	}
		
	void Core::Update()
	{
		while (!KalaWindow::ShouldClose())
		{
			KalaWindow::Update();

			RedrawCallback();
		}
	}

	void Core::RedrawCallback()
	{
		OpenGLLoader::glClearColorPtr(0.1f, 0.1f, 0.1f, 1.0f); //dark gray
		OpenGLLoader::glClearPtr(GL_COLOR_BUFFER_BIT);

		Triangle::Render();

#ifdef KALAKIT_WINDOWS
		SwapBuffers(hdc);
#endif
	}
}