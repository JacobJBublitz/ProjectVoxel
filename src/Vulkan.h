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
		class Instance;

		class PhysicalDevice;

		class SurfaceKHR;

		namespace Internal {
			// <editor-fold Desc="Types">
			using AllocationCallbacks = ::VkAllocationCallbacks;
			using ApplicationInfo = ::VkApplicationInfo;
			using Device = ::VkDevice;
			using ExtensionProperties = ::VkExtensionProperties;
			using Instance = ::VkInstance;
			using InstanceCreateInfo = ::VkInstanceCreateInfo;
			using PhysicalDevice = ::VkPhysicalDevice;
			using PhysicalDeviceProperties = ::VkPhysicalDeviceProperties;
			using QueueFamilyProperties = ::VkQueueFamilyProperties;
			using Result = ::VkResult;
			using SurfaceKHR = ::VkSurfaceKHR;

#ifdef VK_USE_PLATFORM_XCB_KHR
			using XcbSurfaceCreateInfo = ::VkXcbSurfaceCreateInfoKHR;
#else
			using XcbSurfaceCreateInfo = void;
#endif
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

			Result CreateXcbSurfaceKHR(Vulkan::Instance &instance,
			                           XcbSurfaceCreateInfo *pCreateInfo,
			                           AllocationCallbacks *pAllocator,
			                           SurfaceKHR *pSurface);

			void DestroyInstance(Instance instance, const AllocationCallbacks *pAllocator);

			void DestroySurfaceKHR(Vulkan::Instance &instance, SurfaceKHR surface, AllocationCallbacks *pAllocator);

			Result EnumeratePhysicalDevices(Vulkan::Instance &instance,
			                                uint32_t *pPhysicalDeviceCount,
			                                PhysicalDevice *pPhysicalDevices);

			void GetPhysicalDeviceProperties(Vulkan::PhysicalDevice &physicalDevice,
			                                 PhysicalDeviceProperties *pProperties);

			void GetPhysicalDeviceQueueFamilyProperties(Vulkan::PhysicalDevice &physicalDevice,
			                                            uint32_t *pQueueFamilyPropertyCount,
			                                            QueueFamilyProperties *pQueueFamilyProperties);

			// </editor-fold>
		}

		class Instance final {
		public:
			struct FuncPtrs {
				PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = nullptr;
				PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = nullptr;
				PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = nullptr;
				PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = nullptr;

#ifdef VK_USE_PLATFORM_XCB_KHR
				PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR = nullptr;
#else
				void *vkCreateXcbSurfaceKHR = nullptr;
#endif
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
			Internal::PhysicalDevice mHandle;
			Instance *mInstance;

		public:
			PhysicalDevice(Instance *instance, Internal::PhysicalDevice handle);

			Internal::PhysicalDevice GetHandle() noexcept;

			Instance &GetInstance() noexcept;
		};

		class SurfaceKHR final {
		private:
			Internal::SurfaceKHR mHandle;
			Instance *mInstance;

		public:
			SurfaceKHR(Instance *instance, Graphics::Window &window);

			~SurfaceKHR();

			static std::vector<const char *> GetRequiredInstanceExtensions(const Graphics::Window &window);
		};
	}
}