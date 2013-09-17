#ifndef CLCASERIALIZER_H
#define CLCASERIALIZER_H
#include <stdint.h>
#include "CLCAMessage.h"


class CLCASerializer
{
public:
	virtual void Serialize(CLCAMessage* message) = 0;
	virtual void SerializeHead(uint32_t Type,uint32_t number) = 0;
	virtual void SerializeLength() = 0;
	virtual uint8_t* getSerializeChar() = 0;
	uint32_t getLength();

	CLCASerializer();
	virtual ~CLCASerializer();
protected:
	uint32_t FullLength;

};

#endif