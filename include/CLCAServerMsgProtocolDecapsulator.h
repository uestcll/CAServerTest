#ifndef CLCASERVERMSGPROTOCOLDECAPSULATOR_H
#define CLCASERVERMSGPROTOCOLDECAPSULATOR_H

#include "CLProtocolDecapsulator.h"

#include <stdint.h>
#include <vector>

class CLBuffer;

class CLCAServerMsgProtocolDecapsulator : public CLProtocolDecapsulator
{
public:
	CLCAServerMsgProtocolDecapsulator(uint8_t* InChar,uint32_t InSize,uint32_t IsDelete = CANDELETE);
	CLCAServerMsgProtocolDecapsulator();
	virtual ~CLCAServerMsgProtocolDecapsulator();

	virtual int ProtocolDecapsulate();
	virtual std::vector<CLBuffer*>* getDecapsulatorMsgChar() ;
	virtual int addCharToDecapsulate(uint8_t* Inchar,uint32_t InSize,uint32_t IsDelete = CANDELETE);
	virtual uint32_t getLeftSize();

private:
	void decapsualteSingle(uint8_t* buf,uint32_t size);
private:
	uint32_t m_startBlockIndex;//decapsulate first point index;
	uint32_t m_startIndexInBlock; //decapsulate first char in block
	CLBuffer* m_LeftBuffer;
	
};
#endif