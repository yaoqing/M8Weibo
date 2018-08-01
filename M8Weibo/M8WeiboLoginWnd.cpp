#include "M8WeiboLoginWnd.h"
#include "M8WeiboHomeWnd.h"
#include "M8WeiboCoverWnd.h"
#include "M8WeiboApp.h"
#include "M8WeiboParameter.h"
#include "M8WeiboNetworkEnv.h"
#include "M8Weibo.h"
#include "StringW2A.h"
#include "StringA2W.h"

MZ_IMPLEMENT_DYNAMIC(CM8WeiboLoginWnd)

CM8WeiboLoginWnd::CM8WeiboLoginWnd(void)
{
	m_conver = NULL;
}

CM8WeiboLoginWnd::~CM8WeiboLoginWnd(void)
{
	m_conver = NULL;
}

BOOL CM8WeiboLoginWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
	{
		return FALSE;
	}

	CMzString str1 = theApp.GetModuleFileName(NULL);
	int pos = str1.FindReverse('\\');
	CMzString str2 = str1.SubStr(0, pos+1);
	str2 += L"background.jpg";
	m_Picture.SetPos(0,0,GetWidth(),GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR-3);
	m_Picture.SetPaintMode(MZ_PAINTMODE_NORMAL);
	m_Picture.LoadImage(str2, true);
	m_Picture.SetPassDown();

	m_ctlAccount.SetPos(40,120,GetWidth()-40*2,64);
	m_ctlAccount.SetID(MZ_IDC_LOGIN_USER);	
	m_ctlAccount.SetTip2(L"帐号：");
	m_ctlAccount.SetLeftInvalid(100);
	m_ctlAccount.SetSipMode(IM_SIP_MODE_MAIL_LETTER);

	m_ctlPassword.SetPos(40,200,GetWidth()-40*2,64);
	m_ctlPassword.SetID(MZ_IDC_LOGIN_PASSWD);
	m_ctlPassword.SetTip2(L"密码：");
	m_ctlPassword.SetLeftInvalid(100);
	m_ctlPassword.SetSipMode(IM_SIP_MODE_GEL_LETTER);
	m_ctlPassword.EnablePassWord();

	m_btnSavePass.SetPos(60,280,50,64);
	m_btnSavePass.SetID(MZ_IDC_LOGIN_SAVEPASS);
	m_btnSavePass.SetButtonType(MZCV2_CHECKBOX_SELECT);
	m_btnSavePass.SetButtonMode(MZC_BUTTON_MODE_HOLD);

	m_btnAutoLogin.SetPos(260,280,50,64);
	m_btnAutoLogin.SetID(MZ_IDC_LOGIN_AUTO);
	m_btnAutoLogin.SetButtonType(MZCV2_CHECKBOX_SELECT);
	m_btnAutoLogin.SetButtonMode(MZC_BUTTON_MODE_HOLD);

	m_staticSavePass.SetPos(100,280,120,64);
	m_staticSavePass.SetText(L"记住密码");

	m_staticAutoLogin.SetPos(300,280,120,64);
	m_staticAutoLogin.SetText(L"自动登录");

	m_btnLine1.SetPos(40,350,400,5);
	//m_btnLine.SetID(MZ_IDC_LOGIN_CMNONE);
	m_btnLine1.SetButtonType(MZC_BUTTON_LINE_BOTTOM);
	m_btnLine1.SetButtonMode(MZC_BUTTON_MODE_HOLD);

	//m_btnLine2.SetPos(40,490,400,5);
	//m_btnLine.SetID(MZ_IDC_LOGIN_CMNONE);
	//m_btnLine2.SetButtonType(MZC_BUTTON_LINE_BOTTOM);
	//m_btnLine2.SetButtonMode(MZC_BUTTON_MODE_HOLD);

	m_btnCMNONE.SetPos(60,360,50,64);
	m_btnCMNONE.SetID(MZ_IDC_LOGIN_CMNONE);
	m_btnCMNONE.SetButtonType(MZCV2_RADIOBOX_SELECT);
	//m_btnCMNONE.SetButtonMode(MZC_BUTTON_MODE_HOLD);

	m_staticCMNONE.SetPos(100,360,150,64);
	m_staticCMNONE.SetText(L"已接入网络");

	m_btnCMNET.SetPos(60,420,50,64);
	m_btnCMNET.SetID(MZ_IDC_LOGIN_CMNET);
	m_btnCMNET.SetButtonType(MZCV2_RADIOBOX_SELECT);
	//m_btnCMNET.SetButtonMode(MZC_BUTTON_MODE_HOLD);

	m_staticCMNET.SetPos(100,420,120,64);
	m_staticCMNET.SetText(L"CMNET");

	m_btnCMWAP.SetPos(260,420,50,64);
	m_btnCMWAP.SetID(MZ_IDC_LOGIN_CMWAP);
	m_btnCMWAP.SetButtonType(MZCV2_RADIOBOX_SELECT);
	//m_btnCMWAP.SetButtonMode(MZC_BUTTON_MODE_HOLD);

	m_staticCMWAP.SetPos(300,420,120,64);
	m_staticCMWAP.SetText(L"CMWAP");

	m_btnUNINET.SetPos(60,480,50,64);
	m_btnUNINET.SetID(MZ_IDC_LOGIN_UNINET);
	m_btnUNINET.SetButtonType(MZCV2_RADIOBOX_SELECT);
	//m_btnUNINET.SetButtonMode(MZC_BUTTON_MODE_HOLD);

	m_staticUNINET.SetPos(100,480,120,64);
	m_staticUNINET.SetText(L"UNINET");

	m_btnUNIWAP.SetPos(260,480,50,64);
	m_btnUNIWAP.SetID(MZ_IDC_LOGIN_UNIWAP);
	m_btnUNIWAP.SetButtonType(MZCV2_RADIOBOX_SELECT);
	//m_btnUNIWAP.SetButtonMode(MZC_BUTTON_MODE_HOLD);

	m_staticUNIWAP.SetPos(300,480,120,64);
	m_staticUNIWAP.SetText(L"UNIWAP");

	m_btnNoSplash.SetPos(60,540,50,64);
	m_btnNoSplash.SetID(MZ_IDC_LOGIN_NOSPLASH);
	m_btnNoSplash.SetButtonType(MZCV2_CHECKBOX_SELECT);
	m_btnNoSplash.SetButtonMode(MZC_BUTTON_MODE_HOLD);
	m_btnNoSplash.SetState(1);
	theApp.m_parameter->setNoSplash(1);
	theApp.m_parameter->saveConfig();

	m_staticNoSplash.SetPos(100,540,220,64);
	m_staticNoSplash.SetText(L"不再显示应用介绍");

	m_toolbar.SetID(MZ_IDC_LOGIN_TOOLBAR);
	m_toolbar.SetPos(0,GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR-3,GetWidth(),MZM_HEIGHT_TEXT_TOOLBAR+3);
	m_toolbar.SetButton(TOOLBARPRO_LEFT_TEXTBUTTON, TRUE, TRUE, L"登录");
	m_toolbar.SetButton(TOOLBARPRO_RIGHT_TEXTBUTTON, TRUE, TRUE, L"退出");

	AddUiWin(&m_Picture);
	AddUiWin(&m_ctlAccount);
	AddUiWin(&m_ctlPassword);
	AddUiWin(&m_toolbar);
	AddUiWin(&m_btnSavePass);
	AddUiWin(&m_btnAutoLogin);
	AddUiWin(&m_staticSavePass);
	AddUiWin(&m_staticAutoLogin);
	AddUiWin(&m_btnLine1);
	AddUiWin(&m_btnCMNONE);
	AddUiWin(&m_btnCMNET);
	AddUiWin(&m_btnCMWAP);
	AddUiWin(&m_btnUNINET);
	AddUiWin(&m_btnUNIWAP);
	AddUiWin(&m_staticCMNONE);
	AddUiWin(&m_staticCMNET);
	AddUiWin(&m_staticCMWAP);
	AddUiWin(&m_staticUNINET);
	AddUiWin(&m_staticUNIWAP);
	AddUiWin(&m_btnNoSplash);
	AddUiWin(&m_staticNoSplash);
	//AddUiWin(&m_btnLine2);

	int auto_login = theApp.m_parameter->getWeiboAutoLogin();
	SetTimer(m_hWnd, auto_login?MZ_ID_LOGIN_TIMER:MZ_ID_LOGIN_AUTHORIZE, 0, NULL);
	
	return TRUE;
}

void CM8WeiboLoginWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == MZ_ID_LOGIN_TIMER)
	{
		KillTimer(m_hWnd, MZ_ID_LOGIN_TIMER);
		PostMessage(MZ_WM_COMMAND,  MZ_IDC_LOGIN_TOOLBAR, TOOLBARPRO_LEFT_TEXTBUTTON);
	}
	else if (nIDEvent == MZ_ID_LOGIN_AUTHORIZE)
	{
		KillTimer(m_hWnd, MZ_ID_LOGIN_AUTHORIZE);
		if (MzMessageBox(m_hWnd, L"我信任\"M8围脖\"应用程序并授予此应用我的新浪微博帐号操作权限", L"新浪微博帐号授权协议", MB_OKCANCEL) == IDCANCEL)
		{
			PostQuitMessage(0);
		}
	}
}

void CM8WeiboLoginWnd::OnMzCommand(WPARAM wParam, LPARAM lParam)
{
	UINT_PTR id = LOWORD(wParam);
	switch(id)
	{
	case MZ_IDC_LOGIN_TOOLBAR:
		{
			if (MzIsSipOpen()) MzCloseSip();

			if (lParam==TOOLBARPRO_LEFT_TEXTBUTTON)
			{
				if (!m_btnCMNET.GetState())
				{
					if (!theApp.m_net_env.start(m_hWnd, CM8WeiboNetworkEnv::NE_CMNET))
					{
						if (theApp.m_parameter->getWeiboAutoLogin()) Show();
						MzMessageBox(m_hWnd, L"CMNET拨号失败，请检查网络参数设置！", NULL, MB_OK);
						return;
					}
				}
				else if (!m_btnCMWAP.GetState())
				{
					if (!theApp.m_net_env.start(m_hWnd, CM8WeiboNetworkEnv::NE_CMWAP))
					{
						if (theApp.m_parameter->getWeiboAutoLogin()) Show();
						MzMessageBox(m_hWnd, L"CMWAP拨号失败，请检查网络参数设置！", NULL, MB_OK);
						return;
					}
				}
				else if (!m_btnUNINET.GetState())
				{
					if (!theApp.m_net_env.start(m_hWnd, CM8WeiboNetworkEnv::NE_UNINET))
					{
						if (theApp.m_parameter->getWeiboAutoLogin()) Show();
						MzMessageBox(m_hWnd, L"UNINET拨号失败，请检查网络参数设置！", NULL, MB_OK);
						return;
					}
				}
				else if (!m_btnUNIWAP.GetState())
				{
					if (!theApp.m_net_env.start(m_hWnd, CM8WeiboNetworkEnv::NE_UNIWAP))
					{
						if (theApp.m_parameter->getWeiboAutoLogin()) Show();
						MzMessageBox(m_hWnd, L"UNIWAP拨号失败，请检查网络参数设置！", NULL, MB_OK);
						return;
					}
				}
				else
				{
					theApp.m_net_env.stop(m_hWnd);
				}

				CMzString account = m_ctlAccount.GetText();
				CMzString password = m_ctlPassword.GetText();

				if (account.IsEmpty())
				{
					if (theApp.m_parameter->getWeiboAutoLogin()) Show();
					MzMessageBox(m_hWnd, L"帐号不能为空！", NULL, MB_OK);
					return;
				}

				if (password.IsEmpty())
				{
					if (theApp.m_parameter->getWeiboAutoLogin()) Show();
					MzMessageBox(m_hWnd, L"密码不能为空！", NULL, MB_OK);
					return;
				}

				if (!theApp.LoginWeibo(m_hWnd, account, password))
				{
					if (theApp.m_parameter->getWeiboAutoLogin()) Show();
					MzMessageBox(m_hWnd, L"登录失败，请检查账户及密码！", NULL, MB_OK);
					return;
				}

				if (!theApp.RefreshUserInfoSelf(m_hWnd))
				{
					if (theApp.m_parameter->getWeiboAutoLogin()) Show();
					MzMessageBox(m_hWnd, L"获取用户信息失败！", NULL, MB_OK);
					return;
				}

				// 保存帐号密码及名字
				theApp.m_parameter->setAccountList(account.C_Str());
				if (theApp.m_parameter->getWeiboSavePass()) theApp.m_parameter->setWeiboPassword(CStringW2A(account.C_Str()).ptr(), password.C_Str());
				theApp.m_parameter->setWeiboName(CStringW2A(account.C_Str()).ptr(), CStringA2W(theApp.GetUserInfoSelf()->screen_name_).ptr());
				theApp.m_parameter->saveConfig();

				// 切换Home窗口
				RECT rcWork = MzGetWorkArea();
				CM8WeiboHomeWnd *pHomeWnd = new CM8WeiboHomeWnd();
				theApp.m_stack.push(pHomeWnd);
				pHomeWnd->m_user = theApp.GetUserInfoSelf();
				pHomeWnd->m_user_type = CM8WeiboHomeWnd::ALL_FRIENDS;
				pHomeWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pHomeWnd->Show();
				m_conver->DestroyWindow();
				delete m_conver;
				DestroyWindow();
				delete this;
			}
			else if (lParam==TOOLBARPRO_RIGHT_TEXTBUTTON)
			{
				PostQuitMessage(0);
			}
		}
		break;
	case MZ_IDC_LOGIN_SAVEPASS:
		{
			DWORD state = m_btnSavePass.GetState();
			theApp.m_parameter->setWeiboSavePass(state);
			theApp.m_parameter->saveConfig();
		}
		break;
	case MZ_IDC_LOGIN_AUTO:
		{
			DWORD state = m_btnAutoLogin.GetState();
			theApp.m_parameter->setWeiboAutoLogin(state);
			theApp.m_parameter->saveConfig();
		}
		break;
	case MZ_IDC_LOGIN_CMNONE:
		{
			DWORD state = !m_btnCMNONE.GetState();
			theApp.m_parameter->setWeiboCMNONE(state);

			m_btnCMNET.SetState(state);
			theApp.m_parameter->setWeiboCMNET(!state);
			m_btnCMWAP.SetState(state);
			theApp.m_parameter->setWeiboCMWAP(!state);
			m_btnUNINET.SetState(state);
			theApp.m_parameter->setWeiboUNINET(!state);
			m_btnUNIWAP.SetState(state);
			theApp.m_parameter->setWeiboUNIWAP(!state);
			theApp.m_parameter->saveConfig();

			Invalidate();
			UpdateWindow();
		}
		break;
	case MZ_IDC_LOGIN_CMNET:
		{
			DWORD state = !m_btnCMNET.GetState();
			theApp.m_parameter->setWeiboCMNET(state);

			m_btnCMNONE.SetState(state);
			theApp.m_parameter->setWeiboCMNONE(!state);
			m_btnCMWAP.SetState(state);
			theApp.m_parameter->setWeiboCMWAP(!state);
			m_btnUNINET.SetState(state);
			theApp.m_parameter->setWeiboUNINET(!state);
			m_btnUNIWAP.SetState(state);
			theApp.m_parameter->setWeiboUNIWAP(!state);
			theApp.m_parameter->saveConfig();

			Invalidate();
			UpdateWindow();
		}
		break;
	case MZ_IDC_LOGIN_CMWAP:
		{
			DWORD state = !m_btnCMWAP.GetState();
			theApp.m_parameter->setWeiboCMWAP(state);

			m_btnCMNONE.SetState(state);
			theApp.m_parameter->setWeiboCMNONE(!state);
			m_btnCMNET.SetState(state);
			theApp.m_parameter->setWeiboCMNET(!state);
			m_btnUNINET.SetState(state);
			theApp.m_parameter->setWeiboUNINET(!state);
			m_btnUNIWAP.SetState(state);
			theApp.m_parameter->setWeiboUNIWAP(!state);
			theApp.m_parameter->saveConfig();

			Invalidate();
			UpdateWindow();
		}
		break;
	case MZ_IDC_LOGIN_UNINET:
		{
			DWORD state = !m_btnUNINET.GetState();
			theApp.m_parameter->setWeiboUNINET(state);

			m_btnCMNONE.SetState(state);
			theApp.m_parameter->setWeiboCMNONE(!state);
			m_btnCMNET.SetState(state);
			theApp.m_parameter->setWeiboCMNET(!state);
			m_btnCMWAP.SetState(state);
			theApp.m_parameter->setWeiboCMWAP(!state);
			m_btnUNIWAP.SetState(state);
			theApp.m_parameter->setWeiboUNIWAP(!state);
			theApp.m_parameter->saveConfig();

			Invalidate();
			UpdateWindow();
		}
		break;
	case MZ_IDC_LOGIN_UNIWAP:
		{
			DWORD state = !m_btnUNIWAP.GetState();
			theApp.m_parameter->setWeiboUNIWAP(state);

			m_btnCMNONE.SetState(state);
			theApp.m_parameter->setWeiboCMNONE(!state);
			m_btnCMNET.SetState(state);
			theApp.m_parameter->setWeiboCMNET(!state);
			m_btnCMWAP.SetState(state);
			theApp.m_parameter->setWeiboCMWAP(!state);
			m_btnUNINET.SetState(state);
			theApp.m_parameter->setWeiboUNINET(!state);
			theApp.m_parameter->saveConfig();

			Invalidate();
			UpdateWindow();
		}
		break;
	case MZ_IDC_LOGIN_NOSPLASH:
		{
			theApp.m_parameter->setNoSplash(m_btnNoSplash.GetState());
			theApp.m_parameter->saveConfig();
		}
		break;
	}
}

int CM8WeiboLoginWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	 /*switch(message)
	 {
	 }*/
	 return SHK_RET_APPNOEXIT_SHELLTOP;
}
