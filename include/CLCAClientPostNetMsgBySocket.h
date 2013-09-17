#ifndef CLCACLIENTPOSTNETMSGBYSOCKET_H
#define CLCACLIENTPOSTNETMSGBYSOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netint/in.h>
#include "CLCAAddress.h"
#include "CLCAPostNetMsgTool.h"

class CLCAClientPostNetMsgBySocket: public CLCAPostNetMsgTool
{
public:

	CLCAClientPostNetMsgBySocket(uint8_t IPType,uint8_t* IP,uint16_t Port);
	virtual ~CLCAClientPostNetMsgBySocket();

	virtual void PostNetMessage(uint8_t* msg,uint32_t length);
	virtual uint8_t* ReadFormNet();
	
private:
	void Initialize();
	int Connect();
	int writeNetMsg(uint8_t* msg,uint32_t length);

private:
	int sock;
	struct sockaddr* serv_addr;
	int addrSize;
	CLCAAddress* m_addr;
	CLEpoll* epoll;
};
#endif