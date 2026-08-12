#pragma once

#include "Basic.hpp"
#include "Text.hpp"
#include "FileManager.hpp"
#include <string>

namespace Wai::Compiling {
   	enum class LexlingType {
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
		Wai::Compiling::LexlingType type;
		std::string value;
		Wai::Text::TextLocation location;

		// constructors
		Lexling() {
		    type = LexlingType::None;
			value = "";
			location = Wai::Text::TextLocation();
		}
		Lexling(Wai::Compiling::LexlingType _type, std::string _value, Wai::Text::TextLocation _location) {
		    type = _type;
			value = _value;
			location = _location;
		}
	};

	class Lexlings {
	public:
		std::vector<Lexling> lexlings;

		Lexlings() {
			lexlings = std::vector<Lexling>();
		}
	};

	class Lexer {
		// check characters & character ranges
        Wai::Boolean LexingCheckCharacterRange(Wai::Character start, Wai::Character value, Wai::Character end) {
            return (value >= start) && (value <= end);
        }
        Wai::Boolean LexingCheckWhitespaceCharacter(Wai::Character value) {
            return LexingCheckCharacterRange(0, value, 32);
        }

        // check strings
        Wai::Boolean LexingCheckForSubStringAtIndex(std::string& masterString, std::string searchingFor, Wai::CharacterIndex startingIndex) {
            // check if master string has enough room left to even search for
            if (masterString.size() < searchingFor.size() || searchingFor.size() + startingIndex > masterString.size()) {
                return false;
            }

            // search for substring
            for (Wai::CharacterIndex index = 0; index < searchingFor.size(); index++) {
                // if characters differ
                if (masterString[startingIndex + index] != searchingFor[index]) {
                    // not there
                    return false;
                }
            }

            // equal by process of elimination
            return true;
        }

        // check end of file
        Wai::Boolean IndexInRangeOfString(std::string& input, Wai::Index index) {
            return index < input.length();
        }

		// check end of files
		Wai::Boolean FilesIndexInRange(Wai::Files::Files& files, Wai::FileIndex index) {
			return (index < files.files.size());
		}

	public:
	    Wai::Compiling::Lexlings Lex(Wai::Debugging::Log* log, Wai::Files::Files& files) {
			Wai::Compiling::Lexlings output;
			Wai::FileIndex fileIndex = 0;
			Wai::LineNumber lineNumber = 1;
			Wai::CharacterIndex characterIndex = 0;

			// lex over files
			while (FilesIndexInRange(files, fileIndex)) {
				// reset file scoped trackers
				lineNumber = 0;
				characterIndex = 0;

				// setup current file
				Wai::Files::File currentFile = files.files[fileIndex];
				std::string& currentFileString = currentFile.data;
				
				// log
				log->LogNote("Lexing file.", { Wai::Debugging::LogEntrySubdata("filePath", currentFile.path) });

				// lex over characters
				while (IndexInRangeOfString(currentFileString, characterIndex)) {

					// skip whitespace
					while (IndexInRangeOfString(files.files[fileIndex].data, characterIndex), LexingCheckWhitespaceCharacter(files.files[fileIndex].data[characterIndex])) {
						characterIndex++;
					}
				}
			}
	    }
	};
}
