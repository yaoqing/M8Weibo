#pragma once
#include <mzfc_inc.h>

class CStringW2A
{
public:
	CStringW2A(wchar_t *pstr);
	~CStringW2A(void);
	char * ptr();

private:
	char *m_pString;
};
