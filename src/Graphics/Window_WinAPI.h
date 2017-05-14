#pragma once

#define UNICODE
#define VK_USE_PLATFORM_WIN32_KHR
#include "Window.h"

extern "C" {
#include <windows.h>
}

namespace ProjectVoxel {
	namespace Graphics {
		struct Window::Internal {
			HWND hwnd;

			static LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
		};

		namespace WinAPI {
			extern const wchar_t *WINDOW_CLASS_NAME;
			extern const wchar_t *WINDOW_THIS_PROP;
		}
	}
}