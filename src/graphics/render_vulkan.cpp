//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#ifdef KALAWINDOW_SUPPORT_VULKAN

#include <thread>
#include <chrono>
#include <iostream>

#include "core/input.hpp"
#include "graphics/render.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/vulkan.hpp"

#include "graphics/renderprogram.hpp"

using KalaWindow::Core::Input;
using KalaWindow::Graphics::Render;
using KalaWindow::Graphics::Window;
using KalaWindow::Graphics::Renderer_Vulkan;
using KalaWindow::Graphics::VulkanLayers;
using KalaWindow::Graphics::VulkanInstanceExtensions;
using KalaWindow::Graphics::VulkanDeviceExtensions;
using KalaWindow::Graphics::FrameResult;

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

		newWindow->SetMinSize(kvec2{ 640, 480 });
		newWindow->SetMaxSize(kvec2{ 3840, 2160 });

		newWindow->SetRedrawCallback(Redraw);

		activeSleep = newActiveSleep;
		idleSleep = newIdleSleep;

		Renderer_Vulkan::EnableLayer(VulkanLayers::V_IL_khronos_validation);
		Renderer_Vulkan::EnableInstanceExtension(VulkanInstanceExtensions::V_IE_khr_surface);
		Renderer_Vulkan::EnableDeviceExtension(VulkanDeviceExtensions::V_DE_khr_swapchain);
		Renderer_Vulkan::EnableInstanceExtension(VulkanInstanceExtensions::V_IE_khr_win32_surface);

		if (!Renderer_Vulkan::Initialize(2)) return false;
		if (!Renderer_Vulkan::CreateSyncObjects(newWindow)) return false;

		Renderer_Vulkan::CreateVulkanSurface(newWindow);

		if (!Renderer_Vulkan::CreateSwapchain(newWindow)) return false;

		if (!Renderer_Vulkan::CreateRenderPass(newWindow)) return false;
		if (!Renderer_Vulkan::CreateFramebuffers(newWindow)) return false;

		if (!Renderer_Vulkan::CreateCommandPool(newWindow)) return false;
		if (!Renderer_Vulkan::CreateCommandBuffer(newWindow)) return false;

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
		uint32_t imageIndex = 0;
		FrameResult result = Renderer_Vulkan::BeginFrame(newWindow, imageIndex);

		if (result == FrameResult::VK_FRAME_RESIZE_NEEDED)
		{
			Renderer_Vulkan::HardReset(newWindow);
			lastFrameTime = steady_clock::now();
			return;
		}
		else if (result == FrameResult::VK_FRAME_ERROR)
		{
			cout << "    Error: Failed to begin frame! Skipping draw...\n";
			Renderer_Vulkan::HardReset(newWindow);
			lastFrameTime = steady_clock::now();
			return;
		}

		if (!Renderer_Vulkan::RecordCommandBuffer(newWindow, imageIndex))
		{
			cout << "    Error: Failed to record command buffer! Skipping draw...\n";
			Renderer_Vulkan::HardReset(newWindow);
			lastFrameTime = steady_clock::now();
			return;
		}

		if (!Renderer_Vulkan::SubmitFrame(newWindow, imageIndex))
		{
			cout << "    Error: Failed to submit frame! Skipping draw...\n";
			Renderer_Vulkan::HardReset(newWindow);
			lastFrameTime = steady_clock::now();
			return;
		}

		result = Renderer_Vulkan::PresentFrame(newWindow, imageIndex);
		if (result == FrameResult::VK_FRAME_RESIZE_NEEDED)
		{
			Renderer_Vulkan::HardReset(newWindow);
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