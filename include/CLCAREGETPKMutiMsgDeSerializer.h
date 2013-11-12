#ifndef CLCAREGETPKMUTIMSGDESERIALIZER_H
#define CLCAREGETPKMUTIMSGDESERIALIZER_H

#include "CLCADeSerializer.h"
#include "stdint.h"
#include <vector>

class CLCAMessage;
class CLCAREGETPKMsgDeSerializer;

class CLCAREGETPKMutiMsgDeSerializer : public CLCADeSerializer
{
public:
	CLCAREGETPKMutiMsgDeSerializer();
	virtual ~CLCAREGETPKMutiMsgDeSerializer();

	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex);

private:
	void getMsgNumber(uint8_t* buf);

private:

	CLCAREGETPKMsgDeSerializer* SingleDeSer;
	int Msg_number;
};
#endif