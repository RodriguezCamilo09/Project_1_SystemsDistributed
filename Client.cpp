/**
 * @version 1.0
 * @date 08/04/18
 * @author Camilo Rodriguez
 * Class: Client.h
 */
#include "Client.h"

/**
 * Builder
 */
Client::Client(){
	this->portServer=1234;
	this->ipServer="192.168.0.2";
	this->state=true;
}

/**
 * Builder
 */
Client::Client(int port, string ip, int typeClient){
	this->portServer=port;
	this->ipServer=ip;
	this->state=true;
	this->typeClient=typeClient;
}

/**
 * Destroyer
 */
Client::~Client(){}


/**
 * [Client::listenServer description]
 * @param  cli [description]
 * @return     [description]
 */
void * Client::listenToServer(void *cli){
	Client* client=(Client *) cli;
	char messageServer[60];
	char key[] = "1";  // receb files
	char key2[] = "2"; // list files
	char key3[] = "3"; // Count files
	while(1){
		int i = recv(client->getDescriptor(), (void *)&messageServer,60,0);
		if(i!=0){
			cout<<"listening..."<<endl;
			if(strcmp(key,messageServer) != 0){
				client->receiveFile((void *)client);
			}else if (strcmp(key2,messageServer) != 0){
				client->listFiles((void *)client);
			}else if(strcmp(key3,messageServer) != 0){
				client->countFiles((void *)client);
			}
		}else{
			cout<<"Disconnected..."<<endl;
			close(client->getDescriptor());
			exit(EXIT_SUCCESS);
		}
	}
}

/**
 * [Client::writeServer]
 * @param  cli 
 * @param  msg 
 * @return 
 */
void * Client::writeToServer(void * cli, char msg[]){
	Client* client = (Client *) cli;
	int connected=1;
	int i= send(client->getDescriptor(),(void *)msg,sizeof(msg),0);
	sleep(1);
	if(i==-1){
		connected=0;
		cout<<"Disconnected..."<<endl;
		close(client->getDescriptor());
		exit(EXIT_SUCCESS);
	}
}

/**
 * [Client::options]
 * @param  cli 
 * @return     
 */
void * Client::options(void * cli){
	int option;
	cout<<"**************************"<<endl;
	cout<<"* Options Client        **"<<endl;
	cout<<"* 1) Send file          **"<<endl;
	cout<<"* 2) List files Storage **"<<endl;
	cout<<"* 3) Balance Charge     **"<<endl;
	cout<<"**************************"<<endl;
	cin>>option;
	cin.get();
	switch(option){
		case 1: sendFile(cli);
		break;
		case 2: listFiles(cli);
		break;
		default: loadBalance(cli);
	}
}

/**
 * [Client::sendFile]
 * @param  cli 
 * @return     
 */
void * Client::sendFile(void * cli){
	Client * client = (Client *) cli;
	char url[200];
	char buffer[BUFFSIZE];
	char name[200];
	cout<<"File path..."<<endl;
	cin>>url;
	cout<<"Name - Extension"<<endl;
	cin>>name;
	cin.get();
	FILE * file;
	file = fopen(url, "rb");
	char msg[] = "1";
	writeToServer((void *)cli,msg);
	writeToServer((void *)cli,name);
	while(!feof(file)){
		fread(buffer,sizeof(char),BUFFSIZE, file);
		if(send(client->getDescriptor(),buffer,BUFFSIZE,0)==-1){
			cout<<"Error send file..."<<endl;
		}
	}
	char message[80];
	read(client->getDescriptor(),message,sizeof(message));
	printf("\nConfirmation received:\n%s\n",message);
	read(client->getDescriptor(),message,sizeof(message));
	printf("\nMD5SUM:\n%s\n",message);
	fclose(file);
	options(cli);	
}

/**
 * [Client::listStoredFiles]
 * @param  cli 
 * @return     
 */
void * Client::listStoredFiles(void * cli){}

/**
 * [Client::listFiles]
 * @param  cli 
 * @return     
 */
void * Client::listFiles(void * cli){
	Client * client = (Client *) cli;
	vector<string> listOfFiles;
	DIR *dir;
	struct dirent *ent;
	dir = opendir ("./Files");
	if (dir == NULL) 
		cout<<"Error, opening directory..."<<endl;
	cout<<"List files..."<<endl;
	while ((ent = readdir (dir)) != NULL) {
		if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){     
			cout<<ent->d_name<<endl;
			listOfFiles.push_back(ent->d_name);
		}
	}
	closedir (dir);	
	for(int i=0; i<listOfFiles.size(); i++){
		string msg  = listOfFiles[i]; 
		send(client->getDescriptor(),(void *)msg.c_str(),sizeof(msg),0);
	}
	char msg [] ="end"; 
	send(client->getDescriptor(),(void *)msg,sizeof(msg),0);
}

/**
 * [Client::countFiles]
 * @param  cli 
 * @return     
 */
void * Client::countFiles(void * cli){
	Client * client = (Client *) cli;
	int file_count = 0;
	DIR * dirp;
	struct dirent * entry;
	dirp = opendir("./Files"); 
	while ((entry = readdir(dirp)) != NULL) {
	    if (entry->d_type == DT_REG) { 
	         file_count++;
	    }
	}
	closedir(dirp);
	cout<<"Count files"+file_count<<endl;
	char msg = file_count; 
	send(client->getDescriptor(),(void *)&msg,sizeof(msg),0);
}

/**
 * [Client::loadBalance]
 * @return
 */
void * Client::loadBalance(void * cli){}

/**
 * [Client::connectToServer]
 */
void Client::connectToServer(){
	descriptorClient = socket(AF_INET,SOCK_STREAM,0);
	serverInfo.sin_family=AF_INET;
	inet_pton(AF_INET,ipServer.c_str(),&serverInfo.sin_addr);
	serverInfo.sin_port=htons(portServer);
	int conn=connect(descriptorClient,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
	cout<<typeClient<<endl;
	if(conn!=-1){
		if(typeClient==2){
			char msg[] = "1";
			writeToServer((void *)this,msg);
			pthread_t threadListen;
			pthread_create(&threadListen,NULL,listenToServer,(void *)this);
			cout<<"++++++++++"<<endl;
			cout<<"+ Client +"<<endl;
			cout<<"++++++++++"<<endl;
			options((void *)this);
		}else {
			char msg[] = "2";
			writeToServer((void *)this,msg);
			pthread_t threadListen;
			pthread_create(&threadListen,NULL,listenToServer,(void *)this);
			cout<<"xxxxxxxxxxx"<<endl;
			cout<<"x Storage x"<<endl;
			cout<<"xxxxxxxxxxx"<<endl;
		}		
		pthread_t threadWrite;
		pthread_create(&threadWrite,NULL,writeToServer,(void *)this);
		while(1);
	}else{
		cout<<"Unable to connect..."<<endl;
	}
}

/**
 * [Client::receiveFile]
 * @param cli 
 */
void Client::receiveFile(void * cli){
	Client * client = (Client *) cli;
	char buffer[BUFFSIZE];
	int received = -1;
	char messageOfClient[128];
	recv(client->getDescriptor(), (void *)&messageOfClient,128,0);
	cout<<messageOfClient<<endl;
	cout<<"Receive file..."<<endl;
	char url []= "Files/";
	strcpy (url,messageOfClient);	
	FILE * file;
	file = fopen(url,"wb");
	while((received = recv(client->getDescriptor(), buffer, BUFFSIZE, 0)) > 0){
		printf("%s",buffer);
		fwrite(buffer,sizeof(char),1,file);
	}
	sendConfirmation((void *)client);
	sendMD5SUM((void *)client);
	fclose(file);
}

/**
 * [Client::sendConfirmation]
 * @param cli
 */
void Client::sendConfirmation(void * cli){
	Client * client = (Client *) cli;
	char message[80] = "Received...";
	int lengthMessage = strlen(message);
	printf("\nConfirmation send\n");
	if(write(client->getDescriptor(),message,sizeof(message)) == -1)
		perror("Error, confirmation send");
}

/**
 * [Client::sendMD5SUM]
 * @param cli 
 */
void Client::sendMD5SUM(void * cli){
	Client * client = (Client *) cli;
	FILE *tmp;
	char fileName[] = "verification";
	char md5sum[80];
	system("md5sum fileReceived >> verification");
	tmp = fopen(fileName,"r");
	fscanf(tmp,"%s",md5sum);	
	printf("\nMD5SUM:%s\n",md5sum);	
	write(client->getDescriptor(),md5sum,sizeof(md5sum));
	fclose(tmp);
}

/**
 * [Client::getDescriptor]
 * @return 
 */
int Client::getDescriptor(){
	return this->descriptorClient;
}

/**
 * [Client::getState]
 * @return
 */
bool Client::getState(){
	return this->state;
}

/**
 * [Client::setDescriptor]
 * @param descriptor 
 */
void Client::setDescriptorServer(int descriptor){
	this->descriptorClient=descriptor;
}

/**
 * [Client::setState]
 * @param state 
 */
void Client::setState(bool state){
	this->state=state;
}