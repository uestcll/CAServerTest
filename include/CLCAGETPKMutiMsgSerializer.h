#ifndef CLCAGETPKMUTIMSGSERIALIZER_H
#define CLCAGETPKMUTIMSGSERIALIZER_H

#include "CLCASerializer.h"
#include "CLCAGETPKMsgSerializer.h"

class CLCAGETPKMutiMsgSerializer : public CLCASerializer
{

public:
	CLCAGETPKMutiMsgSerializer();
	virtual ~CLCAGETPKMutiMsgSerializer();

	virtual void Serialize(CLCAMessage* message);
	virtual void SerializeHead(uint32_t Type ,uint32_t number);
	virtual void SerializeLength();
	virtual uint8_t* getSerializeChar();

private:
	CLCAGETPKMsgSerializer* SingleMsgSer;
	std::string strMsg;
	uint32_t MsgNum;
	uint32_t MsgType;

};

#endif