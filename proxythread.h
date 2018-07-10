#ifndef PROXYTHREAD_H
#define PROXYTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

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

class ProxyThread : public QThread
{
    Q_OBJECT
public:
    ProxyThread(QObject *parent = 0, int port = 8228);
    ~ProxyThread();
    void Start();

protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition condition;
    bool restart;
    bool abort;

    struct sockaddr cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    int sockServer;

    int serverSocket(std::string host, std::string request, int client);
    int requestAndForward(int sock);
    int sendRequestToHost(std::string host, std::string request);
    int forwardMessage(int socketToRead, int socketToForward);

    int startProxy(int port);

signals:
    void sendRequest(std::string request);
    void sendReply(std::string reply);
};

#endif // PROXYTHREAD_H
