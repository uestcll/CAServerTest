#include "CLDataReceviverBySocket.h"
#include <stdlib.h>

CLDataReceviverBySocket::CLDataReceviverBySocket(CLProtocolDecapsulator* ptcDcps,CLSocket* socket):CLDataReceviver(ptcDcps)
{
	m_socket = socket;
	con_client = 0;
	
}

CLDataReceviverBySocket::CLDataReceviverBySocket(CLSocket* socket)
{
	m_socket = socket;
	con_client = 0;
	
}

CLDataReceviverBySocket::~CLDataReceviverBySocket()
{

}

void CLDataReceviverBySocket::Initialize()
{

}

void* CLDataReceviverBySocket::getData()
{
	uint8_t* buf = 0;
	uint8_t* HeadBuf = 0;
	int readLength = 0;
	uint32_t FullLen = 0;
	uint32_t ReadLen = 0;
	if(con_client->isDcpsHead())
	{
		HeadBuf = new uint8_t[8];
		m_socket->ReadSocket(8,HeadBuf,&ReadLen,con_client->sock);
		FullLen = m_PtcDcps->DecapsulateProtocolHeadForLength(HeadBuf,4);
		con_client->FullLen = FullLen;
		con_client->MsgType = m_PtcDcps->DecapsulateProtocolHeadForMsgType(HeadBuf,0);

		buf = new uint8_t[FullLen];
		delete HeadBuf;
		con_client->buf = buf;
		con_client->HasReadLen = 0;
	}


	readLength = con_client->FullLen - con_client->HasReadLen;
	m_socket->ReadSocket(readLength,con_client->buf+con_client->HasReadLen,&ReadLen,con_client->sock);
	con_client->HasReadLen += ReadLen;

	return con_client;

}

void CLDataReceviverBySocket::setContext(void* pContext)
{
	con_client = (CLCAClientContext*)pContext;

}