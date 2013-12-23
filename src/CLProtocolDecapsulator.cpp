#include "CLProtocolDecapsulator.h"

#include <stdio.h>
#include <stdlib.h>

using namespace std;

CLProtocolDecapsulator::CLProtocolDecapsulator(uint8_t* InChar,uint32_t InSize,uint32_t IsDelete /*= CANDELETE */)
{
	m_NeededBufSize = 0xffffffff;
	ret_vec = new vector<CLBuffer*>;
	input_vec = new vector<PtlDcpstInput*>;
	PtlDcpstInput* input = (PtlDcpstInput*)malloc(sizeof(PtlDcpstInput));
	input->Inchar = InChar;
	input->InSize = InSize;
	input->EnableDelete = IsDelete;
	input_vec->push_back(input);

}

CLProtocolDecapsulator::CLProtocolDecapsulator()
{
	m_NeededBufSize = 0xffffffff;
	ret_vec = new vector<CLBuffer*>;
	input_vec = new vector<PtlDcpstInput*>;
}

CLProtocolDecapsulator::~CLProtocolDecapsulator()
{
	vector<PtlDcpstInput*> ::iterator it;
	PtlDcpstInput* input = 0;

	for(it = input_vec->begin();it != input_vec->end();it++)
	{
		input = (*it);

		if(input->EnableDelete == CANDELETE)
		{
			if(input->Inchar != 0)
				delete input->Inchar;
		}

		free(*it);
	}

	delete input_vec;

	CLBuffer* buffer = 0;

	if(ret_vec != 0)
	{
		vector<CLBuffer*> ::iterator ite;

		for(ite = ret_vec->begin();ite != ret_vec->end();ite++)
		{
			buffer = *ite;

			delete buffer;
		}
		
		delete ret_vec; 
	} 
}

uint32_t CLProtocolDecapsulator::getLeftSize()
{
	return m_NeededBufSize;
}

