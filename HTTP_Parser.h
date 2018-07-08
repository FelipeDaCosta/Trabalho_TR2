#include <string>
#include <sstream>

#ifndef HTTP_PARSER
#define HTTP_PARSER

std::string getHostFromRequest(std::string request);

std::string getURLFromRequest(std::string request);

std::string buildRequest(std::string hostName, std::string URL="/");

std::string getHTTPMethod(std::string request);

#endif