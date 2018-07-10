#include "Spider.h"
#include "HTTP_Parser.h"

#include <iostream>

std::string getHtmlFromHost(std::string host, std::string URL) {
    int sockfd = sendRequestToHost(host, buildRequest(host, URL));
    std::string reply = getResponseFromHost(sockfd);
    return reply;
}

std::vector<std::string> spider(std::string host, std::string rootURL) {
    std::vector<std::string> urlTree;
    std::regex href("href=\"(.*?)\"");
    std::smatch m;
    std::string html =  getHtmlFromHost(host, rootURL);
    if(html.size() > 0) {
        std::cout << "Rodando spider na URL raiz: " << host + rootURL << std::endl;
        while(std::regex_search(html, m, href)) {
            if(m[1].str().find("http://" + host) != std::string::npos) { // So pega no msm host
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
