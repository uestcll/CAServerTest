#ifndef CLCAGETPKMUTIMSGSERIALIZER_H
#define CLCAGETPKMUTIMSGSERIALIZER_H

#include "CLCASerializer.h"

#include <stdint.h>

class CLCAMessage;
class CLCAGETPKMsgSerializer;

class CLCAGETPKMutiMsgSerializer : public CLCASerializer
{

public:
	CLCAGETPKMutiMsgSerializer();
	virtual ~CLCAGETPKMutiMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message , std::vector<CLCAMessage*>* msg_vec , 
		uint32_t MsgType , uint32_t* SerializeLen ,bool IsDelete = true , bool IsHeadSerialize = true );

private:
	CLCAGETPKMsgSerializer* SingleMsgSer;


};

#endif