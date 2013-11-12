#ifndef CLSTATUS_H
#define CLSTATUS_H

class CLStatus
{
public:
	/*
	lReturnCode>=0��ʾ�ɹ�������ʧ��
	*/
	CLStatus(long lReturnCode, long lErrorCode);
	CLStatus(const CLStatus& status);
	virtual ~CLStatus();

public:
	bool IsSuccess();

public:
	const long& m_clReturnCode;
	const long& m_clErrorCode;

private:
	long m_lReturnCode;
	long m_lErrorCode;
};

#endif