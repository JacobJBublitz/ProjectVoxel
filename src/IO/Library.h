#pragma once

#include <string>

namespace ProjectVoxel {
	namespace IO {
		class Library final {
		private:
			void *mHandle;
		public:
			Library(const std::string &name);
			~Library();

			void *GetSymbol(const std::string &name);
		};
	}
}