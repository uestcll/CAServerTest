#ifndef CLPROTOCOLDECAPSULATOR_H
#define CLPROTOCOLDECAPSULATOR_H

class CLProtocolDecapsulator
{
public:
	CLProtocolDecapsulator();
	virtual ~CLProtocolDecapsulator();

	virtual uint32_t DecapsulateProtocolHeadForLength(const uint8_t* buf,const uint32_t IndexOfLength);
	virtual uint32_t DecapsulateProtocolHeadForMsgType(const uint8_t* buf,const uint32_t IndexOfType)
};
#endif