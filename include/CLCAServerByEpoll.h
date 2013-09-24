#ifndef CLCASERVERBYEPOLL_H
#define CLCASERVERBYEPOLL_H
#include "CLCAServer.h"
#include "CLSocket.h"
#include "CLDataReceviver.h"
#include "CLEpoll.h"

class CLCAServerByEpoll:public CLCAServer
{

public:
	
	CLCAServerByEpoll(uint8_t* IP,uint16_t Port,int IPType = AF_INET,int streamType = SOCK_STREAM);
	virtual ~CLCAServerByEpoll();
//	static CLCAServerByEpoll* GetInstance();

	virtual void Initialize();
	virtual int Accept();
	virtual std::list<CLCAClientContext*>* getData();
	virtual int  writeData(int sock,uint8_t* buf);
	virtual void start();


private:
	CLCAClientContext* getClientData(int clientfd);

private:
//	static CLCAServerByEpoll* server;
	CLEpoll* m_epoll;
	int nfds;
	CLDataReceviver* m_drev;

};
#endif