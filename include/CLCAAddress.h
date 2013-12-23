#ifndef CLCAADDRESS_H
#define CLCAADDRESS_H

#include <netinet/in.h>
#include <stdint.h>

class CLCAAddress
{
public:
	CLCAAddress(const uint8_t* IP,uint16_t Port);
	virtual ~CLCAAddress();


	virtual void Initialize() = 0;
	virtual int getAddrSize() = 0;
//	int getSock();
	struct sockaddr* getAddr();

protected:
//	int sock;
	struct sockaddr* addr;
	const uint8_t* m_IP;
	uint16_t m_Port;

};
#endif