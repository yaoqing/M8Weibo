#include "M8WeiboSettingsWnd.h"
#include "M8WeiboCoverWnd.h"
#include "M8WeiboApp.h"
#include "M8WeiboParameter.h"
#include "resource.h"
#include "StringW2A.h"
#include "StringA2W.h"

void CM8WeiboSettingsButton::init()
{
	m_hWnd = NULL;
	m_pressed = false;
	COLOR_OF_BG = RGB(247,247,247);
	COLOR_OF_FG = RGB(78,149,209);//(72,72,72);
	LINE_HEIGHT_OF_TEXT = 28;
	LEFT_PAD_OF_TEXT = 26;
}

void CM8WeiboSettingsButton::OnPaintWin()
{
	RECT rect = GetClientRect();
	RECT* prcWin = &rect;
	HDC hDC = m_dcCanvas.GetDC();
	HBRUSH hBrush = CreateSolidBrush(m_pressed? COLOR_OF_FG: COLOR_OF_BG);
	FillRect(hDC, prcWin, hBrush);
	DeleteObject(hBrush);

	// 输出更多
	LOGFONT font_struct={ 0, 0, 0, 0, 0, 0, 0, 0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"宋体"
	};
	font_struct.lfHeight = LINE_HEIGHT_OF_TEXT;

	HFONT hFont = CreateFontIndirect(&font_struct);
	HGDIOBJ hSavedFont = SelectObject(hDC, hFont);
	SetBkMode(hDC,TRANSPARENT);
	::SetTextColor(hDC, m_pressed? COLOR_OF_BG: COLOR_OF_FG);

	RECT rcTemp = {LEFT_PAD_OF_TEXT, 0, GetWidth(), GetHeight()};
	OffsetRect(&rcTemp, prcWin->left, prcWin->top);

	DrawText(hDC, m_text.C_Str(), -1, &rcTemp, DT_SINGLELINE|DT_LEFT|DT_VCENTER);

	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);
}

int CM8WeiboSettingsButton::OnLButtonDown(UINT fwKeys, int xPos, int yPos)
{
	m_pressed = true;
	OnPaintWin();
	return UiCanvas::OnLButtonDown(fwKeys, xPos, yPos);
}

int CM8WeiboSettingsButton::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	m_pressed = false;
	OnPaintWin();
	SetTimer(m_hWnd, (UINT)GetID(), 100, NULL);
	CM8WeiboSettingsScroolWin *pWnd = (CM8WeiboSettingsScroolWin *)GetParent();
	pWnd->m_dwTimerID = (UINT)GetID();
	return UiCanvas::OnLButtonUp(fwKeys, xPos, yPos);
}

int CM8WeiboSettingsScroolWin::OnLButtonDown(UINT fwKeys, int xPos, int yPos)
{
	KillTimer(m_hWnd, m_dwTimerID);
	return UiScrollWin::OnLButtonDown(fwKeys, xPos, yPos);
}

CM8WeiboSettingsCanvas::CM8WeiboSettingsCanvas(void)
{
	LINE_HEIGHT_OF_TEXT = 28;
	SPACE_OF_LINE = 3;
	LEFT_PAD_TEXT = 24;
	COLOR_OF_TEXT = RGB(127,127,127);//(0,176,80);
	COLOR_OF_BACKGROUND = RGB(247,247,247);
}

CM8WeiboSettingsCanvas::~CM8WeiboSettingsCanvas(void)
{
}

void CM8WeiboSettingsCanvas::OnPaintWin()
{
	RECT rect = GetClientRect();
	RECT* prcWin = &rect;
	HDC hDC = m_dcCanvas.GetDC();
	HBRUSH hBrush = CreateSolidBrush(COLOR_OF_BACKGROUND);
	FillRect(hDC, prcWin, hBrush);
	DeleteObject(hBrush);

	// 输出更多
	LOGFONT font_struct={ 0, 0, 0, 0, 0, 0, 0, 0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"宋体"
	};
	font_struct.lfHeight = LINE_HEIGHT_OF_TEXT;

	HFONT hFont = CreateFontIndirect(&font_struct);
	HGDIOBJ hSavedFont = SelectObject(hDC, hFont);
	SetBkMode(hDC,TRANSPARENT);
	::SetTextColor(hDC, COLOR_OF_TEXT);
	
	int lines = 3;
	int offsetY = (GetHeight()-lines*LINE_HEIGHT_OF_TEXT-(lines-1)*SPACE_OF_LINE)/2;
	RECT rcTemp = {LEFT_PAD_TEXT, offsetY, GetWidth()-2*LEFT_PAD_TEXT, LINE_HEIGHT_OF_TEXT+offsetY};
	OffsetRect(&rcTemp, prcWin->left, prcWin->top);

	DrawText(hDC, L"　版本：1.0.8", -1, &rcTemp, DT_SINGLELINE|DT_CENTER);
	rcTemp.top = rcTemp.bottom+SPACE_OF_LINE;
	rcTemp.bottom = rcTemp.top+LINE_HEIGHT_OF_TEXT;
	DrawText(hDC, L"新浪微博：M8围脖", -1, &rcTemp, DT_SINGLELINE|DT_CENTER);
	rcTemp.top = rcTemp.bottom+SPACE_OF_LINE;
	rcTemp.bottom = rcTemp.top+LINE_HEIGHT_OF_TEXT;
	DrawText(hDC, L"© 2012 姚清，版权所有。", -1, &rcTemp, DT_SINGLELINE|DT_CENTER);

	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);
}

MZ_IMPLEMENT_DYNAMIC(CM8WeiboSettingsWnd)

CM8WeiboSettingsWnd::CM8WeiboSettingsWnd(void)
{
	SPACE_CONTROL = 2;
	LINE_HEIGHT = 1;
	HEIGHT_CONTROL = 72;
	SWITCH_WIDTH = 150;
	STATIC_WIDTH = 240;

	imgNormalGoback = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOBACK_BLUE, true);
	imgPressedGoback = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOBACK_GREEN, true);
	imgNormalGohome = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOHOME_BLUE, true);
	imgPressedGohome = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOHOME_GREEN, true);
}

CM8WeiboSettingsWnd::~CM8WeiboSettingsWnd(void)
{
	m_image_container.RemoveAll();
}

BOOL CM8WeiboSettingsWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
	{
		return FALSE;
	}

	int width = GetWidth();

	m_ctlCaption.SetID(MZ_IDC_SETTINGS_CAPTION);
	m_ctlCaption.SetPos(0, 0, width, MZM_HEIGHT_TEXT_TOOLBAR);
	m_ctlCaption.SetTextColor(RGB(89,89,89));
	m_ctlCaption.SetTextSize(30);
	m_ctlCaption.SetTextWeight(FW_MEDIUM);
	m_ctlCaption.SetText(L"更多");
	AddUiWin(&m_ctlCaption);

	m_button_goback.SetID(MZ_IDC_SETTINGS_GOBACK);
	m_button_goback.SetPos(24, 10, 48, 48);
	m_button_goback.SetButtonType(MZC_BUTTON_NONE);
	m_button_goback.SetImage_Normal(imgNormalGoback);
	m_button_goback.SetImage_Pressed(imgPressedGoback);
	m_ctlCaption.AddChild(&m_button_goback);

	m_button_gohome.SetID(MZ_IDC_SETTINGS_GOHOME);
	m_button_gohome.SetPos(width-24-48, 10, 48, 48);
	m_button_gohome.SetButtonType(MZC_BUTTON_NONE);
	m_button_gohome.SetImage_Normal(imgNormalGohome);
	m_button_gohome.SetImage_Pressed(imgPressedGohome);
	m_ctlCaption.AddChild(&m_button_gohome);

	m_scrool_win.SetID(MZ_IDC_SETTINGS_SCROOLWIN);
	m_scrool_win.SetPos(0, MZM_HEIGHT_TEXT_TOOLBAR, width, GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR);
	m_scrool_win.m_hWnd = m_hWnd;
	m_scrool_win.EnableScrollBarV(true);
	AddUiWin(&m_scrool_win);

	// 设置第一条分割线
	int offset_y = 0;
	m_lines[0].SetPos(0, offset_y, width, LINE_HEIGHT);
	m_lines[0].SetButtonType(MZC_BUTTON_LINE_TOP);
	m_scrool_win.AddChild(&m_lines[0]);

	// 设置是否自动下载用户头像
	offset_y += SPACE_CONTROL;
	m_static_profile.SetPos(0, offset_y, STATIC_WIDTH, HEIGHT_CONTROL);
	m_static_profile.SetText(L"自动下载用户头像");
	m_scrool_win.AddChild(&m_static_profile);

	m_switch_profile.SetID(MZ_IDC_SETTINGS_PROFILE_IMAGE);
	m_switch_profile.SetPos(width-SWITCH_WIDTH, offset_y, SWITCH_WIDTH, HEIGHT_CONTROL);
	m_switch_profile.SetButtonType(MZC_BUTTON_SWITCH);
	m_switch_profile.SetButtonMode(MZC_BUTTON_MODE_HOLD);
	m_switch_profile.SetState(theApp.m_parameter->getAutoDownloadProfileImage());
	m_scrool_win.AddChild(&m_switch_profile);

	// 设置第二条分割线
	offset_y += HEIGHT_CONTROL + SPACE_CONTROL - LINE_HEIGHT;
	m_lines[1].SetPos(0, offset_y, width, LINE_HEIGHT);
	m_lines[1].SetButtonType(MZC_BUTTON_LINE_TOP);
	m_scrool_win.AddChild(&m_lines[1]);
	offset_y += SPACE_CONTROL;

	// 设置是否自动下载小缩略图
	m_static_thumb.SetPos(0, offset_y, STATIC_WIDTH, HEIGHT_CONTROL);
	m_static_thumb.SetText(L"自动下载小缩略图");
	m_scrool_win.AddChild(&m_static_thumb);

	m_switch_thumb.SetID(MZ_IDC_SETTINGS_THUMB_IMAGE);
	m_switch_thumb.SetPos(width-SWITCH_WIDTH, offset_y, SWITCH_WIDTH, HEIGHT_CONTROL);
	m_switch_thumb.SetButtonType(MZC_BUTTON_SWITCH);
	m_switch_thumb.SetButtonMode(MZC_BUTTON_MODE_HOLD);
	m_switch_thumb.SetState(theApp.m_parameter->getAutoDownloadSmallPicture());
	m_scrool_win.AddChild(&m_switch_thumb);

	// 设置第三条分割线
	offset_y += HEIGHT_CONTROL + SPACE_CONTROL - LINE_HEIGHT;
	m_lines[2].SetPos(0, offset_y, width, LINE_HEIGHT);
	m_lines[2].SetButtonType(MZC_BUTTON_LINE_TOP);
	m_scrool_win.AddChild(&m_lines[2]);
	offset_y += SPACE_CONTROL;

	// 设置是否自动下载中缩略图
	m_static_middle.SetPos(0, offset_y, STATIC_WIDTH, HEIGHT_CONTROL);
	m_static_middle.SetText(L"自动下载中缩略图");
	m_scrool_win.AddChild(&m_static_middle);

	m_switch_middle.SetID(MZ_IDC_SETTINGS_MIDDLE_IMAGE);
	m_switch_middle.SetPos(width-SWITCH_WIDTH, offset_y, SWITCH_WIDTH, HEIGHT_CONTROL);
	m_switch_middle.SetButtonType(MZC_BUTTON_SWITCH);
	m_switch_middle.SetButtonMode(MZC_BUTTON_MODE_HOLD);
	m_switch_middle.SetState(theApp.m_parameter->getAutoDownloadMiddlePicture());
	m_scrool_win.AddChild(&m_switch_middle);

	// 设置第四条分割线
	offset_y += HEIGHT_CONTROL + SPACE_CONTROL - LINE_HEIGHT;
	m_lines[3].SetPos(0, offset_y, width, LINE_HEIGHT);
	m_lines[3].SetButtonType(MZC_BUTTON_LINE_TOP);
	m_scrool_win.AddChild(&m_lines[3]);
	offset_y += SPACE_CONTROL;

	// 设置切换网络按钮
	m_btnNetEnv.SetID(MZ_IDC_SETTINGS_NETENV);
	m_btnNetEnv.SetPos(0, offset_y, width, HEIGHT_CONTROL);
	m_btnNetEnv.m_hWnd = m_hWnd;
	m_btnNetEnv.m_text = L"网络连接切换";
	m_btnNetEnv.m_dcCanvas.Create(width, HEIGHT_CONTROL);
	m_btnNetEnv.OnPaintWin();
	m_scrool_win.AddChild(&m_btnNetEnv);

	// 设置第五条分割线
	offset_y += HEIGHT_CONTROL + SPACE_CONTROL - LINE_HEIGHT;
	m_lines[4].SetPos(0, offset_y, width, LINE_HEIGHT);
	m_lines[4].SetButtonType(MZC_BUTTON_LINE_TOP);
	m_scrool_win.AddChild(&m_lines[4]);
	offset_y += SPACE_CONTROL;

	// 设置帐号管理按钮
	m_btnAccount.SetID(MZ_IDC_SETTINGS_ACCOUNT);
	m_btnAccount.SetPos(0, offset_y, width, HEIGHT_CONTROL);
	m_btnAccount.m_hWnd = m_hWnd;
	m_btnAccount.m_text = L"帐号切换";
	m_btnAccount.m_dcCanvas.Create(width, HEIGHT_CONTROL);
	m_btnAccount.OnPaintWin();
	m_scrool_win.AddChild(&m_btnAccount);

	// 设置第六条分割线
	offset_y += HEIGHT_CONTROL + SPACE_CONTROL - LINE_HEIGHT;
	m_lines[5].SetPos(0, offset_y, width, LINE_HEIGHT);
	m_lines[5].SetButtonType(MZC_BUTTON_LINE_TOP);
	m_scrool_win.AddChild(&m_lines[5]);
	offset_y += SPACE_CONTROL;

	// 设置退出帐号按钮
	m_btnExit.SetID(MZ_IDC_SETTINGS_EXIT);
	m_btnExit.SetPos(0, offset_y, width, HEIGHT_CONTROL);
	m_btnExit.m_hWnd = m_hWnd;
	m_btnExit.m_text = L"退出当前帐号";
	m_btnExit.m_dcCanvas.Create(width, HEIGHT_CONTROL);
	m_btnExit.OnPaintWin();
	m_scrool_win.AddChild(&m_btnExit);

	// 设置第七条分割线
	offset_y += HEIGHT_CONTROL + SPACE_CONTROL - LINE_HEIGHT;
	m_lines[6].SetPos(0, offset_y, width, LINE_HEIGHT);
	m_lines[6].SetButtonType(MZC_BUTTON_LINE_TOP);
	m_scrool_win.AddChild(&m_lines[6]);
	offset_y += LINE_HEIGHT;

	// 设置版本、版权信息
	m_canvas.SetPos(0, offset_y, width, GetHeight()-offset_y-MZM_HEIGHT_TEXT_TOOLBAR);
	m_canvas.m_dcCanvas.Create(width, GetHeight()-offset_y-MZM_HEIGHT_TEXT_TOOLBAR);
	m_canvas.OnPaintWin();
	m_scrool_win.AddChild(&m_canvas);

	return TRUE;
}

void CM8WeiboSettingsWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == MZ_IDC_SETTINGS_NETENV)
	{
		KillTimer(m_hWnd, MZ_IDC_SETTINGS_NETENV);
		MzPopupMenu menu;
		menu.AppendMenuItem(MZV2_MID_MIN+1, L"已接入网络");
		menu.AppendMenuItem(MZV2_MID_MIN+2, L"CMNET");
		menu.AppendMenuItem(MZV2_MID_MIN+3, L"CMWAP");
		menu.AppendMenuItem(MZV2_MID_MIN+4, L"UNINET");
		menu.AppendMenuItem(MZV2_MID_MIN+5, L"UNIWAP");
		int result = menu.MzTrackPopupMenu(m_hWnd, TRUE, NULL, TRUE);
		if (result >= MZV2_MID_MIN+1 && result <= MZV2_MID_MIN+5)
		{
			theApp.m_parameter->setWeiboCMNONE(0);
			theApp.m_parameter->setWeiboCMNET(0);
			theApp.m_parameter->setWeiboCMWAP(0);
			theApp.m_parameter->setWeiboUNINET(0);
			theApp.m_parameter->setWeiboUNIWAP(0);
		}
		int prev_type = theApp.m_net_env.NeType();
		switch (result)
		{
		case MZV2_MID_MIN+1:
			{
				theApp.m_net_env.NeType(CM8WeiboNetworkEnv::NE_CMNONE);
				theApp.m_parameter->setWeiboCMNONE(1);
			}
			break;
		case MZV2_MID_MIN+2:
			{
				theApp.m_net_env.NeType(CM8WeiboNetworkEnv::NE_CMNET);
				theApp.m_parameter->setWeiboCMNET(1);
			}
			break;
		case MZV2_MID_MIN+3:
			{
				theApp.m_net_env.NeType(CM8WeiboNetworkEnv::NE_CMWAP);
				theApp.m_parameter->setWeiboCMWAP(1);
			}
			break;
		case MZV2_MID_MIN+4:
			{
				theApp.m_net_env.NeType(CM8WeiboNetworkEnv::NE_UNINET);
				theApp.m_parameter->setWeiboUNINET(1);
			}
			break;
		case MZV2_MID_MIN+5:
			{
				theApp.m_net_env.NeType(CM8WeiboNetworkEnv::NE_UNIWAP);
				theApp.m_parameter->setWeiboUNIWAP(1);
			}
			break;
		}
		if (result >= MZV2_MID_MIN+1 && result <= MZV2_MID_MIN+5)
		{
			theApp.m_parameter->saveConfig();
			int curr_type = theApp.m_net_env.NeType();
			if (prev_type != CM8WeiboNetworkEnv::NE_CMNONE &&
				curr_type == CM8WeiboNetworkEnv::NE_CMNONE)
				theApp.m_net_env.stop(m_hWnd);
			theApp.m_net_env.redial(m_hWnd);
		}
	}
	else if (nIDEvent == MZ_IDC_SETTINGS_ACCOUNT)
	{
		KillTimer(m_hWnd, MZ_IDC_SETTINGS_ACCOUNT);
		wchar_t * account_list = theApp.m_parameter->getAccountList();
		CMzString * szAccounts = NULL;
		int size = theApp.GetAccountList(account_list, szAccounts);
		MzPopupMenu menu;
		for (int i = 0; i < size; i++)
		{
			wchar_t * temp = theApp.m_parameter->getWeiboName(CStringW2A(szAccounts[i].C_Str()).ptr());
			menu.AppendMenuItem(MZV2_MID_MIN+i+1, temp);
			delete [] temp;
		}
		int result = menu.MzTrackPopupMenu(m_hWnd, TRUE, NULL, TRUE);
		if (result >= MZV2_MID_MIN+1 && result <= MZV2_MID_MIN+size)
		{
			theApp.m_parameter->setAccountList(szAccounts[result-MZV2_MID_MIN-1].C_Str());
		}
		delete [] szAccounts;
		delete [] account_list;

		if (result >= MZV2_MID_MIN+1 && result <= MZV2_MID_MIN+size)
		{
			theApp.m_parameter->setWeiboAutoLogin(1);
			RECT rcWork = MzGetWorkArea();
			CM8WeiboCoverWnd *pCoverWnd = new CM8WeiboCoverWnd();
			pCoverWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
			pCoverWnd->Show();
			theApp.GoLogin();
		}
	}
	else if (nIDEvent == MZ_IDC_SETTINGS_EXIT)
	{
		KillTimer(m_hWnd, MZ_IDC_SETTINGS_EXIT);
		if (MzMessageBox(m_hWnd, L"退出当前帐号？", NULL, MB_OKCANCEL) == IDOK)
		{
			theApp.m_parameter->setWeiboAutoLogin(0);
			RECT rcWork = MzGetWorkArea();
			CM8WeiboCoverWnd *pCoverWnd = new CM8WeiboCoverWnd();
			pCoverWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
			pCoverWnd->Show();
			theApp.GoLogin();
		}
	}
}

void CM8WeiboSettingsWnd::OnMzCommand(WPARAM wParam, LPARAM lParam)
{
	UINT_PTR id = LOWORD(wParam);
	switch(id)
	{
	case MZ_IDC_SETTINGS_GOBACK:
		{
			DestroyWindow();
			delete this;
			theApp.PopWnd(this);
		}
		break;
	case MZ_IDC_SETTINGS_GOHOME:
		{
			theApp.GoHome();
		}
		break;
	case MZ_IDC_SETTINGS_PROFILE_IMAGE:
		{
			theApp.m_parameter->setAutoDownloadProfileImage(m_switch_profile.GetState());
			theApp.m_parameter->saveConfig();
		}
		break;
	case MZ_IDC_SETTINGS_THUMB_IMAGE:
		{
			theApp.m_parameter->setAutoDownloadSmallPicture(m_switch_thumb.GetState());
			theApp.m_parameter->saveConfig();
		}
		break;
	case MZ_IDC_SETTINGS_MIDDLE_IMAGE:
		{
			theApp.m_parameter->setAutoDownloadMiddlePicture(m_switch_middle.GetState());
			theApp.m_parameter->saveConfig();
		}
		break;
	}
}

int CM8WeiboSettingsWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	/*switch(message)
	 {
	 }*/
	 return SHK_RET_APPNOEXIT_SHELLTOP;
}
