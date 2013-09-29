#ifndef CLCACLIENTPOSTNETMSGBYSOCKET_H
#define CLCACLIENTPOSTNETMSGBYSOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "CLCAAddress.h"
#include "CLCAPostNetMsgTool.h"
#include "CLSocket.h"
#include "CLDataReceviver.h"
#include "CLEpoll.h"
#include "CLCAClientContext.h"


class CLCAClientPostNetMsgBySocket: public CLCAPostNetMsgTool
{
public:

	CLCAClientPostNetMsgBySocket(uint8_t IPType,uint8_t* IP,uint16_t Port);
	virtual ~CLCAClientPostNetMsgBySocket();

	virtual void PostNetMessage(uint8_t* msg,uint32_t length);
	virtual void* ReadFromNet();

	void startEpollForRead();
	
private:
	void Initialize();
	int Connect();
	int writeNetMsg(uint8_t* msg,uint32_t length);

private:

	CLCAAddress* m_addr;
	CLEpoll* epoll;
	CLSocket * sock;
	CLDataReceviver* recv;
	CLCAClientContext* contextForRecv;
};
#endif