#ifndef CLCAREUPDATEPKMSGDESERIALIZER_H
#define CLCAREUPDATEPKMSGDESERIALIZER_H

#include "CLCADeSerializer.h"

#include <stdint.h>
#include <vector>

class CLCAMessage;
class CLBuffer;

class CLCAREUpdatePKMsgDeSerializer : public CLCADeSerializer
{
public:
	CLCAREUpdatePKMsgDeSerializer();
	virtual ~CLCAREUpdatePKMsgDeSerializer();

	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);
//	virtual std::vector<CLCAMessage*>* DeSerializer(uint8_t* buf,uint32_t size);
};
#endif