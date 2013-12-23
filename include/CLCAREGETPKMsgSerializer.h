#ifndef CLCAREGETPKMSGSERIALIZER_H
#define CLCAREGETPKMSGSERIALIZER_H

#include "CLCASerializer.h"

#include <stdint.h>

class CLCAMessage;

class CLCAREGETPKMsgSerializer : public CLCASerializer
{
public:

	CLCAREGETPKMsgSerializer();
	virtual ~CLCAREGETPKMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message , std::vector<CLCAMessage*>* msg_vec , 
		uint32_t MsgType , uint32_t* SerializeLen , bool IsDelete = true ,bool IsHeadSerialize = true );
	
};
#endif