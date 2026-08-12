#pragma once

#include "Basic.hpp"
#include "Log.hpp"
#include <string>

namespace Wai::JSON {
    enum class LexlingType {
        Invalid,
        LeftSquareBracket,
        RightSquareBracket,
        LeftCurlyBracket,
        RightCurlyBracket,
        Colon,
        Comma,
        False,
        True,
        Null,
        Number,
        String,
        EndOfFile
    };

    class Lexling {
    public:
        Wai::JSON::LexlingType type;
        std::string stringedValue;
        Wai::Text::TextLocation location;

        Lexling() {
            type = Wai::JSON::LexlingType::Invalid;
            stringedValue = "";
            location = Wai::Text::TextLocation();
        }
        Lexling(Wai::JSON::LexlingType _type, std::string _stringedValue, Wai::Text::TextLocation _location) {
            type = _type;
            stringedValue = _stringedValue;
            location = _location;
        }
    };

    enum class ItemType {
        Invalid,
        Object,
        Array,
        String,
        Number,
        False,
        True,
        Null, // is an actual object type, not a dead end marker
    };

    class Item {
    public:
        // the type of item
        Wai::JSON::ItemType type;

        // the name assigned to the item
        Wai::JSON::Lexling name;

        // possible states (null does not require value as type is sufficient)
        Wai::JSON::Lexling stringValue;
        Wai::JSON::Lexling numberValue;
        Wai::Boolean booleanValue;
        std::vector<Wai::JSON::Item> objectBody;
        std::vector<Wai::JSON::Item> arrayBody;
        
        // constructors
        Item() {
            type = Wai::JSON::ItemType::Invalid;
            name = Wai::JSON::Lexling();
            numberValue = Wai::JSON::Lexling();
            booleanValue = false;
            objectBody = std::vector<Wai::JSON::Item>();
            arrayBody = std::vector<Wai::JSON::Item>();
        }
    };

    class Root {
    public:
        std::vector<Wai::JSON::Lexling> lexlings;
        Wai::JSON::Item masterObject;
    };

    class JSONCompiler {
    public:
        // compile json into C++ data
        Wai::JSON::Root CompileJSON(Wai::Debugging::Log* log, std::string& json) {
            // setup output
            Wai::JSON::Root output;

            // lex
            output.lexlings = Lex(log, json);
            if (log->criticalErrorOccured) {
                return output;
            }

            // parse
            output.masterObject = Parse(log, output.lexlings);

            return output;
        }
    
    private:
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

        // lex file
        std::vector<Wai::JSON::Lexling> Lex(Wai::Debugging::Log* log, std::string& input) {
            // setup output
            std::vector<Wai::JSON::Lexling> output = std::vector<Wai::JSON::Lexling>();
            Wai::FileIndex fileIndex = 0;
            Wai::LineNumber lineNumber = 1;
            Wai::CharacterIndex index = 0;

            // lex
            while (IndexInRangeOfString(input, index)) {
                // skip whitespace
                while (IndexInRangeOfString(input, index) && LexingCheckWhitespaceCharacter(input[index])) {
                    // check for new lines
                    if (LexingCheckCharacterRange('\n', input[index], '\n') || LexingCheckCharacterRange('\r', input[index], '\r')) {
                        lineNumber++;
                    }

                    // next character
                    index++;
                }
                if (!IndexInRangeOfString(input, index)) {
                    goto quit;
                }

                // check for lexlings
                if (LexingCheckCharacterRange('[', input[index], '[')) {
                    output.push_back(Wai::JSON::Lexling(Wai::JSON::LexlingType::LeftSquareBracket, "[", Wai::Text::TextLocation(fileIndex, lineNumber, Wai::Buffer((Wai::Address)index, (Wai::Address)index))));
                    index++;
                } else if (LexingCheckCharacterRange(']', input[index], ']')) {
                    output.push_back(Wai::JSON::Lexling(Wai::JSON::LexlingType::RightSquareBracket, "]", Wai::Text::TextLocation(fileIndex, lineNumber, Wai::Buffer((Wai::Address)index, (Wai::Address)index))));
                    index++;
                } else if (LexingCheckCharacterRange('{', input[index], '{')) {
                    output.push_back(Wai::JSON::Lexling(Wai::JSON::LexlingType::LeftCurlyBracket, "{", Wai::Text::TextLocation(fileIndex, lineNumber, Wai::Buffer((Wai::Address)index, (Wai::Address)index))));
                    index++;
                } else if (LexingCheckCharacterRange('}', input[index], '}')) {
                    output.push_back(Wai::JSON::Lexling(Wai::JSON::LexlingType::RightCurlyBracket, "}", Wai::Text::TextLocation(fileIndex, lineNumber, Wai::Buffer((Wai::Address)index, (Wai::Address)index))));
                    index++;
                } else if (LexingCheckCharacterRange(':', input[index], ':')) {
                    output.push_back(Wai::JSON::Lexling(Wai::JSON::LexlingType::Colon, ":", Wai::Text::TextLocation(fileIndex, lineNumber, Wai::Buffer((Wai::Address)index, (Wai::Address)index))));
                    index++;
                } else if (LexingCheckCharacterRange(',', input[index], ',')) {
                    output.push_back(Wai::JSON::Lexling(Wai::JSON::LexlingType::Comma, ",", Wai::Text::TextLocation(fileIndex, lineNumber, Wai::Buffer((Wai::Address)index, (Wai::Address)index))));
                    index++;
                } else if (LexingCheckForSubStringAtIndex(input, "false", index)) {
                    output.push_back(Wai::JSON::Lexling(Wai::JSON::LexlingType::False, "false", Wai::Text::TextLocation(fileIndex, lineNumber, Wai::Buffer((Wai::Address)index, (Wai::Address)(index + (Wai::CharacterIndex)std::string("false").length())))));
                    index += std::string("false").length();
                } else if (LexingCheckForSubStringAtIndex(input, "true", index)) {
                    output.push_back(Wai::JSON::Lexling(Wai::JSON::LexlingType::True, "true", Wai::Text::TextLocation(fileIndex, lineNumber, Wai::Buffer((Wai::Address)index, (Wai::Address)(index + (Wai::CharacterIndex)std::string("true").length())))));
                    index += std::string("true").length();
                } else if (LexingCheckForSubStringAtIndex(input, "null", index)) {
                    output.push_back(Wai::JSON::Lexling(Wai::JSON::LexlingType::Null, "null", Wai::Text::TextLocation(fileIndex, lineNumber, Wai::Buffer((Wai::Address)index, (Wai::Address)(index + (Wai::CharacterIndex)std::string("null").length())))));
                    index += std::string("null").length();
                } else if (LexingCheckCharacterRange('"', input[index], '"')) {
                    // setup string start
                    Wai::CharacterIndex startingIndex = index;
                    
                    // skip past initial double quote
                    index++;

                    // search for end of string
                    while (true) {
                        // check for eof
                        if (!IndexInRangeOfString(input, index)) {
                            // string ended abruptly at the end of the file
                            log->LogError("JSON Error. A string literal did not finish properly.");

                            return output;
                        }

                        // check for new lines
                        if (LexingCheckCharacterRange('\n', input[index], '\n') || LexingCheckCharacterRange('\r', input[index], '\r')) {
                            lineNumber++;
                        }

                        // check for end of string
                        if (LexingCheckCharacterRange('"', input[index], '"')) {
                            // push lexling
                            output.push_back(Wai::JSON::Lexling(Wai::JSON::LexlingType::String, input.substr(startingIndex, index - startingIndex + 1), Wai::Text::TextLocation(fileIndex, lineNumber, Wai::Buffer((Wai::Address)startingIndex, (Wai::Address)index))));

                            // skip past ending quote
                            index++;

                            // exit loop
                            break;
                        }

                        // next index
                        index++;
                    }
                } else if (LexingCheckCharacterRange('0', input[index], '9') || LexingCheckCharacterRange('-', input[index], '-')) {
                    Wai::CharacterIndex startingIndex = index;
                    Wai::CharacterIndex endingIndex = index;

                    // check for negative sign
                    if (LexingCheckCharacterRange('-', input[index], '-')) {
                        // next character
                        index++;

                        // ensure that there is a digit after the minus sign, but don't advance, just check for error
                        if (!IndexInRangeOfString(input, index) || !LexingCheckCharacterRange('0', input[index], '9')) {
                            // number did not contain digits
                            log->LogError("JSON Error. A negative sign was found without digits afterwards.");

                            return output;
                        } else {
                            endingIndex++;
                        }
                    }

                    // check for numeral characters
                    while (IndexInRangeOfString(input, index) && LexingCheckCharacterRange('0', input[index], '9')) {
                        index++;
                        endingIndex++;
                    }

                    // append lexling
                    output.push_back(Wai::JSON::Lexling(Wai::JSON::LexlingType::Number, input.substr(startingIndex, endingIndex - startingIndex), Wai::Text::TextLocation(fileIndex, lineNumber, Wai::Buffer((Wai::Address)startingIndex, (Wai::Address)endingIndex))));

                    // next character
                    index++;
                } else {
                    // error, no lexling was matched
                    log->LogError("JSON Error. An unrecognized character was found in the JSON string. Line Number: '" + std::to_string(lineNumber) + "'");

                    return output;
                }
            }

            // append EndOfFile lexling
            quit:
            output.push_back(Wai::JSON::Lexling(Wai::JSON::LexlingType::EndOfFile, "[EndOfFile]", Wai::Text::TextLocation(fileIndex, lineNumber, Wai::Buffer((Wai::Address)input.length(), (Wai::Address)input.length()))));

            return output;
        }

        // parse lexlings
        Wai::JSON::Item Parse(Wai::Debugging::Log* log, std::vector<Wai::JSON::Lexling>& lexlings) {
            Wai::JSON::Item output;
            Wai::Index currentIndex = 0;

            // check for empty file
            if (lexlings[currentIndex].type == Wai::JSON::LexlingType::EndOfFile) {
                // error, nothing in file
                log->LogError("JSON Error. File is empty.");

                return output;
            }

            // parse root object
            output.type = Wai::JSON::ItemType::Object;
            output.objectBody = ParseObjectBody(log, lexlings, currentIndex);

            return output;
        }

        // parse object body
        std::vector<Wai::JSON::Item> ParseObjectBody(Wai::Debugging::Log* log, std::vector<Wai::JSON::Lexling>& lexlings, Wai::Index& currentIndex) {
            std::vector<Wai::JSON::Item> output;

            // create comma tracker
            Wai::Index commaIndex = 0;

            // if object body is missing container
            if (lexlings[currentIndex].type != Wai::JSON::LexlingType::LeftCurlyBracket) {
                log->LogError("JSON Error. Object body is missing opening left curly bracket. Line Numberr: " + std::to_string(lexlings[currentIndex].location.lineNumber));

                return output;
            // otherwise, skip past bracket
            } else {
                currentIndex++;
            }

            // loop over body
            while (lexlings[currentIndex].type != Wai::JSON::LexlingType::RightCurlyBracket) {
                // setup temp item
                Wai::JSON::Item tempItem;
                tempItem.type = Wai::JSON::ItemType::Invalid;

                // if requires comma
                if (commaIndex > 0 && lexlings[currentIndex].type == Wai::JSON::LexlingType::Comma) {
                    // next lexling index
                    currentIndex++;
                }

                // check for name string
                if (lexlings[currentIndex].type == Wai::JSON::LexlingType::String) {
                    // set name
                    tempItem.name = lexlings[currentIndex];
                    currentIndex++;
                // error, missing name string
                } else {
                    log->LogError("JSON Error. Object member did not have a name string. Line Number: " + std::to_string(lexlings[currentIndex].location.lineNumber));
                    
                    return output;
                }

                // check for colon
                if (lexlings[currentIndex].type == Wai::JSON::LexlingType::Colon) {
                    // skip past colon
                    currentIndex++;
                // colon not found
                } else {
                    log->LogError("JSON Error. Colon was missing after object name's declaration. Line Number: " + std::to_string(lexlings[currentIndex].location.lineNumber));

                    return output;
                }

                // parse data
                if (lexlings[currentIndex].type == Wai::JSON::LexlingType::Null) {
                    // set type
                    tempItem.type = Wai::JSON::ItemType::Null;

                    // advance
                    currentIndex++;
                } else if (lexlings[currentIndex].type == Wai::JSON::LexlingType::False) {
                    // set type
                    tempItem.type = Wai::JSON::ItemType::False;
                    tempItem.booleanValue = false;

                    // advance
                    currentIndex++;
                } else if (lexlings[currentIndex].type == Wai::JSON::LexlingType::True) {
                    // set type
                    tempItem.type = Wai::JSON::ItemType::True;
                    tempItem.booleanValue = true;

                    // advance
                    currentIndex++;
                } else if (lexlings[currentIndex].type == Wai::JSON::LexlingType::String) {
                    // set type
                    tempItem.type = Wai::JSON::ItemType::String;
                    tempItem.stringValue = lexlings[currentIndex];

                    // advance
                    currentIndex++;
                } else if (lexlings[currentIndex].type == Wai::JSON::LexlingType::Number) {
                    // set type
                    tempItem.type = Wai::JSON::ItemType::Number;
                    tempItem.numberValue = lexlings[currentIndex];

                    // advance
                    currentIndex++;
                } else if (lexlings[currentIndex].type == Wai::JSON::LexlingType::LeftCurlyBracket) {
                    // set item
                    tempItem.type = Wai::JSON::ItemType::Object;
                    tempItem.objectBody = ParseObjectBody(log, lexlings, currentIndex);
                } else if (lexlings[currentIndex].type == Wai::JSON::LexlingType::LeftSquareBracket) {
                    // set item
                    tempItem.type = Wai::JSON::ItemType::Array;
                    tempItem.arrayBody = ParseArrayBody(log, lexlings, currentIndex);
                // error
                } else {
                    log->LogError("JSON Error. While parsing an object body, an invalid lexling appeared. Line Number: " + std::to_string(lexlings[currentIndex].location.lineNumber));

                    return output;
                }

                // append item to list
                output.push_back(tempItem);

                // check for error
                if (log->normalErrorOccured) {
                    return output;
                }

                // next comma index
                commaIndex++;
            }

            // check for right curly brace
            if (lexlings[currentIndex].type == Wai::JSON::LexlingType::RightCurlyBracket) {
                currentIndex++;
            }

            return output;
        }

        // parse array body
        std::vector<Wai::JSON::Item> ParseArrayBody(Wai::Debugging::Log* log, std::vector<Wai::JSON::Lexling>& lexlings, Wai::Index& currentIndex) {
            std::vector<Wai::JSON::Item> output;

            // create comma tracker
            Wai::Index commaIndex = 0;

            // if object body is missing container
            if (lexlings[currentIndex].type != Wai::JSON::LexlingType::LeftSquareBracket) {
                log->LogError("JSON Error. Array body is missing opening left square bracket. Line Number: " + std::to_string(lexlings[currentIndex].location.lineNumber));

                return output;
            // otherwise, skip past bracket
            } else {
                currentIndex++;
            }

            // loop over body
            while (lexlings[currentIndex].type != Wai::JSON::LexlingType::RightSquareBracket) {
                // setup temp item
                Wai::JSON::Item tempItem;
                tempItem.type = Wai::JSON::ItemType::Invalid;

                // if requires comma
                if (commaIndex > 0 && lexlings[currentIndex].type == Wai::JSON::LexlingType::Comma) {
                    // next lexling index
                    currentIndex++;
                }

                // parse data
                if (lexlings[currentIndex].type == Wai::JSON::LexlingType::Null) {
                    // set type
                    tempItem.type = Wai::JSON::ItemType::Null;

                    // advance
                    currentIndex++;
                } else if (lexlings[currentIndex].type == Wai::JSON::LexlingType::False) {
                    // set type
                    tempItem.type = Wai::JSON::ItemType::False;
                    tempItem.booleanValue = false;

                    // advance
                    currentIndex++;
                } else if (lexlings[currentIndex].type == Wai::JSON::LexlingType::True) {
                    // set type
                    tempItem.type = Wai::JSON::ItemType::True;
                    tempItem.booleanValue = true;

                    // advance
                    currentIndex++;
                } else if (lexlings[currentIndex].type == Wai::JSON::LexlingType::String) {
                    // set type
                    tempItem.type = Wai::JSON::ItemType::String;
                    tempItem.stringValue = lexlings[currentIndex];

                    // advance
                    currentIndex++;
                } else if (lexlings[currentIndex].type == Wai::JSON::LexlingType::Number) {
                    // set type
                    tempItem.type = Wai::JSON::ItemType::Number;
                    tempItem.numberValue = lexlings[currentIndex];

                    // advance
                    currentIndex++;
                } else if (lexlings[currentIndex].type == Wai::JSON::LexlingType::LeftCurlyBracket) {
                    // set item
                    tempItem.type = Wai::JSON::ItemType::Object;
                    tempItem.objectBody = ParseObjectBody(log, lexlings, currentIndex);
                } else if (lexlings[currentIndex].type == Wai::JSON::LexlingType::LeftSquareBracket) {
                    // set item
                    tempItem.type = Wai::JSON::ItemType::Array;
                    tempItem.arrayBody = ParseArrayBody(log, lexlings, currentIndex);
                // error
                } else {
                    log->LogError("JSON Error. While parsing an array body, an invalid lexling appeared. Line Number: " + std::to_string(lexlings[currentIndex].location.lineNumber));

                    return output;
                }

                // append item to list
                output.push_back(tempItem);

                // check for errors
                if (log->normalErrorOccured) {
                    return output;
                }

                // next comma index
                commaIndex++;
            }

            // check for right square brace
            if (lexlings[currentIndex].type == Wai::JSON::LexlingType::RightSquareBracket) {
                currentIndex++;
            }

            return output;
        }

    public:
        // turn json root into json string
        std::string JSONToString(Wai::JSON::Root parsedJSON) {
            return JSONItemToString(parsedJSON.masterObject, false, 0) + "\n";
        }

    private:
        // stringify JSON
        std::string JSONItemToString(Wai::JSON::Item item, Wai::Boolean requiresName, Wai::TabDepth tabDepth) {
            std::string output = "";
            Wai::Tabs tabs = Wai::Tabs("\t", tabDepth);

            // add name if required
            if (requiresName) {
                output += tabs.GenerateTabs() + item.name.stringedValue + ": ";
            } else {
                output += tabs.GenerateTabs();
            }

            // append each item based on type
            if (item.type == Wai::JSON::ItemType::Object) {
                // append opening brace
                output += "{\n";

                // append sub-items
                for (Wai::Index i = 0; i < item.objectBody.size(); i++) {
                    // append sub-objects
                    output += JSONItemToString(item.objectBody[i], true, tabDepth + 1);

                    // append comma if necessary
                    if (i < item.objectBody.size() - 1) {
                        output += ",";
                    }

                    // next line
                    output += "\n";
                }

                // append closing brace
                output += tabs.GenerateTabs() + "}";
            } else if (item.type == Wai::JSON::ItemType::Array) {
                // append opening brace
                output += "[\n";

                // append sub-items
                for (Wai::Index i = 0; i < item.arrayBody.size(); i++) {
                    // append sub-objects
                    output += JSONItemToString(item.arrayBody[i], false, tabDepth + 1);

                    // append comma if necessary
                    if (i < item.arrayBody.size() - 1) {
                        output += ",";
                    }

                    // next line
                    output += "\n";
                }

                // append closing brace
                output += tabs.GenerateTabs() + "]";
            } else if (item.type == Wai::JSON::ItemType::False) {
                output += "false";
            } else if (item.type == Wai::JSON::ItemType::True) {
                output += + "true";
            } else if (item.type == Wai::JSON::ItemType::Null) {
                output += + "null";
            } else if (item.type == Wai::JSON::ItemType::String) {
                output += item.stringValue.stringedValue;
            } else if (item.type == Wai::JSON::ItemType::Number) {
                output += item.numberValue.stringedValue;
            } else {
                output += "[INVALID]";
            }

            return output;
        }
    };
}
