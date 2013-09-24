#ifndef CLCASERVER_H
#define CLCASERVER_H
#include <iostream>
#include <list>
#include <stdint.h>
#include "CLSocket.h"
#include "CLCAClientContext.h"


class CLCAServer
{
public:
	CLCAServer(const uint8_t* IP,const uint16_t Port,const int IPType = AF_INET,const int streamType = SOCK_STREAM);
	virtual ~CLCAServer();

	virtual void Initialize() = 0;
	virtual int Accept() = 0;
	virtual std::list<CLCAClientContext*>* getData() = 0;
	virtual int  writeData(int sock,uint8_t* buf) = 0;
	virtual void start() = 0;

protected:
	std::list<CLCAClientContext*>* client_lists;
	CLSocket * m_sock;


};
#endif