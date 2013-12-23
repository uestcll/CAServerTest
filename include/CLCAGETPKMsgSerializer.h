#ifndef CLCAGETPKMSGSERIALIZER_H
#define CLCAGETPKMSGSERIALIZER_H

#include "CLCASerializer.h"

#include <stdint.h>

class CLCAMessage;

//���Ƕ�CLCAGETMessage������л��������������紫��
class CLCAGETPKMsgSerializer : public CLCASerializer
{
public:
	CLCAGETPKMsgSerializer();
	virtual ~CLCAGETPKMsgSerializer();

	virtual uint8_t* Serialize(CLCAMessage* message , std::vector<CLCAMessage*>* msg_vec , 
		uint32_t MsgType , uint32_t* SerializeLen ,bool IsDelete = true ,bool IsHeadSerialize = true );

};
#endif