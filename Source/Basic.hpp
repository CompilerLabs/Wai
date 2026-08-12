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
	typedef float F32;
	typedef double F64;

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

    // tabs
    class Tabs {
    public:
        // style of tabs
        std::string tabStyle;
        Wai::TabDepth tabDepth;

        // constructors
        Tabs() {
            tabStyle = "\t";
            tabDepth = 0;
        }
        Tabs(std::string _tabStyle) {
            tabStyle = _tabStyle;
            tabDepth = 0;
        }
        Tabs(Wai::TabDepth _tabDepth) {
            tabStyle = "\t";
            tabDepth = _tabDepth;
        }
        Tabs(std::string _tabStyle, Wai::TabDepth _tabDepth) {
            tabStyle = _tabStyle;
            tabDepth = _tabDepth;
        }

        // generate tabs
        std::string GenerateTabs(Wai::TabDepth tabCount) {
            std::string output = "";

            // append tabs
            for (Wai::TabDepth i = 0; i < tabCount; i++) {
                output += tabStyle;
            }
            
            return output;
        }
        std::string GenerateTabs() {
            return GenerateTabs(tabDepth);
        }
    };
}
