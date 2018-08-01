#pragma once

#include <mzfc_inc.h>

#define MZ_IDC_LOGIN_USER			101
#define MZ_IDC_LOGIN_PASSWD			102
#define MZ_IDC_LOGIN_TOOLBAR		103
#define MZ_IDC_LOGIN_SAVEPASS		104
#define MZ_IDC_LOGIN_AUTO			105
#define MZ_IDC_LOGIN_CMNONE			106
#define MZ_IDC_LOGIN_CMNET			107
#define MZ_IDC_LOGIN_CMWAP			108
#define MZ_IDC_LOGIN_UNINET			109
#define MZ_IDC_LOGIN_UNIWAP			110
#define MZ_IDC_LOGIN_NOSPLASH		111

#define MZ_ID_LOGIN_AUTHORIZE		199
#define MZ_ID_LOGIN_TIMER			200

class CM8WeiboCoverWnd;
class CM8WeiboLoginWnd : public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CM8WeiboLoginWnd);
public:
	CM8WeiboLoginWnd(void);
	~CM8WeiboLoginWnd(void);

	UiPicture m_Picture;
	UiSingleLineEdit m_ctlAccount;
	UiSingleLineEdit m_ctlPassword;
	UiToolBarPro m_toolbar;

	UiStatic m_staticSavePass;
	UiButton m_btnSavePass;
	UiStatic m_staticAutoLogin;
	UiButton m_btnAutoLogin;

	UiButton m_btnLine1;
//	UiButton m_btnLine2;

	UiButton m_btnCMNONE;
	UiStatic m_staticCMNONE;
	UiButton m_btnCMNET;
	UiStatic m_staticCMNET;
	UiButton m_btnCMWAP;
	UiStatic m_staticCMWAP;

	UiButton m_btnUNINET;
	UiStatic m_staticUNINET;
	UiButton m_btnUNIWAP;
	UiStatic m_staticUNIWAP;

	UiStatic m_staticNoSplash;
	UiButton m_btnNoSplash;

	CM8WeiboCoverWnd *m_conver;

protected:
	virtual BOOL OnInitDialog();
	virtual void OnTimer(UINT_PTR nIDEvent);
	virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);

};
