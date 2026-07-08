#pragma once

#include "Basic.hpp"

namespace Wai::Lexing {
	enum LexlingType {
		None,
		Name,
		LeftParenthesis,
		RightParenthesis,
		LeftCurlyBracket,
		RightCurlyBracket,
		StringLiteral,
		TypeMarker,
		OffsetMarker,
		COUNT
	};

	class Lexling {
	public:
		LexlingType type;
		std::string value;
	};

	class Lexlings {
	public:
		std::vector<Lexling> lexlings;

		Lexlings() {
			lexlings = std::vector<Lexling>();
		}
	};

	void Lex(FileManagement::Files files) {
		return;
	}
}
