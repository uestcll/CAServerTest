#ifndef CLCAGETPKMUTIMSGDESERIALIZER_H
#define CLCAGETPKMUTIMSGDESERIALIZER_H

#include "CLCADeSerializer.h"
#include "stdint.h"

class CLCAMessage;
class CLCAGETPKMsgDeSerializer;

class CLCAGETPKMutiMsgDeSerializer : public CLCADeSerializer
{

public:
	CLCAGETPKMutiMsgDeSerializer();
	virtual ~CLCAGETPKMutiMsgDeSerializer();

	virtual CLCAMessage* DeSerializer(uint8_t* buf);

private:
	void getMsgNumber(uint8_t* buf);
	int getMsgLength(uint8_t* buf);

private:

	CLCAGETPKMsgDeSerializer* SingleDeSer;
	int Msg_number;
};
#endif