#pragma once

#include "Lexer.cpp"

namespace Wai::Compiling {
	class Compiler {
	public:
		Lexing::Lexlings lexlings;

		void Compile() {
			std::cout << "Starting compilation..." << std::endl;
		}
	};
}
