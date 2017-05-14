#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace ProjectVoxel {
	namespace Graphics {
		class Window {
		protected:
			std::vector<std::function<void(const Window &)>> mCloseCallbacks;
		public:
			virtual ~Window();

			static std::shared_ptr<Window> Create();

			void AddCloseCallback(std::function<void(const Window &)> callback);

			virtual void HandleEvents() = 0;

			virtual void SetTitle(const std::string &title) = 0;
			virtual void SetVisible(bool visible) = 0;
		};
	}
}