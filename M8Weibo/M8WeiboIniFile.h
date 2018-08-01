#pragma once

// Include MZFC header file
#include <mzfc_inc.h>

struct tagIniLineStruct
{
	int valid;
	char *buffer;
	tagIniLineStruct *next;

	tagIniLineStruct()
	{
		memset(this, 0, sizeof(tagIniLineStruct));
	}
};

class CM8WeiboIniFile
{
public:
	CM8WeiboIniFile(wchar_t *pFileName);
	~CM8WeiboIniFile(void);

	void saveToFile();
	char *getKey(char *key);
	bool setKey(char *key, char *value);

private:
	wchar_t *m_pFileName;
	tagIniLineStruct * first;

	void TrimLRSpace(char * first, char * last);
	void TrimMidSpace(char * first, char * last);
};
