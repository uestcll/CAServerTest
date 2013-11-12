#ifndef CLCAGETPKMSGDESERIALIZER_H
#define CLCAGETPKMSGDESERIALIZER_H

#include "CLCADeSerializer.h"
#include "CLCAMessage.h"

class CLCAGETPKMsgDeSerializer:public CLCADeSerializer
{
public:
	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);
	CLCAGETPKMsgDeSerializer();
	virtual ~CLCAGETPKMsgDeSerializer();
};
#endif