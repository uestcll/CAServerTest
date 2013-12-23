#ifndef CLCAREGETPKMSGDESERIALIZER_H
#define CLCAREGETPKMSGDESERIALIZER_H

#include "CLCADeSerializer.h"

#include <stdint.h>
#include <vector>

class CLCAMessage;
class CLBuffer;

class CLCAREGETPKMsgDeSerializer : public CLCADeSerializer
{
public:
	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);
//	virtual std::vector<CLCAMessage*>* DeSerializer(uint8_t* buf,uint32_t size);

	CLCAREGETPKMsgDeSerializer();
	virtual ~CLCAREGETPKMsgDeSerializer();
};
#endif