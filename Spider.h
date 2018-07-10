#include <string.h>
#include <regex>
#include <vector>

#include "HTTP_Parser.h"
#include "proxy.h"

#ifndef SPIDER_H
#define SPIDER_H


std::string getHtmlFromHost(std::string host, std::string URL="/");

std::vector<std::string> spider(std::string host, std::string rootURL);

#endif
