#ifndef CLCAADDRESSIPV4_H
#define CLCAADDRESSIPV4_H

#include "CLCAAddress.h"

#include <stdint.h>
#include <netinet/in.h>

class CLCAAddressIPV4 : public CLCAAddress
{
public:
	CLCAAddressIPV4(const uint8_t* IP,uint16_t Port);
	virtual ~CLCAAddressIPV4();

	virtual void Initialize();
	virtual int getAddrSize();
	
private:
	struct sockaddr_in* addr_ipv4;
};
#endif