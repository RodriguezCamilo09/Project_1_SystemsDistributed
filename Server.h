/**
 * @version 1.0
 * @date 04/04/18
 * @author Camilo Rodriguez
 * Class: Server.h
 */

#include <string.h>
#include "ClientInfo.h"

#define maxClients 20 	// Maximum number of clients
#define BUFFSIZE 1

class Server{

private:	
	bool state; // Flag, verify state of connection
	int port;	// Connection port	
	int idBind;	// Linker identification	
	int descriptorServer; // It returns the operating system when allocating a socket
	int countClients; // Number of clients
	int countStorage; // Number of storage machines
	struct sockaddr_in serverInfo; // Socket information and the connection that is made
	vector<ClientInfo *> clientsDescriptor;	// List of clients connected
	vector<ClientInfo *> clientsTemporal;	// Auxiliary list of clients
	vector<ClientInfo *> clientsStorage;	// List of storage machines

public:
	/**
	 * Builder
	 */
	Server(void);
	Server(int);	// Port

	/**
	 * Destroyer
	 */
	~Server();

	/**
	 * Basic methods of connection
	 */
	void initializeServer(void); 	
	void acceptClients(void);		
	void closeServer(void); 		
	void runServer(void);
	static void * startServer(void *);
	vector<ClientInfo *> selectStorage();

	/**
	 * Methods for sending files
	 */
	void receiveFile(void*, void *, void *, void *);
	void sendConfirmation(void*);	// Confirmation of arrival of the file
	void sendMD5(void*);	// Check if a file on Linux arrived full and secure

	/**
	 * Setters	
	 */
	void setPort(int);
	void setIdBind(int);
	void setState(bool);
	void setServerInfo(int);
	void setCountClients(int);
	void setCountStorage(int);	
	void setDescriptorServer(int);

	/**
	 * Getters
	 */
	int getPort(void);
	int getIdBind(void);
	bool getState(void);
	int getCountClients(void);
	int getCountStorage(void);
	struct sockaddr_in getServerInfo(void);
	vector<ClientInfo *> getClients(void);
	vector<ClientInfo *> getClientsTemporal();

};