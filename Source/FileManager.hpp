#pragma once

#include "Basic.hpp"
#include <string>
#include <vector>

namespace Wai::FileManager {
	// one file
	class File {
	public:
		std::string path;
		std::string data;
	};

	// multiple files
	class Files {
	public:
		// files allocation
		std::vector<Wai::FileManager::File> files;
	};
}
