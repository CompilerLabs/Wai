#pragma once

#include "FileManager.hpp"
#include "JSON.hpp"
#include "Lexer.hpp"

namespace Wai::Compiling {
	class Workspace {
	public:
		Wai::Files::FileManager files;

		// import settings
		void ImportSettingsFromJSON(Wai::JSON::Root* root) {
			
		}
	};

	class Compiler {
	public:
		// turn raw strings into amd64 byte code
		void Compile(Wai::Debugging::Log* log, Wai::Compiling::Workspace* workspace) {
			
		}
	};
}
