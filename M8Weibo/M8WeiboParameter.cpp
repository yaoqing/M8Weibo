#include "M8WeiboParameter.h"
#include "M8WeiboIniFile.h"

CM8WeiboParameter::CM8WeiboParameter(void)
{
	m_pIniFile = NULL;
}

CM8WeiboParameter::~CM8WeiboParameter(void)
{
	delete m_pIniFile;
}

void CM8WeiboParameter::readConfig(CMzString modulFileName)
{
	m_confFileName = modulFileName.SubStr(0, modulFileName.Length()-3);
	m_confFileName += L"ini";
	m_pIniFile = new CM8WeiboIniFile(m_confFileName.C_Str());
}

void CM8WeiboParameter::saveConfig()
{
	m_pIniFile->saveToFile();
}

wchar_t * CM8WeiboParameter::A2W(char* str)
{
	int size = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0)+1;
	wchar_t *buffer = new wchar_t[size];
	memset(buffer, 0, sizeof(wchar_t)*size);
	MultiByteToWideChar(CP_ACP, 0, str, -1, buffer, size);
	return buffer;
}

char * CM8WeiboParameter::W2A(wchar_t* str)
{
	int size = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, FALSE)+1;
	char *buffer = new char[size];
	memset(buffer, 0, sizeof(char)*size);
	WideCharToMultiByte(CP_ACP, 0, str, -1, buffer, size, NULL, FALSE);
	return buffer;
}

int CM8WeiboParameter::getCommonConfNumber(char *key)
{
	char *buffer = m_pIniFile->getKey(key);
	if (buffer == NULL) return 0;
	int retval = atoi(buffer);
	delete [] buffer;
	return retval;
}

wchar_t * CM8WeiboParameter::getCommonConfString(char *key)
{
	char *value = NULL;
	if ((value = m_pIniFile->getKey(key)) == NULL)
		return NULL;
	wchar_t * buffer = A2W(value);
	delete [] value;
	return buffer;
}

bool CM8WeiboParameter::setCommonConfNumber(char *key, int value)
{
	wchar_t buffer[16];
	swprintf(buffer, L"%d", value);
	return setCommonConfString(key, buffer);
}

bool CM8WeiboParameter::setCommonConfString(char *key, wchar_t *value)
{
	char *buffer = W2A(value);
	m_pIniFile->setKey(key, buffer);
	delete [] buffer;
	return true;
}

/*wchar_t * CM8WeiboParameter::getWeiboAccount(char * index)
{
	char account[128];
	sprintf(account, "Account.%s", index);
	return getCommonConfString(account);
}*/

wchar_t * CM8WeiboParameter::getWeiboPassword(char * index)
{
	char password[128];
	sprintf(password, "Password.%s", index);
	return getCommonConfString(password);
}

/*bool CM8WeiboParameter::setWeiboAccount(char * index, wchar_t *value)
{
	char account[128];
	sprintf(account, "Account.%s", index);
	return setCommonConfString(account, value);
}*/

bool CM8WeiboParameter::setWeiboPassword(char * index, wchar_t *value)
{
	char password[128];
	sprintf(password, "Password.%s", index);
	return setCommonConfString(password, value);
}

wchar_t * CM8WeiboParameter::getWeiboName(char * index)
{
	char Name[128];
	sprintf(Name, "Name.%s", index);
	return getCommonConfString(Name);
}

bool CM8WeiboParameter::setWeiboName(char * index, wchar_t *value)
{
	char Name[128];
	sprintf(Name, "Name.%s", index);
	return setCommonConfString(Name, value);
}

int CM8WeiboParameter::getWeiboAutoLogin()
{
	return getCommonConfNumber("AutoLogin");
}

int CM8WeiboParameter::getWeiboSavePass()
{
	return getCommonConfNumber("SavePass");
}

int CM8WeiboParameter::getWeiboCMNONE()
{
	return getCommonConfNumber("CMNONE");
}

int CM8WeiboParameter::getWeiboCMNET()
{
	return getCommonConfNumber("CMNET");
}

int CM8WeiboParameter::getWeiboCMWAP()
{
	return getCommonConfNumber("CMWAP");
}

int CM8WeiboParameter::getWeiboUNINET()
{
	return getCommonConfNumber("UNINET");
}

int CM8WeiboParameter::getWeiboUNIWAP()
{
	return getCommonConfNumber("UNIWAP");
}

bool CM8WeiboParameter::setWeiboAutoLogin(int value)
{
	return setCommonConfNumber("AutoLogin", value);
}

bool CM8WeiboParameter::setWeiboSavePass(int value)
{
	return setCommonConfNumber("SavePass", value);
}

bool CM8WeiboParameter::setWeiboCMNONE(int value)
{
	return setCommonConfNumber("CMNONE", value);
}

bool CM8WeiboParameter::setWeiboCMNET(int value)
{
	return setCommonConfNumber("CMNET", value);
}

bool CM8WeiboParameter::setWeiboCMWAP(int value)
{
	return setCommonConfNumber("CMWAP", value);
}

bool CM8WeiboParameter::setWeiboUNINET(int value)
{
	return setCommonConfNumber("UNINET", value);
}

bool CM8WeiboParameter::setWeiboUNIWAP(int value)
{
	return setCommonConfNumber("UNIWAP", value);
}

int CM8WeiboParameter::getAutoDownloadSmallPicture()
{
	return getCommonConfNumber("AutoDownloadSmallPicture");
}

bool CM8WeiboParameter::setAutoDownloadSmallPicture(int value)
{
	return setCommonConfNumber("AutoDownloadSmallPicture", value);
}

int CM8WeiboParameter::getAutoDownloadMiddlePicture()
{
	return getCommonConfNumber("AutoDownloadMiddlePicture");
}

bool CM8WeiboParameter::setAutoDownloadMiddlePicture(int value)
{
	return setCommonConfNumber("AutoDownloadMiddlePicture", value);
}

int CM8WeiboParameter::getAutoDownloadProfileImage()
{
	return getCommonConfNumber("AutoDownloadProfileImage");
}

bool CM8WeiboParameter::setAutoDownloadProfileImage(int value)
{
	return setCommonConfNumber("AutoDownloadProfileImage", value);
}

int CM8WeiboParameter::getNoSplash()
{
	return getCommonConfNumber("NoSplash");
}

bool CM8WeiboParameter::setNoSplash(int value)
{
	return setCommonConfNumber("NoSplash", value);
}

wchar_t * CM8WeiboParameter::getAccountList()
{
	return getCommonConfString("AccountList");
}

bool CM8WeiboParameter::setAccountList(wchar_t * value)
{
	CMzString strAccounts;
	wchar_t * account_list = getCommonConfString("AccountList");
	if (*account_list == 0)
	{
		strAccounts = value;
	}
	else
	{
		wchar_t * account = wcsstr(account_list, value);
		if (account)
		{
			strAccounts = value;
			wchar_t * temp = new wchar_t[wcslen(account_list)+1];
			wchar_t * prev, *next;
			prev = next = account_list;
			while ((next = wcschr(prev, L',')) != NULL)
			{
				wmemcpy(temp, prev, next-prev);
				temp[next-prev] = 0;
				prev = next+1;
				if (*temp && wcscmp(temp, value))
				{
					strAccounts += L",";
					strAccounts += temp;
				}
			}
			if (prev)
			{
				if (*prev && wcscmp(prev, value))
				{
					strAccounts += L",";
					strAccounts += prev;
				}
			}
			delete [] temp;
		}
		else
		{
			strAccounts = value;
			strAccounts += L",";
			strAccounts += account_list;
		}
	}

	return setCommonConfString("AccountList", strAccounts.C_Str());
}

wchar_t * CM8WeiboParameter::getFirstAccount()
{
	wchar_t * account_list = getCommonConfString("AccountList");
	if (account_list == NULL)
	{
		setCommonConfString("AccountList", L"");
		account_list = new wchar_t[1];
		account_list[0] = 0;
	}

	wchar_t * account = wcschr(account_list, L',');
	if (account == NULL)
	{
		return account_list;
	}
	else
	{
		wchar_t * temp = new wchar_t[account-account_list+1];
		wmemcpy(temp, account_list, account-account_list);
		temp[account-account_list] = 0;
		delete [] account_list;
		return temp;
	}
}