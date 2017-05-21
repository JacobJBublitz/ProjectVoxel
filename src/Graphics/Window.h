#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
extern "C" {
#include <GLFW/glfw3.h>
}

namespace ProjectVoxel {
	namespace Graphics {
		class Window final {
		private:
			std::vector<std::function<void(const Window &)>> mCloseCallbacks;

			GLFWwindow *mHandle;

			Window(bool openGLContext, uint8_t major, uint8_t minor);
		public:
			Window();
			Window(uint8_t major, uint8_t minor);
			~Window();

			void AddCloseCallback(std::function<void(const Window &)> callback);

			GLFWwindow *GetHandle() noexcept;

			void HandleEvents();

			void SetTitle(const std::string &title);
			void SetVisible(bool visible);
		};
	}
}
