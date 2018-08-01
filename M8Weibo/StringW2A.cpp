#include "StringW2A.h"

CStringW2A::CStringW2A(wchar_t *pstr)
{
	m_pString = NULL;
	int size = WideCharToMultiByte(CP_ACP, 0, pstr, -1, NULL, 0, NULL, FALSE)+1;
	char *buffer = new char[size];
	memset(buffer, 0, sizeof(char)*size);
	WideCharToMultiByte(CP_ACP, 0, pstr, -1, buffer, size, NULL, FALSE);
	m_pString = buffer;
}

CStringW2A::~CStringW2A(void)
{
	delete []m_pString;
}

char *CStringW2A::ptr()
{
	return m_pString;
}
