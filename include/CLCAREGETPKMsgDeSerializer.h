#ifndef CLCAREGETPKMSGDESERIALIZER_H
#define CLCAREGETPKMSGDESERIALIZER_H
#include "CLCAMessage.h"
#include "CLCADeSerializer.h"

class CLCAREGETPKMsgDeSerializer : public CLCADeSerializer
{
public:
	virtual CLCAMessage* DeSerializer(uint8_t* buf);
	CLCAREGETPKMsgDeSerializer();
	virtual ~CLCAREGETPKMsgDeSerializer();
};
#endif