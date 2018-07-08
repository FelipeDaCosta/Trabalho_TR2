#include "Spider.h"

std::string getHtmlFromHost(std::string host, std::string URL) {
    TCP_Client *client = new TCP_Client(80);
    if(client->connectToHost(host)) {
        client->sendRequest(buildRequest(host, URL));
        return client->getResponse();
    }
    return "";
}

std::vector<std::string> spider(std::string host, std::string rootURL) {
    std::vector<std::string> urlTree;
    std::regex href("href=\"(.*?)\"");
    std::smatch m;
    std::string html =  getHtmlFromHost(host, rootURL);
    if(html.size() > 0) {
        std::cout << "Rodando spider na URL raiz: " << rootURL << std::endl;
        while(std::regex_search(html, m, href)) {
            if(m[1].str().find("http://" + rootURL) != std::string::npos) { // So pega no msm host
                std::cout << "\t" << m[1] << std::endl;
                urlTree.push_back(m[1]);
            }
            html = m.suffix();
        }
    }
    else {
        std::cout << "Erro na tentativa de conexao na URL " << rootURL << std::endl;
    }
    return urlTree;
}