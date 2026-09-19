#pragma once

#include "Basic.hpp"
#include <string>
#include <vector>

namespace Wai::OS {
    class TerminalArguments {
    public:
        std::string jsonSettingsFilePath;

        // constructors
        TerminalArguments() {
            jsonSettingsFilePath = "";
        }
        TerminalArguments(CompilerLabs::Logs::Log* log, int argc, char* argv[]) {
            // open vector
            std::vector<std::string> args(argv, argv + argc);

            // parse args
            ParseArguments(log, args);
        }
        TerminalArguments(CompilerLabs::Logs::Log* log, std::vector<std::string> arguments) {
            ParseArguments(log, arguments);
        }

        // parse os command line arguments
        void ParseArguments(CompilerLabs::Logs::Log* log, std::vector<std::string> arguments) {
            // check for no args
            if (arguments.size() < 2) {
                // log
                log->LogCriticalError("No arguments given to application to pass to compiler.");

                // setup blank
                jsonSettingsFilePath = "";
                
                return;
            }

            // otherwise, setup args
            jsonSettingsFilePath = arguments[1];

            return;
        }
    };
}
