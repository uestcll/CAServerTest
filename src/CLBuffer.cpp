#include "CLBuffer.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

CLBuffer::CLBuffer()
{
	buf_vec = new vector<Buffer*>;
	buf_size = 0;
	Succession_Index = 0;
	Block_Index = 0;
	InBlock_Index = 0;

	iovecIndex = 0;
	iovecBlock_Index = 0;
	iovecInBlock_Index = 0;
	iovecBufferSize = 0;
	ioves = 0;
}

CLBuffer::~CLBuffer()
{
	vector<Buffer*> ::iterator it;
	for(it = buf_vec->begin();it != buf_vec->end();it++)
	{
		if((*it)->EnableDelete > 1)
			delete (*it)->Inchar;
		free(*it);
	}

	if(ioves != 0)
		free(ioves);

	delete buf_vec;
}

void CLBuffer::addBuf(uint8_t* Char,uint32_t Size,bool IsNeedNew /* = false */,uint32_t IsDelete /* = 0 */)
{
	if(Size == 0 || Char == 0)
		return;

	Buffer* buf = (Buffer*)malloc(sizeof(Buffer));

	buf->EnableDelete = IsDelete;

	if(IsNeedNew)
	{
		buf->Inchar = new uint8_t[Size+1];
		memcpy(buf->Inchar,Char,Size);

		buf->Inchar[Size] = '\0';
		buf->EnableDelete = CANDELETE;
	}
	else
		buf->Inchar = Char;
	
	buf->InSize = Size;
	
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
		buf = buf_vec->at(i);
		IndexToNextBlock += buf->InSize;

		if(index >= IndexToNextBlock){
			IndexInBlockStart = IndexToNextBlock;
			continue;
		}

		InBlockIndex = index - IndexInBlockStart;

		if(index + size <= IndexToNextBlock )
		{
			*IsDelete = false;
			retbuf = (uint8_t*)(buf->Inchar 
				+ InBlockIndex);

			return retbuf;
		}
		else
			blockIndex = i;

		break;
	}

	retbuf = new uint8_t[size+1];
	memset(retbuf,0,size+1);
	uint32_t LeftCopySize = size;
	uint32_t HasCopySize = 0;
	uint32_t copysize = 0;

	while(LeftCopySize > 0)
	{
		buf = buf_vec->at(blockIndex);
		if(buf->InSize - InBlockIndex <= LeftCopySize)
			copysize = buf->InSize - InBlockIndex;
		else
			copysize = LeftCopySize;

		memcpy(retbuf+HasCopySize,buf->Inchar+InBlockIndex,copysize);
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
	buf = buf_vec->at(Block_Index);
	
	if(InBlock_Index + size <= buf->InSize)
	{
		*IsDelete = false;
		ret_buf = (uint8_t*)(buf->Inchar + InBlock_Index);
		Succession_Index += size;

		InBlock_Index += size;

		if(InBlock_Index == buf->InSize)
		{
			InBlock_Index = 0;
			Block_Index ++;
		}

		return ret_buf;

	}

	ret_buf = new uint8_t[size+1];
	memset(ret_buf,0,size+1);
	uint32_t LeftCopySize = size;
	uint32_t HasCopySize = 0;
	uint32_t copysize = 0;

	while(LeftCopySize > 0)
	{
		buf = buf_vec->at(Block_Index);

		if(buf->InSize - InBlock_Index <= LeftCopySize)
			copysize = buf->InSize - InBlock_Index;
		else
			copysize = LeftCopySize;
		
		memcpy(ret_buf+HasCopySize,buf->Inchar+InBlock_Index,copysize);
		HasCopySize += copysize;
		LeftCopySize -= copysize;
		
		if(LeftCopySize == 0)
		{
			if(InBlock_Index + copysize < buf->InSize)
			{
				InBlock_Index += copysize;
				break;
			}
		}
		
		Block_Index++;
		InBlock_Index = 0;
	}


	Succession_Index += size;

	return ret_buf;

}

int CLBuffer::setSuccessionIndex(uint32_t index /* = 0 */)
{
	if(index >= buf_size)
		return -1; //error return

	if(Succession_Index == index)
		return 0;

	

	if(index == 0)
	{
		InBlock_Index = 0;
		Block_Index = 0;
		Succession_Index = 0;
		return 0;
	}

	Buffer* buf = 0;
	buf = buf_vec->at(Block_Index);

	if( ((index > Succession_Index) && (index - Succession_Index < buf->InSize - InBlock_Index))
		|| ((index < Succession_Index) && (Succession_Index - index <= InBlock_Index)))
	{
		InBlock_Index = InBlock_Index + index - Succession_Index;
		Succession_Index = index;

		return 0;
	}

	uint32_t currentIndex = 0;
	uint32_t BlockIndex = 0;

	while(currentIndex <= index)
	{
		buf = buf_vec->at(BlockIndex);

		if(currentIndex + buf->InSize > index)
		{
			Block_Index = BlockIndex;
			InBlock_Index = index - currentIndex;
			break;
		}
		
		BlockIndex++;
		currentIndex += buf->InSize;


	}

	return 0;
}

int CLBuffer::getSuccessionIndex()
{
	return Succession_Index;
}

uint32_t CLBuffer::getBufferSize()
{
	return buf_size;
}

int CLBuffer::setIovecIndex(uint32_t index)
{
	if(index >= buf_size)
		return -1; //error return

	iovecIndex = index;
	iovecBufferSize = 0;
	iovecCount = 0;

	if(index == 0)
	{
		iovecInBlock_Index = 0;
		iovecBlock_Index = 0;
		return 0;
	}

	uint32_t currentIndex = 0;
	Buffer* buf = 0;
	uint32_t BlockIndex = 0;

	while(currentIndex <= index)
	{
		buf = buf_vec->at(BlockIndex);

		if(currentIndex + buf->InSize > index)
		{
			iovecBlock_Index = BlockIndex;
			iovecInBlock_Index = index - currentIndex;
			break;
		}

		BlockIndex++;
		currentIndex += buf->InSize;


	}

	return 0;
}

int CLBuffer::BufferToIovec()
{
	if(iovecBlock_Index >= buf_vec->size())
		return -1;

	if(ioves != 0)
		free(ioves);

	iovecCount = buf_vec->size() - iovecBlock_Index;
	iovec* vec = (iovec*)malloc(sizeof(iovec) * iovecCount);
	iovecBufferSize = 0;


	int i = 0;
	Buffer* buffer = 0;
	while(i< iovecCount)
	{
		buffer = buf_vec->at(iovecBlock_Index);
		vec[i].iov_base = buffer->Inchar + iovecInBlock_Index;
		vec[i].iov_len = buffer->InSize - iovecInBlock_Index;

		iovecBufferSize += vec[i].iov_len;
		iovecBlock_Index++;
		iovecInBlock_Index = 0;
		i++;
	}

	ioves = vec;
}

struct iovec* CLBuffer::getBufferToIovec()
{
	return ioves;
}

int CLBuffer::getIovecCount()
{
	return iovecCount;
}

uint32_t CLBuffer::getBufferToIovecBufferSize()
{
	return iovecBufferSize;
}

uint32_t CLBuffer::getIovecIndex()
{
	return iovecIndex;
}