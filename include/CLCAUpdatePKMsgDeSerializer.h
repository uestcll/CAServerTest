#ifndef CLCAUPDATEPKMSGDESERIALIZER_H
#define CLCAUPDATEPKMSGDESERIALIZER_H

#include "CLCADeSerializer.h"
#include "CLCAMessage.h"
#include "CLBuffer.h"

#include <vector>

class CLCAUpdatePKMsgDeSerializer : public CLCADeSerializer
{
public:
	CLCAUpdatePKMsgDeSerializer();
	virtual ~CLCAUpdatePKMsgDeSerializer();

	std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);
};
#endif