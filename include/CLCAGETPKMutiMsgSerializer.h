#ifndef CLCAGETPKMUTIMSGSERIALIZER_H
#define CLCAGETPKMUTIMSGSERIALIZER_H

#include "CLCASerializer.h"
#include "CLCAGETPKMsgSerializer.h"
#include "CLCAGETPKMessage.h"
#include <list>

class CLCAGETPKMutiMsgSerializer : public CLCASerializer
{

public:
	CLCAGETPKMutiMsgSerializer();
	virtual ~CLCAGETPKMutiMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message);
	virtual void SerializeHead(uint32_t Type ,uint32_t number);
	virtual uint8_t* getSerializeChar();

private:
	CLCAGETPKMsgSerializer* SingleMsgSer;
	uint8_t* HeadBuf;
	uint8_t* MsgBuf;
	uint32_t MsgNum;
	uint32_t MsgType;
	std::list<CLCAGETPKMessage*>* msg_list;

};

#endif