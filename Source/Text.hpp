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
}
