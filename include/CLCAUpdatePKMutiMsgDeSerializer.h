#ifndef CLCAUPDATEPKMUTIMSGDESERIALIZER_H
#define CLCAUPDATEPKMUTIMSGDESERIALIZER_H

#include "CLCADeSerializer.h"

#include <stdint.h>
#include <vector>

class CLCAMessage;
class CLBuffer;
class CLCAUpdatePKMsgDeSerializer;

class CLCAUpdatePKMutiMsgDeSerializer : public CLCADeSerializer
{
public:
	CLCAUpdatePKMutiMsgDeSerializer();
	virtual ~CLCAUpdatePKMutiMsgDeSerializer();

	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);
//	virtual std::vector<CLCAMessage*>* DeSerializer(uint8_t* buf,uint32_t size);

private:
	void getMsgNumber(uint8_t* buf);
private:
	CLCAUpdatePKMsgDeSerializer* SingleMsgDeser;
	uint32_t msgNum;


};
#endif