#include "CLDataReceviverBySocket.h"
#include <stdlib.h>

CLDataReceviverBySocket::CLDataReceviverBySocket(CLProtocolDecapsulator* ptcDcps,CLSocket* socket):CLDataReceviver(ptcDcps)
{
	m_socket = socket;
	con_client = 0;
	
}

CLDataReceviverBySocket::CLDataReceviverBySocket(CLSocket* socket):CLDataReceviver()
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
		HeadBuf = new uint8_t[12];
		m_socket->ReadSocket(12,HeadBuf,&ReadLen,con_client->sock);
		FullLen = m_PtcDcps->DecapsulateProtocolHeadForLength(HeadBuf,4);
		con_client->FullLen = FullLen;
		con_client->MsgType = m_PtcDcps->DecapsulateProtocolHeadForMsgType(HeadBuf,0);

		if(con_client->MsgType == PK_FORMGET)
		{
			uint32_t* num = (uint32_t*)(HeadBuf+8);
			con_client->MsgNum = *num;
			buf = new uint8_t[FullLen-4];
			memset(buf,0,FullLen-4);

		}
		else
		{
			buf = new uint8_t[FullLen];
			memset(buf,0,FullLen);
			memcpy(buf,HeadBuf+8,4);
		}
		con_client->FullLen -= 4;
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