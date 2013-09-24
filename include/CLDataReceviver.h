#ifndef CLDATARECEVIVER_H
#define CLDATARECEVIVER_H

#include "CLProtocolDecapsulator.h"

class CLDataReceviver
{
public:
	CLDataReceviver(CLProtocolDecapsulator* ptcDcps);
	CLDataReceviver();
	virtual ~CLDataReceviver();

	void setProtocolDecapsulator(CLProtocolDecapsulator* ptcDcps);
	virtual void setContext(void* pContext);
	virtual void* getData() = 0;
	virtual void Initialize() = 0;

protected:
	CLProtocolDecapsulator* m_PtcDcps;
	bool isDeleted;
	void* m_pContext;
};
#endif