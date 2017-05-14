#include "Window_XCB.h"

#include <cstring>

using namespace ProjectVoxel::Graphics::XCB;

// <editor-fold Desc="Connection">

Connection::Connection() {
	mHandle = xcb_connect(nullptr, &mScreenp);
}

Connection::~Connection() {
	xcb_disconnect(mHandle);
}

void Connection::Flush() {
	xcb_flush(mHandle);
}

Screen Connection::GetScreen() {
	return std::move(Screen(*this, mScreenp));
}

Connection::operator xcb_connection_t *() {
	return mHandle;
}

// </editor-fold>

// <editor-fold Desc="Screen">

Screen::Screen(Connection &connection, int screenp)
		: mConnection(connection) {
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(xcb_get_setup(connection));
	for (int s = screenp; s > 0; s--) {
		xcb_screen_next(&iter);
	}
	mHandle = iter.data;
}

Screen::operator xcb_screen_t *() {
	return mHandle;
}

// </editor-fold>

// <editor-fold Desc="Window">

Window::Window()
		: mConnection(), mScreen(mConnection.GetScreen()) {
	mHandle = xcb_generate_id(mConnection);

	std::uint32_t eventMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	std::uint32_t valueList[] = {static_cast<xcb_screen_t *>(mScreen)->black_pixel, 0};

	xcb_create_window(
			mConnection,
	        XCB_COPY_FROM_PARENT,
	        mHandle,
	        static_cast<xcb_screen_t *>(mScreen)->root,
	        0, 0,
	        800, 600,
	        0,
	        XCB_WINDOW_CLASS_INPUT_OUTPUT,
	        static_cast<xcb_screen_t *>(mScreen)->root_visual,
	        eventMask,
	        valueList
	);

	mWMProtocols = GetAtom("WM_PROTOCOLS");
	mWMDeleteWindow = GetAtom("WM_DELETE_WINDOW");

	xcb_change_property(
			mConnection,
	        XCB_PROP_MODE_REPLACE,
	        mHandle,
	        mWMProtocols,
	        4,
	        32,
	        1,
	        &mWMDeleteWindow
	);

	SetTitle("Project Voxel [XCB]");

	mConnection.Flush();
}

Window::~Window() {
	xcb_destroy_window(mConnection, mHandle);
}

xcb_atom_t Window::GetAtom(const char *name) {
	xcb_intern_atom_cookie_t atomCookie = xcb_intern_atom(mConnection, 0, (uint16_t) std::strlen(name), name);
	xcb_intern_atom_reply_t *atomReply = xcb_intern_atom_reply(mConnection, atomCookie, nullptr);
	xcb_atom_t atom = atomReply->atom;
	free(atomReply);
	return atom;
}

Connection &Window::GetConnection() noexcept {
	return mConnection;
}

void Window::HandleEvents() {
	xcb_generic_event_t *event;
	xcb_client_message_event_t *cm;

	while ((event = xcb_poll_for_event(mConnection))) {
		switch (event->response_type & ~0x80) {
			case XCB_CLIENT_MESSAGE:
				cm = (xcb_client_message_event_t *) event;

				if (cm->data.data32[0] == mWMDeleteWindow) {
					for (const auto &callback : mCloseCallbacks) {
						callback(*this);
					}
				}

				break;
			default:
				break;
		}

		free(event);
	}
}

void Window::SetTitle(const std::string &title) {
	xcb_change_property(
			mConnection,
	        XCB_PROP_MODE_REPLACE,
	        mHandle,
	        XCB_ATOM_WM_NAME,
	        XCB_ATOM_STRING,
	        8,
			(uint32_t) title.length(),
	        title.c_str()
	);

	mConnection.Flush();
}

void Window::SetVisible(bool visible) {
	if (visible) {
		xcb_map_window(mConnection, mHandle);
	} else {
		xcb_unmap_window(mConnection, mHandle);
	}

	mConnection.Flush();
}

Window::operator xcb_window_t() noexcept {
	return mHandle;
}

// </editor-fold>