#ifndef CLCAREUPDATEPKMUTIMSGDESERIALIZER_H
#define CLCAREUPDATEPKMUTIMSGDESERIALIZER_H

#include "CLCADeSerializer.h"
#include "CLCAREUpdatePKMsgDeSerializer.h"

class CLCAREUpdatePKMutiMsgDeSerializer : public CLCADeSerializer
{
public:
	CLCAREUpdatePKMutiMsgDeSerializer();
	virtual ~CLCAREUpdatePKMutiMsgDeSerializer();

	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);

private:
	void getMsgNumber(uint8_t* buf);

private:
	CLCAREUpdatePKMsgDeSerializer* SingleMsgDeser;
	uint32_t MsgNum;

};
#endif