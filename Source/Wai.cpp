#include "Application.hpp"

int main(int argc, char* argv[]) {
    // create application
    Wai::Application application = Wai::Application(argc, argv);
    if (application.log.CheckAnyErrorOccured()) {
        return 1;
    }

    // run Wai
    application.Run();

    return 0;
}
