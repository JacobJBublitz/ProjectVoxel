#include "Window_WinAPI.h"

#include <iostream>

const wchar_t *ProjectVoxel::Graphics::WinAPI::WINDOW_CLASS_NAME = L"ProjectVoxelWindow";
const wchar_t *ProjectVoxel::Graphics::WinAPI::WINDOW_THIS_PROP = L"ProjectVoxelThisProperty";

ProjectVoxel::Graphics::Window::Window() 
	: mInternal(std::make_unique<Internal>()) {

	WNDCLASSEX winClass = {};
	if (!GetClassInfoEx(GetModuleHandle(nullptr), WinAPI::WINDOW_CLASS_NAME, &winClass)) {
		winClass.cbSize = sizeof(WNDCLASSEX);
		winClass.style = CS_OWNDC;
		winClass.lpfnWndProc = Internal::WinProc;
		winClass.cbClsExtra = 0;
		winClass.cbWndExtra = 0;
		winClass.hInstance = GetModuleHandle(nullptr);
		winClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		winClass.hbrBackground = nullptr;
		winClass.lpszMenuName = nullptr;
		winClass.lpszClassName = WinAPI::WINDOW_CLASS_NAME;
		winClass.hIconSm = nullptr;

		if (!RegisterClassEx(&winClass)) {
			throw "Failed to register window class.";
		}
	}

	DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION | WS_MAXIMIZEBOX | WS_THICKFRAME;
	DWORD exStyle = WS_EX_APPWINDOW;
	mInternal->hwnd = CreateWindowEx(exStyle, WinAPI::WINDOW_CLASS_NAME, L"Project Voxel", style, 0, 0, 800, 600, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
	if (!mInternal->hwnd) {
		throw "Failed to create window.";
	}

	SetProp(mInternal->hwnd, WinAPI::WINDOW_THIS_PROP, this);
}

ProjectVoxel::Graphics::Window::~Window() {
	RemoveProp(mInternal->hwnd, WinAPI::WINDOW_THIS_PROP);
	DestroyWindow(mInternal->hwnd);
}

VkSurfaceKHR ProjectVoxel::Graphics::Window::CreateVulkanSurface(VkInstance instance) {
	VkWin32SurfaceCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.hinstance = GetModuleHandle(nullptr);
	createInfo.hwnd = mInternal->hwnd;

	VkSurfaceKHR surface;
	VkResult result = vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface);
	if (result != VK_SUCCESS) {
		throw "Failed to create surface.";
	}

	return surface;
}

std::vector<const char *> ProjectVoxel::Graphics::Window::GetRequiredVulkanInstanceExtensions() {
	return { "VK_KHR_win32_surface" };
}

void ProjectVoxel::Graphics::Window::HandleEvents() {
	MSG msg;

	while (PeekMessage(&msg, mInternal->hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void ProjectVoxel::Graphics::Window::SetVisible(bool visible) {
	ShowWindow(mInternal->hwnd, visible ? SW_SHOW : SW_HIDE);
}

LRESULT CALLBACK ProjectVoxel::Graphics::Window::Internal::WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept {
	Window *pThis = (Window *) GetProp(hwnd, WinAPI::WINDOW_THIS_PROP);
	if (!pThis) {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	switch (uMsg) {
	case WM_CLOSE:
		for (auto &callback : pThis->mCloseCallbacks) {
			callback(*pThis);
		}
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}