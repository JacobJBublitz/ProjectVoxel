#include "Window.h"

#ifdef ProjectVoxel_HAS_WinAPI
#include "Window_WinAPI.h"
#endif
#ifdef ProjectVoxel_HAS_XCB
#include "Window_XCB.h"
#endif

using namespace ProjectVoxel::Graphics;

Window::~Window() {}

void Window::AddCloseCallback(std::function<void(const Window &)> callback) {
	mCloseCallbacks.push_back(callback);
}

std::shared_ptr<Window> Window::Create() {
#ifdef ProjectVoxel_HAS_WinAPI
	return std::make_shared<WinAPI::Window>();
#endif
#ifdef ProjectVoxel_HAS_XCB
	return std::make_shared<XCB::Window>();
#endif

	return nullptr;
}
