#include "Server.h"

/**
 * Builder
 */
Server::Server(){
	this->port=7009;	
	this->countClients=0;
	this->countStorage=0;
}
/**
 * Builder
 */
Server::Server(int port){
	this->port=port;
	this->countClients=0;
	this->countStorage=0;
}

/**
 * Destroyer
 */
Servidor::~Servidor(){
}

/**
 * [Server::initializeServer description]
 */
void Server::initializeServer(){
	descriptorServer=socket(AF_INET, SOCK_STREAM, 0);
	serverInfo.sin_family=AF_INET;
	serverInfo.sin_addr.s_addr=htonl(INADDR_ANY);
	serverInfo.sin_port=htons(port);
	idBind=bind(descriptorServer, (struct sockaddr *)&serverInfo, sizeof(serverInfo));	
	listen(descriptorServer, maxClients);
	if(descriptorServer==-1 && idBind==-1)
		cout<<"Error iniciando el servidor: "<<endl;
}
