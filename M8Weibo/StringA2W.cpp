#include "StringA2W.h"

CStringA2W::CStringA2W(char *pstr)
{
	m_pString = NULL;
	int size = MultiByteToWideChar(CP_ACP, 0, pstr, -1, NULL, 0)+1;
	wchar_t *buffer = new wchar_t[size];
	memset(buffer, 0, sizeof(wchar_t)*size);
	MultiByteToWideChar(CP_ACP, 0, pstr, -1, buffer, size);
	m_pString = buffer;
}

CStringA2W::~CStringA2W(void)
{
	delete []m_pString;
}

wchar_t *CStringA2W::ptr()
{
	return m_pString;
}
