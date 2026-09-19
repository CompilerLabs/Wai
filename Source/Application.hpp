#pragma once

#include <iostream>
#include <vector>
#include "OSCMD.hpp"
#include "FileManager.hpp"

namespace Wai {
    // the highest level of the program
    class Application {
        Wai::OS::TerminalArguments terminalArgumentHandler;
        Wai::Files::FileManager fileManager;

    public:
        // log
        CompilerLabs::Logs::Log log;

        // constructors
        Application() {
            terminalArgumentHandler = Wai::OS::TerminalArguments();
        }
        Application(std::vector<std::string> _terminalArguments) {
            terminalArgumentHandler.ParseArguments(&log, _terminalArguments);
        }
        Application(int argc, char* argv[]) {
            terminalArgumentHandler = Wai::OS::TerminalArguments(&log, argc, argv);
        }

        // run compiler
        void Run() {
            // get json settings file
            log.LogNote("Application start. Searching for compilation settings...");
            std::string jsonFilePath = terminalArgumentHandler.jsonSettingsFilePath;
            if (log.CheckAnyErrorOccured()) {
                return;
            }

            // load json settings file
            log.LogNote("Application found settings file.");
            Wai::Files::File settings = fileManager.LoadTextFile(&log, jsonFilePath);
            if (log.CheckAnyErrorOccured()) {
                return;
            }

            // parse json
            log.LogNote("Parsing settings file.");
            CompilerLabs::JSON::JSONCompiler jsonCompiler;
            CompilerLabs::JSON::Root root = jsonCompiler.CompileJSON(&log, settings.data);
            if (log.CheckAnyErrorOccured()) {
                return;
            }

            // log
            log.LogNote("Application is loading settings into a workspace.");

            // import settings
            // Wai::Compiling::Workspace workspace;
            // workspace.ImportSettingsFromJSON(&root);

            // compile
            // compiler.Compile(&log, &workspace);
        }
    };
}
