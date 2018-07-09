main: HTTP_Parser TCP_Client
	g++ main.cpp TCP_Client.o HTTP_Parser.o -std=c++11

TCP_Client:
	g++ TCP_Client.cpp -c -std=c++11


spider: TCP_Client HTTP_Parser
	g++ Spider.cpp HTTP_Parser.o TCP_Client.o -std=c++11

HTTP_Parser:
	g++ HTTP_Parser.cpp -c

proxy: HTTP_Parser
	g++ proxy.cpp -c

aracne: proxy
	g++ aracne.cpp proxy.o HTTP_Parser.o -o aracne