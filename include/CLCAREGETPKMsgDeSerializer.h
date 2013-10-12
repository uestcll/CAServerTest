#ifndef CLCAREGETPKMSGDESERIALIZER_H
#define CLCAREGETPKMSGDESERIALIZER_H

class CLCAREGETPKMsgDeSerializer : public CLCADeSerializer
{
public:
	virtual CLCAMessage* DeSerializer(uint8_t* buf);
	CLCAREGETPKMsgDeSerializer();
	virtual ~CLCAREGETPKMsgDeSerializer();
};
#endif