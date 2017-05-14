#pragma once

#define VK_NO_PROTOTYPES
extern "C" {
#include <vulkan/vulkan.h>
}

#include <array>
#include <string>
#include <vector>

namespace ProjectVoxel {
	namespace Vulkan {
		class Instance;

		class PhysicalDevice;

		class Instance final {
		private:
			VkInstance mHandle = nullptr;

			PFN_vkCreateInstance vkCreateInstance;
			PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
			PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;

			PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
			PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
			PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
		public:
			friend class PhysicalDevice;

			Instance(const std::vector<const char *> &extensions,
			         const std::vector<const char *> &layers);

			~Instance();

			std::vector<PhysicalDevice> GetPhysicalDevices();

			void *GetProcAddr(const char *pName) const;

			explicit operator VkInstance() noexcept;
		};

		class PhysicalDevice final {
		private:
			VkPhysicalDevice mHandle;
			Instance &mInstance;
			VkPhysicalDeviceProperties mProperites;
			std::vector<VkQueueFamilyProperties> mQueueFamilyProperties;

		public:
			PhysicalDevice(Instance &instance, VkPhysicalDevice device);
			PhysicalDevice(const PhysicalDevice &other);

			const VkPhysicalDeviceProperties &GetDeviceProperties() const noexcept;

			Instance &GetInstance() noexcept;

			explicit operator VkPhysicalDevice() noexcept;
		};
	}
}