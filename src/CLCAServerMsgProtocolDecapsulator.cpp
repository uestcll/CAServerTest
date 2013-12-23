#include "CLCAServerMsgProtocolDecapsulator.h"
#include "CLBuffer.h"

#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

using namespace std;

CLCAServerMsgProtocolDecapsulator::CLCAServerMsgProtocolDecapsulator(uint8_t* InChar,
	uint32_t InSize,uint32_t IsDelete /* = CANDELETE*/):CLProtocolDecapsulator(InChar,InSize,IsDelete)
{
	m_startBlockIndex = 0;
	m_startIndexInBlock = 0;
	m_NeededBufSize = 0xffffffff;
	m_LeftBuffer = 0;
	
}

CLCAServerMsgProtocolDecapsulator::CLCAServerMsgProtocolDecapsulator():CLProtocolDecapsulator()
{
	m_startBlockIndex = 0;
	m_startIndexInBlock = 0;
	m_NeededBufSize = 0xffffffff;
	m_LeftBuffer = 0;
}

CLCAServerMsgProtocolDecapsulator::~CLCAServerMsgProtocolDecapsulator()
{

}

int CLCAServerMsgProtocolDecapsulator::ProtocolDecapsulate()
{
	if(input_vec->size() == 0){
		m_NeededBufSize = 0xffffffff;
		return -1;
	}

	if(input_vec->size() < m_startBlockIndex+1)
	{
		return -1;
	}

	PtlDcpstInput* in = input_vec->at(m_startBlockIndex);


	while(m_startBlockIndex < input_vec->size())
	{
		decapsualteSingle(in->Inchar,in->InSize);
		m_startBlockIndex++;
	}

	return 0;

}

int CLCAServerMsgProtocolDecapsulator::addCharToDecapsulate(uint8_t* Inchar,uint32_t InSize,uint32_t IsDelete /* = CANDELETE */)
{
	if(Inchar == 0 || InSize == 0)
		return -1;

	PtlDcpstInput* in = (PtlDcpstInput*)malloc(sizeof(PtlDcpstInput));
	in->EnableDelete = IsDelete;
	in->Inchar = Inchar;
	in->InSize = InSize;
	input_vec->push_back(in);
	
	return 0;
}

void CLCAServerMsgProtocolDecapsulator::decapsualteSingle(uint8_t* buf,uint32_t size )
{
	uint32_t FullLength = 0;
	bool IsDeleted = false;
	uint32_t* Length = 0;
	uint32_t LeftBufferSize = 0;
	uint32_t index = 0;

	if(m_LeftBuffer != 0)
	{
		if(m_NeededBufSize == 0 )
			return;

		uint32_t MsgNeededSize = 0;
		LeftBufferSize = m_LeftBuffer->getBufferSize();
		
		if(m_NeededBufSize == 0xffffffff && LeftBufferSize < 8)
		{
			if(m_LeftBuffer->getBufferSize() + size < 8)
			{
				m_LeftBuffer->addBuf(buf,size);
				m_NeededBufSize = 0xffffffff;
		
				return;
			}
			else
			{
				uint8_t* Buf = new uint8_t[8];
				memset(Buf,0,8);
				
				uint8_t* buf_LeftBuffer = m_LeftBuffer->getBuf(0,LeftBufferSize,&IsDeleted);
				memcpy(Buf,buf_LeftBuffer,LeftBufferSize);
				
				if(IsDeleted)
					delete buf_LeftBuffer;

				memcpy(Buf+LeftBufferSize,buf,8 - LeftBufferSize);
				Length = (uint32_t*)(Buf+4);
				FullLength = ntohl(*Length);
				
				delete Buf;
				//uint8_t* Buf = (uint8_t*)m_LeftBuffer->getBuf(4,4,&IsDeleted);
				
			
			}
		}
		else
		{
			
			uint8_t* Buf = (uint8_t*)m_LeftBuffer->getBuf(4,4,&IsDeleted);
			Length = (uint32_t*)Buf;
			FullLength = ntohl(*Length);
			
			if(IsDeleted)
				delete Buf;
			
		}

		MsgNeededSize = FullLength + 8 - LeftBufferSize;
		
		if(MsgNeededSize > size)
		{
			m_LeftBuffer->addBuf(buf,size);
			m_NeededBufSize = MsgNeededSize - size;
			return;
		}
		else if(MsgNeededSize == size)
		{
			m_LeftBuffer->addBuf(buf,size);
			ret_vec->push_back(m_LeftBuffer);
			m_LeftBuffer = 0;
			m_NeededBufSize = 0;
		
			return;
		}

		m_LeftBuffer->addBuf(buf,MsgNeededSize);
		ret_vec->push_back(m_LeftBuffer);
		m_LeftBuffer = 0;
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

			m_LeftBuffer = buffer;
			m_NeededBufSize = 0xffffffff;
			break;
		}

		Length = (uint32_t*)(buf+index+4);
		FullLength = ntohl(*Length);

		if(FullLength > size - (index + 8))
		{
			buffer->addBuf(buf+index ,size - index);
			index = size;
			m_LeftBuffer = buffer;
			m_NeededBufSize = FullLength - (size - index - 8);
			
			break;
		}

		buffer->addBuf(buf + index, FullLength + 8);
		ret_vec->push_back(buffer);
		index += (FullLength + 8);
		
		if(index == size)
		{
			m_LeftBuffer = 0;
			m_NeededBufSize = 0;
		}
	}

	return;
}

vector<CLBuffer*>* CLCAServerMsgProtocolDecapsulator::getDecapsulatorMsgChar()
{
	vector<CLBuffer*>* ret = ret_vec;
	ret_vec = new vector<CLBuffer*>;
	return ret;

}

uint32_t CLCAServerMsgProtocolDecapsulator::getLeftSize()
{
	return m_NeededBufSize;
}
