#ifndef CLPROTOCOLDECAPSULATOR_H
#define CLPROTOCOLDECAPSULATOR_H

#include "CLBuffer.h"

#include <stdint.h>
#include <vector>


typedef Buffer PtlDcpstInput;
/*struct PtlDcpstInput
{
	uint8_t* Inchar;
	uint32_t InSize;
	uint32_t EnableDelete; // >1 can delete, =1 has deleted,=0 cannot delete 
};

*/

//CLProtocolDecapsulator类 负责协议解析，且addCharToDecapsulate加入的Inchar将自动释放内存。

class CLProtocolDecapsulator
{

public:
	CLProtocolDecapsulator(uint8_t* InChar,uint32_t InSize,uint32_t IsDelete = CANDELETE);
	CLProtocolDecapsulator();
	virtual ~CLProtocolDecapsulator();

	//every CLBuffer return a msg buf
	virtual int ProtocolDecapsulate() = 0;
	virtual int addCharToDecapsulate(uint8_t* Inchar,uint32_t InSize,uint32_t IsDelete = CANDELETE) = 0;
	virtual uint32_t getLeftSize();
	virtual std::vector<CLBuffer*>* getDecapsulatorMsgChar() = 0;

protected:
	uint32_t m_NeededBufSize;
	std::vector<PtlDcpstInput*>* input_vec;
	std::vector<CLBuffer*>* ret_vec; //every Buffer return a msg buf

};
#endif