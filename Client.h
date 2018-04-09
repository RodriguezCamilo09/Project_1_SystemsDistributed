/**
 * @version 1.0
 * @date 08/04/18
 * @author Camilo Rodriguez
 * Class: client.h
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <cstdio>
#include <stdlib.h>
#include <pthread.h>
#include <sstream>
#include <vector>
#include <dirent.h>


#define BUFFSIZE 1

using namespace std;

class Client{

private:
	bool state;						// Flag, verify state of connection
	int portServer;					// Server connection port
	int typeClient;					// Type -> 1) Client 2) Storage
	string ipServer;				// IP address of the server
	int descriptorClient;
	struct sockaddr_in serverInfo;
	
public:
	// Builder
	Client(void);
	Client(int, string, int); // Port server - IP server - Type client

	// Destroyer
	~Client();

	// Methods basics
	void connectToServer(void);
	void receiveFile(void*);
	void sendConfirmation(void*); 		// Confirmation of send of the file
	void sendMD5(void*);				// To validate the file

	static void * listenToServer(void*);
	static void * writeToServer(void*,char *);
	static void * options(void *);
	static void * sendFile(void *);
	static void * listFiles(void *);
	static void * listStoredFiles(void *);
	static void * countFiles(void *);		
	static void * loadBalance(void *);
	
	// Setters
	void setState(bool);
	void setPortServer(int);
	void setTypeClient(int);
	void setIPServer(string);
	void setDescriptorServer(int);

	// Getters
	

};