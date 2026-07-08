#pragma once

#include "Basic.hpp"
#include <string>
#include <vector>

namespace Wai::OS {
    class TerminalArguments {
    public:
        std::vector<std::string> filePaths;

        // constructors
        TerminalArguments() {
            filePaths = std::vector<std::string>();
        }
        TerminalArguments(int argc, char* argv[]) {
            // open vector
            std::vector<std::string> args(argv, argv + argc);

            // parse args
            ParseArguments(args);
        }
        TerminalArguments(std::vector<std::string> arguments) {
            ParseArguments(arguments);
        }

        // parse os command line arguments
        void ParseArguments(std::vector<std::string> arguments) {
            // check for no args
            if (arguments.size() < 2) {
                // setup as null
                filePaths.push_back("");

                return;
            }

            // otherwise, setup args
            for (Wai::Index index = 2; index < arguments.size(); index++) {
                filePaths.push_back(arguments[index]);
            }

            return;
        }
    };
}
