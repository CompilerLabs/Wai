#pragma once

#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdint.h>

/* Basic Types */
namespace Wai {
	// integer types
	typedef uint8_t U8;
	typedef uint16_t U16;
	typedef uint32_t U32;
	typedef uint64_t U64;
	typedef int8_t S8;
	typedef int16_t S16;
	typedef int32_t S32;
	typedef int64_t S64;

	// pointer types
	typedef void* Address;

	// boolean type
	typedef bool Boolean;

	// floating points
	typedef float Float32;
	typedef double Float64;

    // character defines
    typedef Wai::U8 Character;
    typedef Wai::U64 TabDepth;

    // generic defines
    typedef Wai::U8 Byte;
    typedef Wai::U64 Length;
    typedef Wai::U64 Index;
    typedef Wai::U64 Increase;

    // text location defines
    typedef Wai::Index FileIndex;
    typedef Wai::U64 LineNumber;
    typedef Wai::Index CharacterIndex;

    // default list increase
    const Wai::Increase DefaultListIncrease = 1024;

    // error class
    class Error {
    public:
        bool occured;
        std::string reason;
        std::string extraInformation;

        // constructors
        Error() {
            occured = false;
            reason = "No problem was detected.";
            extraInformation = "";
        }
        Error(bool _occured, std::string _reason) {
            occured = _occured;
            reason = _reason;
            extraInformation = "";
        }
        Error(bool _occured, std::string _reason, std::string _extraInformation) {
            occured = _occured;
            reason = _reason;
            extraInformation = _extraInformation;
        }

        // to json string
        std::string ToJSONString() {
            std::string output = "";

            // create json header
            output += "\"error\": {\n";

            // add occured value
            output += "\t\"occured\": ";
            if (occured) {
                output += "true";
            } else {
                output += "false";
            }
            output += ",\n";

            // add reason
            output += "\t\"reason\": \"" + reason + "\"";

            // add extra information
            if (extraInformation != "") {
                output += ",\n\t" + extraInformation + "\n";
            } else {
                output += "\n";
            }

            // create json footer
            output += "}\n";

            return output;
        }
    };

    // buffer
    class Buffer {
    public:
        Wai::Address start;
        Wai::Address end;

        // constructors
        Buffer() {
            start = 0;
            end = 0;
        }
        Buffer(Wai::Address _start, Wai::Address _end) {
            start = _start;
            end = _end;
        }
        Buffer(char* cString) {
            start = cString;
            end = (Wai::Address)(strlen(cString) - 2);
        }

        // length of buffer
        Wai::Length Length() {
            return (Wai::Length)((Wai::U64)end - (Wai::U64)start + 1);
        }

        // open
        void Allocate(Wai::Length _allocationSize) {
            start = malloc(_allocationSize);
            if (start != 0) {
                end = (Wai::Address)((Wai::U64)start + _allocationSize - 1);
            } else {
                end = 0;
            }
        }

        // close
        void Deallocate() {
            if (start != 0) {
                // clear buffer
                free(start);

                // zero out values
                start = 0;
                end = 0;
            }
        }

        // check if buffer is empty
        Wai::Boolean IsEmpty() {
            return start == 0 && end == 0;
        }
    };

    // list type
    class List {
    public:
        Wai::Buffer allocation; // the raw buffer
        Wai::Length fillLength; // the amount of bytes used
        Wai::Increase increase; // the amount that the list grows by every reallocation

        // constructors
        List() {
            allocation = Wai::Buffer();
            fillLength = 0;
            increase = Wai::DefaultListIncrease;
        }
        List(Wai::Increase _increase) {
            allocation.Allocate(_increase);
            fillLength = 0;
            increase = _increase;
        }

        // calculate content buffer
        Wai::Buffer CalculateFillBuffer() {
            return Wai::Buffer(allocation.start, allocation.start + fillLength - 1);
        }

        // append a buffer
        void AppendBuffer(Wai::Buffer newData) {
            // calculate new list fill length
            Wai::Length newFillLength = allocation.Length() + newData.Length();

            // check for reallocation
            if (newFillLength > allocation.Length()) {
                // allocate new buffer
                Wai::Buffer newAllocation = Wai::Buffer();
                newAllocation.Allocate(newFillLength);

                // copy old data to new buffer
                for (Wai::Length i = 0; i < allocation.Length(); i++) {
                    ((Wai::Byte*)newAllocation.start)[i] = ((Wai::Byte*)allocation.start)[i];
                }

                // deallocate old buffer
                allocation.Deallocate();

                // install new buffer
                allocation = newAllocation;
            }

            // append new data
            for (Wai::Length i = 0; i < newData.Length(); i++) {
                ((Wai::Byte*)(allocation.start + fillLength - 1))[i] = ((Wai::Byte*)newData.start)[i];
            }
        }

        // append a list
        void AppendList(Wai::List newData) {
            AppendBuffer(newData.CalculateFillBuffer());
        }

        // destructors
        ~List() {
            allocation.Deallocate();
        }
    };

    class Current {
    public:
        Wai::Buffer range;
        Wai::Address progress;

        Current() {
            range = Wai::Buffer();
            progress = 0;
        }
        Current(Wai::Buffer _range, Wai::Address _progress) {
            range = _range;
            progress = _progress;
        }
    };

    // file loader
    class FileLoader {
    public:
        Wai::Error error;

        // load file into std::string
        std::string LoadTextFile(std::string _filePath) {
            // null init error
            error = Wai::Error();

            // open file stream
            std::ifstream file(_filePath);

            // check for error
            if (!file) {
                // setup error
                error = Wai::Error(true, "Text file could not be loaded / found.", "\"file_path\": \"" + _filePath + "\"");

                return "";
            }

            // read file into string
            std::ostringstream stringstream;
            stringstream << file.rdbuf();
            return stringstream.str();
        }
    };
}

/* Files */
namespace Wai::FileManagement {
	// one file
	class File {
	public:
		std::string path;
		std::string data;
	};

	// multiple files
	class Files {
	public:
		std::vector<File> files;
	};
}
