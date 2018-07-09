#include <iostream>
#include <string>
#include <strings.h>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "HTTP_Parser.h"

#ifndef REQUESTS_H
#define REQUESTS_H

int serverSocket(std::string host, std::string request, int client);

int requestAndForward(int sock);

std::string getResponseFromHost(int connectedSocket, bool verbose=false);

int sendRequestToHost(std::string host, std::string request);

int forwardMessage(int socketToRead, int socketToForward);

int startProxy(int port);

int runProxy(int sockServer);

int getRequestAndForward(int sockBrowser, bool verbose=false);

#endif //REQUESTS_H