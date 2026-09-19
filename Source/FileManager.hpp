#pragma once

#include "Basic.hpp"
#include <string>
#include <vector>

namespace Wai::Files {
	// one file
	class File {
	public:
		std::string path;
		std::string data;

        // constructors
        File() {
            path = "";
            data = "";
        }
        File(std::string _path, std::string _data) {
            path = _path;
            data = _data;
        }
	};

    class Files {
    public:
        std::vector<Wai::Files::File> files;
    };

	// multiple files
	class FileManager {
	public:
        // load file into std::string
        Wai::Files::File LoadTextFile(CompilerLabs::Logs::Log* log, std::string _filePath) {
            // open file stream
            std::ifstream file(_filePath);

            // check for error
            if (!file) {
                // setup error
                log->LogCriticalError("Text file could not be loaded / found.", {
                    CompilerLabs::Logs::LogEntrySubdata("file_path", _filePath)
                });

                return Wai::Files::File();
            }

            // read file into string
            std::ostringstream stringstream;
            stringstream << file.rdbuf();
            std::string data = stringstream.str();

            // return file
            return Wai::Files::File(_filePath, data);
        }
	};
}
