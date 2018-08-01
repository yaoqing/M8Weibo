#include "M8WeiboFinderWnd.h"
#include "M8WeiboApp.h"
#include "M8WeiboHomeWnd.h"
#include "M8WeiboUserListWnd.h"
#include "StringW2A.h"
#include "StringA2W.h"

MZ_IMPLEMENT_DYNAMIC(CM8WeiboFinderWnd)

CM8WeiboFinderWnd::CM8WeiboFinderWnd(void)
{
}

CM8WeiboFinderWnd::~CM8WeiboFinderWnd(void)
{
}

BOOL CM8WeiboFinderWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
	{
		return FALSE;
	}

	int width = GetWidth();

	m_ctlCaption.SetID(MZ_IDC_FINDER_CAPTION);
	m_ctlCaption.SetPos(0, 0, width, MZM_HEIGHT_TEXT_TOOLBAR);
	m_ctlCaption.SetTextColor(RGB(89,89,89));
	m_ctlCaption.SetTextSize(30);
	m_ctlCaption.SetTextWeight(FW_MEDIUM);
	m_ctlCaption.SetText(L"微博搜索");
	AddUiWin(&m_ctlCaption);

	//m_btnCancel.SetID(MZ_IDC_FINDER_CANCEL);
	//m_btnCancel.SetPos(20, 15, 100, 40);
	//m_btnCancel.SetButtonType(MZC_BUTTON_ROUND_WHITE);
	//m_btnCancel.SetText(L"取消");
	//m_btnCancel.SetTextColor(0X59BB9B);
	//m_ctlCaption.AddChild(&m_btnCancel);

	//m_btnSend.SetID(MZ_IDC_FINDER_SEND);
	//m_btnSend.SetPos(w-20-100, 15, 100, 40);
	//m_btnSend.SetButtonType(MZC_BUTTON_ROUND_WHITE);
	//m_btnSend.SetText(L"发送");
	//m_btnSend.SetTextColor(0X59BB9B);
	//m_ctlCaption.AddChild(&m_btnSend);

	int height_of_edit = 64;
	m_ctlEdit.SetID(MZ_IDC_FINDER_EDIT);
	m_ctlEdit.SetPos(3, MZM_HEIGHT_TEXT_TOOLBAR+3, width-3*2, height_of_edit);
	m_ctlEdit.SetTip2(L"昵称：");
	m_ctlEdit.SetLeftInvalid(100);
	m_ctlEdit.SetSipMode(IM_SIP_MODE_GEL_PY);
	AddUiWin(&m_ctlEdit);
/*
	m_btnSearch.SetID(MZ_IDC_FINDER_SEARCH);
	m_btnSearch.SetPos(0,MZM_HEIGHT_TEXT_TOOLBAR+height_of_edit, 50, 64);
	m_btnSearch.SetButtonType(MZCV2_CHECKBOX_SELECT);
	m_btnSearch.SetButtonMode(MZC_BUTTON_MODE_HOLD);
	AddUiWin(&m_btnSearch);
*/
	m_btnUser.SetPos(20,132,50,64);
	m_btnUser.SetID(MZ_IDC_FINDER_USER);
	m_btnUser.SetButtonType(MZCV2_RADIOBOX_SELECT);
	//m_btnCMNET.SetButtonMode(MZC_BUTTON_MODE_HOLD);
	m_btnWeibo.SetState(0);
	AddUiWin(&m_btnUser);

	m_staticUser.SetPos(70,132,60,64);
	m_staticUser.SetText(L"搜人");
	AddUiWin(&m_staticUser);

	m_btnWeibo.SetPos(180,132,50,64);
	m_btnWeibo.SetID(MZ_IDC_FINDER_WEIBO);
	m_btnWeibo.SetButtonType(MZCV2_RADIOBOX_SELECT);
	//m_btnCMWAP.SetButtonMode(MZC_BUTTON_MODE_HOLD);
	m_btnWeibo.SetState(1);
	AddUiWin(&m_btnWeibo);

	m_staticWeibo.SetPos(230,132,90,64);
	m_staticWeibo.SetText(L"搜微博");
	AddUiWin(&m_staticWeibo);

	m_toolbar.SetID(MZ_IDC_FINDER_TOOLBAR);
	m_toolbar.SetPos(0,GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR-3,GetWidth(),MZM_HEIGHT_TEXT_TOOLBAR+3);
	m_toolbar.SetButton(TOOLBARPRO_LEFT_TEXTBUTTON, TRUE, TRUE, L"取消");
	m_toolbar.SetButton(TOOLBARPRO_RIGHT_TEXTBUTTON, TRUE, TRUE, L"搜索");
	AddUiWin(&m_toolbar);

	m_scrool_win.SetID(MZ_IDC_FINDER_SCROOLWIN);
	m_scrool_win.SetPos(0, 196, width, GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR-MZM_HEIGHT_ICON_TOOLBAR-height_of_edit-64);
	m_scrool_win.EnableScrollBarV(true);
	AddUiWin(&m_scrool_win);

	m_ctlEdit.SetFocus(true);
	MzOpenSip();

	return TRUE;
}

void CM8WeiboFinderWnd::OnMzCommand( WPARAM wParam, LPARAM lParam )
{
	UINT_PTR id = LOWORD(wParam);
	switch(id)
	{
	case MZ_IDC_FINDER_TOOLBAR:
		{
			int nIndex = lParam;
			if (nIndex==TOOLBARPRO_LEFT_TEXTBUTTON)
			{
				if (MzIsSipOpen()) MzCloseSip();
				DestroyWindow();
				delete this;
				theApp.PopWnd(this);
			}
			else if (nIndex==TOOLBARPRO_RIGHT_TEXTBUTTON)
			{
				//MzMessageBox(m_hWnd, L"应用没有搜索权限", NULL, MB_OK);
				//return;
				CMzString szText = m_ctlEdit.GetText();
				if (szText.Length() == 0) return;
				if (MzIsSipOpen()) MzCloseSip();
				RECT rcWork = MzGetWorkArea();
				if (!m_btnUser.GetState())
				{
					CM8WeiboUserListWnd *pUserListWnd = new CM8WeiboUserListWnd();
					pUserListWnd->m_user = theApp.GetUserInfoSelf();
					pUserListWnd->m_user_type = CM8WeiboUserListWnd::USER_SEARCH;
					pUserListWnd->m_search_name = m_ctlEdit.GetText();
					pUserListWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
					pUserListWnd->Show();
					DestroyWindow();
					delete this;
					theApp.PopWnd(this);
					theApp.m_stack.push(pUserListWnd);
				}
				else
				{
					CM8WeiboHomeWnd *pWeiboSearchWnd = new CM8WeiboHomeWnd();
					pWeiboSearchWnd->m_user = theApp.GetUserInfoSelf();
					pWeiboSearchWnd->m_user_type = CM8WeiboHomeWnd::WB_SEARCH;
					pWeiboSearchWnd->m_search_name = m_ctlEdit.GetText();
					pWeiboSearchWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
					pWeiboSearchWnd->Show();
					DestroyWindow();
					delete this;
					theApp.PopWnd(this);
					theApp.m_stack.push(pWeiboSearchWnd);
				}
			}
		}
		break;
	case MZ_IDC_FINDER_USER:
		{
			DWORD state = m_btnUser.GetState();
			m_btnWeibo.SetState(!state);
			m_ctlEdit.SetTip2(L"昵称：");
			Invalidate();
			UpdateWindow();
		}
		break;
	case MZ_IDC_FINDER_WEIBO:
		{
			DWORD state = m_btnWeibo.GetState();
			m_btnUser.SetState(!state);
			m_ctlEdit.SetTip2(L"标签：");
			Invalidate();
			UpdateWindow();
		}
		break;
	}
}


int CM8WeiboFinderWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	 /*switch(message)
	 {
	 }*/
	 return SHK_RET_APPNOEXIT_SHELLTOP;
}
