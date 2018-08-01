#pragma once

// Include MZFC header file
#include <mzfc_inc.h>

class CM8WeiboIniFile;
class CM8WeiboParameter
{
public:
	CM8WeiboParameter(void);
	~CM8WeiboParameter(void);

	void readConfig(CMzString modulFileName);
	void saveConfig();
	//wchar_t * getWeiboAccount(char * index);
	wchar_t * getWeiboPassword(char * index);
	//bool setWeiboAccount(char * index, wchar_t *value);
	bool setWeiboPassword(char * index, wchar_t *value);
	wchar_t * getWeiboName(char * index);
	bool setWeiboName(char * index, wchar_t *value);

	int getWeiboAutoLogin();
	int getWeiboSavePass();
	int getWeiboCMNONE();
	int getWeiboCMNET();
	int getWeiboCMWAP();
	int getWeiboUNINET();
	int getWeiboUNIWAP();
	bool setWeiboAutoLogin(int value);
	bool setWeiboSavePass(int value);
	bool setWeiboCMNONE(int value);
	bool setWeiboCMNET(int value);
	bool setWeiboCMWAP(int value);
	bool setWeiboUNINET(int value);
	bool setWeiboUNIWAP(int value);

	int getAutoDownloadSmallPicture();
	bool setAutoDownloadSmallPicture(int value);
	int getAutoDownloadMiddlePicture();
	bool setAutoDownloadMiddlePicture(int value);
	int getAutoDownloadProfileImage();
	bool setAutoDownloadProfileImage(int value);

	int getNoSplash();
	bool setNoSplash(int value);

	wchar_t * getAccountList();
	bool setAccountList(wchar_t * value);

	wchar_t * getFirstAccount();

	static wchar_t * A2W(char* str);
	static char * W2A(wchar_t* str);

private:
	CMzString m_confFileName;
	CM8WeiboIniFile * m_pIniFile;

	int getCommonConfNumber(char *key);
	wchar_t * getCommonConfString(char *key);
	bool setCommonConfString(char *key, wchar_t *value);
	bool setCommonConfNumber(char *key, int value);
};
