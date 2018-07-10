#include "proxy.h"

int sendRequestToHost(std::string host, std::string request) {
    struct addrinfo hints;
	struct addrinfo *resp;
	bzero((char *) &hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if(getaddrinfo(host.c_str(), "80", &hints, &resp) != 0){
		std::cout << "Erro ao resolver host: " << host << std::endl;
		exit(1);
	}
	// Cria socket do cliente que vai enviar a requisicao e receber a resposta
	int sockClient = socket(resp->ai_family, resp->ai_socktype, resp->ai_protocol);
	if(sockClient < 0){
		std::cout << "Erro ao criar client socket" << std::endl;
		exit(-1);
	}

	// Conecta e envia o request para o browser
	if(connect(sockClient, resp->ai_addr, resp->ai_addrlen) < 0){
		std::cout << "Erro ao conectar no host: " << host << std::endl;
		exit(-1);
	}
	if(send(sockClient, request.c_str(), request.size(), 0) < 0){
		std::cout << "Erro ao enviar resposta para o browser" << std::endl;
		exit(-1);
	}
    return sockClient;
}

// Faz a leitura de um socket (ja conectado) e retorna o resulado
std::string getResponseFromHost(int connectedSocket, bool verbose) {
	const int SIZE_OF_BUFFER = 524288;
	char buffer[SIZE_OF_BUFFER];
	int recv_size, total_recv_size = 0;
	while((recv_size = recv(connectedSocket, buffer + total_recv_size, SIZE_OF_BUFFER - total_recv_size, 0)) > 0){
		if(verbose)
			std::cout << "Recebeu mensagem de tamanho " << recv_size << std::endl;
		total_recv_size += recv_size;
	}
	if(verbose)
		std::cout << " Finalizou de receber a resposta de tamanho total: " << total_recv_size << std::endl;
	return std::string(buffer);
}

// Le a mensagem no socket usado para fazer o pedido (socketToRead)
// e reenvia a mensagem para o socket do browser (socketToForward)
int forwardMessage(int socketToRead, int socketToForward) {
	std::string reply = getResponseFromHost(socketToRead);
	send(socketToForward, reply.c_str(), reply.size(), 0);
	return socketToForward;
}

int getRequestAndForward(int sockBrowser, bool verbose){
	const int BUFFER_SIZE = 32768;
	char buffer[BUFFER_SIZE];
	recv(sockBrowser, buffer, BUFFER_SIZE, 0);

	std::string newReq(buffer);
	std::string method = getHTTPMethod(newReq);
	if(method != "GET"){
		if(verbose)
			std::cout << "Proxy nao suporta metodo " << method << std::endl; 
//		exit(0); // Sai do novo processo
	}
	
	
	std::string host = getHostFromRequest(newReq);
	std::string requestMethod = getHTTPMethod(newReq);
	// Funciona melhor assim
	newReq = replaceHeader(newReq, "Accept-Encoding: ", "identity");
	newReq = replaceHeader(newReq, "Connection: ", "close");
	// Alguns sites dao problema quando a url vai com o host
	newReq = fixRequestURL(newReq);


	// Faz um pedido para o host e retorna a socket qe fez o pedido
    int sockClient = sendRequestToHost(host, newReq);
    forwardMessage(sockClient, sockBrowser);
	close(sockBrowser);
	close(sockClient);
	return 0;
}

int startProxy(int port) {
		// Structs sockaddr servidor
	struct sockaddr_in serv_addr;

	// Zerando a struct
	bzero((char *) &serv_addr, sizeof(serv_addr));
	// Setando atributos do addr
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	// Criando socket do servidor Proxy
	int sockServer = socket(AF_INET, SOCK_STREAM, 0);
	if(sockServer < 0){
		std::cout << "Erro ao criar socket" << std::endl;
		exit(-1);
	}

	// Seta opcao de reutilizar o socket caso ainda esteja em uso
	int opt = 1;
    if(setsockopt(sockServer, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cout << "Erro ao setar opcaos do socket" << std::endl;
		exit(-1);
    }

	// Faz o bind do socket com a porta
	if(bind(sockServer, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		std::cout << "Falha no bind!" << std::endl;
        exit(-1);
	}

	// Comeca a ouvir a porta
	if(listen(sockServer, 500) < 0) {
		std::cout << "Erro na socket_server ao ouvir a porta " << port << std::endl;
		exit(-1);
	}
	return sockServer;
}

int runProxy(int sockServer) {
	struct sockaddr cli_addr;
	socklen_t clilen = sizeof(cli_addr);

	while(1){
		int newsockfd = accept(sockServer, &cli_addr, (socklen_t*) &clilen);
    	pid_t pid = fork();
    	if(pid == 0){
			getRequestAndForward(newsockfd);
			_exit(0);
		}
		close(newsockfd);
	}
}
