//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#ifdef KALAWINDOW_SUPPORT_VULKAN

#include <thread>
#include <chrono>
#include <iostream>

#include "graphics/render.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/vulkan.hpp"

#include "graphics/renderprogram.hpp"

using KalaWindow::Graphics::Render;
using KalaWindow::Graphics::Window;
using KalaWindow::Graphics::Renderer_Vulkan;
using KalaWindow::Graphics::VulkanExtensions;
using KalaWindow::Graphics::FrameResult;

using std::thread;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;
using std::chrono::steady_clock;
using dur = std::chrono::steady_clock::duration;
using std::chrono::time_point;
using std::cout;

static Window* mainWindow{};
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
		if (!Render::Initialize(
			title,
			width,
			height))
		{
			return false;
		}

		mainWindow = Window::windows.front().get();
		mainWindow->SetMinSize(kvec2{ 640, 480 });
		mainWindow->SetMaxSize(kvec2{ 3840, 2160 });

		mainWindow->SetRedrawCallback(Redraw);

		activeSleep = newActiveSleep;
		idleSleep = newIdleSleep;

		Renderer_Vulkan::EnableExtension(VulkanExtensions::VE_Surface);
		Renderer_Vulkan::EnableExtension(VulkanExtensions::VE_KhrSwapchain);
		Renderer_Vulkan::EnableExtension(VulkanExtensions::VE_Win32Surface);

		Renderer_Vulkan::Initialize();

		Renderer_Vulkan::CreateVulkanSurface(mainWindow);

		Renderer_Vulkan::CreateSwapchain(mainWindow);

		Renderer_Vulkan::CreateRenderPass();
		Renderer_Vulkan::CreateFramebuffers();

		Renderer_Vulkan::CreateCommandPool();
		Renderer_Vulkan::CreateCommandBuffer();

		return true;
	}

	void RenderProgram::Render()
	{
		isInitialized = true;
		while (isInitialized)
		{
			Window::Update(mainWindow);

			if (!mainWindow->IsIdle())
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
		uint32_t imageIndex = 0;
		FrameResult result = Renderer_Vulkan::BeginFrame(imageIndex);

		if (result == FrameResult::VK_FRAME_RESIZE_NEEDED)
		{
			Renderer_Vulkan::DestroySwapchain();
			Renderer_Vulkan::CreateSwapchain(mainWindow);
			Renderer_Vulkan::CreateRenderPass();
			Renderer_Vulkan::CreateFramebuffers();
			Renderer_Vulkan::CreateCommandBuffer();
			lastFrameTime = steady_clock::now();
			return;
		}
		else if (result == FrameResult::VK_FRAME_ERROR)
		{
			cout << "    Error: Failed to begin frame! Skipping draw...\n";
			lastFrameTime = steady_clock::now();
			return;
		}

		if (!Renderer_Vulkan::RecordCommandBuffer(imageIndex))
		{
			cout << "    Error: Failed to record command buffer! Skipping draw...\n";
			lastFrameTime = steady_clock::now();
			return;
		}

		if (!Renderer_Vulkan::SubmitFrame(imageIndex))
		{
			cout << "    Error: Failed to submit frame! Skipping draw...\n";
			lastFrameTime = steady_clock::now();
			return;
		}

		result = Renderer_Vulkan::PresentFrame(imageIndex);
		if (result == FrameResult::VK_FRAME_RESIZE_NEEDED)
		{
			Renderer_Vulkan::DestroySwapchain();
			Renderer_Vulkan::CreateSwapchain(mainWindow);
			Renderer_Vulkan::CreateRenderPass();
			Renderer_Vulkan::CreateFramebuffers();
			Renderer_Vulkan::CreateCommandBuffer();
			lastFrameTime = steady_clock::now();
			return;
		}
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

#endif //KALAWINDOW_SUPPORT_VULKAN