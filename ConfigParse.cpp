#include "WebServer.hpp"
#include "ServerConfig.hpp"

void WebServer::checkfile(const std::string filename)
{
    if(filename.substr(filename.size() - 5) != ".conf")
        throw InvalidName();
    
}
std::string WebServer::trim(std::string &line)

{
    size_t start = line.find_first_not_of(" \t");
    size_t end   = line.find_last_not_of(" \t");
    if (start == std::string::npos)
        return "";
    return line.substr(start, end - start + 1);
}


void WebServer::readline(const std::string filename)
{
    std::ifstream file(filename.c_str());

    ServerConfig currentServer;

    if(!file.is_open())
        throw CantOpen();

    bool in_Server = false;
    std::string line;

    while(std::getline(file, line))
    {
        line  = trim(line);
        if(line.empty())
            continue;

        if(line == "server {")
        {
            if(in_Server == true)
                throw InvalidName();
            currentServer = ServerConfig();
            in_Server == true;
            continue;
        }
        if(line.find("listen") == 0 )
        {
            
            size_t pos = line.find(" ");
            std::string value = line.substr(pos + 1);
            value.erase(value.size() - 1); 

            size_t colon = value.find(":");
            currentServer.host = value.substr(0, colon);
            currentServer.port = atoi(value.substr(colon + 1).c_str());
            std::cout << "dentro" << std::endl;
        }
        if(line.find("root")== 0)
        {
            
        }


        
        
    }
    std::cout << "Host: " << currentServer.host << " Port: " << currentServer.port << std::endl;


}