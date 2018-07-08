#include <string.h>
#include <regex>
#include <vector>

#include "TCP_Client.h"
#include "HTTP_Parser.h"

#ifndef SPIDER_H
#define SPIDER_H


std::string getHtmlFromHost(std::string host, std::string URL="/");

std::vector<std::string> spider(std::string host, std::string rootURL);

#endif