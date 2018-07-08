#include <iostream>
#include <string>
#include <strings.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "HTTP_Parser.h"

#ifndef TCP_CLIENT
#define TCP_CLIENT

class TCP_Client {
    private:
        int sockin;
        int port;
        struct sockaddr_in server_addr;
    public:
        TCP_Client(int portNo);
        ~TCP_Client();
        bool connectToHost(std::string hostName);
        bool sendRequest(std::string request);
        std::string getResponse(int bufferSize=512);
        std::string get(std::string host);
};

#endif