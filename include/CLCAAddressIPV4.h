#ifndef CLCAADDRESSIPV4_H
#define CLCAADDRESSIPV4_H

#include "CLCAAddress.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

class CLCAAddressIPV4 : public CLCAAddress
{
public:
	virtual void Initialize();
	virtual int getAddrSize();
	CLCAAddressIPV4(const uint8_t* IP,const uint16_t Port);
	virtual ~CLCAAddressIPV4();
private:
	struct sockaddr_in* addr_ipv4;
};
#endif