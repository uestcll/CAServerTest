#ifndef CLCAGETPKMSGDESERIALIZER_H
#define CLCAGETPKMSGDESERIALIZER_H
#include "CLCADeSerializer.h"
#include "CLCAMessage.h"

class CLCAGETPKMsgDeSerializer:public CLCADeSerializer
{
public:
	virtual CLCAMessage* DeSerializer(uint8_t* buf);
	CLCAGETPKMsgDeSerializer();
	virtual ~CLCAGETPKMsgDeSerializer();
};
#endif