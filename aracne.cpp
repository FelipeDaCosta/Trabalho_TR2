#include "proxy.h"

int main(int argc, char *argv[]){
	// Socket do servidor e porta
	int portNo = 8228;
	if(argc >= 2){
		portNo = atoi(argv[1]);
	}
	int sockServer = startProxy(portNo);
	std::cout << "Aracne iniciado, ouvindo na porta " << portNo << std::endl;
	runProxy(sockServer);
  	close(sockServer);
	return 0;
}
