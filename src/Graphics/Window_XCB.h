#pragma once


#include "Window.h"

#include "Renderer.h"

extern "C" {
#include <xcb/xcb.h>
}

namespace ProjectVoxel {
	namespace Graphics {
		namespace XCB {
			class Connection;
			class Screen;
			class Window;

			class Connection final {
			private:
				xcb_connection_t *mHandle;
				int mScreenp;
			public:
				Connection();
				~Connection();

				void Flush();

				Screen GetScreen();

				operator xcb_connection_t *();
			};

			class Screen final {
			private:
				Connection &mConnection;
				xcb_screen_t *mHandle;

			public:
				Screen(Connection &connection, int screenp);

				operator xcb_screen_t *();
			};

			class Window : public Graphics::Window {
			private:
				Connection mConnection;
				Screen mScreen;
				xcb_window_t mHandle;

				xcb_atom_t mWMProtocols;
				xcb_atom_t mWMDeleteWindow;

			public:
				Window();
				~Window();

				xcb_atom_t GetAtom(const char *name);

				Connection &GetConnection() noexcept;

				void HandleEvents();

				void SetTitle(const std::string &title);

				void SetVisible(bool visible);

				operator xcb_window_t() noexcept;
			};
		}
	}
}