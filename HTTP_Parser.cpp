#include "HTTP_Parser.h"

#include <iostream>

std::string getHostFromRequest(std::string request) {
    std::istringstream strStream(request);
    std::string line;
    std::getline(strStream, line); // pega primeira linha e nao faz nada
    std::getline(strStream, line); // Pega segunda linha com o host
    if(line.size() > 7)
        return line.substr(6, line.size() - 7);
}

std::string getURLFromRequest(std::string request) {
    std::istringstream strStream(request);
    std::string line;
    std::getline(strStream, line);
    if(line.size() > 14)
        // Substring tirando o GET inicial e a versao do HTTP no final
        return line.substr(4, line.size() - (5 + 9));
}

std::string getHTTPMethod(std::string request) {
    std::istringstream strStream(request);
    std::string line;
    std::getline(strStream, line);
    if(line.size() > 3) 
        return line.substr(0, line.find_first_of(" "));
}

std::string buildRequest(std::string hostName, std::string URL) {
    return "GET " + URL +" HTTP/1.1\r\nHost:" + hostName + "\r\nConnection: close\r\n\r\n";
}

std::string addHeader(std::string request, std::string header) {
    if(request.find(header) == std::string::npos) {
        return request + "\n" + header;
    }
    return request;
}

std::string replaceHeader(std::string request, std::string name, std::string value) {
    std::string finalReq(request);
    int position_header;
    if((position_header = request.find(name)) != std::string::npos) {
        int position_value_start = request.find_first_of(":", position_header);
        int position_value_end = request.find_first_of("\r", position_header);
        finalReq.replace(position_value_start, (position_value_end-position_value_start), ": " + value );
    }
    return finalReq;
}

std::string fixRequestURL(std::string request) {
    int end_host_pos = request.find_first_of("/", 14);
    return request.substr(0, 4) + request.substr(end_host_pos, request.size() - end_host_pos);
}