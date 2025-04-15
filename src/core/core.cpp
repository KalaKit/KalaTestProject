//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>
#include <cstdlib>

//external
#include "window.hpp"
#include "opengl.hpp"
#include "opengl_loader.hpp"

//project
#include "core.hpp"
#include "triangle.hpp"

using std::cout;
using std::cin;

using KalaKit::KalaWindow;
using KalaKit::OpenGL;
using KalaKit::OpenGLLoader;
using KalaKit::DebugType;
using Graphics::Triangle;

namespace Project
{
	void Core::Initialize()
	{
		bool initializeOpenGL = false;
		bool initialized = KalaWindow::Initialize(
			"window", 
			800, 
			600,
			initializeOpenGL);
		if (!initialized)
		{
			cout << "Error: Failed to initialize KalaWindow!\n";
			return;
		}

		//KalaWindow::SetDebugType(DebugType::DEBUG_WAYLAND_CALLBACK_CHECK);

		if (initializeOpenGL)
		{
			OpenGLLoader::glDisablePtr(GL_BLEND);      //no transparency
			OpenGLLoader::glDisablePtr(GL_CULL_FACE);  //don't discard faces
			OpenGLLoader::glDisablePtr(GL_DEPTH_TEST); //no depth test
	
			Triangle::Initialize();
	
			KalaWindow::SetRedrawCallback(RedrawCallback);
		}
	}
		
	void Core::Update()
	{
		cout << "!!!!! UPDATE START !!!!!\n";

		while (!KalaWindow::ShouldClose())
		{
			KalaWindow::Update();

			//RedrawCallback();
		}

		cout << "!!!!! UPDATE END !!!!!\n";
	}

	void Core::RedrawCallback()
	{
		OpenGLLoader::glClearColorPtr(0.1f, 0.1f, 0.1f, 1.0f); //dark gray
		OpenGLLoader::glClearPtr(GL_COLOR_BUFFER_BIT);

		Triangle::Render();

		KalaWindow::SwapBuffers(OpenGL::context);
	}
}