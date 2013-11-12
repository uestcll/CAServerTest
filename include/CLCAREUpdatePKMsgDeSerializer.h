#ifndef CLCAREUPDATEPKMSGDESERIALIZER_H
#define CLCAREUPDATEPKMSGDESERIALIZER_H

#include "CLCADeSerializer.h"

class CLCAREUpdatePKMsgDeSerializer : public CLCADeSerializer
{
public:
	CLCAREUpdatePKMsgDeSerializer();
	virtual ~CLCAREUpdatePKMsgDeSerializer();

	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);
};
#endif