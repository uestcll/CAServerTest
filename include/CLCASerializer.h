#ifndef CLCASERIALIZER_H
#define CLCASERIALIZER_H


#include "CLCAMessage.h"
#include <stdint.h>


class CLCASerializer
{
public:
	virtual uint8_t* Serialize(CLCAMessage* message) = 0;
	virtual void SerializeHead(uint32_t Type,uint32_t number) = 0;
	virtual uint8_t* getSerializeChar() = 0;
	uint32_t getLength();
	uint32_t getFullLength();

	CLCASerializer();
	virtual ~CLCASerializer();
protected:
	uint32_t FullLength;
	bool ReStart;

};

#endif