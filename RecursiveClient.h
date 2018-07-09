#include <iostream>
#include <unordered_map>

#include "TCP_Client.h"
#include "proxy.h"

#ifndef RECURSIVE_CLIENT_H
#define RECURSIVE_CLIENT_H

class RecursiveClient {
    public:
        RecursiveClient();
        std::string getPage(std::string host, std::string URL);
        bool dumpPage(std::string host, std::string URL="/");
        bool dumpPageAndTree(std::string host, std::string URL="/");
    private:
        // Paginas baixadas ficam neste map
        std::unordered_map<std::string, std::string> pages;
};

#endif