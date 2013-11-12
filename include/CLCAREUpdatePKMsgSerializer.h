#ifndef CLCAREUPDATEPKMSGSERIALIZER_H
#define CLCAREUPDATEPKMSGSERIALIZER_H

#include "CLCASerializer.h"

class CLCAREUpdatePKMsgSerializer : public CLCASerializer
{
public:
	CLCAREUpdatePKMsgSerializer();
	virtual ~CLCAREUpdatePKMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message);
	virtual void SerializeHead(uint32_t Type,uint32_t number);
	virtual uint8_t* getSerializeChar();

private:
	uint8_t* HeadBuf;
	uint8_t* MsgBuf;

};
#endif