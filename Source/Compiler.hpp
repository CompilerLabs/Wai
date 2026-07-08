#pragma once

#include "Lexer.hpp"
#include <iostream>

namespace Wai::Compiling {
	class Compiler {
	public:
		Lexing::Lexlings lexlings;

		void Compile() {
			std::cout << "Starting compilation..." << std::endl;
		}
	};
}
