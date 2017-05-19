#pragma once

#include "Graphics/Window.h"

#include <array>
#include <string>
#include <vector>

#define VK_NO_PROTOTYPES
extern "C" {
#include <vulkan/vulkan.h>
}

namespace ProjectVoxel {
	namespace Vulkan {
		class Device;

		class Instance;

		class PhysicalDevice;

		class SurfaceKHR;

		namespace Internal {
			// <editor-fold Desc="Types">
			using AllocationCallbacks = ::VkAllocationCallbacks;
			using ApplicationInfo = ::VkApplicationInfo;
			using Device = ::VkDevice;
			using DeviceCreateInfo = ::VkDeviceCreateInfo;
			using DeviceQueueCreateInfo = ::VkDeviceQueueCreateInfo;
			using ExtensionProperties = ::VkExtensionProperties;
			using Instance = ::VkInstance;
			using InstanceCreateInfo = ::VkInstanceCreateInfo;
			using PhysicalDevice = ::VkPhysicalDevice;
			using PhysicalDeviceFeatures = ::VkPhysicalDeviceFeatures;
			using PhysicalDeviceProperties = ::VkPhysicalDeviceProperties;
			using QueueFamilyProperties = ::VkQueueFamilyProperties;
			using Result = ::VkResult;
			using SurfaceKHR = ::VkSurfaceKHR;

#ifdef VK_USE_PLATFORM_WIN32_KHR
			using Win32SurfaceCreateInfoKHR = ::VkWin32SurfaceCreateInfoKHR;
#else
			using Win32SurfaceCreateInfoKHR = void;
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
			using XcbSurfaceCreateInfoKHR = ::VkXcbSurfaceCreateInfoKHR;
#else
			using XcbSurfaceCreateInfoKHR = void;
#endif
			namespace ProcTypes {
				typedef Result(VKAPI_PTR *CreateWin32SurfaceKHR)(Instance, Win32SurfaceCreateInfoKHR *,
				                                                 AllocationCallbacks *, SurfaceKHR *);

				typedef Result(VKAPI_PTR *CreateXcbSurfaceKHR)(Instance, XcbSurfaceCreateInfoKHR *,
				                                               AllocationCallbacks *, SurfaceKHR *);
			}

			// </editor-fold>

			void *GetInstanceProcAddr(Instance instance, const char *pName);

			// <editor-fold Desc="GetInstanceProcAddr(nullptr, ...)">

			Result CreateInstance(const InstanceCreateInfo *pCreateInfo,
			                      const AllocationCallbacks *pAllocator,
			                      Instance *pInstance);

			Result EnumerateInstanceExtensionProperties(const char *pLayerName,
			                                            uint32_t *pPropertyCount,
			                                            ExtensionProperties *pProperties);

			// </editor-fold>

			// <editor-fold Desc="GetInstanceProcAddr(instance, ...)">

			Result CreateDevice(Vulkan::PhysicalDevice &physicalDevice, const DeviceCreateInfo *pCreateInfo,
			                    const AllocationCallbacks *pAllocator, Device *pDevice);

			Result CreateWin32SurfaceKHR(Vulkan::Instance &instance,
			                             Win32SurfaceCreateInfoKHR *pCreateInfo,
			                             AllocationCallbacks *pAllocator,
			                             SurfaceKHR *pSurface);

			Result CreateXcbSurfaceKHR(Vulkan::Instance &instance,
			                           XcbSurfaceCreateInfoKHR *pCreateInfo,
			                           AllocationCallbacks *pAllocator,
			                           SurfaceKHR *pSurface);

			void DestroyDevice(Vulkan::Device &device, const AllocationCallbacks *pAllocator);

			void DestroyInstance(Instance instance, const AllocationCallbacks *pAllocator);

			void DestroySurfaceKHR(Vulkan::Instance &instance, SurfaceKHR surface, AllocationCallbacks *pAllocator);

			Result EnumeratePhysicalDevices(Vulkan::Instance &instance,
			                                uint32_t *pPhysicalDeviceCount,
			                                PhysicalDevice *pPhysicalDevices);

			void *GetDeviceProcAddr(Vulkan::Device &device, const char *pName);

			void GetPhysicalDeviceFeatures(Vulkan::PhysicalDevice &physicalDevice,
			                               PhysicalDeviceFeatures *pFeatures);

			void GetPhysicalDeviceProperties(Vulkan::PhysicalDevice &physicalDevice,
			                                 PhysicalDeviceProperties *pProperties);

			void GetPhysicalDeviceQueueFamilyProperties(Vulkan::PhysicalDevice &physicalDevice,
			                                            uint32_t *pQueueFamilyPropertyCount,
			                                            QueueFamilyProperties *pQueueFamilyProperties);

			Result GetPhysicalDeviceSurfaceSupportKHR(Vulkan::PhysicalDevice &physicalDevice, uint32_t queueFamilyIndex,
			                                          Vulkan::SurfaceKHR &surface, bool *pSupported);

			// </editor-fold>
		}

		class Instance final {
		public:
			struct FuncPtrs {
				PFN_vkCreateDevice vkCreateDevice = nullptr;
				PFN_vkDestroyDevice vkDestroyDevice = nullptr;
				PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = nullptr;
				PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = nullptr;
				PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = nullptr;
				PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures = nullptr;
				PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = nullptr;
				PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = nullptr;
				PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR = nullptr;

				Internal::ProcTypes::CreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = nullptr;
				Internal::ProcTypes::CreateXcbSurfaceKHR vkCreateXcbSurfaceKHR = nullptr;
			};

		private:
			FuncPtrs mFuncPtrs;
			Internal::Instance mHandle;
		public:
			Instance(const std::vector<const char *> &extensions, const std::vector<const char *> &layers);

			~Instance();

			static bool IsExtensionSupported(const char *extension, const char *layer = nullptr);

			static bool IsLayerSupported(const char *extension);

			FuncPtrs &GetFuncPtrs() noexcept;

			Internal::Instance GetHandle() noexcept;

			std::vector<PhysicalDevice> GetPhysicalDevices();
		};

		class PhysicalDevice final {
		private:
			Internal::PhysicalDeviceFeatures mFeatures;
			Internal::PhysicalDevice mHandle;
			Instance *mInstance;
			Internal::PhysicalDeviceProperties mProperties;

		public:
			PhysicalDevice(Instance *instance, Internal::PhysicalDevice handle);

			const Internal::PhysicalDeviceFeatures &GetFeatures() const noexcept;

			Internal::PhysicalDevice GetHandle() noexcept;

			Instance &GetInstance() noexcept;

			std::vector<uint32_t> GetPresentQueues(SurfaceKHR &surface);

			const Internal::PhysicalDeviceProperties &GetProperties() const noexcept;

			std::vector<Internal::QueueFamilyProperties> GetQueueProperties();
		};

		class Device final {
		private:
			Internal::Device mHandle;
			PhysicalDevice &mPhysicalDevice;

		public:
			Device(PhysicalDevice &physicalDevice, const std::vector<uint32_t> &queues,
			       const std::vector<const char *> &extensions, const std::vector<const char *> &layers);

			~Device();

			Internal::Device GetHandle() noexcept;

			Instance &GetInstance() noexcept;
		};

		class SurfaceKHR final {
		private:
			Internal::SurfaceKHR mHandle;
			Instance *mInstance;

		public:
			SurfaceKHR(Instance *instance, Graphics::Window &window);

			~SurfaceKHR();

			Internal::SurfaceKHR GetHandle() noexcept;

			static std::vector<const char *> GetRequiredInstanceExtensions(const Graphics::Window &window);
		};
	}
}
