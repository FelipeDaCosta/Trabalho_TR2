#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>

#include <stdio.h>

#include "HTTP_Parser.h"
#include "TCP_Client.h"

int main(int argc, char *argv[]) {  
    TCP_Client *client = new TCP_Client(80);
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[1048576];
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = 8228;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    int opt = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cout << "Erro ao setar opcaos do socket" << std::endl;
    }

    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Falha no bind!" << std::endl;
        return 0;
    }
    listen(sockfd, 10);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    read(newsockfd, buffer, 4095);
    std::string host = getHostFromRequest(std::string(buffer));
    client->connectToHost(host);
    client->sendRequest(buildRequest(host));
    std::string request = client->getResponse();
    write(newsockfd, request.c_str(), request.size());
    close(newsockfd);
    close(sockfd);
    return 0;
}