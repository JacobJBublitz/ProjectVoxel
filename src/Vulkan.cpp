#if ProjectVoxel_HAS_WinAPI
#define VK_USE_PLATFORM_WIN32_KHR

#include "Graphics/Window_WinAPI.h"

#endif
#ifdef ProjectVoxel_HAS_XCB
#define VK_USE_PLATFORM_XCB_KHR

#include "Graphics/Window_XCB.h"

#endif

#include "Vulkan.h"

#include "IO/Library.h"

#include <cstring>

using namespace ProjectVoxel;
using namespace ProjectVoxel::Vulkan;

#ifdef ProjectVoxel_HAS_WinAPI
static constexpr const char *VULKAN_LIBRARY_NAME = "vulkan-1.dll";
#else
static constexpr const char *VULKAN_LIBRARY_NAME = "vulkan";
#endif

void *Internal::GetInstanceProcAddr(Instance instance, const char *pName) {
	static ProjectVoxel::IO::Library vulkanLibrary(VULKAN_LIBRARY_NAME);
	static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
			(PFN_vkGetInstanceProcAddr) vulkanLibrary.GetSymbol("vkGetInstanceProcAddr");


	void *proc = (void *) vkGetInstanceProcAddr(instance,
	                                            pName);
	if (!proc) {
		throw std::string("No Vulkan instance function '") + pName + "'.";
	}

	return proc;
}

// <editor-fold Desc="GetInstanceProcAddr(nullptr, ...)">

Internal::Result Internal::CreateInstance(const InstanceCreateInfo *pCreateInfo,
                                          const AllocationCallbacks *pAllocator,
                                          Instance *pInstance) {
	static PFN_vkCreateInstance vkCreateInstance =
			(PFN_vkCreateInstance) GetInstanceProcAddr(nullptr,
			                                           "vkCreateInstance");

	return vkCreateInstance(pCreateInfo, pAllocator, pInstance);
}

Internal::Result Internal::EnumerateInstanceExtensionProperties(const char *pLayerName,
                                                                uint32_t *pPropertyCount,
                                                                ExtensionProperties *pProperties) {
	static PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties =
			(PFN_vkEnumerateInstanceExtensionProperties) GetInstanceProcAddr(nullptr,
			                                                                 "vkEnumerateInstanceExtensionProperties");

	return vkEnumerateInstanceExtensionProperties(pLayerName, pPropertyCount, pProperties);
}

// </editor-fold>

// <editor-fold Desc="GetInstanceProcAddr(instance, ...)">

Internal::Result Internal::CreateDevice(Vulkan::PhysicalDevice &physicalDevice, const DeviceCreateInfo *pCreateInfo,
                                        const AllocationCallbacks *pAllocator, Device *pDevice) {
	if (!physicalDevice.GetInstance().GetFuncPtrs().vkCreateDevice) {
		physicalDevice.GetInstance().GetFuncPtrs().vkCreateDevice =
				(PFN_vkCreateDevice) GetInstanceProcAddr(physicalDevice.GetInstance().GetHandle(), "vkCreateDevice");
	}

	return physicalDevice.GetInstance().GetFuncPtrs().vkCreateDevice(physicalDevice.GetHandle(), pCreateInfo,
	                                                                 pAllocator, pDevice);
}

Internal::Result Internal::CreateWin32SurfaceKHR(Vulkan::Instance &instance, Win32SurfaceCreateInfoKHR *pCreateInfo,
                                                 AllocationCallbacks *pAllocator, SurfaceKHR *pSurface) {
	if (!instance.GetFuncPtrs().vkCreateWin32SurfaceKHR) {
		instance.GetFuncPtrs().vkCreateWin32SurfaceKHR =
				(ProcTypes::CreateWin32SurfaceKHR) GetInstanceProcAddr(instance.GetHandle(), "vkCreateWin32SurfaceKHR");
	}

	return instance.GetFuncPtrs().vkCreateWin32SurfaceKHR(instance.GetHandle(), pCreateInfo, pAllocator, pSurface);
}

Internal::Result Internal::CreateXcbSurfaceKHR(Vulkan::Instance &instance,
                                               XcbSurfaceCreateInfoKHR *pCreateInfo,
                                               AllocationCallbacks *pAllocator,
                                               SurfaceKHR *pSurface) {
	if (!instance.GetFuncPtrs().vkCreateXcbSurfaceKHR) {
		instance.GetFuncPtrs().vkCreateXcbSurfaceKHR =
				(ProcTypes::CreateXcbSurfaceKHR) GetInstanceProcAddr(instance.GetHandle(), "vkCreateXcbSurfaceKHR");
	}

	return instance.GetFuncPtrs().vkCreateXcbSurfaceKHR(instance.GetHandle(), pCreateInfo, pAllocator, pSurface);
}

void Internal::DestroyDevice(Vulkan::Device &device, const AllocationCallbacks *pAllocator) {
	if (!device.GetInstance().GetFuncPtrs().vkDestroyDevice) {
		device.GetInstance().GetFuncPtrs().vkDestroyDevice = (PFN_vkDestroyDevice) GetInstanceProcAddr(
				device.GetInstance().GetHandle(), "vkDestroyDevice");
	}

	return device.GetInstance().GetFuncPtrs().vkDestroyDevice(device.GetHandle(), pAllocator);
}

void Internal::DestroyInstance(Instance instance, const AllocationCallbacks *pAllocator) {
	PFN_vkDestroyInstance vkDestroyInstance =
			(PFN_vkDestroyInstance) GetInstanceProcAddr(instance,
			                                            "vkDestroyInstance");

	return vkDestroyInstance(instance, pAllocator);
}

void Internal::DestroySurfaceKHR(Vulkan::Instance &instance, SurfaceKHR surface, AllocationCallbacks *pAllocator) {
	if (!instance.GetFuncPtrs().vkDestroySurfaceKHR) {
		instance.GetFuncPtrs().vkDestroySurfaceKHR =
				(PFN_vkDestroySurfaceKHR) GetInstanceProcAddr(instance.GetHandle(), "vkDestroySurfaceKHR");
	}

	return instance.GetFuncPtrs().vkDestroySurfaceKHR(instance.GetHandle(), surface, pAllocator);
}

Internal::Result Internal::EnumeratePhysicalDevices(Vulkan::Instance &instance,
                                                    uint32_t *pPhysicalDeviceCount,
                                                    PhysicalDevice *pPhysicalDevices) {
	if (!instance.GetFuncPtrs().vkEnumeratePhysicalDevices) {
		instance.GetFuncPtrs().vkEnumeratePhysicalDevices =
				(PFN_vkEnumeratePhysicalDevices) GetInstanceProcAddr(instance.GetHandle(),
				                                                     "vkEnumeratePhysicalDevices");
	}

	return instance.GetFuncPtrs().vkEnumeratePhysicalDevices(instance.GetHandle(),
	                                                         pPhysicalDeviceCount,
	                                                         pPhysicalDevices);
}

void *Internal::GetDeviceProcAddr(Vulkan::Device &device, const char *pName) {
	if (!device.GetInstance().GetFuncPtrs().vkGetDeviceProcAddr) {
		device.GetInstance().GetFuncPtrs().vkGetDeviceProcAddr =
				(PFN_vkGetDeviceProcAddr) GetInstanceProcAddr(device.GetInstance().GetHandle(), pName);
	}

	return (void *) device.GetInstance().GetFuncPtrs().vkGetDeviceProcAddr(device.GetHandle(), pName);
}

void Internal::GetPhysicalDeviceFeatures(Vulkan::PhysicalDevice &physicalDevice, PhysicalDeviceFeatures *pFeatures) {
	if (!physicalDevice.GetInstance().GetFuncPtrs().vkGetPhysicalDeviceFeatures) {
		physicalDevice.GetInstance().GetFuncPtrs().vkGetPhysicalDeviceFeatures =
				(PFN_vkGetPhysicalDeviceFeatures) GetInstanceProcAddr(physicalDevice.GetInstance().GetHandle(),
				                                                      "vkGetPhysicalDeviceFeatures");
	}

	return physicalDevice.GetInstance().GetFuncPtrs().vkGetPhysicalDeviceFeatures(physicalDevice.GetHandle(),
	                                                                              pFeatures);
}

void Internal::GetPhysicalDeviceProperties(Vulkan::PhysicalDevice &physicalDevice,
                                           PhysicalDeviceProperties *pProperties) {
	if (!physicalDevice.GetInstance().GetFuncPtrs().vkGetPhysicalDeviceProperties) {
		physicalDevice.GetInstance().GetFuncPtrs().vkGetPhysicalDeviceProperties =
				(PFN_vkGetPhysicalDeviceProperties) GetInstanceProcAddr(physicalDevice.GetInstance().GetHandle(),
				                                                        "vkGetPhysicalDeviceProperties");
	}

	return physicalDevice.GetInstance().GetFuncPtrs().vkGetPhysicalDeviceProperties(physicalDevice.GetHandle(),
	                                                                                pProperties);
}

void Internal::GetPhysicalDeviceQueueFamilyProperties(Vulkan::PhysicalDevice &physicalDevice,
                                                      uint32_t *pQueueFamilyPropertyCount,
                                                      QueueFamilyProperties *pQueueFamilyProperties) {
	if (!physicalDevice.GetInstance().GetFuncPtrs().vkGetPhysicalDeviceQueueFamilyProperties) {
		physicalDevice.GetInstance().GetFuncPtrs().vkGetPhysicalDeviceQueueFamilyProperties =
				(PFN_vkGetPhysicalDeviceQueueFamilyProperties) GetInstanceProcAddr(
						physicalDevice.GetInstance().GetHandle(),
						"vkGetPhysicalDeviceQueueFamilyProperties");
	}

	return physicalDevice.GetInstance().GetFuncPtrs().vkGetPhysicalDeviceQueueFamilyProperties(
			physicalDevice.GetHandle(),
			pQueueFamilyPropertyCount,
			pQueueFamilyProperties);
}

Internal::Result Internal::GetPhysicalDeviceSurfaceSupportKHR(Vulkan::PhysicalDevice &physicalDevice,
                                                              uint32_t queueFamilyIndex, Vulkan::SurfaceKHR &surface,
                                                              bool *pSupported) {
	if (!physicalDevice.GetInstance().GetFuncPtrs().vkGetPhysicalDeviceSurfaceSupportKHR) {
		physicalDevice.GetInstance().GetFuncPtrs().vkGetPhysicalDeviceSurfaceSupportKHR =
				(PFN_vkGetPhysicalDeviceSurfaceSupportKHR) GetInstanceProcAddr(physicalDevice.GetInstance().GetHandle(),
				                                                               "vkGetPhysicalDeviceSurfaceSupportKHR");
	}

	VkBool32 supported;
	Internal::Result result = physicalDevice.GetInstance().GetFuncPtrs().vkGetPhysicalDeviceSurfaceSupportKHR(
			physicalDevice.GetHandle(), queueFamilyIndex, surface.GetHandle(), &supported);
	*pSupported = supported == VK_TRUE;
	return result;
}

// </editor-fold>

// <editor-fold Desc="Device">

Device::Device(PhysicalDevice &physicalDevice, const std::vector<uint32_t> &queues,
               const std::vector<const char *> &extensions, const std::vector<const char *> &layers)
		: mPhysicalDevice(physicalDevice) {
	std::vector<Internal::DeviceQueueCreateInfo> queueCreateInfos;

	std::vector<std::vector<float>> queuePriorities(queues.size());

	for (uint32_t queueIndex = 0; queueIndex < queues.size(); queueIndex++) {
		if (queues[queueIndex] > 0) {
			queuePriorities[queueIndex] = std::vector<float>(queues[queueIndex], 1.0f);

			Internal::DeviceQueueCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.queueFamilyIndex = queueIndex;
			createInfo.queueCount = queues[queueIndex];
			createInfo.pQueuePriorities = queuePriorities[queueIndex].data();

			queueCreateInfos.push_back(createInfo);
		}
	}

	Internal::DeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.queueCreateInfoCount = (uint32_t) queueCreateInfos.size();
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.enabledLayerCount = (uint32_t) layers.size();
	createInfo.ppEnabledLayerNames = layers.data();
	createInfo.enabledExtensionCount = (uint32_t) extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.pEnabledFeatures = &mPhysicalDevice.GetFeatures();

	Internal::Result result = Internal::CreateDevice(mPhysicalDevice, &createInfo, nullptr, &mHandle);
	if (result != VK_SUCCESS) {
		throw "Failed to create device.";
	}
}

Device::~Device() {
	Internal::DestroyDevice(*this, nullptr);
	mHandle = nullptr;
}

Internal::Device Device::GetHandle() noexcept {
	return mHandle;
}

Instance &Device::GetInstance() noexcept {
	return mPhysicalDevice.GetInstance();
}

// </editor-fold>

// <editor-fold Desc="Instance">

Instance::Instance(const std::vector<const char *> &extensions, const std::vector<const char *> &layers) {
	Internal::ApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = "Project Voxel";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Project Voxel";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

	Internal::InstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledLayerCount = (uint32_t) layers.size();
	createInfo.ppEnabledLayerNames = layers.data();
	createInfo.enabledExtensionCount = (uint32_t) extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();

	Internal::Result result = Internal::CreateInstance(&createInfo, nullptr, &mHandle);
	if (result != VK_SUCCESS) {
		throw "Failed to create instance.";
	}
}

Instance::~Instance() {
	Internal::DestroyInstance(mHandle, nullptr);
	mHandle = nullptr;
}

Instance::FuncPtrs &Instance::GetFuncPtrs() noexcept {
	return mFuncPtrs;
}

Internal::Instance Instance::GetHandle() noexcept {
	return mHandle;
}

std::vector<PhysicalDevice> Instance::GetPhysicalDevices() {
	uint32_t deviceCount;
	Internal::Result result = Internal::EnumeratePhysicalDevices(*this, &deviceCount, nullptr);
	if (result != VK_SUCCESS) {
		throw "Failed to get physical device count.";
	}

	std::vector<Internal::PhysicalDevice> deviceHandles(deviceCount);
	result = Internal::EnumeratePhysicalDevices(*this, &deviceCount, deviceHandles.data());
	if (result != VK_SUCCESS) {
		throw "Failed to get physical device handles.";
	}

	std::vector<PhysicalDevice> devices;
	for (uint32_t i = 0; i < deviceCount; i++) {
		devices.push_back(std::move(PhysicalDevice(this, deviceHandles[i])));
	}

	return std::move(devices);
}

bool Instance::IsExtensionSupported(const char *extension, const char *layer) {
	uint32_t extensionCount;
	Internal::Result result = Internal::EnumerateInstanceExtensionProperties(layer, &extensionCount, nullptr);
	if (result != VK_SUCCESS) {
		throw "Failed to get instance extension count.";
	}

	std::vector<Internal::ExtensionProperties> extensions(extensionCount);
	result = Internal::EnumerateInstanceExtensionProperties(layer, &extensionCount, extensions.data());
	if (result != VK_SUCCESS) {
		throw "Failed to get instance extensions.";
	}

	for (const auto &extProp : extensions) {
		if (std::strcmp(extProp.extensionName, extension) == 0) {
			return true;
		}
	}

	return false;
}

// </editor-fold>

// <editor-fold Desc="PhysicalDevice">

PhysicalDevice::PhysicalDevice(Instance *instance, Internal::PhysicalDevice handle) {
	mInstance = instance;
	mHandle = handle;

	Internal::GetPhysicalDeviceFeatures(*this, &mFeatures);
	Internal::GetPhysicalDeviceProperties(*this, &mProperties);
}

const Internal::PhysicalDeviceFeatures &PhysicalDevice::GetFeatures() const noexcept {
	return mFeatures;
}

Internal::PhysicalDevice PhysicalDevice::GetHandle() noexcept {
	return mHandle;
}

Instance &PhysicalDevice::GetInstance() noexcept {
	return *mInstance;
}

std::vector<uint32_t> PhysicalDevice::GetPresentQueues(SurfaceKHR &surface) {
	uint32_t queueCount;
	Internal::GetPhysicalDeviceQueueFamilyProperties(*this, &queueCount, nullptr);

	std::vector<uint32_t> queuesSupportingSurface;

	for (uint32_t i = 0; i < queueCount; i++) {
		bool supportsSurface;

		Internal::Result result = Internal::GetPhysicalDeviceSurfaceSupportKHR(*this, i, surface, &supportsSurface);
		if (result != VK_SUCCESS) {
			throw "Failed to check if queue supports surface.";
		}

		if (supportsSurface) {
			queuesSupportingSurface.push_back(i);
		}
	}

	return std::move(queuesSupportingSurface);
}

const Internal::PhysicalDeviceProperties &PhysicalDevice::GetProperties() const noexcept {
	return mProperties;
}

std::vector<Internal::QueueFamilyProperties> PhysicalDevice::GetQueueProperties() {
	uint32_t queueCount;
	Internal::GetPhysicalDeviceQueueFamilyProperties(*this, &queueCount, nullptr);

	std::vector<Internal::QueueFamilyProperties> queues(queueCount);
	Internal::GetPhysicalDeviceQueueFamilyProperties(*this, &queueCount, queues.data());

	return std::move(queues);
}

// </editor-fold>

// <editor-fold Desc="SurfaceKHR">

SurfaceKHR::SurfaceKHR(Instance *instance, Graphics::Window &window) {
	mInstance = instance;
#ifdef ProjectVoxel_HAS_WinAPI
	try {
		Graphics::WinAPI::Window &winAPIWindow = dynamic_cast<Graphics::WinAPI::Window &>(window);

		Internal::Win32SurfaceCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.hinstance = GetModuleHandle(nullptr);
		createInfo.hwnd = winAPIWindow.GetHWnd();

		Internal::Result result = Internal::CreateWin32SurfaceKHR(*instance, &createInfo, nullptr, &mHandle);
		if (result == VK_SUCCESS) {
			return;
		}
	} catch (std::bad_cast e) {
		// Not a WinAPI window.
	}
#endif
#ifdef ProjectVoxel_HAS_XCB
	try {
		Graphics::XCB::Window &xcbWindow = dynamic_cast<Graphics::XCB::Window &>(window);

		VkXcbSurfaceCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.connection = xcbWindow.GetConnection();
		createInfo.window = xcbWindow;

		Internal::Result result = Internal::CreateXcbSurfaceKHR(*instance, &createInfo, nullptr, &mHandle);
		if (result == VK_SUCCESS) {
			return;
		}
	} catch (std::bad_cast e) {
		// Not a XCB window.
	}
#endif

	throw "Failed to create a surface.";
}

SurfaceKHR::~SurfaceKHR() {
	Internal::DestroySurfaceKHR(*mInstance, mHandle, nullptr);
	mHandle = nullptr;
}

Internal::SurfaceKHR SurfaceKHR::GetHandle() noexcept {
	return mHandle;
}

std::vector<const char *> SurfaceKHR::GetRequiredInstanceExtensions(const Graphics::Window &window) {
#ifdef ProjectVoxel_HAS_WinAPI
	try {
		const Graphics::WinAPI::Window &winAPIWindow = dynamic_cast<const Graphics::WinAPI::Window &>(window);

		return { "VK_KHR_surface", "VK_KHR_win32_surface" };
	} catch (std::bad_cast e) {
		// Not a WinAPI window.
	}
#endif
#ifdef ProjectVoxel_HAS_XCB
	try {
		const Graphics::XCB::Window &xcbWindow = dynamic_cast<const Graphics::XCB::Window &>(window);

		return {"VK_KHR_surface", "VK_KHR_xcb_surface"};
	} catch (std::bad_cast e) {
		// Not a XCB window.
	}
#endif

	return {};
}

// </editor-fold>
