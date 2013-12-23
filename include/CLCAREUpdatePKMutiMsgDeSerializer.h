#ifndef CLCAREUPDATEPKMUTIMSGDESERIALIZER_H
#define CLCAREUPDATEPKMUTIMSGDESERIALIZER_H

#include "CLCADeSerializer.h"

#include <stdint.h>
#include <vector>

class CLBuffer;
class CLCAMessage;
class CLCAREUpdatePKMsgDeSerializer;

class CLCAREUpdatePKMutiMsgDeSerializer : public CLCADeSerializer
{
public:
	CLCAREUpdatePKMutiMsgDeSerializer();
	virtual ~CLCAREUpdatePKMutiMsgDeSerializer();

	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);
//	virtual std::vector<CLCAMessage*>* DeSerializer(uint8_t* buf,uint32_t size);

private:
	void getMsgNumber(uint8_t* buf);

private:
	CLCAREUpdatePKMsgDeSerializer* SingleMsgDeser;
	uint32_t MsgNum;

};
#endif