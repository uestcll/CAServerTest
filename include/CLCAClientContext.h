#ifndef CLCACLIENTCONTEXT_H
#define CLCACLIENTCONTEXT_H

#include "CLSocket.h"
#include <stdint.h>

class CLCAClientContext
{
public:
	CLSocket* m_sock;
	uint8_t* buf;
	uint32_t MsgType;
	uint32_t MsgNum;
//	uint32_t HasLeftLen;
	uint32_t HasReadLen;
	uint32_t FullLen;

public:
	CLCAClientContext(CLSocket* sock);
	virtual ~CLCAClientContext();
	bool isReadFull();
	bool isSock(int sockfd);
	bool isDcpsHead();
};
#endif