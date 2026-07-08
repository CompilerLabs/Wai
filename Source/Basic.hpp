#pragma once

#include <string>
#include <vector>
#include <stdint.h>

/* Basic Types */
namespace Wai {
	// intger types
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

	// floating points
	typedef float Float32;
	typedef double Float64;

    // character defines
    typedef Wai::U8 Character;
    typedef Wai::U64 TabDepth;

    // generic defines
    typedef Wai::U64 Length;
    typedef Wai::U64 Index;

    // text location defines
    typedef Wai::Index FileIndex;
    typedef Wai::U64 LineNumber;
    typedef Wai::Index CharacterIndex;
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
