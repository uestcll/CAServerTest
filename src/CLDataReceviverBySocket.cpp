#include "CLDataReceviverBySocket.h"
#include <stdlib.h>



CLDataReceviverBySocket::CLDataReceviverBySocket():CLDataReceviver()
{
	m_socket = 0;
	m_ReadSize  = 0;
}

CLDataReceviverBySocket::~CLDataReceviverBySocket()
{

}

void CLDataReceviverBySocket::Initialize()
{

}

void* CLDataReceviverBySocket::getData(uint32_t readSize /* = 0xffffffff */)
{
	if(readSize == 0xffffffff)
		m_ReadSize = buf_size;
	else
		m_ReadSize = readSize;
	m_buf = new uint8_t[m_ReadSize];
	m_socket->ReadSocket(m_ReadSize,m_buf,&m_HasReadSize);
	
}

void CLDataReceviverBySocket::setContext(void* pContext)
{

	m_socket = (CLSocket*)pContext;

}