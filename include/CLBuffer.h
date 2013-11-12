#ifndef CLBUFFER_H
#define CLBUFFER_H

#include <vector>
#include <iostream>
#include <stdint.h>
#include <sys/uio.h>

#define CANDELETE 2
#define CANNOTDELETE 0
#define DELETED 1

class CLProtocolDecapsulator;
struct Buffer
{
	uint8_t* Inchar;
	uint32_t InSize;
	uint32_t EnableDelete; // >1 can delete, =1 has deleted,=0 cannot delete 
};
class CLBuffer
{
public:
	CLBuffer();
	virtual ~CLBuffer();

	
	uint8_t* getBuf(uint32_t index,uint32_t size,bool* IsDelete); //IsDelete = true delete the char
	uint8_t* getSuccessionBuf(uint32_t size,bool* IsDelete);
	int setSuccessionIndex(uint32_t index = 0);
	int getSuccessionIndex();
	uint32_t getBufferSize();

	void setIovecIndex(uint32_t index);
	struct iovec* getBufferToIovec();
	int getIovecCount();
	uint32_t getBufferToIovecBufferSize();
	uint32_t getIovecIndex();

//	friend class CLProtocolDecapsulator;

	void addBuf(uint8_t* Char,uint32_t Size,uint32_t IsDelete = 0);

private:
	std::vector<Buffer*>* buf_vec;
	uint32_t buf_size;
	uint32_t Succession_Index;
	uint32_t Block_Index;
	uint32_t InBlock_Index;

	uint32_t iovecIndex;
	uint32_t iovecBlock_Index;
	uint32_t iovecInBlock_Index;
	int iovecCount;
	uint32_t iovecBufferSize;
};

#endif