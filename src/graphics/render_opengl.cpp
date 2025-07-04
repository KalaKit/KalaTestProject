//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#ifdef KALAWINDOW_SUPPORT_OPENGL

#include <thread>
#include <chrono>
#include <iostream>

#include "core/input.hpp"
#include "graphics/render.hpp"
#include "graphics/window.hpp"
#include "graphics/opengl/opengl.hpp"
#include "graphics/opengl/opengl_loader.hpp"

#include "graphics/renderprogram.hpp"

using KalaWindow::Core::Input;
using KalaWindow::Graphics::Render;
using KalaWindow::Graphics::Window;
using KalaWindow::Graphics::Renderer_OpenGL;
using KalaWindow::Graphics::OpenGLLoader;

using std::thread;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;
using std::chrono::steady_clock;
using dur = std::chrono::steady_clock::duration;
using std::chrono::time_point;
using std::cout;

static Window* newWindow{};
time_point<steady_clock> lastFrameTime{};

namespace KalaTestProject::Graphics
{
	bool RenderProgram::Initialize(
		const string& title,
		unsigned int width,
		unsigned int height,
		unsigned int newActiveSleep,
		unsigned int newIdleSleep)
	{
		Render::Initialize();

		newWindow = Window::Initialize(
			title,
			width,
			height);
		if (newWindow == nullptr) return false;

		if (!Input::Initialize(newWindow)) return false;

		if (!Renderer_OpenGL::Initialize(newWindow))
		{
			return false;
		}

		newWindow->SetMinSize(kvec2{ 640, 480 });
		newWindow->SetMaxSize(kvec2{ 3840, 2160 });

		newWindow->SetRedrawCallback(Redraw);

		activeSleep = newActiveSleep;
		idleSleep = newIdleSleep;

		return true;
	}

	void RenderProgram::Render()
	{
		isInitialized = true;
		while (isInitialized)
		{
			Window::Update(newWindow);

			if (!newWindow->IsIdle())
			{
				Redraw();
				SleepFor(activeSleep);
			}
			else SleepFor(idleSleep);
		}

		Render::Shutdown();
	}

	void RenderProgram::Redraw()
	{
		OpenGLLoader::glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //dark gray
		OpenGLLoader::glClear(GL_COLOR_BUFFER_BIT);

		//Triangle::Render();

		Renderer_OpenGL::SwapOpenGLBuffers(newWindow);
	}

	void RenderProgram::SleepFor(unsigned int ms)
	{
		milliseconds convertedMS = milliseconds(ms);

		time_point<steady_clock> now = steady_clock::now();
		dur frameDuration = now - lastFrameTime;

		if (frameDuration < convertedMS) sleep_for(convertedMS - frameDuration);
		lastFrameTime = steady_clock::now();
	}
}

#endif //KALAWINDOW_SUPPORT_OPENGL