#ifndef CLCASERIALIZER_H
#define CLCASERIALIZER_H


#include <stdint.h>
#include <vector>

class CLCAMessage;

class CLCASerializer
{
public:
	CLCASerializer();
	virtual ~CLCASerializer();

	// 单消息序列化时，参数二、三均为0
	//多消息序列化时，参数一为0
	virtual uint8_t* Serialize(CLCAMessage* message , std::vector<CLCAMessage*>* msg_vec , 
		uint32_t MsgType , uint32_t* SerializeLen ,bool IsDelete = true , bool IsHeadSerialize = true ) = 0;



};

#endif