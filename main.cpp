#include <string>
#include <unistd.h>
#include "WebServer.hpp"




int main(int argc, char **argv)
{
    try {
        WebServer ws;
        std::string config;

        if (argc == 1)
            config = "default.conf";
        else if (argc == 2)
            config = argv[1];
        else
            return 1;

        ws.checkfile(config);
        ws.readline(config);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
