#ifndef CLCAGETPKMUTIMSGDESERIALIZER_H
#define CLCAGETPKMUTIMSGDESERIALIZER_H

#include "CLCADeSerializer.h"
#include "stdint.h"
#include <vector>

class CLCAMessage;
class CLCAGETPKMsgDeSerializer;

class CLCAGETPKMutiMsgDeSerializer : public CLCADeSerializer
{

public:
	CLCAGETPKMutiMsgDeSerializer();
	virtual ~CLCAGETPKMutiMsgDeSerializer();

	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);

private:
	void getMsgNumber(uint8_t* buf);

private:

	CLCAGETPKMsgDeSerializer* SingleDeSer;
	int Msg_number;
};
#endif