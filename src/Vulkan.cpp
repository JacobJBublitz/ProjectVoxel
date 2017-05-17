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

Internal::Result Internal::CreateWin32SurfaceKHR(Vulkan::Instance &instance, Win32SurfaceCreateInfoKHR *pCreateInfo, AllocationCallbacks *pAllocator, SurfaceKHR *pSurface) {
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
}

Internal::PhysicalDevice PhysicalDevice::GetHandle() noexcept {
	return mHandle;
}

Instance &PhysicalDevice::GetInstance() noexcept {
	return *mInstance;
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