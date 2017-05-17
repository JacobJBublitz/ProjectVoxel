#include "Window_WinAPI.h"

#include <iostream>

using namespace ProjectVoxel::Graphics::WinAPI;

const wchar_t *ProjectVoxel::Graphics::WinAPI::WINDOW_CLASS_NAME = L"ProjectVoxelWindow";
const wchar_t *ProjectVoxel::Graphics::WinAPI::WINDOW_THIS_PROP = L"ProjectVoxelThisProperty";

Window::Window() {
	WNDCLASSEX winClass = {};
	if (!GetClassInfoEx(GetModuleHandle(nullptr), WINDOW_CLASS_NAME, &winClass)) {
		winClass.cbSize = sizeof(WNDCLASSEX);
		winClass.style = CS_OWNDC;
		winClass.lpfnWndProc = WinProc;
		winClass.cbClsExtra = 0;
		winClass.cbWndExtra = 0;
		winClass.hInstance = GetModuleHandle(nullptr);
		winClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		winClass.hbrBackground = nullptr;
		winClass.lpszMenuName = nullptr;
		winClass.lpszClassName = WINDOW_CLASS_NAME;
		winClass.hIconSm = nullptr;

		if (!RegisterClassEx(&winClass)) {
			throw "Failed to register window class.";
		}
	}

	DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION | WS_MAXIMIZEBOX | WS_THICKFRAME;
	DWORD exStyle = WS_EX_APPWINDOW;
	mHWnd = CreateWindowEx(exStyle, WINDOW_CLASS_NAME, L"Project Voxel", style, 0, 0, 800, 600, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
	if (!mHWnd) {
		throw "Failed to create window.";
	}

	SetProp(mHWnd, WINDOW_THIS_PROP, this);
}

Window::~Window() {
	RemoveProp(mHWnd, WINDOW_THIS_PROP);
	DestroyWindow(mHWnd);
}

HWND Window::GetHWnd() noexcept {
	return mHWnd;
}

void Window::HandleEvents() {
	MSG msg;

	while (PeekMessage(&msg, mHWnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::SetTitle(const std::string &title) {
	std::vector<wchar_t> wideTitle;
	int wideTitleLength = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, title.c_str(), -1, nullptr, 0);

	wideTitle.resize(wideTitleLength, 0);
	MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, title.c_str(), -1, wideTitle.data(), (int) wideTitle.size());

	SetWindowText(mHWnd, wideTitle.data());
}

void Window::SetVisible(bool visible) {
	ShowWindow(mHWnd, visible ? SW_SHOW : SW_HIDE);
}

LRESULT CALLBACK Window::WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept {
	Window *pThis = (Window *) GetProp(hwnd, WINDOW_THIS_PROP);
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