#ifndef CLCADESERIALIZER_H
#define CLCADESERIALIZER_H

#include <stdint.h>
#include <vector>


class CLBuffer;
class CLCAMessage;

// ����CLCAMessage�ķ���ʼ������
class CLCADeSerializer
{
public:
	CLCADeSerializer();
	virtual ~CLCADeSerializer();

	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex) = 0;
	virtual std::vector<CLCAMessage*>* DeSerializer(uint8_t* buf,uint32_t size) ;

};
#endif