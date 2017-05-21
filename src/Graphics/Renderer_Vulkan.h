#pragma once

#include "Renderer.h"

#include "../Vulkan.h"

namespace ProjectVoxel {
	namespace Graphics {
		namespace Vulkan {
			using namespace ProjectVoxel::Vulkan;

			class Renderer final : public Graphics::Renderer {
			private:
				Window mWindow;

				std::unique_ptr<Instance> mInstance;
				std::unique_ptr<SurfaceKHR> mSurface;
				std::unique_ptr<Device> mDevice;
			public:
				Renderer();
				~Renderer();

				Window &GetWindow() noexcept;

				void SwapBuffers();
			};
		}
	}
}
