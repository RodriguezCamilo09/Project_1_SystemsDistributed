/**
 * @version 1.0
 * @date 03/04/18
 * @author Camilo Rodriguez
 * Class: clientInfo.h
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <time.h>
#include <cstdio>
#include <string.h>

using namespace std;

#define BUFFSIZE 1

/**
 * class responsible for managing the most important 
 * information of the host in the architecture
 */
class ClientInfo{
private:
	int id;		// Identification host
	bool state; // Connection status
	int descriptorClient; // It returns the operating system when allocating a socket
	int countFiles; // Number of file
	struct sockaddr_in clientInfo; // socket information and the connection that is made

public:
	/**
	 * Builder
	 */
	ClientInfo(int, struct sockaddr_in);
	/**
	 * Destroyer
	 */
	~ClientInfo();
	
	// Confirms file submission
	void sendConfirmation(void*);
	// Check if a file on Linux arrived full and secure
	void sendMD5SUM(void*);

	/**
	 * Getters
	 */
	int getId(void);
	int getDescriptorClient(void);
	bool getState(void);
	int getCountFiles(void);
	struct sockaddr_in getClientInfo();

	/**
	 * Setters
	 */
	void setId(int);
	void setState(bool);
	void setCountFiles(int);
};