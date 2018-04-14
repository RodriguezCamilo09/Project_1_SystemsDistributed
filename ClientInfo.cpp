/**
 * @version 1.0
 * @date 03/04/18
 * @author Camilo Rodriguez
 * Class: clientInfo.cpp
 */

#include "ClientInfo.h"

/**
 * Builder
 */
ClientInfo::ClientInfo(int desCli, struct sockaddr_in info){
	this->descriptorClient=desCli;
	this->clientInfo=info;
	this->state=true;
	this->countFiles=0;
}

/**
 * Destroyer
 */
ClientInfo::~ClientInfo(){}

/**
 * [ClientInfo::getId]
 * @return [int -> Host identification]
 */
int ClientInfo::getId(){
	return this->id;
}

/**
 * [ClientInfo::getDescriptorClient]
 * @return  [int -> ]
 */
int ClientInfo::getDescriptorClient(void){
	return this->descriptorClient;	
}

/**
 * [ClientInfo::getClientInfo]
 * @return [struct]
 */
struct sockaddr_in ClientInfo::getClientInfo(){
	return this->clientInfo;
}

/**
 * [ClientInfo::getState]
 * @return [bool -> Connection status]
 */
bool ClientInfo::getState(){
	return this->state;
}

/**
 * [ClientInfo::getCountFiles]
 * @return [int -> number of files]
 */
int ClientInfo::getCountFiles(){
	return this->countFiles;
}

/**
 * [ClientInfo::setId]
 * @param id [Host identification]
 */
void ClientInfo::setId(int id){
	this->id=id;
}

/**
 * [ClientInfo::setState]
 * @param state [Connection status]
 */
void ClientInfo::setState(bool state){
	this->state=state;
}

/**
 * [ClientInfo::setCountFiles]
 * @param countFiles [Number of files]
 */
void ClientInfo::setCountFiles(int countFiles){
	this->countFiles=countFiles;
}