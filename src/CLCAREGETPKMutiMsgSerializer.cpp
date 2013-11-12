#include "CLCAREGETPKMutiMsgSerializer.h"
#include <sys/socket.h>
#include <string.h>

using namespace std;

CLCAREGETPKMutiMsgSerializer::CLCAREGETPKMutiMsgSerializer()
{
	SingleMsgSer = new CLCAREGETPKMsgSerializer;
	msg_list = new list<CLCAMessage*>;
	MsgNum = 0;
	HeadBuf = 0;
}

CLCAREGETPKMutiMsgSerializer::~CLCAREGETPKMutiMsgSerializer()
{
	delete SingleMsgSer;

	if(HeadBuf != 0)
		delete HeadBuf;

	list<CLCAMessage*>::iterator it;
	for(it = msg_list->begin();it!= msg_list->end();it++)
	{
		if(*it != 0)
			delete *it;
	}

	delete msg_list;
}

void CLCAREGETPKMutiMsgSerializer::SerializeHead(uint32_t Type ,uint32_t number)
{
	if(HeadBuf != 0)
		delete HeadBuf;

	HeadBuf = new uint8_t[12];
	memset(HeadBuf,0,12);

	uint32_t* msgType = (uint32_t*)HeadBuf;
	*MsgType = htonl(Type);

	uint32_t* len = (uint32_t*)(HeadBuf + 4);
	FullLength += 4;
	*len = htonl(FullLength);

	uint32_t* num = (uint32_t*)(HeadBuf+8);
	*num = htonl(msg_list->size());



}

uint8_t* CLCAREGETPKMutiMsgSerializer::Serialize(CLCAMessage* message)
{
	if(ReStart)
	{
		FullLength = 0;
		ReStart = false;

	}

	CLCAREGETPKMessage* msg = dynamic_cast<CLCAREGETPKMessage*>message;

	if(msg == 0)
		return 0;

	FullLength += msg->FullLength;
	msg_list->push_back(msg->copy());

	return 0;
}

uint8_t* CLCAREGETPKMutiMsgSerializer::getSerializeChar()
{
	if(msg_list->size() == 0)
	{
		ReStart = true;
		return 0;

	}

	if(HeadBuf == 0)
		return 0;

	CLCAREGETPKMessage* msg = 0;
	list<CLCAREGETPKMessage*>::iterator it;

	uint8_t* buf = new uint8_t[FullLength + 12];
	memset(buf,0,FullLength+12);
	memcpy(buf,HeadBuf,12);

	uint8_t* tempbuf = 0;
	uint32_t HasWriteLen = 12;
	for(it = msg_list->begin();it != msg_list->end();it ++ )
	{
		msg = (*it);
		tempbuf = SingleMsgSer->Serialize(msg);
		memcpy(buf+HasWriteLen,tempbuf,msg->FullLength);

		delete tempbuf;
		delete msg;

		tempbuf = 0;
		msg = 0;

	}

	delete HeadBuf;
	HeadBuf = 0;

	ReStart = true;
	clear_list();

	return buf;

}

void CLCAREGETPKMutiMsgSerializer::clear_list()
{
	msg_list->clear();
}