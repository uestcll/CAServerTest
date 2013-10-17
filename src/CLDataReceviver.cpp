#include "CLDataReceviver.h"

CLDataReceviver::CLDataReceviver(uint32_t BufSize /* = BUFSIZE */)
{
	buf_size = BufSize;
	m_buf = 0;
	m_HasReadSize = 0xffffffff;
}

CLDataReceviver::~CLDataReceviver()
{
	
}

void CLDataReceviver::setContext(void* pContext)
{
	m_pContext = pContext;
}
uint32_t CLDataReceviver::getDataSize()
{
	return m_HasReadSize;
}
