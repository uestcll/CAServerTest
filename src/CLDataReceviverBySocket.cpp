#include "CLDataReceviverBySocket.h"
#include "CLSocket.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>

CLDataReceviverBySocket::CLDataReceviverBySocket():CLDataReceviver()
{
	m_socket = 0;
	m_ReadSize  = 0;
	m_IsReceviveAll = true;
}

CLDataReceviverBySocket::~CLDataReceviverBySocket()
{

}

void CLDataReceviverBySocket::Initialize()
{

}

void* CLDataReceviverBySocket::getData(uint32_t readSize /* = 0xffffffff */)
{
	m_IsReceviveAll = true;
	m_errno = -1;

	if(readSize == 0xffffffff || readSize < buf_size)
		m_ReadSize = buf_size;
	else
		m_ReadSize = readSize;

	m_buf = new uint8_t[m_ReadSize ];
	memset(m_buf,0,m_ReadSize );
	int len = m_socket->ReadSocket(m_ReadSize,m_buf,&m_HasReadSize);

	if(len < 0)
	{
		m_errno = errno;
		delete m_buf;
		m_buf = 0;
		return 0;
	}

	if(m_HasReadSize == 0)
	{
		delete m_buf;
		m_buf = 0;
		return 0;
	}

	  //

	if(m_HasReadSize < m_ReadSize/3)
	{
		uint8_t* buf = m_buf;
		m_buf = new uint8_t[m_HasReadSize]; //
		memcpy(m_buf,buf,m_HasReadSize); //
		delete buf; //
	}

	if(m_HasReadSize == m_ReadSize)
		m_IsReceviveAll = false;

	return m_buf;
}

void CLDataReceviverBySocket::setContext(void* pContext)
{

	m_socket = (CLSocket*)pContext;

}