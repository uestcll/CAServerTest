#ifndef CLCAUPDATEPKMSGSERIALIZER_H
#define CLCAUPDATEPKMSGSERIALIZER_H

#include "CLCASerializer.h"
#include <stdint.h>

class CLCAUpdatePKMsgSerializer : public CLCASerializer
{
public:
	CLCAUpdatePKMsgSerializer();
	virtual ~CLCAUpdatePKMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message);
	virtual void SerializeHead(uint32_t Type,uint32_t number);
	virtual uint8_t* getSerializeChar();

private:
	uint8_t* HeadBuf;
	uint8_t* MsgBuf;
	uint32_t m_nType;
	uint32_t m_number;



};
#endif