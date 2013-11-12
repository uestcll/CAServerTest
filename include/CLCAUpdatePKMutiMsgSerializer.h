#ifndef CLCAUPDATEPKMUTIMSGSERIALIZER_H
#define CLCAUPDATEPKMUTIMSGSERIALIZER_H

#include "CLCASerializer.h"
#include "CLCAUpdatePKMsgSerializer.h"
#include "CLCAMessage.h"

#include <list>

class CLCAUpdatePKMutiMsgSerializer : public CLCASerializer
{
public:
	CLCAUpdatePKMutiMsgSerializer();
	virtual ~CLCAUpdatePKMutiMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message);
	virtual void SerializeHead(uint32_t Type,uint32_t number);
	virtual uint8_t* getSerializeChar();


private:
	CLCAUpdatePKMsgSerializer* SingleMsgSer;
	uint8_t* HeadBuf;
	uint8_t* MsgBuf;
	std::list<CLCAMessage*>* msg_list;
};
#endif