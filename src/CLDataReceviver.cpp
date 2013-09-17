#include "CLDataReceviver.h"

CLDataReceviver::CLDataReceviver(CLProtocolDecapsulator* ptcDcps)
{
	m_PtcDcps = ptcDcps;
	isDeleted = false;
}

CLDataReceviver::CLDataReceviver()
{
	m_PtcDcps = new CLProtocolDecapsulator;
	isDeleted = true;
}

CLDataReceviver::~CLDataReceviver()
{
	if(isDeleted)
	delete m_PtcDcps;
}

void CLDataReceviver::setContext(void* pContext)
{
	m_pContext = pContext;
}

void CLDataReceviver::setProtocolDecapsulator(CLProtocolDecapsulator* ptcDcps)
{
	if(isDeleted)
		delete m_PtcDcps;
	m_PtcDcps = ptcDcps;
	isDeleted = false;
}