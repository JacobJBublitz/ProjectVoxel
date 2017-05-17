#pragma once

#define UNICODE
#define VK_USE_PLATFORM_WIN32_KHR
#include "Window.h"

extern "C" {
#include <windows.h>
}

namespace ProjectVoxel {
	namespace Graphics {
		namespace WinAPI {
			class Window final : public Graphics::Window {
			private:
				HWND mHWnd;

				static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
			public:
				Window();
				~Window();
				
				HWND GetHWnd() noexcept;

				void HandleEvents();
				
				void SetTitle(const std::string &title);
				
				void SetVisible(bool visible);
			};

			extern const wchar_t *WINDOW_CLASS_NAME;
			extern const wchar_t *WINDOW_THIS_PROP;
		}
	}
}