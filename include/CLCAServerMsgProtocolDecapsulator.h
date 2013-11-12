#ifndef CLCASERVERMSGPROTOCOLDECAPSULATOR_H
#define CLCASERVERMSGPROTOCOLDECAPSULATOR_H

class CLProtocolDecapsulator;

class CLCAServerMsgProtocolDecapsulator : public CLProtocolDecapsulator
{
public:
	CLCAServerMsgProtocolDecapsulator(uint8_t* InChar,uint32_t InSize);
	CLCAServerMsgProtocolDecapsulator();
	virtual ~CLCAServerMsgProtocolDecapsulator();

	virtual void ProtocolDecapsulate();
	virtual std::vector<CLBuffer*>* getDecapsulatorMsgChar() ;
	virtual void addCharToDecapsulate(uint8_t* Inchar,uint32_t InSize);

private:
	void decapsualteSingle(uint8_t* buf,uint32_t size);
private:
	uint32_t startBlockIndex;//decapsulate first point index;
	uint32_t startIndexInBlock; //decapsulate first char in block
	CLBuffer* LeftBuffer;
	
};
#endif