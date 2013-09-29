#include "CLCAGETPKMutiMsgDeSerializer.h"
#include "CLCAMessage.h"
#include "CLCAGETPKMessage.h"
#include "CLCAGETPKMsgDeSerializer.h"

CLCAGETPKMutiMsgDeSerializer::CLCAGETPKMutiMsgDeSerializer()
{
	Msg_number = 0;
	SingleDeSer = new CLCAGETPKMsgDeSerializer;
}

CLCAGETPKMutiMsgDeSerializer::~CLCAGETPKMutiMsgDeSerializer()
{
	delete SingleDeSer;
}

void CLCAGETPKMutiMsgDeSerializer::getMsgNumber(uint8_t* buf)
{
	uint32_t* num = (uint32_t*)buf;
	Msg_number = *num;

}

int CLCAGETPKMutiMsgDeSerializer::getMsgLength(uint8_t* buf)
{
	uint32_t* LengthOfName = (uint32_t*)buf;
	int strl = *LengthOfName+ ((*LengthOfName)%4 == 0?0:(4- (*LengthOfName%4)));
	return strl+12;
}

CLCAMessage* CLCAGETPKMutiMsgDeSerializer::DeSerializer(uint8_t* buf)
{
	uint8_t* temp = buf;
	getMsgNumber(temp);
	temp += 4;

	CLCAGETPKMessage* msg = new CLCAGETPKMessage[Msg_number];
	int NextIndex = 0;
	CLCAGETPKMessage* temp_msg = 0;
	for(int i = 0;i<Msg_number;i++)
	{
		NextIndex = getMsgLength(temp);
		temp_msg = (CLCAGETPKMessage*)SingleDeSer->DeSerializer(temp);
		msg[i] = *temp_msg;
		delete temp_msg;
		temp += NextIndex;
	}

}


