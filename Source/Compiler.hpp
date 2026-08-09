#pragma once

#include "Lexer.hpp"
#include <iostream>

namespace Wai::Compiling {
	class Compiler {
	public:
		Wai::Compiling::Lexlings lexlings;

		void Compile() {
			std::cout << "Starting compilation..." << std::endl;
		}
	};
}
