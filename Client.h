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
	bool state;	
	int portServer;
	int typeClient;
	string ipServer;
	int descriptorClient;
	struct sockaddr_in serverInfo;
	
public:
	Client();
	~Client();
	
};