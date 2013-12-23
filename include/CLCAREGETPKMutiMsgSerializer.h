#ifndef CLCAREGETPKMUTIMSGSERIALIZER_H
#define CLCAREGETPKMUTIMSGSERIALIZER_H

#include "CLCASerializer.h"

#include <list>
#include <stdint.h>

class CLCAMessage;
class CLCAREGETPKMsgSerializer;

class CLCAREGETPKMutiMsgSerializer : public CLCASerializer
{
public:
	CLCAREGETPKMutiMsgSerializer();
	virtual ~CLCAREGETPKMutiMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message , std::vector<CLCAMessage*>* msg_vec , 
		uint32_t MsgType , uint32_t* SerializeLen , bool IsDelete = true ,bool IsHeadSerialize = true );

private:
	CLCAREGETPKMsgSerializer* SingleMsgSer;



};
#endif