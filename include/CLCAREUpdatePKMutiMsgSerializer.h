#ifndef CLCAREUPDATEPKMUTIMSGSERIALIZER_H
#define CLCAREUPDATEPKMUTIMSGSERIALIZER_H

#include "CLCASerializer.h"
#include "CLCAREUpdatePKMsgSerializer.h"
#include <list>

class CLCAREUpdatePKMutiMsgSerializer : public CLCASerializer
{
public:
	CLCAREUpdatePKMutiMsgSerializer();
	virtual ~CLCAREUpdatePKMutiMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message);
	virtual void SerializeHead(uint32_t Type,uint32_t number);
	virtual uint8_t* getSerializeChar();

private:
	CLCAREUpdatePKMsgSerializer* SingleMsgSer;
	uint8_t* HeadBuf;
	std::list<CLCAMessage*>* msg_list;

};
#endif