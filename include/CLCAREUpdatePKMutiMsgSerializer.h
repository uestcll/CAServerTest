#ifndef CLCAREUPDATEPKMUTIMSGSERIALIZER_H
#define CLCAREUPDATEPKMUTIMSGSERIALIZER_H

#include "CLCASerializer.h"

#include <list>
#include <stdint.h>

class CLCAMessage;
class CLCAREUpdatePKMsgSerializer;

class CLCAREUpdatePKMutiMsgSerializer : public CLCASerializer
{
public:
	CLCAREUpdatePKMutiMsgSerializer();
	virtual ~CLCAREUpdatePKMutiMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message , std::vector<CLCAMessage*>* msg_vec , 
		uint32_t MsgType , uint32_t* SerializeLen , bool IsDelete = true ,bool IsHeadSerialize = true );

private:
	CLCAREUpdatePKMsgSerializer* SingleMsgSer;


};
#endif