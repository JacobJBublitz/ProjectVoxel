#include "Library.h"

#include <iostream>
#include <vector>

#define UNICODE
#define WIN32_LEAN_AND_MEAN
extern "C" {
#include <Windows.h>
}

using namespace ProjectVoxel::IO;

Library::Library(const std::string &name) {
	mHandle = LoadLibraryA(name.c_str());
	if (!mHandle) {
		throw "Failed to load library '" + name + "'.";
	}
}

Library::~Library() {
	FreeLibrary((HMODULE) mHandle);
}

void *Library::GetSymbol(const std::string &name) {
	void *sym = (void *) GetProcAddress((HMODULE) mHandle, name.c_str());
	if (!sym) {
		throw "Failed to find symbol '" + name + "'.";
	}

	return sym;
}