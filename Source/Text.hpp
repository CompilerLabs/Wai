#pragma once

#include "Basic.hpp"

namespace Wai::Text {
    // string length
    typedef Wai::Length StringLength;

    // text location
    class TextLocation {
    public:
        Wai::FileIndex fileIndex;
        Wai::LineNumber lineNumber;
        Wai::Buffer range;

        // constructors
        TextLocation(Wai::FileIndex _fileIndex, Wai::LineNumber _lineNumber, Wai::Buffer _range) {
            fileIndex = _fileIndex;
            lineNumber = _lineNumber;
            range = _range;
        }
        TextLocation() {
            fileIndex = 0;
            lineNumber = 0;
            range = Wai::Buffer();
        }
    };

    // string class
    class String {
        // string data
        Wai::List data;

        // calculate string length (does NOT include trailing zero)
        Wai::Text::StringLength CalculateCStringLength(char* stringData) {
            Wai::Text::StringLength output = 0;
            
            // search for null terminator
            while (stringData[output] != '\0') {
                // next byte
                output++;
            }

            return output;
        }

    public:
        // constructors
        String() {
            data = Wai::List(Wai::DefaultListIncrease);
        }
        String(char* stringData) {
            Open(stringData, Wai::DefaultListIncrease);
        }
        String(char* stringData, Wai::Increase increase) {
            Open(stringData, increase);
        }

        // append a CString
        void AppendCString(char* cString) {
            data.AppendBuffer(Wai::Buffer(cString));
        }

        // open new string
        void Open(char* stringData, Wai::Increase increase) {
            // append string data
            Wai::Text::StringLength stringLength = CalculateCStringLength(stringData);

            // create list
            data = Wai::List(increase);

            // append string if necessary
            if (stringData != 0) {
                data.AppendBuffer(Wai::Buffer(stringData));
            }
        }

        // index characters operator
        Wai::Character operator[](Wai::CharacterIndex index) {
            return ((Wai::Character*)data.allocation.start)[index];
        }

        // get length
        Wai::Text::StringLength Length() {
            return data.fillLength;
        }
    };
}
