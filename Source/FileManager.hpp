#pragma once

#include "Basic.hpp"
#include "Log.hpp"
#include <string>
#include <vector>

namespace Wai::FileManager {
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
	
	// one file
	class File {
	public:
		std::string path;
		std::string data;
	};

	// multiple files
	class Files {
	public:
		// files allocation
		std::vector<Wai::FileManager::File> files;
	};
}
