#ifndef CLCAUPDATEPKMSGSERIALIZER_H
#define CLCAUPDATEPKMSGSERIALIZER_H

#include "CLCASerializer.h"

#include <stdint.h>

class CLCAMessage;

class CLCAUpdatePKMsgSerializer : public CLCASerializer
{
public:
	CLCAUpdatePKMsgSerializer();
	virtual ~CLCAUpdatePKMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message , std::vector<CLCAMessage*>* msg_vec , 
		uint32_t MsgType , uint32_t* SerializeLen , bool IsDelete = true ,bool IsHeadSerialize = true );


};
#endif