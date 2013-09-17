#include "CLProtocolDecapsulator.h"

CLProtocolDecapsulator::CLProtocolDecapsulator()
{

}

CLProtocolDecapsulator::~CLProtocolDecapsulator()
{

}

uint32_t CLProtocolDecapsulator::DecapsulateProtocolHeadForLength(const uint8_t* buf,const uint32_t IndexOfLength)
{
	uint32_t* len = (uint32_t*)(buf+IndexOfLength);
	return *len;
}

uint32_t CLProtocolDecapsulator::DecapsulateProtocolHeadForMsgType(const uint8_t* buf,const uint32_t IndexOfType)
{
	uint32_t* MsgType = (uint32_t*)(buf+IndexOfType);
	return *MsgType;
}