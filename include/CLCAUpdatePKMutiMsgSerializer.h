#ifndef CLCAUPDATEPKMUTIMSGSERIALIZER_H
#define CLCAUPDATEPKMUTIMSGSERIALIZER_H

#include "CLCASerializer.h"

#include <list>
#include <stdint.h>

class CLCAMessage;
class CLCAUpdatePKMsgSerializer;

class CLCAUpdatePKMutiMsgSerializer : public CLCASerializer
{
public:
	CLCAUpdatePKMutiMsgSerializer();
	virtual ~CLCAUpdatePKMutiMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message , std::vector<CLCAMessage*>* msg_vec , 
		uint32_t MsgType , uint32_t* SerializeLen , bool IsDelete = true ,bool IsHeadSerialize = true );


private:
	CLCAUpdatePKMsgSerializer* SingleMsgSer;

};
#endif