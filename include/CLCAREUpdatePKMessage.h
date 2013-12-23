#ifndef CLCAREUPDATEPKMESSAGE_H
#define CLCAREUPDATEPKMESSAGE_H

#include "CLCAMessage.h"

#include <stdint.h>

class CLCAREUpdatePKMessage : public CLCAMessage
{
public:
	uint16_t IsSuccess;
	uint16_t ErrorNo;
	uint32_t EchoId;

public:
	CLCAREUpdatePKMessage(uint16_t isSuccess,uint16_t errorNo,uint32_t echoId);
	virtual ~CLCAREUpdatePKMessage();

	virtual CLCAMessage* copy();
};
#endif