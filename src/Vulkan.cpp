#include "Vulkan.h"

#include "IO/Library.h"

#include <memory>

using namespace ProjectVoxel;

Vulkan::Instance::Instance(const std::vector<const char *> &extensions, const std::vector<const char *> &layers) {
	vkCreateInstance = reinterpret_cast<PFN_vkCreateInstance>(GetProcAddr("vkCreateInstance"));
	vkEnumerateInstanceExtensionProperties = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(GetProcAddr("vkEnumerateInstanceExtensionProperties"));
	vkEnumerateInstanceLayerProperties = reinterpret_cast<PFN_vkEnumerateInstanceLayerProperties>(GetProcAddr("vkEnumerateInstanceLayerProperties"));

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = "Project Voxel";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Project Voxel";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 5);

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledLayerCount = (uint32_t) layers.size();
	createInfo.ppEnabledLayerNames = layers.data();
	createInfo.enabledExtensionCount = (uint32_t) extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkResult result = vkCreateInstance(&createInfo, nullptr, &mHandle);
	if (result != VK_SUCCESS) {
		throw "Failed to create Vulkan instance.";
	}

	vkEnumeratePhysicalDevices = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(GetProcAddr("vkEnumeratePhysicalDevices"));
	vkGetPhysicalDeviceProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceProperties>(GetProcAddr("vkGetPhysicalDeviceProperties"));
	vkGetPhysicalDeviceQueueFamilyProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(GetProcAddr("vkGetPhysicalDeviceQueueFamilyProperties"));
}

Vulkan::Instance::~Instance() {
	if (mHandle) {
		reinterpret_cast<PFN_vkDestroyInstance >(GetProcAddr("vkDestroyInstance"))(mHandle, nullptr);
	}
}

std::vector<Vulkan::PhysicalDevice> Vulkan::Instance::GetPhysicalDevices() {
	uint32_t deviceCount;
	VkResult result = vkEnumeratePhysicalDevices(mHandle, &deviceCount, nullptr);
	if (result != VK_SUCCESS) {
		throw "Failed to retrieve physical device count.";
	}

	std::vector<VkPhysicalDevice> deviceHandles(deviceCount);
	result = vkEnumeratePhysicalDevices(mHandle, &deviceCount, deviceHandles.data());
	if (result != VK_SUCCESS) {
		throw "Failed to retrieve physical device handles.";
	}

	std::vector<PhysicalDevice> devices;
	for (uint32_t i = 0; i < deviceCount; i++) {
		devices.push_back(std::move(PhysicalDevice(*this, deviceHandles[i])));
	}

	return std::move(devices);
}

void *Vulkan::Instance::GetProcAddr(const char *pName) const {
	static IO::Library vulkan("vulkan");
	static PFN_vkGetInstanceProcAddr func = (PFN_vkGetInstanceProcAddr) vulkan.GetSymbol("vkGetInstanceProcAddr");

	void *proc = reinterpret_cast<void *>(func(mHandle, pName));
	if (!proc) {
		throw std::string("Failed to load instance function '") + pName + "'.";
	}
	return proc;
}

Vulkan::Instance::operator VkInstance() noexcept {
	return mHandle;
}

Vulkan::PhysicalDevice::PhysicalDevice(Instance &instance, VkPhysicalDevice device)
		: mHandle(device), mInstance(instance) {
	mInstance.vkGetPhysicalDeviceProperties(mHandle, &mProperites);

	uint32_t queueFamilyCount;
	mInstance.vkGetPhysicalDeviceQueueFamilyProperties(mHandle, &queueFamilyCount, nullptr);
	mQueueFamilyProperties.resize(queueFamilyCount);
	mInstance.vkGetPhysicalDeviceQueueFamilyProperties(mHandle, &queueFamilyCount, mQueueFamilyProperties.data());
}

Vulkan::PhysicalDevice::PhysicalDevice(const PhysicalDevice &other)
		: mHandle(other.mHandle), mInstance(other.mInstance), mProperites(other.mProperites),
		  mQueueFamilyProperties(other.mQueueFamilyProperties) {}

Vulkan::Instance &Vulkan::PhysicalDevice::GetInstance() noexcept {
	return mInstance;
}

Vulkan::PhysicalDevice::operator VkPhysicalDevice() noexcept {
	return mHandle;
}