#ifndef CLPROTOCOLDECAPSULATOR_H
#define CLPROTOCOLDECAPSULATOR_H

#include <stdint.h>
#include <vector>
#include <iostream>

class CLBuffer;
typedef Buffer PtlDcpstInput;
/*struct PtlDcpstInput
{
	uint8_t* Inchar;
	uint32_t InSize;
	uint32_t EnableDelete; // >1 can delete, =1 has deleted,=0 cannot delete 
};

*/
class CLProtocolDecapsulator
{
public:
	CLProtocolDecapsulator(uint8_t* InChar,uint32_t InSize);
	CLProtocolDecapsulator();
	virtual ~CLProtocolDecapsulator();

	virtual void ProtocolDecapsulate() = 0;
	virtual void addCharToDecapsulate(uint8_t* Inchar,uint32_t InSize) = 0;
	virtual uint32_t getLeftSize();
	virtual std::vector<CLBuffer*>* getDecapsulatorMsgChar() = 0;


protected:
	uint32_t m_NeededBufSize;
	std::vector<PtlDcpstInput*>* input_vec;
	std::vector<CLBuffer*>* ret_vec; //every Buffer return a msg buf

};
#endif