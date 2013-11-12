#ifndef CLCAUPDATEPKMUTIMSGDESERIALIZER_H
#define CLCAUPDATEPKMUTIMSGDESERIALIZER_H

#include "CLCADeSerializer.h"
#include "CLCAUpdatePKMsgDeSerializer.h"

class CLCAUpdatePKMutiMsgDeSerializer : public CLCADeSerializer
{
public:
	CLCAUpdatePKMutiMsgDeSerializer();
	virtual ~CLCAUpdatePKMutiMsgDeSerializer();

	virtual vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);

private:
	void getMsgNumber(uint8_t* buf);
private:
	CLCAUpdatePKMsgDeSerializer* SingleMsgDeser;
	uint32_t msgNum;


};
#endif