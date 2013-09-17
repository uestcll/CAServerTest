#ifndef CLCACLIENTCONTEXT_H
#define CLCACLIENTCONTEXT_H

class CLCAClientContext
{
public:
	int sock;
	uint8_t* buf;
	uint32_t MsgType;
	uint32_t MsgNum;
//	uint32_t HasLeftLen;
	uint32_t HasReadLen;
	uint32_t FullLen;

public:
	CLCAClientContext(int sock);
	bool isReadFull();
	bool isSock(int sockfd);
	bool isDcpsHead();
};
#endif