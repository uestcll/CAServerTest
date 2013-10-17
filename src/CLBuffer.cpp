#include "CLBuffer.h"
#include <stdio.h>
#include <string.h>

using namespace std;

CLBuffer::CLBuffer()
{
	buf_vec = new std::vector<Buffer*>;
	buf_size = 0;
	Succession_Index = 0;
	Block_Index = 0;
	InBlock_Index = 0;
}

CLBuffer::~CLBuffer()
{
	vector<Buffer*> ::iterator it;
	for(it = buf_vec->begin();it != buf_vec->end();it++)
	{
		free(*it);
	}

	delete buf_vec;
}

void CLBuffer::addBuf(uint8_t* Char,uint32_t Size)
{
	if(Size == 0 || Char == 0)
		return;

	Buffer* buf = (Buffer*)malloc(sizeof(Buffer));
	buf->OutChar = Char;
	buf->OutSize = Size;
	buf_vec->push_back(buf);
	buf_size += Size;
}

uint8_t* CLBuffer::getBuf(uint32_t index,uint32_t size,bool* IsDelete)
{
	if(size == 0)
		return 0;
	if(index + size > buf_size)
		return 0;
	*IsDelete = true;
	uint32_t vecSize = buf_vec->size();
	uint32_t blockIndex = 0;
	uint32_t InBlockIndex = 0;
	uint32_t IndexToNextBlock = 0;
	uint32_t IndexInBlockStart = 0;
	Buffer* buf = 0;
	uint8_t* retbuf = 0;
	for(int i = 0;i<vecSize;i++)
	{
		buf = buf_vec[i];
		IndexToNextBlock += buf->OutSize;
		if(index >= IndexToNextBlock){
			IndexInBlockStart = IndexToNextBlock;
			continue;
		}

		InBlockIndex = index - IndexInBlockStart;
		if(index + size <= IndexToNextBlock )
		{
			*IsDelete = false;
			retbuf = (uint8_t*)(buf->OutChar 
				+ InBlockIndex);
			return retbuf;
		}
		else
			blockIndex = i;
	}

	retbuf = new uint8_t[size];
	memset(retbuf,0,size);
	uint32_t LeftCopySize = size;
	uint32_t HasCopySize = 0;
	uint32_t copysize = 0;
	while(LeftCopySize > 0)
	{
		buf = buf_vec[blockIndex];
		if(buf->OutSize - InBlockIndex <= LeftCopySize)
			copysize = buf->OutSize - InBlockIndex;
		else
			copysize = LeftCopySize;
		memcpy(retbuf+HasCopySize,buf->OutChar+InBlockIndex,copysize);
		HasCopySize += copysize;
		LeftCopySize -= copysize;
		blockIndex++;
		InBlockIndex = 0;
	}

	return retbuf;


}

uint8_t* CLBuffer::getSuccessionBuf(uint32_t size,bool* IsDelete)
{
	if(size == 0)
		return 0;
	if(Succession_Index + size > buf_size)
		return 0;
	*IsDelete = true;
	Buffer* buf = 0;
	uint8_t* ret_buf = 0;
	buf = buf_vec[Block_Index];
	if(InBlock_Index + size <= buf->OutSize)
	{
		*IsDelete = false;
		ret_buf = (uint8_t*)(buf->OutChar + InBlock_Index);
		Succession_Index += size;
		return ret_buf;
	}

	retbuf = new uint8_t[size];
	memset(retbuf,0,size);
	uint32_t LeftCopySize = size;
	uint32_t HasCopySize = 0;
	uint32_t copysize = 0;
	while(LeftCopySize > 0)
	{
		buf = buf_vec[Block_Index];
		if(buf->OutSize - InBlock_Index <= LeftCopySize)
			copysize = buf->OutSize - InBlock_Index;
		else
			copysize = LeftCopySize;
		memcpy(retbuf+HasCopySize,buf->OutChar+InBlock_Index,copysize);
		HasCopySize += copysize;
		LeftCopySize -= copysize;
		if(LeftCopySize == 0)
		{
			if(InBlock_Index + copysize < buf->OutSize)
			{
				InBlock_Index += copysize;
				break;
			}
		}
		Block_Index++;
		InBlock_Index = 0;
	}

	Succession_Index += size;
	return retbuf;

}

void CLBuffer::setSuccessionIndex(uint32_t index /* = 0 */)
{
	if(index >= buf_size)
		return; //error return
	Succession_Index = index;
	if(index == 0)
	{
		InBlock_Index = 0;
		Block_Index = 0;
		return;
	}
	uint32_t currentIndex = 0;
	Buffer* buf = 0;
	uint32_t BlockIndex = 0;
	while(currentIndex <= index)
	{
		buf = buf_vec[BlockIndex];
		if(currentIndex + buf->OutSize > index)
		{
			Block_Index = BlockIndex;
			InBlock_Index = index - currentIndex;
			break;
		}
		
		BlockIndex++;
		currentIndex += buf->OutSize;


	}
}

uint32_t CLBuffer::getBufferSize()
{
	return buf_size;
}