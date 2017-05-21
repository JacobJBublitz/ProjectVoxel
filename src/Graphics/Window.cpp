#include "Window.h"

#ifdef ProjectVoxel_HAS_WinAPI
#include "Window_WinAPI.h"
#endif
#ifdef ProjectVoxel_HAS_XCB
#include "Window_XCB.h"
#endif

using namespace ProjectVoxel::Graphics;

Window::Window()
		: Window(false, 0, 0) {}

Window::Window(uint8_t major, uint8_t minor)
		: Window(true, major, minor) {}

Window::Window(bool openGLContext, uint8_t major, uint8_t minor) {
	if (glfwInit() != GLFW_TRUE) {
		throw "Failed to initialize GLFW";
	}

	glfwDefaultWindowHints();
	if (openGLContext) {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	} else {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	mHandle = glfwCreateWindow(800, 600, "Project Voxel", nullptr, nullptr);
	if (!mHandle) {
		throw "Failed to create window.";
	}

	glfwSetWindowUserPointer(mHandle, this);

	glfwSetWindowCloseCallback(mHandle, [](GLFWwindow *win) {
		Window *pThis = (Window *) glfwGetWindowUserPointer(win);

		for (auto &callback : pThis->mCloseCallbacks) {
			callback(*pThis);
		}
	});
}

Window::~Window() {}

void Window::AddCloseCallback(std::function<void(const Window &)> callback) {
	mCloseCallbacks.push_back(callback);
}

GLFWwindow* Window::GetHandle() noexcept {
	return mHandle;
}

void Window::HandleEvents() {
	glfwPollEvents();
}

void Window::SetTitle(const std::string &title) {
	glfwSetWindowTitle(mHandle, title.c_str());
}

void Window::SetVisible(bool visible) {
	if (visible) {
		glfwShowWindow(mHandle);
	} else {
		glfwHideWindow(mHandle);
	}
}
