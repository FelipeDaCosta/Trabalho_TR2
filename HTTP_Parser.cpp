#include "HTTP_Parser.h"

std::string getHostFromRequest(std::string request) {
    std::istringstream strStream(request);
    std::string line;
    std::getline(strStream, line); // pega primeira linha e nao faz nada
    std::getline(strStream, line); // Pega segunda linha com o host
    return line.substr(6, line.size() - 6);
}

std::string getURLFromRequest(std::string request) {
    std::istringstream strStream(request);
    std::string line;
    std::getline(strStream, line);
    // Substring tirando o GET inicial e a versao do HTTP no final
    return line.substr(4, line.size() - (4 + 9));
}

std::string buildRequest(std::string hostName, std::string URL) {
    return "GET " + URL +" HTTP/1.1\r\nHost:" + hostName + "\r\nConnection: close\r\n\r\n";
}