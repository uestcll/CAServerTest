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

	virtual void Initialize(CLDataReceviver* recv);
	virtual int Accept();
	virtual std::vector<CLIENT_MSG_INFO>* getData();
	virtual int  writeData(std::vector<CLCAMessage*>* msg_vec,int sock,bool IsMutiMsg,uint32_t MsgId);
	virtual void start();
	virtual void run();


private:
	CLIENT_CLBUFFER_INFO* getClientData(int clientfd);
	int HandleAccept(int clientfd);
	void FreeClientFd(int clientfd);
	void CheckForClientFd();

private:
//	static CLCAServerByEpoll* server;
	CLEpoll* m_epoll;
	int nfds;
	std::vector<CLIENT_CLBUFFER_INFO*>* client_MsgBuf;

};
#endif