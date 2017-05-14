#include "Renderer_Vulkan.h"

using namespace ProjectVoxel::Graphics::Vulkan;

Renderer::Renderer()
		: mWindow(Window::Create()) {
	std::vector<const char *> extensions, layers;
	for (const auto &ext : Vulkan::SurfaceKHR::GetRequiredInstanceExtensions(*mWindow.get())) {
		if (!Vulkan::Instance::IsExtensionSupported(ext)) {
			throw "Window does not support Vulkan.";
		}

		extensions.push_back(ext);
	}

	mInstance = std::make_unique<Vulkan::Instance>(extensions, layers);

	mSurface = std::make_unique<Vulkan::SurfaceKHR>(mInstance.get(), *mWindow.get());

	auto devices = mInstance->GetPhysicalDevices();
}

Renderer::~Renderer() {}

std::shared_ptr<ProjectVoxel::Graphics::Window> Renderer::GetWindow() noexcept {
	return mWindow;
}

void Renderer::SwapBuffers() {}