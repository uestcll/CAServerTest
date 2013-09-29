#include "CLCAGETPKMessage.h"
#include <string.h>

CLCAGETPKMessage::CLCAGETPKMessage(uint32_t lengthOfName,uint8_t* name,uint32_t pkType,uint32_t echoID):CLCAMessage(PK_FORSGET)
{
	LengthOfName = lengthOfName;
	Name = new uint8_t[lengthOfName];
	memcpy(Name,name,lengthOfName);
	uint32_t reservedLen = lengthOfName%4 == 0?0:(4-lengthOfName%4);
	if(reservedLen >0)
	{
		Reserved = new uint8_t[reservedLen];
		memset(Reserved,0,reservedLen);
	}
	else
		Reserved = 0;

	PKType = pkType;
	EchoID = echoID;
	FullLength = 4+LengthOfName+reservedLen+4+4;

}

CLCAGETPKMessage::~CLCAGETPKMessage()
{
	if(Name != 0)
		delete Name;
	if(Reserved!= 0)
		delete Reserved;

	
}

CLCAGETPKMessage::CLCAGETPKMessage():CLCAMessage(PK_FORSGET)
{
	

}

CLCAMessage* CLCAGETPKMessage::copy()
{
	return new CLCAGETPKMessage(LengthOfName,Name,PKType,EchoID);
}

CLCAGETPKMessage& CLCAGETPKMessage::operator = (const CLCAGETPKMessage& msg)
{
	this->EchoID = msg.EchoID;
	this->FullLength = msg.FullLength;
	this->LengthOfName = msg.LengthOfName;
	memcpy(this->Name,msg.Name,this->LengthOfName);
	this->PKType = msg.PKType;
	this->Reserved = msg.Reserved;
	return *this;
}