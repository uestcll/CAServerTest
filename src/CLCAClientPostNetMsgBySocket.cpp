#include "CLCAClientPostNetMsgBySocket.h"
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



using namespace std;

CLCAClientPostNetMsgBySocket::CLCAClientPostNetMsgBySocket(uint8_t IPType,uint8_t* IP,uint16_t Port):CLCAPostNetMsgTool(IPType,IP,Port)
{
	Initialize();
}

void CLCAClientPostNetMsgBySocket::PostNetMessage(uint8_t* msg,uint32_t length)
{
	writeNetMsg(msg,length);
}

void CLCAClientPostNetMsgBySocket::Initialize()
{
	epoll = 0;
	sock = new CLSocket(m_IP,m_Port,false,m_IPType);
	sock->setNonBlock();
	recv = new CLDataReceviverBySocket(sock);
	contextForRecv = new CLCAClientContext(sock->getSock());
	recv->setContext(contextForRecv);
	Connect();
}

int CLCAClientPostNetMsgBySocket::Connect()
{
	return sock->ConnectSocket();
}

int CLCAClientPostNetMsgBySocket::writeNetMsg(uint8_t* msg,uint32_t length)
{

	uint8_t* buf = msg;
	uint32_t len = length;
	uint32_t HasWriteLen = 0;
	uint32_t writeLen = 0;
	while(HasWriteLen<length)
	{
	
		writeLen = sock->WriteSocket(msg+HasWriteLen,len);
		HasWriteLen += writeLen;
		len -= writeLen;
	}


}

void CLCAClientPostNetMsgBySocket::startEpollForRead()
{
	epoll = new CLEpoll(100);
	epoll->Register_ctl(EPOLL_CTL_ADD,sock->getSock());
}

void* CLCAClientPostNetMsgBySocket::ReadFromNet()
{
	startEpollForRead();
	while(true)
	{
		int fds = epoll->EpollWait();
		for(int i = 0;i<fds ; i++)
		{
			if(epoll->getEventFd(i) == sock->getSock())
			{
				contextForRecv = (CLCAClientContext*)recv->getData();
				if(contextForRecv->isReadFull() && contextForRecv != 0)
					return contextForRecv;
			}
			else
				continue;
		}

	}
}