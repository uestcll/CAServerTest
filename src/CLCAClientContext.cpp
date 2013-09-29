#include "CLCAClientContext.h"

CLCAClientContext::CLCAClientContext(CLSocket* sock)
{
	this->m_sock = sock;
	this->buf = 0;
	this->FullLen = 0;
	this->HasReadLen = 0;
	this->MsgType = 0;
	this->MsgNum = 0;
}

CLCAClientContext::~CLCAClientContext()
{
	if(buf != 0)
		delete buf;
	if(m_sock != 0)
		delete m_sock;


}

bool CLCAClientContext::isReadFull()
{
	return (FullLen > 0)&& (FullLen == HasReadLen);
}

bool CLCAClientContext::isSock(int sockfd)
{
	return (m_sock->getSock() == sockfd);
}

bool CLCAClientContext::isDcpsHead()
{
	return (FullLen == 0);
}