#pragma once

#include <iostream>
#include <vector>
#include "Compiler.hpp"
#include "OSCMD.hpp"

namespace Wai {
    class Application {
        Wai::OS::TerminalArguments terminalArgumentHandler;
        Wai::Compiling::Compiler compiler;

    public:
        // constructors
        Application() {
            terminalArgumentHandler = Wai::OS::TerminalArguments();
        }
        Application(std::vector<std::string> _termainalArguments) {
            terminalArgumentHandler.ParseArguments(_termainalArguments);
        }
        Application(int argc, char* argv[]) {
            terminalArgumentHandler = Wai::OS::TerminalArguments(argc, argv);
        }

        // run compiler
        void Run() {
            // starting message
            std::cout << "Running Compiler!" << std::endl;

            // compile
            compiler.Compile();
        }
    };
}
