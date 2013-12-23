#ifndef CLCAUPDATEPKMSGDESERIALIZER_H
#define CLCAUPDATEPKMSGDESERIALIZER_H

#include "CLCADeSerializer.h"

#include <vector>
#include <stdint.h>

class CLCAMessage;
class CLBuffer;

class CLCAUpdatePKMsgDeSerializer : public CLCADeSerializer
{
public:
	CLCAUpdatePKMsgDeSerializer();
	virtual ~CLCAUpdatePKMsgDeSerializer();

	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);
//	virtual std::vector<CLCAMessage*>* DeSerializer(uint8_t* buf,uint32_t size);
};
#endif