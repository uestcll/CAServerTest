#include "CLCAClientContext.h"

CLCAClientContext::CLCAClientContext(int sock)
{
	this->sock = sock;
	this->buf = 0;
	this->FullLen = 0;
	this->HasReadLen = 0;
	this->MsgType = 0;
	this->MsgNum = 0;
}

bool CLCAClientContext::isReadFull()
{
	return (FullLen > 0)&& (FullLen == HasReadLen)
}

bool CLCAClientContext::isSock(int sockfd)
{
	return (sock == sockfd)
}

bool CLCAClientContext::isDcpsHead()
{
	return (FullLen == 0)
}