#pragma once
#include <mzfc_inc.h>


class CStringA2W
{
public:
	CStringA2W(char *pstr);
	~CStringA2W(void);
	wchar_t *ptr();
private:
	wchar_t *m_pString;
};
