#ifndef CLCAGETPKMESSAGE_H
#define CLCAGETPKMESSAGE_H

#include "CLCAMessage.h"
#include <stdint.h>
// 这是查找服务器公钥的查找类。
// LengthOfName长度包括Name字符串的结束字符.
class CLCAGETPKMessage : public CLCAMessage
{
public:
	uint32_t LengthOfName;
	uint8_t* Name;
	uint8_t* Reserved;
	uint32_t PKType;
	uint32_t EchoID;

public:
	CLCAGETPKMessage(uint32_t lengthOfName,uint8_t* name,uint32_t pkType,uint32_t echoID);
	virtual ~CLCAGETPKMessage();	
	
	CLCAGETPKMessage& operator = (const CLCAGETPKMessage&);
	CLCAGETPKMessage();

	virtual CLCAMessage* copy();

private:
	CLCAGETPKMessage(const CLCAGETPKMessage&);

public:
	uint32_t FullLength;
};

#endif