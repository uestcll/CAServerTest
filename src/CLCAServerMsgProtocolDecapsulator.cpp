#include "CLCAServerMsgProtocolDecapsulator.h"
#include <string.h>

using namespace std;

CLCAServerMsgProtocolDecapsulator::CLCAServerMsgProtocolDecapsulator(uint8_t* InChar,
	uint32_t InSize):CLProtocolDecapsulator(InChar,InSize)
{
	startBlockIndex = 0;
	startIndexInBlock = 0;
	m_NeededBufSize = 0;
	LeftBuffer = 0;
	
}

CLCAServerMsgProtocolDecapsulator::~CLCAServerMsgProtocolDecapsulator()
{

}

void CLCAServerMsgProtocolDecapsulator::ProtocolDecapsulate()
{
	if(input_vec->size() == 0){
		m_NeededBufSize = 0xffffffff;
		return;
	}

	if(input_vec->size() < startBlockIndex+1)
	{
		m_NeededBufSize = 0xffffffff;
		return;
	}

	PtlDcpstInput* in = input_vec[startBlockIndex];
	while(startBlockIndex < input_vec->size())
	{
		decapsualteSingle(in->Inchar,in->InSize);
		startBlockIndex++;
	}

	

}

void CLCAServerMsgProtocolDecapsulator::addCharToDecapsulate(uint8_t* Inchar,uint32_t InSize)
{
	PtlDcpstInput* in = (PtlDcpstInput*)malloc(sizeof(PtlDcpstInput));
	in->EnableDelete = 0;
	in->Inchar = Inchar;
	in->InSize = InSize;
	input_vec->push_back(in);
}

void CLCAServerMsgProtocolDecapsulator::decapsualteSingle(uint8_t* buf,uint32_t size)
{
	uint32_t FullLength = 0;
	bool IsDeleted = false;
	uint32_t* Length = 0;
	uint32_t LeftBufferSize = 0;
	uint32_t index = 0;
	if(LeftBuffer != 0)
	{
		if(m_NeededBufSize == 0 )
			return;
		uint32_t MsgNeededSize = 0;
		LeftBufferSize = LeftBuffer->getBufferSize();
		if(m_NeededBufSize == 0xffffffff && LeftBufferSize < 8)
		{
			if(LeftBuffer->getBufferSize() + size < 8)
			{
				LeftBuffer->addBuf(buf,size);
				m_NeededBufSize = 0xffffffff;
				return;
			}
			else
			{
				uint8_t* Buf = new uint8_t[8];
				memset(Buf,0,8);
				uint8_t* buf_LeftBuffer = LeftBuffer->getBuf(0,LeftBufferSize,&IsDeleted);
				memcpy(Buf,buf_LeftBuffer,LeftBufferSize);
				if(IsDeleted)
					delete buf_LeftBuffer;
				memcpy(Buf+LeftBufferSize,buf,8 - LeftBufferSize);
				Length = (uint32_t*)(Buf+4);
				FullLength = *Length;
				delete Buf;
				//uint8_t* Buf = (uint8_t*)LeftBuffer->getBuf(4,4,&IsDeleted);
				
			
			}
		}
		else
		{
			
			uint8_t* Buf = (uint8_t*)LeftBuffer->getBuf(4,4,&IsDeleted);
			Length = (uint32_t*)Buf;
			FullLength = *Length;
			if(IsDeleted)
				delete Buf;
			
		}

		MsgNeededSize = FullLength + 8 - LeftBufferSize;
		if(MsgNeededSize > size)
		{
			LeftBuffer->addBuf(buf,size);
			m_NeededBufSize = MsgNeededSize - size;
			return;
		}
		else if(MsgNeededSize == size)
		{
			LeftBuffer->addBuf(buf,size);
			ret_vec->push_back(LeftBuffer);
			LeftBuffer = 0;
			m_NeededBufSize = 0;
			return;
		}

		LeftBuffer->addBuf(buf,MsgNeededSize);
		ret_vec->push_back(LeftBuffer);
		LeftBuffer = 0;
		m_NeededBufSize = 0;
		index += MsgNeededSize;
	}

	CLBuffer* buffer = 0;
	while(index < size)
	{
		buffer = new CLBuffer;
		if(index + 8 > size)
		{
			buffer->addBuf(buf+index,size - index);
			index = size;
			LeftBuffer = buffer;
			m_NeededBufSize = 0xffffffff;
			break;
		}

		Length = (uint32_t*)(buf+index+4);
		FullLength = *Length;

		if(FullLength > size - (index + 8))
		{
			buffer->addBuf(buf+index ,size - index);
			index = size;
			LeftBuffer = buffer;
			m_NeededBufSize = FullLength - (size - index - 8);
			break;
		}

		buffer->addBuf(buf + index, FullLength + 8);
		ret_vec->push_back(buffer);
		index += (FullLength + 8);
		if(index == size)
		{
			LeftBuffer = 0;
			m_NeededBufSize = 0xffffffff;
		}
	}

	return;
}

void* CLCAServerMsgProtocolDecapsulator::getDecapsulatorChar()
{
	vector<CLBuffer*>* ret = ret_vec;
	ret_vec = 0;
	return ret;

}

uint32_t CLCAServerMsgProtocolDecapsulator::getLeftSize()
{
	return m_NeededBufSize;
}
