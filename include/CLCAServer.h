#ifndef CLCASERVER_H
#define CLCASERVER_H
#include <iostream>
#include <list>
#include <stdint.h>
#include "CLSocket.h"
#include "CLCAClientContext.h"

typedef vector<int,uint8_t*> VEC; 
class CLCAServer
{
public:
	CLCAServer(const int8_t* IP,const uint16_t Port,const int IPType = AF_INET,const int streamType = SOCK_STREAM);
	virtual ~CLCAServer();

	virtual void Initialize() = 0;
	virtual int Accept() = 0;
	virtual list<CLCAClientContext*>* getData() = 0;
	virtual int  writeData(int sock,uint8_t* buf) = 0;
	virtual void start() = 0;

protected:
	std::list<CLCAClientContext*>* client_lists;
	CLSocket * m_sock;


};
#endif