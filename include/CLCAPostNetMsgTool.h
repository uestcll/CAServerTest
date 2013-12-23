#ifndef CLCAPOSTNETMSGTOOL_H
#define CLCAPOSTNETMSGTOOL_H

#include <stdint.h>
#include <vector>

class CLCAMessage;

#define TYPEOFIPV4 0
#define TYPEOFIPV6 1


class CLCAPostNetMsgTool
{
public:
	virtual void PostNetMessage(std::vector<CLCAMessage*>* vec , int type) = 0;
	virtual void PostSingleNetMessage(CLCAMessage* msg) = 0;
	virtual void* ReadFromNet() = 0;

	CLCAPostNetMsgTool(uint8_t IPType,uint8_t* IP,uint16_t Port);
	virtual ~CLCAPostNetMsgTool();
protected:
	uint8_t m_IPType;
	uint8_t* m_IP;//ipv6 128λ  ipv4 32λ
	uint16_t m_Port;
};
#endif