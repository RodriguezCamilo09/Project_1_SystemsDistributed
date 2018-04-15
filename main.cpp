/**
 * @version 1.0
 * @date 08/04/18
 * @author Camilo Rodriguez
 * Class: Main.cpp
 */

#include <iostream>
#include "Server.h"
#include "Client.h"

using namespace std;

int main(int argc, char *argv[]) {
	int type=0;
	int port=0;
	cout<<"------------------------------"<<endl;
	cout<<"--Type machina              --"<<endl;
	cout<<"--1) Server                 --"<<endl;
	cout<<"--2) Client                 --"<<endl;
	cout<<"--3) Storage                --"<<endl;
	cout<<"------------------------------"<<endl;		
	cin>>type;
	cin.get();
	cout <<"Port: ";
	cin >> port;
	cin.get();	
	int val=type;	
	if(val==1){		
		Server server;			
		server.setPort(port);
		server.initializeServer();
		server.runServer();			
		while(1);
	}if(val==2 || val == 3){		
		cout<< "Host - IP: ";
		string host;
		getline(cin,host);		
		Client * client = new Client(port,host,val);
		client->connectToServer();
	}
	return 0;	
}