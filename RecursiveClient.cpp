#include "RecursiveClient.h"

RecursiveClient::RecursiveClient()  {
}

std::string RecursiveClient::getPage(std::string host, std::string URL) {
    std::string key = host + URL;
    if(this->pages.find(key) == this->pages.end()) {
        std::cout << "URL:  " << key << " nao foi baixada." << std::endl;
        return "";
    }
    return this->pages[key];
}

bool RecursiveClient::dumpPage(std::string host, std::string URL) {
    if(this->pages.find(URL) == this->pages.end()) {
        int sockfd = sendRequestToHost(host, buildRequest(host, URL));
        std::string reply = getResponseFromHost(sockfd);
        this->pages[host + URL] = reply;
        return true;
    }
    return false;
}

bool RecursiveClient::dumpPageAndTree(std::string host, std::string URL) {
    return true;
}

int main() {
    std::string host;
    std::string URL;
    RecursiveClient *rc = new RecursiveClient();
    rc->dumpPage("samotabr.com", "/");
    rc->dumpPage("samotabr.com", "/galeria/ilustracoes");
    rc->dumpPage("samotabr.com", "/animacoes");
    while(true) {
        std::cout << "Digite o host: ";
        std::cin >> host;
        std::cout << "Digite a URL: ";
        std::cin >> URL;
        std::string reply = rc->getPage(host, URL);
        std::cout << reply << std::endl;
    }
    return 0;
}