#ifndef CLCAREGETPKMSGDESERIALIZER_H
#define CLCAREGETPKMSGDESERIALIZER_H
#include "CLCAMessage.h"
#include "CLCADeSerializer.h"

class CLCAREGETPKMsgDeSerializer : public CLCADeSerializer
{
public:
	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);
	CLCAREGETPKMsgDeSerializer();
	virtual ~CLCAREGETPKMsgDeSerializer();
};
#endif