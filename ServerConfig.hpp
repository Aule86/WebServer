#pragma once

#include <string>
#include <map>
#include <vector>
#include <stdexcept>

struct LocationConfig
{
    std::string path;          // / , /images, /upload
    std::string root;          // root específico del location
    std::string index;         // index.html
    bool autoindex;

    LocationConfig()
        : path("/")
        , root("")
        , index("index.html")
        , autoindex(false)
    {}
};

class ServerConfig
{
    public:
        // ---- Core ----
        std::string host;          // 127.0.0.1
        int         port;          // 8080
        std::string server_name;   // localhost

        // ---- Paths ----
        std::string root;          // /html
        std::string index;         // index.html

        // ---- Limits ----
        size_t      client_max_body_size;

        // ---- Locations ----
        std::vector<LocationConfig> locations;

        // ---- Constructor ----
        ServerConfig(): host("0.0.0.0"), port(0), server_name(""), root(""), index("index.html"), client_max_body_size(1000000){}

        private:
            std::vector<ServerConfig> servers;


    // ---- Utils ----
    void addLocation(const LocationConfig &loc)
    {
        locations.push_back(loc);
    }

    const LocationConfig* getLocation(const std::string &uri) const
    {
        for (size_t i = 0; i < locations.size(); i++)
        {
            if (uri.find(locations[i].path) == 0)
                return &locations[i];
        }
        return NULL;
    }
};
