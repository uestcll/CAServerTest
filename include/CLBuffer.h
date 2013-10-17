#ifndef CLBUFFER_H
#define CLBUFFER_H

#include <vector>
#include <iostream>
#include <stdint.h>

class CLProtocolDecapsulator;
struct Buffer
{
	uint8_t* OutChar;
	uint32_t OutSize;

};
class CLBuffer
{
public:
	CLBuffer();
	virtual ~CLBuffer();

	
	uint8_t* getBuf(uint32_t index,uint32_t size,bool* IsDelete); //IsDelete = true delete the char
	uint8_t* getSuccessionBuf(uint32_t size,bool* IsDelete);
	void setSuccessionIndex(uint32_t index = 0);
	uint32_t getBufferSize();
	friend class CLProtocolDecapsulator;

private:
	void addBuf(uint8_t* Char,uint32_t Size);

private:
	std::vector<Buffer*>* buf_vec;
	uint32_t buf_size;
	uint32_t Succession_Index;
	uint32_t Block_Index;
	uint32_t InBlock_Index;
};

#endif