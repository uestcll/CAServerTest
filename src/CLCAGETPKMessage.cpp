#include "CLCAGETPKMessage.h"

#include <string.h>

CLCAGETPKMessage::CLCAGETPKMessage(uint32_t lengthOfName,uint8_t* name,uint32_t pkType,uint32_t echoID):CLCAMessage(PK_FORSGET)
{
	LengthOfName = lengthOfName;
	Name = new uint8_t[lengthOfName + 1];
	memcpy(Name,name,lengthOfName + 1);


	PKType = pkType;
	EchoID = echoID;
//	FullLength = 4+LengthOfName+4+4;

}

CLCAGETPKMessage::~CLCAGETPKMessage()
{
	if(Name != 0)
		delete Name;

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
	this->LengthOfName = msg.LengthOfName;
	memcpy(this->Name,msg.Name,this->LengthOfName);
	this->PKType = msg.PKType;

	return *this;
}