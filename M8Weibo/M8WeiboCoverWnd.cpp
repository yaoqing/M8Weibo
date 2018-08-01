#include "M8WeiboCoverWnd.h"
#include "M8WeiboLoginWnd.h"
#include "M8WeiboApp.h"
#include "M8WeiboParameter.h"
#include "resource.h"
#include "StringW2A.h"
#include "StringA2W.h"

CM8WeiboCoverCanvas::CM8WeiboCoverCanvas(void)
{
	imgCover = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_COVER, true);
}

CM8WeiboCoverCanvas::~CM8WeiboCoverCanvas(void)
{
	m_image_container.RemoveAll();
}

void CM8WeiboCoverCanvas::PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate)
{
	imgCover->Draw(hdcDst, prcWin);
}

MZ_IMPLEMENT_DYNAMIC(CM8WeiboCoverWnd)

CM8WeiboCoverWnd::CM8WeiboCoverWnd(void)
{
}

CM8WeiboCoverWnd::~CM8WeiboCoverWnd(void)
{
}

BOOL CM8WeiboCoverWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
	{
		return FALSE;
	}

	RECT rcWork = MzGetWorkArea();
	cover.SetPos(0,0,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork));
	AddUiWin(&cover);

	int auto_login = theApp.m_parameter->getWeiboAutoLogin();
	SetTimer(m_hWnd, MZ_ID_COVER_TIMER, auto_login?0:1000, 0);
	return TRUE;
}

void CM8WeiboCoverWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == MZ_ID_COVER_TIMER)
	{
		KillTimer(m_hWnd, MZ_ID_COVER_TIMER);

		wchar_t *account = theApp.m_parameter->getFirstAccount();
		wchar_t *password = theApp.m_parameter->getWeiboPassword(CStringW2A(account).ptr());
		int savePass= theApp.m_parameter->getWeiboSavePass();
		int autoLogin = theApp.m_parameter->getWeiboAutoLogin();
		int cm_none = theApp.m_parameter->getWeiboCMNONE();
		int cm_net = theApp.m_parameter->getWeiboCMNET();
		int cm_wap = theApp.m_parameter->getWeiboCMWAP();
		int uni_net = theApp.m_parameter->getWeiboUNINET();
		int uni_wap = theApp.m_parameter->getWeiboUNIWAP();
		if (cm_none == 0 && cm_net == 0 && cm_wap == 0 && uni_net == 0 && uni_wap == 0)
		{
			cm_none = 1;
			theApp.m_parameter->setWeiboCMNONE(cm_none);
		}

		RECT rcWork = MzGetWorkArea();
		CM8WeiboLoginWnd *pLoginWnd = new CM8WeiboLoginWnd();
		pLoginWnd->m_conver = this;
		pLoginWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
		pLoginWnd->m_ctlAccount.SetText(account);
		pLoginWnd->m_ctlPassword.SetText(password);
		pLoginWnd->m_btnSavePass.SetState(savePass);
		pLoginWnd->m_btnAutoLogin.SetState(autoLogin);
		pLoginWnd->m_btnCMNONE.SetState(!cm_none);
		pLoginWnd->m_btnCMNET.SetState(!cm_net);
		pLoginWnd->m_btnCMWAP.SetState(!cm_wap);
		pLoginWnd->m_btnUNINET.SetState(!uni_net);
		pLoginWnd->m_btnUNIWAP.SetState(!uni_wap);
		int auto_login = theApp.m_parameter->getWeiboAutoLogin();
		if (!auto_login) pLoginWnd->Show();

		delete [] account;
		delete [] password;
	}
}

int CM8WeiboCoverWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	/*switch(message)
	 {
	 }*/
	 return SHK_RET_APPNOEXIT_SHELLTOP;
}