#include "proxythread.h"

int ProxyThread::sendRequestToHost(std::string host, std::string request) {
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
    if(::connect(sockClient, resp->ai_addr, resp->ai_addrlen) < 0){
        std::cout << "Erro ao conectar no host: " << host << std::endl;
        exit(-1);
    }
    if(send(sockClient, request.c_str(), request.size(), 0) < 0){
        std::cout << "Erro ao enviar resposta para o browser" << std::endl;
        exit(-1);
    }
    return sockClient;
}

// Le a mensagem no socket usado para fazer o pedido (socketToRead)
// e reenvia a mensagem para o socket do browser (socketToForward)
int ProxyThread::forwardMessage(int socketToRead, int socketToForward) {
    // Lendo a resposta do host
    const int SIZE_OF_BUFFER = 524288;
    char buffer[SIZE_OF_BUFFER];
    int recv_size, total_recv_size = 0;
    while((recv_size = recv(socketToRead, buffer + total_recv_size, SIZE_OF_BUFFER - total_recv_size, 0)) > 0){
        std::cout << "Recebeu mensagem de tamanho " << recv_size << std::endl;
        total_recv_size += recv_size;
    }
    std::cout << " Finalizou de receber a resposta de tamanho total: " << total_recv_size << std::endl;

    // Enviando a resposta do host de volta para o browser
    std::string reply(buffer);
    send(socketToForward, reply.c_str(), reply.size(), 0);
    emit sendReply(reply);
    return socketToForward;
}

int ProxyThread::requestAndForward(int sockBrowser){
    const int BUFFER_SIZE = 32768;
    char buffer[BUFFER_SIZE];
    recv(sockBrowser, buffer, BUFFER_SIZE, 0);

    std::string  newReq(buffer);
    std::string method = getHTTPMethod(newReq);
    if(method != "GET"){
        std::cout << "Proxy nao suporta metodo " << method << std::endl;
        exit(0);
    }

    std::string host = getHostFromRequest(newReq);
    // Funciona melhor assim
    newReq = replaceHeader(newReq, "Accept-Encoding: ", "identity");
    newReq = replaceHeader(newReq, "Connection: ", "close");
    // Alguns sites dao problema quando a url vai com o host
    newReq = fixRequestURL(newReq);


    emit sendRequest(newReq);

    // Faz um pedido para o host e retorna a socket qe fez o pedido
    int sockClient = sendRequestToHost(host, newReq);
    forwardMessage(sockClient, sockBrowser);
    close(sockBrowser);
    close(sockClient);

    // Manda o request
    return 0;
}

ProxyThread::ProxyThread(QObject *parent, int port)
{
    restart = false;
    abort = false;

    // Structs sockaddr servidor
    struct sockaddr_in serv_addr;

    // Zerando a struct
    bzero((char *) &serv_addr, sizeof(serv_addr));
    // Setando atributos do addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // Criando socket do servidor Proxy
    int socketServer = socket(AF_INET, SOCK_STREAM, 0);
    if(socketServer < 0){
        std::cout << "Erro ao criar socket" << std::endl;
        exit(-1);
    }

    // Seta opcao de reutilizar o socket caso ainda esteja em uso
    int opt = 1;
    if(setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cout << "Erro ao setar opcaos do socket" << std::endl;
        exit(-1);
    }

    // Faz o bind do socket com a porta
    if(bind(socketServer, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Falha no bind!" << std::endl;
        exit(-1);
    }

    // Comeca a ouvir a porta
    if(listen(socketServer, 500) < 0) {
        std::cout << "Erro na socket_server ao ouvir a porta " << port << std::endl;
        exit(-1);
    }
    this->sockServer = socketServer;
}

ProxyThread::~ProxyThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
    ::close(sockServer);
}

void ProxyThread::Start()
{
    if (!isRunning()) {
        start(LowPriority);
    } else {
        restart = true;
        condition.wakeOne();
    }
}

void ProxyThread::run()
{
    forever {
        mutex.lock();

        int newsockfd = accept(sockServer, &cli_addr, (socklen_t*) &clilen);
//        pid_t pid = fork();
//        if(pid == 0){
            requestAndForward(newsockfd);
//            _exit(0);
//        }
        close(newsockfd);

        if (abort) {
            return;
        }
        mutex.unlock();
    }
}
