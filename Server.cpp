/**
 * @version 1.0
 * @date 04/04/18
 * @author Camilo Rodriguez
 * Class: Server.cpp
 */

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
Server::~Server(){}

/**
 * [Server::initializeServer]
 */
void Server::initializeServer(){	
	descriptorServer=socket(AF_INET, SOCK_STREAM, 0);
	serverInfo.sin_family=AF_INET;
	serverInfo.sin_addr.s_addr=htonl(INADDR_ANY);
	serverInfo.sin_port=htons(port);
	idBind=bind(descriptorServer, (struct sockaddr *)&serverInfo, sizeof(serverInfo));	
	listen(descriptorServer, maxClients);
	if(descriptorServer==-1 && idBind==-1)
		cout<<"Error, starting the server..."<<endl;
}

/**
 * [Server::closeServer]
 */
void Server::closeServer(){
	exit(EXIT_SUCCESS);
}

/**
 * [recevClient]
 * @param  ser 
 * @return     
 */
void * recevClient(void * ser){
	Server * server = (Server *) ser;

	vector<ClientInfo *> listClients = server->getClientsTemporal();
	cout<<"ListClients"<<endl;
	ClientInfo * client = (ClientInfo * listClients[server->getCountClients()-1]);
	char messageOfClient[128];
	char key[]= "1";

	while(client->getState()){
		cout<<"listening" <<endl;
		int i=recv(client->getDescriptorClient(), (void *)&messageOfClient,128,0);
		cout<<"listening" <<endl;
		sleep(1);
		if(i!=0){
			if(strcmp (key,messageOfClient) != 0){
				recv(client->getDescriptorClient(), (void *)&messageOfClient,128,0);
				cout<<messageOfClient<<endl;
				vector <ClientInfo *> select = server->selectStorage();
				server->receiveFile((void *)client,(void *) select[0],(void *)select[1], (void *) messageOfClient);
			}
		}else{
			cout<<"the client with IP was disconnected: "<<inet_ntoa(client->getClientInfo().sin_addr)<<" Error "<<endl;
			client->setState(false);
			close(client->getDescriptorClient());
		}
	}

}

/**
 * [Server::acceptClients]
 */
void Server::acceptClients(){

	int descriptorClient;
	countClients=0;
	countStorage=0;
	char client[] = "1"; 
	char storage[] = "2";
	
	while((countClients+countStorage)<maxClients){ 
		struct sockaddr_in clientInfo;
		int size=sizeof(struct sockaddr_in);
		cout<<"accepting clients..."<<endl;
		descriptorClient=accept(this->descriptorServer, (struct sockaddr *)&clientInfo, (socklen_t*) &size);						
		if(descriptorClient!=-1){
			cout<<"Connected client" <<endl;
			char messageOfClient[128];
			int i=recv(descriptorClient, (void *)&messageOfClient,128,0);
			if(i!=0){
				cout<<messageOfClient<<endl;
				if(strcmp (client,messageOfClient) == 0){
					
					clientsTemporal.push_back(new ClientInfo(descriptorClient,clientInfo));
					clientsTemporal[countClients]->setId(countClients);
					pthread_t clientThread;
					pthread_create(&clientThread,NULL,&recevClient,(void *)this);
					countClients++;
					cout<<"Client"<<endl;
				}else{
					clientDescriptorStorage.push_back(new ClientInfo(descriptorClient,clientInfo));
					//pthread_t clientThread;
					//pthread_create(&clientThread,NULL,&recevClient,(void *) clientDescriptorStorage[countStorage]);
					clientDescriptorStorage[CountStorage]->setId(CountStorage);
					CountStorage++;
					cout<<"Storage"<<endl;
				}
			}else{
				cout<<"the client with IP was disconnected..."<<endl;
				close(descriptorClient);
			}
		}
	}
}

/**
 * [Server::selectStorage]
 */
vector<ClientInfo *> Server::selectStorage(){
	vector<ClientInfo *> storageSelection;
	char messageToClient[]= "3";
	for(int i=0; i<clientsTemporal.size(); i++){
		ClientInfo * client = clientsTemporal[i];
		if(send(client->getDescriptorClient(), (void *)messageToClient, sizeof(messageToClient),0)!=-1){
			sleep(1);
			char messageOfClient[128];
			cout<<"Start selection"<<endl;
			int i=recv(client->getDescriptorClient(), (void *)&messageOfClient,128,0);
			cout<<"End selection"<<endl;
			client->setCountFiles(*(int*)messageOfClient);
		}
	}
	for(int i=0; i<clientsTemporal.size(); i++){
		for(int j=i+1; j<clientsTemporal.size(); j++){
			if(clientsTemporal[i]>clientsTemporal[j]){
				ClientInfo * clientAux= clientsTemporal[i];
				clientsTemporal[i]=clientsTemporal[j];
				clientsTemporal[j]= clientAux;
			}
		}
	}
	for(int i=0; i<clientsTemporal.size(); i++){
		cout<<"Order"+i<<endl;
		cout<<clientsTemporal[i]<<endl;
	}
	storageSelection.push_back(clientsTemporal[0]);
	storageSelection.push_back(clientsTemporal[1]);
	return storageSelection;
}

/**
 * [Server::startServer]
 * @param  Server 
 * @return        
 */
void * Server::startServer(void * Server){
	Server * server=(Server *) Server;
	server->acceptClients();
}

/**
 * [Server::runServer]
 */
void Server::runServer(){
	pthread_t thread;
	pthread_create(&thread,NULL,&startServer,(void *) this);
}

/**
 * [Server::receiveFile]
 * @param cli      
 * @param sel1     
 * @param sel2     
 * @param fileName 
 */
void Server::receiveFile(void * cli, void * sel1, void * sel2, void * fileName){
	ClientInfo * client = (ClientInfo *) cli;
	ClientInfo * selectStorage1 = (ClientInfo *) sel1;
	ClientInfo * selectStorage2 = (ClientInfo *) sel2;
	char * nameFile = (char *)fileName;
	char buffer[BUFFSIZE];
	int received = -1;
	char msg[] = "1";
	if(send(selectStorage1->getDescriptorClient(),(void *)msg,sizeof(msg),0)==-1){
		cout<<"Error when sending the file"<<endl;
	}
	if(send(selectStorage2->getDescriptorClient(),(void *)msg,sizeof(msg),0)==-1){
	  cout<<"Error when sending the file"<<endl;
	}
	//FILE * file;
	//file = fopen("Files/fileReceived","wb");
	if(send(selectStorage1->getDescriptorClient(),(void *)nameFile,sizeof(nameFile),0)==-1){
		cout<<"Error when sending the file"<<endl;
	}
	if(send(selectStorage2->getDescriptorClient(),(void *)nameFile,sizeof(nameFile),0)==-1){
	  cout<<"Error when sending the file"<<endl;
	}
	while((received = recv(client->getDescriptorClient(), buffer, BUFFSIZE, 0)) > 0){
		printf("%s",buffer);
		//fwrite(buffer,sizeof(char),1,file);
		if(send(selectStorage1->getDescriptorClient(),buffer,BUFFSIZE,0)==-1){
		  cout<<"Error when sending the file"<<endl;
		}
		if(send(selectStorage2->getDescriptorClient(),buffer,BUFFSIZE,0)==-1){
		  cout<<"Error when sending the file"<<endl;
		}
	}
	sendConfirmation((void *)client);
	sendMD5SUM((void *)client);
	//fclose(file);
}

/**
 * [Server::sendConfirmation]
 * @param cli
 */
void Server::sendConfirmation(void * cli){
	ClientInfo * client = (ClientInfo *) cli;
	char message[80] = "Received";
	int lengthMessage = strlen(message);
	printf("\nConfirmation send\n");
	if(write(client->getDescriptorClient(),message,sizeof(message)) == -1)
			perror("Error sending confirmation");

	
}

/**
 * [Server::sendMD5SUM]
 * @param cli 
 */
void Server::sendMD5SUM(void * cli){
	ClientInfo * client = (ClientInfo *) cli;
	FILE *tmp;
	char fileName[] = "verification";
	char md5sum[80];
	system("md5sum fileReceived >> verification");
	tmp = fopen(fileName,"r");
	fscanf(tmp,"%s",md5sum);	
	printf("\nMD5SUM:%s\n",md5sum);	
	write(client->getDescriptorClient(),md5sum,sizeof(md5sum));
	fclose(tmp);

}

/**
 * [Server::getDescriptorServer]
 * @return
 */
int Server::getDescriptorServer(){
	return this->descriptorServer;
}

/**
 * [Server::getIdBind]
 * @return
 */
int Server::getIdBind(){
	return this->idBind;
}
	
/**
 * [Server::getPort]
 * @return 
 */
int Server::getPort(){
	return this->port;
}

/**
 * [Server::getCountClients]
 * @return
 */
int Server::getCountClients(){
	return this->countClients;
}

/**
 * [Server::getServerInfo]
 * @return 
 */
struct sockaddr_in Server::getServerInfo(){
	return this->serverInfo;
}

vector<ClientInfo *> Server::getClients(){
	return this->clientsDescriptor;
}

vector<ClientInfo *> Server::getClientsTemporal(){
	return this->clientsTemporal;
}

/**
 * [Server::setDescriptorServer]
 * @param descriptor 
 */
void Server::setDescriptorServer(int descriptor){
	this->descriptorServer=descriptor;
}

/**
 * [Server::setIdBind]
 * @param bind 
 */
void Server::setIdBind(int bind){
	this->idBind=bind;
}

/**
 * [Server::setPort]
 * @param port 
 */
void Server::setPort(int port){
	this->port=port;
}

/**
 * [Server::setServerInfo]
 * @param info
 */
void Server::setServerInfo(struct sockaddr_in info){
	this->serverInfo=info;
}