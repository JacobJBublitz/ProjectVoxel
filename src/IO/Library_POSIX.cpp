#include "Library.h"

extern "C" {
#include <dlfcn.h>
}

using namespace ProjectVoxel::IO;

Library::Library(const std::string &name) {
	mHandle = dlopen(("lib" + name + ".so").c_str(), RTLD_NOW | RTLD_LOCAL);
	if (!mHandle) {
		char *msg = dlerror();
		throw "Failed to load library '" + name + "'. " + msg;
	}
}

Library::~Library() {
	if (mHandle) {
		dlclose(mHandle);
	}
}

void* Library::GetSymbol(const std::string &name) {
	void *sym = dlsym(mHandle, name.c_str());
	if (!sym) {
		throw "Failed to find symbol '" + name + "'.";
	}

	return sym;
}