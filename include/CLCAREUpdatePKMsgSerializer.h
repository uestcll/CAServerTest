#ifndef CLCAREUPDATEPKMSGSERIALIZER_H
#define CLCAREUPDATEPKMSGSERIALIZER_H

#include "CLCASerializer.h"

#include <stdint.h>

class CLCAMessage;

class CLCAREUpdatePKMsgSerializer : public CLCASerializer
{
public:
	CLCAREUpdatePKMsgSerializer();
	virtual ~CLCAREUpdatePKMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message, std::vector<CLCAMessage*>* msg_vec , 
		uint32_t MsgType , uint32_t* SerializeLen , bool IsDelete = true ,bool IsHeadSerialize = true );


};
#endif