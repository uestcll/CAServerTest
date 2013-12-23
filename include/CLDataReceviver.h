#ifndef CLDATARECEVIVER_H
#define CLDATARECEVIVER_H


#include <stdint.h>

const uint32_t BUFSIZE =  4*1024;

class CLDataReceviver
{
public:
	CLDataReceviver(uint32_t BufSize = BUFSIZE);
	virtual ~CLDataReceviver();

	virtual void setContext(void* pContext);
	virtual void* getData(uint32_t readSize) = 0;
	virtual void Initialize() = 0;
	virtual uint32_t getDataSize();
	virtual int geterrno();
	bool IsReceviveAll();

protected:
	uint8_t* m_buf;
	uint32_t buf_size;
	void* m_pContext;
	uint32_t m_HasReadSize;
	int m_errno;
	bool m_IsReceviveAll;
};
#endif