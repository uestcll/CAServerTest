#ifndef CLCAREGETPKMESSAGE_H
#define CLCAREGETPKMESSAGE_H

#include "CLCAMessage.h"
#include <stdint.h>

class CLCAREGETPKMessage : public CLCAMessage
{

public:
	uint16_t isSuccess;
	uint16_t errorno;
	uint32_t len;
	uint32_t EchoId;
	uint8_t* PublicKey;
	uint8_t* Reserved;
public:
	CLCAREGETPKMessage(uint16_t IsSuccess,uint16_t errorNo,uint32_t Len,uint8_t* pk,uint32_t echoId);
	virtual ~CLCAREGETPKMessage();

	virtual CLCAMessage* copy();


};

#endif