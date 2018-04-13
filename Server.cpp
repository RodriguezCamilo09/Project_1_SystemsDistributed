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

/**
 * [Server::closeServer description]
 */
void Server::closeServer(){
	exit(EXIT_SUCCESS);
}

void * recevClient(void * ser){
	Server * server = (Server *) ser;

	vector<ClientInfo *> listClients = server->getClientesTemporal();
	cout<<"ListClients"<<endl;
	CllientInfo * client = (ClientInfo * listClients[server->getCountClients()-1]);
	char menssageOfClient[128];
	char key[]= "1";

	while(client->getState()){
		cout<<"Listen" <<endl;
		int i=recv(client->getDescriptorClient(), (void *)&menssageOfClient,128,0);
		cout<<"Listen" <<endl;
		sleep(1);
		if(i!=0){
			if(strcmp (key,menssageOfClient) != 0){
				recv(client->getDescriptorClient(), (void *)&menssageOfClient,128,0);
				cout<<menssageOfClient<<endl;
				vector <ClientInfo *> seleccion = server->seleccionarStorage();
				server->recevibirArchivo((void *)cliente,(void *) seleccion[0],(void *)seleccion[1], (void *) mensajeDeCliente);
			}
		}else{
			cout<<"Se desconecto el cliente con IP: "<<inet_ntoa(cliente->getClienteInfor().sin_addr)<<" con error "<<endl;
			cliente->setEstado(false);
			close(cliente->getDescriptorCliente());
		}
	}

}
lunes 8:30am
casa de mujer