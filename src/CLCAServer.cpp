#include "CLCAAddressIPV4.h"
#include "CLCAServer.h"
#include <iostream>

CLCAServer::CLCAServer(const uint8_t* IP,const uint16_t Port,const int IPType /* = AF_INET */,const int streamType /* = SOCK_STREAM */)
{
	m_sock = new CLSocket(IP,Port,false,IPType,streamType);
	client_lists = new std::list<CLCAClientContext*>;
}

CLCAServer::~CLCAServer()
{
	delete m_sock;
	delete client_lists;
}

