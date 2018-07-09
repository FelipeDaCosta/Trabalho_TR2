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
    const int SIZE_OF_BUFFER = 2097152;
    char buffer[SIZE_OF_BUFFER];
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
    std::cout << "Ouvindo na porta " << portno << std::endl;
    while(true) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if(newsockfd < 0)
            continue;
        pid_t pid = fork();
        if(pid == 0) {
            recv(newsockfd, buffer, SIZE_OF_BUFFER, 0);
            std::string request(buffer);
            std::cout << "-------------" << std::endl;
            std::cout << "Recebido request:" << std::endl;
            std::cout << request << std::endl;
            std::string httpMethod = getHTTPMethod(request);
            if(httpMethod == "GET" && request.find("firefox") == std::string::npos) {
                std::string host = getHostFromRequest(request);
                if(client->connectToHost(host)) {
                    client->sendRequest(addHeader(request, "Connection: close"));
                    std::string reply = client->getResponse();
                    write(newsockfd, reply.c_str(), reply.size());
                }
            }
            close(newsockfd);
            _exit(0);
        }
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}