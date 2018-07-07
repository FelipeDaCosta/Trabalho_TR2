#include "TCP_Client.h"
#include <string.h>

/*
*   Construtor do cliente TCP, recebe a porta que o cliente vai se conectar e cria
*       o descritor do socket
*/
TCP_Client::TCP_Client(int portNo) : port(portNo), 
                                     sockin(socket(AF_INET, SOCK_STREAM, 0)) {
    if(sockin < 0) {
        std::cerr << "Erro ao criar socket no cliente TCP" << std::endl;
        exit(-1);
    }
}

bool TCP_Client::connectToHost(std::string hostName) {
    struct hostent *hostServer;
    hostServer = gethostbyname(hostName.c_str());
    if(hostServer == nullptr) {
        std::cerr << "Erro na tentativa de resolucao do host: " << hostName << std::endl;
        return false;
    }
         
    //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
    server_addr.sin_addr =  *((struct in_addr **) hostServer->h_addr_list)[0];

    // Criando estrutura de addr do servidor
    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr)); // Zerando a struct
    bcopy((char *) hostServer->h_addr, (char *) 
          &server_addr.sin_addr.s_addr, hostServer->h_length);
    server_addr.sin_addr = *((struct in_addr**) hostServer->h_addr_list)[0];
    server_addr.sin_port = htons(this->port);
    server_addr.sin_family = AF_INET;

    // Iniciando a conexao
    if(connect(sockin, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Erro na tentativa de conexao com o host: " << hostName  << std::endl;
        return false;
    }
    std::cout << "Conexao realizada com o host: " << hostName  << std::endl;
    return true;
}

bool TCP_Client::sendRequest(std::string request) {
    if(send(sockin, request.c_str(), request.size(), 0) < 0) {
        std::cout << "Envio de request falhou" << std::endl;
        return false;
    }
    return true;
}

std::string TCP_Client::getResponse(int bufferSize) {
    char buffer[bufferSize];
    std::string response;
    while(recv(sockin, buffer, bufferSize, 0) > 0) {
        response += buffer;
    }
    return response;
}