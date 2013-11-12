#ifndef CLCAREGETPKMUTIMSGSERIALIZER_H
#define CLCAREGETPKMUTIMSGSERIALIZER_H

#include "CLCASerializer.h"
#include "CLCAREGETPKMessage.h"
#include "CLCAREGETPKMsgSerializer.h"
#include <list>

class CLCAREGETPKMutiMsgSerializer : public CLCASerializer
{
public:
	CLCAREGETPKMutiMsgSerializer();
	virtual ~CLCAREGETPKMutiMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message);
	virtual void SerializeHead(uint32_t Type ,uint32_t number);
	virtual uint8_t* getSerializeChar();

private:
	void clear_list();

private:
	CLCAREGETPKMsgSerializer* SingleMsgSer;
	uint8_t* HeadBuf;
	uint8_t* MsgBuf;
	uint32_t MsgNum;
	uint32_t MsgType;
	std::list<CLCAMessage*>* msg_list;


};
#endif