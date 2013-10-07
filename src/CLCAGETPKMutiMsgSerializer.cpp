#include "CLCAGETPKMutiMsgSerializer.h"
#include <string.h>
#include <iostream>

using namespace std;

CLCAGETPKMutiMsgSerializer::CLCAGETPKMutiMsgSerializer()
{
	SingleMsgSer = new CLCAGETPKMsgSerializer;
	msg_list = new list<CLCAGETPKMessage*>;
	
}

CLCAGETPKMutiMsgSerializer::~CLCAGETPKMutiMsgSerializer()
{
	delete SingleMsgSer;
	list<CLCAGETPKMessage*>::iterator it;
	for(it = msg_list->begin();it != msg_list->end();it++)
	{
		if((*it) != 0)
			delete (*it);

	}

	delete msg_list;
}

void CLCAGETPKMutiMsgSerializer::SerializeHead(uint32_t Type /* = PK_FORMGET */,uint32_t number)
{
	MsgType = Type;
	MsgNum = number;
	uint8_t* Head = new uint8_t[12];
	memset(Head,0,12);
	FullLength += 4;
	uint32_t* type = (uint32_t*)Head;
	uint32_t* len = (uint32_t*)(Head+4);
	uint32_t* num = (uint32_t*)(Head+8);
	*type = MsgType;
	*len = FullLength;
	*num = MsgNum;
	HeadBuf = Head;
}

uint8_t* CLCAGETPKMutiMsgSerializer::Serialize(CLCAMessage* message)
{
	
		FullLength += message->FullLength;
		msg_list->push_back((CLCAGETPKMessage*)message->copy());
		return 0;
}

uint8_t* CLCAGETPKMutiMsgSerializer::getSerializeChar()
{
	uint8_t* buf = new uint8_t[FullLength+12];
	memset(buf,0,FullLength+12);
	CLCAGETPKMessage* msg = 0;
	uint32_t HasWriteLen = 12;
	uint8_t* tempbuf = 0;
	list<CLCAGETPKMessage*>::iterator it;
	for(it = msg_list->begin();it != msg_list->end();it++)
	{
		tempbuf = SingleMsgSer->Serialize(*it);
		memcpy(buf+HasWriteLen,tempbuf,(*it)->FullLength);
		delete tempbuf;
		tempbuf = 0;

	}

	return buf;
}

