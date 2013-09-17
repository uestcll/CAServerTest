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
	if(m_IPType == TYPEOFIPV4)
		m_addr = new CLCAAddressIPV4(m_IP,m_Port);
	else
		m_addr = new CLCAAddressIPV4(m_IP,m_Port);
	
	m_addr->Initialize();
	sock = m_addr->getSock();
	serv_addr = m_addr->getAddr();
	addrSize = m_addr->getAddrSize();
	Connect();
}

int CLCAClientPostNetMsgBySocket::Connect()
{
	return connect(sock,serv_addr,addrSize);
}

int CLCAClientPostNetMsgBySocket::writeNetMsg(uint8_t* msg,uint32_t length)
{

	uint8_t* buf = msg;
	uint32_t len = length;
	uint32_t writeLen = 0;
	uint32_t writeBuf = 0;
	while(len > 0)
	{
		if(len > 1024)
		{
			writeLen = 1024;
			len = len - 1024;
		}
		else
		{
			writeLen = len;
			len = 0;
		}

		write(sock,buf+writeBuf,writeLen);
		writeBuf += writeLen;
	}


}

void CLCAClientPostNetMsgBySocket::startThreadForRead()
{
	epoll = new CLEpoll(100);
	epoll->
}

uint8_t* CLCAClientPostNetMsgBySocket::ReadFormNet()
{
	uint32_t ReadLen = 0;
	uint32_t HasLeftLen = 0;
	uint32_t PReadLen = 0;
	uint32_t HasReadLen = 0;
	bool First = false;
	uint8_t Head[8];
	uint8_t* buf = 0;
	while(!First)
	{
		 if(read(sock,Head,8) == 8)
			 First = true;
	}

	uint32_t Len = *(uint32_t*)(Head+4);
	if(Len > 0)
		buf = new uint8_t[Len+8];
	else
		return buf;

	memcpy(buf,Head,8);
	HasLeftLen = Len;
	HasReadLen = 8;
	while(HasLeftLen > 0)
	{
		if(HasLeftLen > 1024)
			PReadLen = 1024;
		else
			PReadLen = HasLeftLen;

		ReadLen = read(sock,buf+HasReadLen,PReadLen);
		HasReadLen += ReadLen;
		HasLeftLen -= ReadLen;
	}

	return buf;
}