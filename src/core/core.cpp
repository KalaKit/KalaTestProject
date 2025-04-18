//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <string>

//external
#include "window.hpp"
#include "opengl.hpp"
#include "opengl_loader.hpp"
#include "enums.hpp"

//project
#include "core.hpp"
#include "triangle.hpp"

using std::cout;
using std::cin;
using std::filesystem::current_path;
using std::string;

using KalaKit::KalaWindow;
using KalaKit::OpenGL;
using KalaKit::OpenGLLoader;
using KalaKit::DebugType;
using Graphics::Triangle;
using KalaKit::PopupAction;
using KalaKit::PopupType;
using KalaKit::PopupResult;

namespace Project
{
	void Core::Initialize()
	{
		//KalaWindow::SetDebugType(DebugType::DEBUG_FREETYPE_FONT_TEST);

		bool initializeOpenGL = true;
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

		//initialize window title font
		string fontPath = (current_path() / "files" / "fonts" / "roboto-condensed" / "RobotoCondensed-Regular.ttf").string();

		bool loadedFont = KalaWindow::freeType->LoadFont(fontPath, 24);
		if (loadedFont)
		{
			cout << "Successfully loaded font '" << fontPath << "'!\n";
		}

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

			if (OpenGL::isInitialized) RedrawCallback();
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