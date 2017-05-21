#include "Renderer_Vulkan.h"

#include <algorithm>

using namespace ProjectVoxel::Graphics::Vulkan;

static bool _isDeviceSuitable(const PhysicalDevice &physicalDevice) {
	return true;
}

static size_t _getDeviceScore(const PhysicalDevice &physicalDevice) {
	size_t score = 0;

	switch (physicalDevice.GetProperties().deviceType) {
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			score += 1000;
			break;
		default:
			break;
	}

	return score;
}

Renderer::Renderer()
		: mWindow() {
	std::vector<const char *> extensions, layers;
	for (const auto &ext : Vulkan::SurfaceKHR::GetRequiredInstanceExtensions(mWindow)) {
		if (!Vulkan::Instance::IsExtensionSupported(ext)) {
			throw "Window does not support Vulkan.";
		}

		extensions.push_back(ext);
	}

	mInstance = std::make_unique<Instance>(extensions, layers);

	mSurface = std::make_unique<SurfaceKHR>(mInstance.get(), mWindow);

	auto devices = mInstance->GetPhysicalDevices();

	std::vector<PhysicalDevice> suitableDevices;
	for (auto &device : devices) {
		if (_isDeviceSuitable(device)) {
			suitableDevices.push_back(device);
		}
	}

	std::sort(suitableDevices.begin(), suitableDevices.end(), [](const PhysicalDevice &a, const PhysicalDevice &b) {
		return _getDeviceScore(a) < _getDeviceScore(b);
	});

	extensions.clear();
	layers.clear();

	bool hasGraphicsQueue = false;
	uint32_t graphicsQueueIndex = 0;

	auto queueProperties = suitableDevices[0].GetQueueProperties();
	for (uint32_t i = 0; i < queueProperties.size(); i++) {
		if (!hasGraphicsQueue) {
			if (queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				graphicsQueueIndex = i;

				hasGraphicsQueue = true;
			}
		}
	}

	auto presentQueues = suitableDevices[0].GetPresentQueues(*mSurface);

	std::vector<uint32_t> queues(std::max(graphicsQueueIndex, presentQueues[0]) + 1, 0);
	queues[graphicsQueueIndex] = 1;
	queues[presentQueues[0]] = 1;

	mDevice = std::make_unique<Device>(suitableDevices[0], queues, extensions, layers);
}

Renderer::~Renderer() {}

ProjectVoxel::Graphics::Window &Renderer::GetWindow() noexcept {
	return mWindow;
}

void Renderer::SwapBuffers() {}
