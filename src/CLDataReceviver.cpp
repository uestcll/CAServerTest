#include "CLDataReceviver.h"

CLDataReceviver::CLDataReceviver(uint32_t BufSize /* = BUFSIZE */)
{
	buf_size = BufSize;
	m_buf = 0;
	m_HasReadSize = 0xffffffff;
	m_errno = -1;
	m_IsReceviveAll = true;
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

int CLDataReceviver::geterrno()
{
	return m_errno;
}

bool CLDataReceviver::IsReceviveAll()
{
	return m_IsReceviveAll;
}