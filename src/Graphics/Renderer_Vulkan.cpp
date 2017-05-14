#include "Renderer_Vulkan.h"

using namespace ProjectVoxel::Graphics::Vulkan;

Renderer::Renderer()
		: mWindow(Window::Create()), mInstance({}, {}) {
	auto devices = mInstance.GetPhysicalDevices();
}

Renderer::~Renderer() {}

std::shared_ptr<ProjectVoxel::Graphics::Window> Renderer::GetWindow() noexcept {
	return mWindow;
}

void Renderer::SwapBuffers() {}