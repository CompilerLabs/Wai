#include "Application.hpp"

int main(int argc, char* argv[]) {
    // create application
    Wai::Application application = Wai::Application(argc, argv);

    // run Wai
    application.Run();
}
