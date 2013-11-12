#include "CLCAGETPKMutiMsgSerializer.h"
#include <string.h>
#include <iostream>
#include <sys/socket.h>

using namespace std;

CLCAGETPKMutiMsgSerializer::CLCAGETPKMutiMsgSerializer()
{
	SingleMsgSer = new CLCAGETPKMsgSerializer;
	msg_list = new list<CLCAMessage*>;
	HeadBuf = 0;
	
}

CLCAGETPKMutiMsgSerializer::~CLCAGETPKMutiMsgSerializer()
{
	delete SingleMsgSer;

	if(HeadBuf != 0)
	{
		delete HeadBuf;
		HeadBuf = 0;
	}
	
	list<CLCAMessage*>::iterator it;
	for(it = msg_list->begin();it!= msg_list->end();it++)
	{
		if(*it != 0)
			delete *it;
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
	*type = htonl(MsgType);
	*len = htonl(FullLength);
	*num = htonl(msg_list->size());
	
	if(HeadBuf != 0)
		delete HeadBuf;

	HeadBuf = Head;
}

uint8_t* CLCAGETPKMutiMsgSerializer::Serialize(CLCAMessage* message)
{
	if(ReStart)
	{
		FullLength = 0;
		ReStart = false;
	}

	CLCAGETPKMessage* msg = dynamic_cast<CLCAGETPKMessage*>(message);
	if(msg == 0)
		return 0;

	FullLength += message->FullLength;
	msg_list->push_back(msg->copy());
	return 0;
}

uint8_t* CLCAGETPKMutiMsgSerializer::getSerializeChar()
{
	if(msg_list->size() == 0)
	{
		ReStart = true;
		return 0;

	}

//	if(HeadBuf == 0)
//		return 0;

	uint8_t* buf = new uint8_t[FullLength+12];
	memset(buf,0,FullLength+12);
	memcpy(buf,HeadBuf,12);
	CLCAMessage* msg = 0;

	uint32_t HasWriteLen = 12;
	uint8_t* tempbuf = 0;

	list<CLCAMessage*>::iterator it;
	for(it = msg_list->begin();it != msg_list->end();it++)
	{
		tempbuf = SingleMsgSer->Serialize(*it);
		memcpy(buf+HasWriteLen,tempbuf,(*it)->FullLength);
		
		delete tempbuf;
		delete (*it);

		tempbuf = 0;
		
	}

	clearList();
	delete HeadBuf ;
	HeadBuf = 0;

	ReStart = true;
	return buf;
}

void CLCAGETPKMutiMsgSerializer::clearList()
{

	msg_list->clear();
}