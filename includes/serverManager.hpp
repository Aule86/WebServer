#ifndef  SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "webserv.hpp"

class WebServer;
class ServerManager
{
    public:
        ServerManager();
        ~ServerManager();
        void start();
        void stop();


    private:
        std::string ip;
        size_t ports;
        size_t n_servers;
};

#endif