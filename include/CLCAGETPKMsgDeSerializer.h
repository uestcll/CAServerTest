#ifndef CLCAGETPKMSGDESERIALIZER_H
#define CLCAGETPKMSGDESERIALIZER_H

#include "CLCADeSerializer.h"

#include <vector>
#include <stdint.h>

class CLCAMessage;
class CLBuffer;

class CLCAGETPKMsgDeSerializer:public CLCADeSerializer
{
public:
	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);
//	virtual std::vector<CLCAMessage*>* DeSerializer(uint8_t* buf,uint32_t size);

	CLCAGETPKMsgDeSerializer();
	virtual ~CLCAGETPKMsgDeSerializer();
};
#endif