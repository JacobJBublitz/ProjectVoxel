#pragma once

#include "Renderer.h"

#include "../Vulkan.h"

namespace ProjectVoxel {
	namespace Graphics {
		namespace Vulkan {
			using namespace ProjectVoxel::Vulkan;

			class Renderer final : public Graphics::Renderer {
			private:
				std::shared_ptr<Window> mWindow;

				std::unique_ptr<Vulkan::Instance> mInstance;
				std::unique_ptr<Vulkan::SurfaceKHR> mSurface;
			public:
				Renderer();
				~Renderer();

				std::shared_ptr<Window> GetWindow() noexcept;

				void SwapBuffers();
			};
		}
	}
}