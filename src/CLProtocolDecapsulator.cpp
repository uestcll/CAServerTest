#include "CLProtocolDecapsulator.h"
#include <stdio.h>

CLProtocolDecapsulator::CLProtocolDecapsulator(uint8_t* InChar,uint32_t InSize)
{
	m_NeededBufSize = 0xffffffff;
	ret_vec = new std::vector<CLBuffer*>;
	input_vec = new vector<PtlDcpstInput*>;
	PtlDcpstInput* input = (PtlDcpstInput*)malloc(sizeof(PtlDcpstInput));
	input->Inchar = InChar;
	input->InSize = InSize;
	input->EnableDelete = 0;
	input_vec->push_back(input);

}

CLProtocolDecapsulator::CLProtocolDecapsulator()
{
	m_NeededBufSize = 0xffffffff;
	ret_vec = new std::vector<CLBuffer*>;
	input_vec = new vector<PtlDcpstInput*>;
}

CLProtocolDecapsulator::~CLProtocolDecapsulator()
{
	vector<PtlDcpstInput*> ::iterator it;
	while(it = input_vec->begin();it != input_vec->end();it++)
	{
		if((*it)->Inchar != 0)
			delete (*it)->Inchar;
		free(*it);
	}

	delete input_vec;

	if(ret_vec != 0)
	{
		vector<CLBuffer*> ::iterator ite;
		while(ite = ret_vec->begin();ite != ret_vec->end();ite++)
		{
			delete (*ite);
		}
		
		delete ret_vec;
	}
}

uint32_t CLProtocolDecapsulator::getLeftSize()
{
	return m_NeededBufSize;
}

