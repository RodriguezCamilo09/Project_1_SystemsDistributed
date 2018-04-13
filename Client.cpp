#include "Client.h"

Client::Client(){
	this->portServer=1234;
	this->ipServer="192.168.0.2";
	this->state=true;
}

Client::Client(int port, string ip, int typeClient){
	this->portServer=port;
	this->ipServer=ip;
	this->state=true;
	this->typeClient= typeClient;
}

Client::~Cliente(){

}


/**
 * [Client::listenServer description]
 * @param  cli [description]
 * @return     [description]
 */
void * Client::listenServer(void *cli){
	Client* client=(Client *) cli;
	char messageServer[60];
	char key[] = "1";  // receb files
	char key2[] = "2"; // list files
	char key3[] = "3"; // Count files
	while(1){
		int i = recv(client->getDescriptor(), (void *)$messageServer,60,0);
		if(i!=0){
			cout<<"listening"<<endl;
			if(strcmp(key,messageServer) != 0){
				client->receiveFile((void *)client);
			}else if (strcmp(key2,messageServer) != 0){
				client->listFiles((void *)client);
			}else if(strcmp(key3,messageServer) != 0){
				client->coutnFiles((void *)client);
			}
		}else{
			cout<<"Se desconecto del servido"<<endl;
			close(client->getDescriptor());
			exit(EXIT_SUCCESS);
		}
	}
}

/**
 * [Client::writeServer description]
 * @param  cli [description]
 * @param  msg [description]
 * @return     [description]
 */
void * Client::writeServer(void * cli, char msg[]){
	Client* client = (Client *) cli;
	int connected=1;
	int i= send(client->getDescriptor(),(void *)msg,sizeof(msg),0);
	sleep(1);
	if(i==-1){
		connected=0;
		cout<<"Desconectado"<<endl;
		close(client->getDescriptor());
		exit(EXIT_SECCESS);
	}
}

/**
 * [Client::options description]
 * @param  cli [description]
 * @return     [description]
 */
void * Client::options(void * cli){
	int option;
	count<<"Options of the client"<<endl;
	count<<"1) Send file"<<endl;
	count<<"2) List files of thestorage"<<endl;
	count<<"3) Balance of charge"<<endl;
	cin>>option;
	cin.get();
	switch(option){
		case 1: sendFile(cli);
		break;
		case 2: listFiles(cli);
		break;
		case 3: balanceCharge(cli);
		break;
	}
}