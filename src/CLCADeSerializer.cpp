#include "CLCADeSerializer.h"
#include "CLBuffer.h"

CLCADeSerializer::CLCADeSerializer()
{

}

CLCADeSerializer::~CLCADeSerializer()
{

}

std::vector<CLCAMessage*>* CLCADeSerializer::DeSerializer(uint8_t* buf,uint32_t size)
{
	if(buf == 0 || size == 0)
		return 0;

	CLBuffer* buffer = new CLBuffer;
	buffer->addBuf(buf,size);

	std::vector<CLCAMessage*>* vec = DeSerializer(buffer,0);

	delete buffer;

	return vec;
}