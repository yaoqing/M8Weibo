#include "M8WeiboUserWnd.h"
#include "M8WeiboFinderWnd.h"
#include "M8WeiboHomeWnd.h"
#include "M8WeiboWriteWnd.h"
#include "M8WeiboUserListWnd.h"
#include "M8WeiboSettingsWnd.h"
#include "M8WeiboParameter.h"
#include "M8WeiboApp.h"
#include "resource.h"
#include "StringW2A.h"
#include "StringA2W.h"

CM8WeiboUserCanvas::CM8WeiboUserCanvas()
{
}

CM8WeiboUserCanvas::~CM8WeiboUserCanvas()
{
}

int CM8WeiboUserCanvas::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	POINT pos = {xPos, yPos};
	BOOL isInRect =PtInRect(&(m_parentWnd->m_rcFollow), pos);
	if (isInRect)
	{
		if (m_parentWnd->IsFriend())
		{// 取消关注
			if (MzMessageBox(m_hWnd, L"取消关注？", NULL, MB_OKCANCEL) == IDOK)
			{
				if (theApp.PutFriendshipsDestroy(m_hWnd, m_uid))
				{
					m_parentWnd->RefreshFriendships();
				}
				else
				{
					MzMessageBox(m_hWnd, L"取消关注失败！", NULL, MB_OK, 1000, TRUE);
				}
			}
		}
		else
		{// 加关注
			if (theApp.PutFriendshipsCreate(m_hWnd, m_uid))
			{
				m_parentWnd->RefreshFriendships();
			}
			else
			{
				MzMessageBox(m_hWnd, L"关注失败！", NULL, MB_OK, 1000, TRUE);
			}
		}
	}
	return UiCanvas::OnLButtonUp(fwKeys, xPos, yPos);
}


CM8WeiboUserScroolCanvas::CM8WeiboUserScroolCanvas(void)
{
}

CM8WeiboUserScroolCanvas::~CM8WeiboUserScroolCanvas(void)
{
}

int CM8WeiboUserScroolCanvas::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	POINT pos = {xPos, yPos};
	BOOL isInFriends =PtInRect(&(m_parentWnd->m_rcFriends), pos);
	BOOL isInStatuses =PtInRect(&(m_parentWnd->m_rcStatuses), pos);
	BOOL isInFollowers =PtInRect(&(m_parentWnd->m_rcFollowers), pos);
	BOOL isInFavourites =PtInRect(&(m_parentWnd->m_rcFavourites), pos);
	if (isInFriends)
	{
		RECT rcWork = MzGetWorkArea();
		CM8WeiboUserListWnd *pUserListWnd = new CM8WeiboUserListWnd();
		pUserListWnd->m_user = m_parentWnd->GetUserInfo();
		pUserListWnd->m_user_type = CM8WeiboUserListWnd::FRIEND;
		pUserListWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
		pUserListWnd->Show();
		theApp.m_stack.push(pUserListWnd);
	}
	if (isInStatuses)
	{
		RECT rcWork = MzGetWorkArea();
		CM8WeiboHomeWnd *pHomeWnd = new CM8WeiboHomeWnd();
		pHomeWnd->m_user = m_parentWnd->GetUserInfo();
		pHomeWnd->m_user_type = CM8WeiboHomeWnd::THE_FRIEND;
		pHomeWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
		pHomeWnd->Show();
		theApp.m_stack.push(pHomeWnd);
	}
	if (isInFollowers)
	{
		RECT rcWork = MzGetWorkArea();
		CM8WeiboUserListWnd *pUserListWnd = new CM8WeiboUserListWnd();
		pUserListWnd->m_user = m_parentWnd->GetUserInfo();
		pUserListWnd->m_user_type = CM8WeiboUserListWnd::FOLLOWER;
		pUserListWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
		pUserListWnd->Show();
		theApp.m_stack.push(pUserListWnd);
	}
	if (isInFavourites)
	{
		if (theApp.isSelf(m_parentWnd->GetUserInfo()->id_))
		{
			RECT rcWork = MzGetWorkArea();
			CM8WeiboHomeWnd *pHomeWnd = new CM8WeiboHomeWnd();
			pHomeWnd->m_user = m_parentWnd->GetUserInfo();
			pHomeWnd->m_user_type = CM8WeiboHomeWnd::FAVOURITE;
			pHomeWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
			pHomeWnd->Show();
			theApp.m_stack.push(pHomeWnd);
		}
	}

	return UiCanvas::OnLButtonUp(fwKeys, xPos, yPos);
}

MZ_IMPLEMENT_DYNAMIC(CM8WeiboUserWnd)

CM8WeiboUserWnd::CM8WeiboUserWnd(void)
{
	isSelf = false;
	isFriend = false;
	memset(&m_rcFollow, 0, sizeof(RECT));
	memset(&m_rcToken, 0, sizeof(RECT));

	imgNormalGoback = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOBACK_BLUE, true);
	imgPressedGoback = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOBACK_GREEN, true);
	imgNormalGohome = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOHOME_BLUE, true);
	imgPressedGohome = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOHOME_GREEN, true);
	imgNormalRefresh = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_REFRESH_BLUE, true);
	imgPressedRefresh = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_REFRESH_GREEN, true);
	imgTokenBoy = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_TOKEN_BOY, true);
	imgTokenGirl = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_TOKEN_GIRL, true);
	imgMale = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_MALE, true);
	imgFemale = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_FEMALE, true);

	MZ_STATUS_COUNT_PER_LINE = 13;

	// 定义各种字体的颜色
	COLOR_OF_BG_AUTHOR = RGB(249,249,249);
	COLOR_OF_BG_CONTENT = RGB(249,249,249);
	COLOR_OF_TEXT_CONTENT = RGB(30,73,125);//RGB(89,89,89);
	COLOR_OF_TEXT_LABLE = RGB(127,127,127);
	COLOR_OF_TEXT_AUTHOR = RGB(0,0,0);//(63,63,63);
	COLOR_OF_SPE_LINE = RGB(216,216,216);
	COLOR_OF_EDG_BUTTON = RGB(189,228,215);//(225,225,225);
	COLOR_OF_BG_BUTTON = RGB(247,247,247);
	COLOR_OF_BG_FOLLOW_1 =  RGB(185,205,229);
	COLOR_OF_BG_FOLLOW_2 = RGB(146,208,80);
	COLOR_OF_TEXT_FOLLOW = RGB(247,247,247);

	// 定义各种字体的行高
	TEXT_LINE_HEIGHT = 26;
	AUTHOR_LINE_HEIGHT = 28;
	SPACE_LINE_HEIGHT = 4;

	// 定义头像的边长
	TOKEN_LENGTH = 50;
	// 定义微博作者总高度
	HEIGHT_OF_AUTHOR = 100;
	// 定义头像与作者之间的空白
	PAD_SPACE_OF_TOKEN_AND_AUTHOR = 16;

	// 定义微博作者内空白
	PAD_LEFT_AUTHOR = 16;
	PAD_TOP_AUTHOR = 16;
	PAD_RIGHT_AUTHOR = 16;
	PAD_BOTTOM_AUTHOR = 10;

	// 定义关注按钮的外空白
	MARGIN_LEFT_FOLLOW = 320;
	MARGIN_TOP_FOLLOW = 28;
	MARGIN_RIGHT_FOLLOW = 20;
	MARGIN_BOTTOM_FOLLOW = 24;

	// 定义用户信息内空白
	PAD_LEFT_CONTENT = 20;
	PAD_TOP_CONTENT = 4;
	PAD_RIGHT_CONTENT = 20;
	PAD_BOTTOM_CONTENT = 4;

	// 矩形之间的垂直间距
	V_SPACE_IN_RECT = 24;
	PAD_TOP_RECT = 16;
	PAD_BOTTOM_RECT = 16;
	WIDTH_OF_LABLE = 136;

	// 定义圆角半径
	ROUND_WIDTH = 16;
}

CM8WeiboUserWnd::~CM8WeiboUserWnd(void)
{
}

void CM8WeiboUserWnd::SetUserInfo(M8::m8_user *p_user)
{
	m_user = *p_user;
	if (strcmp(m_user.id_, theApp.GetUserInfoSelf()->id_) == 0)
	{
		isSelf = true;
	}
}

BOOL CM8WeiboUserWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
	{
		return FALSE;
	}

	int width = GetWidth();

	m_ctlCaption.SetID(MZ_IDC_USER_CAPTION);
	m_ctlCaption.SetPos(0, 0, width, MZM_HEIGHT_TEXT_TOOLBAR);
	m_ctlCaption.SetTextColor(RGB(89,89,89));
	m_ctlCaption.SetTextSize(30);
	m_ctlCaption.SetTextWeight(FW_MEDIUM);
	if (isSelf)
	{
		m_ctlCaption.SetText(L"我的资料");
	}
	else
	{
		m_ctlCaption.SetText(L"资料");
	}
	AddUiWin(&m_ctlCaption);

	m_button_goback.SetID(MZ_IDC_USER_GOBACK);
	m_button_goback.SetPos(24, 10, 48, 48);
	m_button_goback.SetButtonType(MZC_BUTTON_NONE);
	m_button_goback.SetImage_Normal(imgNormalGoback);
	m_button_goback.SetImage_Pressed(imgPressedGoback);
	m_ctlCaption.AddChild(&m_button_goback);

	m_button_gohome.SetID(MZ_IDC_USER_GOHOME);
	m_button_gohome.SetPos(width-24-48, 10, 48, 48);
	m_button_gohome.SetButtonType(MZC_BUTTON_NONE);
	if (isSelf)
	{
		m_button_gohome.SetImage_Normal(imgNormalRefresh);
		m_button_gohome.SetImage_Pressed(imgPressedRefresh);
	}
	else
	{
		m_button_gohome.SetImage_Normal(imgNormalGohome);
		m_button_gohome.SetImage_Pressed(imgPressedGohome);
	}
	m_ctlCaption.AddChild(&m_button_gohome);

	m_toolbar.SetID(MZ_IDC_USER_TOOLBAR);
	m_toolbar.SetPos(0, GetHeight()-MZM_HEIGHT_ICON_TOOLBAR, width, MZM_HEIGHT_ICON_TOOLBAR);
	if (strcmp(m_user.id_,theApp.GetUserInfoSelf()->id_) ==0)
	{
		m_toolbar.SetButton(0, false, IDB_PNG_HOME_BLUE, IDB_PNG_HOME_GREEN);
		m_toolbar.SetButton(1, false, IDB_PNG_MESSAGE_BLUE, IDB_PNG_MESSAGE_GREEN);
		m_toolbar.SetButton(2, true, IDB_PNG_USER_BLUE, IDB_PNG_USER_GREEN);
		m_toolbar.SetButton(3, false, IDB_PNG_MORE_BLUE, IDB_PNG_MORE_GREEN);
		m_toolbar.SetButton(4, false, IDB_PNG_SETTINGS_BLUE, IDB_PNG_SETTINGS_GREEN);
	}
	else
	{
		m_toolbar.SetButton(0, false, IDB_PNG_REFRESH_BLUE, IDB_PNG_REFRESH_GREEN);
		m_toolbar.SetButton(2, false, IDB_PNG_SPEAKER_BLUE, IDB_PNG_SPEAKER_GREEN);
		m_toolbar.SetButton(4, false, IDB_PNG_REMOVE_BLUE, IDB_PNG_REMOVE_GREEN);
	}
	if (isSelf)
	{
		if (theApp.m_stack.size() == 1 || theApp.m_stack.size() == 2) AddUiWin(&m_toolbar);
	}
	else
	{
		AddUiWin(&m_toolbar);
	}

	m_canvas.SetID(MZ_IDC_USER_CANVAS);
	m_canvas.SetPos(0, MZM_HEIGHT_TEXT_TOOLBAR, width, HEIGHT_OF_AUTHOR);
	m_canvas.m_dcCanvas.Create(width, HEIGHT_OF_AUTHOR);
	m_canvas.m_parentWnd = this;
	m_canvas.m_hWnd = m_hWnd;
	m_canvas.m_uid = m_user.id_;

	// 填充作者背景
	RECT rcOrigin = {0,0,width,HEIGHT_OF_AUTHOR};
	HDC hDC = m_canvas.m_dcCanvas.GetDC();
	SetBkMode(hDC,TRANSPARENT);
	HBRUSH hBrush = CreateSolidBrush(COLOR_OF_BG_AUTHOR);
	FillRect(hDC, &rcOrigin, hBrush);
	DeleteObject(hBrush);

	// 画微博作者头像
	RECT rcToken;
	rcToken.left = rcOrigin.left+PAD_LEFT_AUTHOR;
	rcToken.right = rcToken.left+TOKEN_LENGTH;
	rcToken.top = rcOrigin.top+PAD_TOP_AUTHOR+(HEIGHT_OF_AUTHOR-TOKEN_LENGTH-PAD_TOP_AUTHOR-PAD_BOTTOM_AUTHOR)/2;
	rcToken.bottom = rcToken.top+TOKEN_LENGTH;
	
	m_rcToken = rcToken;
	CMzString profile_image_path;
	CMzString profile_token_name = theApp.UpdateProfileTokenName(m_user.id_, m_user.profile_image_url_, profile_image_path);
	CMzString profile_image_name = theApp.FindTokenFileName(profile_image_path, profile_token_name);
	if (profile_image_name.Length() > 0 || theApp.m_parameter->getAutoDownloadProfileImage())
	{
		if (profile_image_name.Length() == 0)
		{
			theApp.DownloadTokenFile(m_hWnd, m_user.id_, true, m_user.profile_image_url_, profile_image_path, profile_token_name);
			profile_image_name = profile_image_path + profile_token_name;
		}
		ImagingHelper::DrawImage(hDC, &rcToken, profile_image_name.C_Str());
	}
	else
	{
		if (m_user.gender_[0] == 'm')
			imgTokenBoy->Draw(hDC, &rcToken);
		else
			imgTokenGirl->Draw(hDC, &rcToken);
	}

	// 作者所占块位置
	RECT rcAuthor = {PAD_LEFT_AUTHOR+TOKEN_LENGTH+PAD_SPACE_OF_TOKEN_AND_AUTHOR,
		PAD_TOP_AUTHOR,width-PAD_RIGHT_AUTHOR,
		HEIGHT_OF_AUTHOR-PAD_BOTTOM_AUTHOR};
	LOGFONT font_struct={ 0, 0, 0, 0, 0, 0, 0, 0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"FZLanTingHei-R-GBK"
	};

	// 输出作者
	font_struct.lfHeight = AUTHOR_LINE_HEIGHT;
	HFONT hFont = CreateFontIndirect(&font_struct);
	HGDIOBJ hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, COLOR_OF_TEXT_AUTHOR);
	CStringA2W author(m_user.screen_name_);
	int len_author = wcslen(author.ptr());
	DrawText(hDC, author.ptr(), len_author, &rcAuthor, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	SIZE sizeAuthor = {0, 0};
	GetTextExtentPoint(hDC, author.ptr(), len_author, &sizeAuthor);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	// 画性别头像
	RECT rcGender = rcAuthor;
	rcGender.left += sizeAuthor.cx + PAD_SPACE_OF_TOKEN_AND_AUTHOR;
	rcGender.right = rcGender.left + 28;
	rcGender.top += (rcGender.bottom-rcGender.top-28)/2;
	rcGender.bottom = rcGender.top + 28;
	if (m_user.gender_[0] == 'm')
		imgMale->Draw(hDC, &rcGender);
	else
		imgFemale->Draw(hDC, &rcGender);

	// 画关注按钮
	if (!isSelf)
	{
		m_rcFollow = rcOrigin;
		m_rcFollow.left += MARGIN_LEFT_FOLLOW;
		m_rcFollow.right -= MARGIN_RIGHT_FOLLOW;
		m_rcFollow.top += MARGIN_TOP_FOLLOW;
		m_rcFollow.bottom -= MARGIN_BOTTOM_FOLLOW;
		OnDrawFollow(hDC, true);
	}

	AddUiWin(&m_canvas);

	// 输出引用换行占位符
	HPEN hPen = CreatePen(PS_SOLID, 1, COLOR_OF_SPE_LINE);
	HGDIOBJ hSavedPen = SelectObject(hDC, hPen);
	MoveToEx(hDC, 0, rcOrigin.top+HEIGHT_OF_AUTHOR-1, NULL);
	LineTo(hDC, width, rcOrigin.top+HEIGHT_OF_AUTHOR-1);
	SelectObject(hDC, hSavedPen);
	DeleteObject(hPen);

	// 创建滚动窗口
	RECT rcScrool = {0, MZM_HEIGHT_TEXT_TOOLBAR+HEIGHT_OF_AUTHOR, width, GetHeight()-MZM_HEIGHT_ICON_TOOLBAR};
	if (isSelf && theApp.m_stack.size() > 2) rcScrool.bottom += MZM_HEIGHT_ICON_TOOLBAR;
	m_scrool_win.SetID(MZ_IDC_USER_SCROOLWIN);
	m_scrool_win.SetPos(rcScrool.left, rcScrool.top, rcScrool.right-rcScrool.left, rcScrool.bottom-rcScrool.top);
	m_scrool_win.EnableScrollBarV(true);
	AddUiWin(&m_scrool_win);

	// 按每行制定字符个数换行
	CStringA2W text(m_user.description_);
	int lines = theApp.SplitString(text.ptr(), MZ_STATUS_COUNT_PER_LINE);
	CMzString *p_lines =  new CMzString[lines];
	theApp.SplitString(lines, p_lines, text.ptr(), MZ_STATUS_COUNT_PER_LINE);
	if (lines == 0)
	{
		delete [] p_lines;
		lines = 1;
		p_lines =  new CMzString[1];
	}

	// 设置用户信息的宽度和高度
	FULL_WIDTH_OF_STATUS = width;
	FULL_HEIGHT_OF_STATUS = PAD_TOP_CONTENT+PAD_BOTTOM_CONTENT+3*V_SPACE_IN_RECT
		+3*(PAD_TOP_RECT+PAD_BOTTOM_RECT+TEXT_LINE_HEIGHT)
		+2*(PAD_TOP_RECT+PAD_BOTTOM_RECT+SPACE_LINE_HEIGHT+2*TEXT_LINE_HEIGHT);
	if (lines > 1)
	{
		FULL_HEIGHT_OF_STATUS += (TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT)*(lines-1);
	}

	// 如果不满屏，扩展到满屏高度
	if (FULL_HEIGHT_OF_STATUS < rcScrool.bottom-rcScrool.top)
	{
		FULL_HEIGHT_OF_STATUS = rcScrool.bottom-rcScrool.top;
	}

	// 添加微博子窗口
	m_scrool_canvas.SetID(MZ_IDC_USER_SCANVAS);
	m_scrool_canvas.SetPos(0, 0, FULL_WIDTH_OF_STATUS, FULL_HEIGHT_OF_STATUS);
	m_scrool_canvas.m_dcCanvas.Create(FULL_WIDTH_OF_STATUS, FULL_HEIGHT_OF_STATUS);
	m_scrool_win.AddChild(&m_scrool_canvas);
	m_scrool_canvas.m_hWnd = m_hWnd;
	m_scrool_canvas.m_parentWnd = this;
	hDC = m_scrool_canvas.m_dcCanvas.GetDC();
	SetBkMode(hDC,TRANSPARENT);

	// 填充用户信息背景
	rcOrigin.left = 0;
	rcOrigin.top = 0;
	rcOrigin.right = FULL_WIDTH_OF_STATUS;
	rcOrigin.bottom = FULL_HEIGHT_OF_STATUS;
	hBrush = CreateSolidBrush(COLOR_OF_BG_CONTENT);
	FillRect(hDC, &rcOrigin, hBrush);
	DeleteObject(hBrush);

	// 输出引用换行占位符
	hPen = CreatePen(PS_SOLID, 1, COLOR_OF_SPE_LINE);
	hSavedPen = SelectObject(hDC, hPen);
	MoveToEx(hDC, 0, 0, NULL);
	LineTo(hDC, width, 0);
	SelectObject(hDC, hSavedPen);
	DeleteObject(hPen);

	// 初始化偏移量
	int offsetLeft = PAD_LEFT_CONTENT;
	int offsetTop = PAD_TOP_CONTENT;
	int offsetRight = PAD_RIGHT_CONTENT;
	int offsetBottom = PAD_BOTTOM_CONTENT;

	// 认证块所占块位置
	RECT rcCert = rcOrigin;
	rcCert.left += offsetLeft;
	rcCert.top += offsetTop+V_SPACE_IN_RECT;
	rcCert.right -= offsetRight;
	rcCert.bottom = rcCert.top+(PAD_TOP_RECT+PAD_BOTTOM_RECT+TEXT_LINE_HEIGHT)*3;
	if (lines > 1)
	{
		rcCert.bottom += (TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT)*(lines-1);
	}

	// 描绘认证块边框
	hPen = CreatePen(PS_SOLID, 1, COLOR_OF_EDG_BUTTON);
	hBrush = CreateSolidBrush(COLOR_OF_BG_BUTTON);
	hSavedPen = SelectObject(hDC, hPen);
	HGDIOBJ hSavedBrush = SelectObject(hDC, hBrush);
	//RoundRect(hDC, 10, 10, width-10, 100-10, 9, 9);
	RoundRect(hDC, rcCert.left, rcCert.top, rcCert.right, rcCert.bottom, ROUND_WIDTH, ROUND_WIDTH);
	SelectObject(hDC, hSavedBrush);
	SelectObject(hDC, hSavedPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);

	// 认证信息位置
	RECT rcCert11 = rcCert;
	rcCert11.top += PAD_TOP_RECT;
	rcCert11.bottom = rcCert11.top+TEXT_LINE_HEIGHT;
	rcCert11.right = rcCert11.left+WIDTH_OF_LABLE;

	// 输出认证信息
	wcscpy(font_struct.lfFaceName, L"宋体");
	font_struct.lfHeight = TEXT_LINE_HEIGHT;
	font_struct.lfWeight = 0;
	hFont = CreateFontIndirect(&font_struct);
	hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, COLOR_OF_TEXT_LABLE);
	DrawText(hDC, L"新浪认证:  ", -1, &rcCert11, DT_SINGLELINE|DT_RIGHT);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	RECT rcCert12 = rcCert11;
	rcCert12.left = rcCert12.right;
	rcCert12.right = rcCert.right;

	// 输出认证信息
	hFont = CreateFontIndirect(&font_struct);
	hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, COLOR_OF_TEXT_CONTENT);
	if (m_user.verified_[0] == '0')
	{
		DrawText(hDC, L"未认证", -1, &rcCert12, DT_SINGLELINE|DT_LEFT);
	}
	else
	{
		DrawText(hDC, L"认证用户", -1, &rcCert12, DT_SINGLELINE|DT_LEFT);
	}
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	// 描绘分割线
	hPen = CreatePen(PS_SOLID, 1, COLOR_OF_EDG_BUTTON);
	hSavedPen = SelectObject(hDC, hPen);
	MoveToEx(hDC, rcCert11.left, rcCert11.bottom+PAD_BOTTOM_RECT, NULL);
	LineTo(hDC, rcCert12.right, rcCert12.bottom+PAD_BOTTOM_RECT);
	SelectObject(hDC, hSavedPen);
	DeleteObject(hPen);

	// 所在地位置
	RECT rcCert21 = rcCert11;
	rcCert21.top = rcCert21.bottom+PAD_TOP_RECT+PAD_BOTTOM_RECT;
	rcCert21.bottom = rcCert21.top+TEXT_LINE_HEIGHT;

	// 输出所在地
	hFont = CreateFontIndirect(&font_struct);
	hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, COLOR_OF_TEXT_LABLE);
	DrawText(hDC, L"所在地:  ", -1, &rcCert21, DT_SINGLELINE|DT_RIGHT);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	RECT rcCert22 = rcCert12;
	rcCert22.top = rcCert22.bottom+PAD_TOP_RECT+PAD_BOTTOM_RECT;
	rcCert22.bottom = rcCert22.top+TEXT_LINE_HEIGHT;

	// 输出所在地
	hFont = CreateFontIndirect(&font_struct);
	hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, COLOR_OF_TEXT_CONTENT);
	DrawText(hDC, CStringA2W(m_user.location_).ptr(), -1, &rcCert22, DT_SINGLELINE|DT_LEFT);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	// 描绘分割线
	hPen = CreatePen(PS_SOLID, 1, COLOR_OF_EDG_BUTTON);
	hSavedPen = SelectObject(hDC, hPen);
	MoveToEx(hDC, rcCert21.left, rcCert21.bottom+PAD_BOTTOM_RECT, NULL);
	LineTo(hDC, rcCert22.right, rcCert22.bottom+PAD_BOTTOM_RECT);
	SelectObject(hDC, hSavedPen);
	DeleteObject(hPen);

	// 简介位置
	RECT rcCert31 = rcCert21;
	rcCert31.top = rcCert31.bottom+PAD_TOP_RECT+PAD_BOTTOM_RECT;
	rcCert31.bottom = rcCert31.top+TEXT_LINE_HEIGHT;

	// 输出简介
	hFont = CreateFontIndirect(&font_struct);
	hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, COLOR_OF_TEXT_LABLE);
	DrawText(hDC, L"自我介绍:  ", -1, &rcCert31, DT_SINGLELINE|DT_RIGHT);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	RECT rcCert32 = rcCert22;
	rcCert32.top = rcCert32.bottom+PAD_TOP_RECT+PAD_BOTTOM_RECT;
	rcCert32.bottom = rcCert32.top+TEXT_LINE_HEIGHT;

	// 输出简介
	hFont = CreateFontIndirect(&font_struct);
	hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, COLOR_OF_TEXT_CONTENT);
	for (int i = 0; i < lines; i++)
	{
		DrawText(hDC, p_lines[i], -1, &rcCert32, DT_SINGLELINE|DT_LEFT);
		rcCert32.top = rcCert32.bottom+SPACE_LINE_HEIGHT;
		rcCert32.bottom = rcCert32.top+TEXT_LINE_HEIGHT;
	}
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);
	delete [] p_lines;

	// 关注、微博、粉丝、收藏所占块位置
	RECT rcOther = rcCert;
	rcOther.top = rcCert.bottom+V_SPACE_IN_RECT;
	rcOther.bottom = rcOther.top+(PAD_TOP_RECT+PAD_BOTTOM_RECT+TEXT_LINE_HEIGHT*2+SPACE_LINE_HEIGHT)*2;

	// 描绘关注、微博、粉丝、收藏边框
	hPen = CreatePen(PS_SOLID, 1, COLOR_OF_EDG_BUTTON);
	hBrush = CreateSolidBrush(COLOR_OF_BG_BUTTON);
	hSavedPen = SelectObject(hDC, hPen);
	hSavedBrush = SelectObject(hDC, hBrush);
	//RoundRect(hDC, 10, 10, width-10, 100-10, 9, 9);
	RoundRect(hDC, rcOther.left, rcOther.top, rcOther.right, rcOther.bottom, ROUND_WIDTH, ROUND_WIDTH);
	MoveToEx(hDC, rcOther.left, rcOther.top+(rcOther.bottom-rcOther.top)/2, NULL);
	LineTo(hDC, rcOther.right, rcOther.top+(rcOther.bottom-rcOther.top)/2);
	MoveToEx(hDC, rcOther.left+(rcOther.right-rcOther.left)/2, rcOther.top, NULL);
	LineTo(hDC, rcOther.left+(rcOther.right-rcOther.left)/2, rcOther.bottom);
	SelectObject(hDC, hSavedBrush);
	SelectObject(hDC, hSavedPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);

	// 关注、微博、粉丝、收藏位置
	RECT rcFriends = rcOther;
	rcFriends.right = rcOther.left+(rcOther.right-rcOther.left)/2;
	rcFriends.bottom = rcOther.top+(rcOther.bottom-rcOther.top)/2;
	m_rcFriends = rcFriends;

	RECT rcStatuses = rcOther;
	rcStatuses.left = rcOther.right-(rcOther.right-rcOther.left)/2;
	rcStatuses.bottom = rcOther.top+(rcOther.bottom-rcOther.top)/2;
	m_rcStatuses = rcStatuses;

	RECT rcFollowers = rcOther;
	rcFollowers.right = rcOther.left+(rcOther.right-rcOther.left)/2;
	rcFollowers.top = rcOther.bottom-(rcOther.bottom-rcOther.top)/2;
	m_rcFollowers = rcFollowers;

	RECT rcFavourites = rcOther;
	rcFavourites.left = rcOther.right-(rcOther.right-rcOther.left)/2;
	rcFavourites.top = rcOther.bottom-(rcOther.bottom-rcOther.top)/2;
	m_rcFavourites = rcFavourites;

	// 输出关注、微博、粉丝、收藏数字
	OnDrawCounts(hDC);

	// 输出关注、微博、粉丝、收藏名称
	font_struct.lfHeight = TEXT_LINE_HEIGHT;
	font_struct.lfWeight = 0;
	hFont = CreateFontIndirect(&font_struct);
	hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, COLOR_OF_TEXT_LABLE);

	RECT rcTemp = rcFriends;
	rcTemp.top += PAD_TOP_RECT+TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT;
	rcTemp.bottom = rcTemp.top+TEXT_LINE_HEIGHT;
	rcTemp.left += ROUND_WIDTH;
	rcTemp.right -= ROUND_WIDTH;
	DrawText(hDC, L"关注", -1, &rcTemp, DT_SINGLELINE|DT_CENTER);

	rcTemp = rcStatuses;
	rcTemp.top += PAD_TOP_RECT+TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT;
	rcTemp.bottom = rcTemp.top+TEXT_LINE_HEIGHT;
	rcTemp.left += ROUND_WIDTH;
	rcTemp.right -= ROUND_WIDTH;
	DrawText(hDC, L"微博", -1, &rcTemp, DT_SINGLELINE|DT_CENTER);

	rcTemp = rcFollowers;
	rcTemp.top += PAD_TOP_RECT+TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT;
	rcTemp.bottom = rcTemp.top+TEXT_LINE_HEIGHT;
	rcTemp.left += ROUND_WIDTH;
	rcTemp.right -= ROUND_WIDTH;
	DrawText(hDC, L"粉丝", -1, &rcTemp, DT_SINGLELINE|DT_CENTER);

	rcTemp = rcFavourites;
	rcTemp.top += PAD_TOP_RECT+TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT;
	rcTemp.bottom = rcTemp.top+TEXT_LINE_HEIGHT;
	rcTemp.left += ROUND_WIDTH;
	rcTemp.right -= ROUND_WIDTH;
	DrawText(hDC, L"收藏", -1, &rcTemp, DT_SINGLELINE|DT_CENTER);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	if (!isSelf) SetTimer(m_hWnd, MZ_ID_USER_TIMER, 0, NULL);
	return TRUE;
}

void CM8WeiboUserWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == MZ_ID_USER_TIMER)
	{
		KillTimer(m_hWnd, MZ_ID_USER_TIMER);
		RefreshFriendships();
	}
}

void CM8WeiboUserWnd::OnMzCommand(WPARAM wParam, LPARAM lParam)
{
	UINT_PTR id = LOWORD(wParam);
	switch(id)
	{
	case MZ_IDC_USER_GOBACK:
		{
			DestroyWindow();
			delete this;
			theApp.PopWnd(this);
		}
		break;
	case MZ_IDC_USER_GOHOME:
		{
			if (isSelf)
			{// 刷新
				RefreshCounts();
				RefreshFriendships();
			}
			else
			{// 回首页
				theApp.GoHome();
			}
		}
		break;
	case MZ_IDC_USER_TOOLBAR+1:
		{
			if (isSelf)
			{// 回首页
				theApp.GoHome();
			}
			else
			{// 刷新
				RefreshCounts();
				RefreshFriendships();
			}
		}
		break;
	case MZ_IDC_USER_TOOLBAR+2:
		{
			RECT rcWork = MzGetWorkArea();
			CM8WeiboHomeWnd *pHomeWnd = new CM8WeiboHomeWnd();
			pHomeWnd->m_user = theApp.GetUserInfoSelf();
			pHomeWnd->m_user_type = CM8WeiboHomeWnd::MENTIONS;
			pHomeWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
			pHomeWnd->Show();
			DestroyWindow();
			delete this;
			theApp.PopWnd(this);
			theApp.m_stack.push(pHomeWnd);
		}
		break;
	case MZ_IDC_USER_TOOLBAR+3:
		{
			if (!isSelf)
			{
				RECT rcWork = MzGetWorkArea();
				CM8WeiboWriteWnd *pWriteWnd = new CM8WeiboWriteWnd();
				char buffer[WB_REAL_LEN(SCREEN_NAME)+2];
				sprintf(buffer, "@%s ", m_user.screen_name_);
				pWriteWnd->m_strPrefix = CStringA2W(buffer).ptr();
				pWriteWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pWriteWnd->Show();
				theApp.m_stack.push(pWriteWnd);
			}
		}
		break;
	case MZ_IDC_USER_TOOLBAR+4:
		{
			if (isSelf)
			{
				RECT rcWork = MzGetWorkArea();
				CM8WeiboFinderWnd *pFinderWnd = new CM8WeiboFinderWnd();
				pFinderWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pFinderWnd->Show();
				theApp.m_stack.push(pFinderWnd);
			}
		}
		break;
	case MZ_IDC_USER_TOOLBAR+5:
		{
			if (isSelf)
			{
				RECT rcWork = MzGetWorkArea();
				CM8WeiboSettingsWnd *pSettingsWnd = new CM8WeiboSettingsWnd();
				pSettingsWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pSettingsWnd->Show();
				theApp.m_stack.push(pSettingsWnd);
			}
			else
			{
				MzPopupMenu menu;
				menu.AppendMenuItem(MZV2_MID_MIN+1, L"更新头像");
				//if (isFollower) menu.AppendMenuItem(MZV2_MID_MIN+2, L"删除粉丝");
				int result = menu.MzTrackPopupMenu(m_hWnd, TRUE, NULL, TRUE);
				switch (result)
				{
				case MZV2_MID_MIN+1:
					{
						CMzString profile_image_path;
						CMzString profile_token_name = theApp.UpdateProfileTokenName(m_user.id_, m_user.profile_image_url_, profile_image_path);

						WIN32_FIND_DATA fd;
						CMzString image_name = profile_image_path + profile_token_name + L"*";
						HANDLE handle = FindFirstFile(image_name.C_Str(), &fd);
						do
						{
							if (handle == INVALID_HANDLE_VALUE) break;
							image_name = profile_image_path + fd.cFileName;
							DeleteFile(image_name.C_Str());
						} while (FindNextFile(handle, &fd));
						CloseHandle(handle);

						if (theApp.DownloadTokenFile(m_hWnd, m_user.id_, true, m_user.profile_image_url_, profile_image_path, profile_token_name))
						{
							PostMessage(MZ_WM_COMMAND, MZ_IDC_USER_DRAW_TOKEN, 0);
						}
					}
					break;
				/*case MZV2_MID_MIN+2:
					{
						if (theApp.PutFollowersDestroy(m_hWnd, m_user.id_))
						{
							MzMessageBox(m_hWnd, L"删除粉丝成功！", NULL, MB_OK, 1000, TRUE);
						}
					}
					break;*/
				}
			}
		}
		break;
	case MZ_IDC_USER_DRAW_FOLLOW:
		{
			HDC hDC = m_canvas.m_dcCanvas.GetDC();
			OnDrawFollow(hDC, false);
		}
		break;
	case MZ_IDC_USER_DRAW_TOKEN:
		{
			CMzString profile_image_path;
			CMzString profile_token_name = theApp.UpdateProfileTokenName(m_user.id_, m_user.profile_image_url_, profile_image_path);
			CMzString profile_image_name = theApp.FindTokenFileName(profile_image_path, profile_token_name);
			HDC hDC = m_canvas.m_dcCanvas.GetDC();
			ImagingHelper::DrawImage(hDC, &m_rcToken, profile_image_name.C_Str());
		}
		break;
	}
}

int CM8WeiboUserWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	/*switch(message)
	 {
	 }*/
	 return SHK_RET_APPNOEXIT_SHELLTOP;
}

void CM8WeiboUserWnd::OnDrawFollow(HDC hDC, bool first)
{
	if (first) return;
	SetBkMode(hDC,TRANSPARENT);
	LOGFONT font_struct={ 0, 0, 0, 0, 0, 0, 0, 0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"宋体"
	};

	HPEN hPen = CreatePen(PS_SOLID, 1, isFriend? COLOR_OF_BG_FOLLOW_1 : COLOR_OF_BG_FOLLOW_2);
	HGDIOBJ hSavedPen = SelectObject(hDC, hPen);
	HBRUSH hBrush = CreateSolidBrush(isFriend? COLOR_OF_BG_FOLLOW_1 : COLOR_OF_BG_FOLLOW_2);
	HGDIOBJ hSavedBrush = SelectObject(hDC, hBrush);
	RoundRect(hDC, m_rcFollow.left, m_rcFollow.top, m_rcFollow.right, m_rcFollow.bottom, ROUND_WIDTH, ROUND_WIDTH);
	SelectObject(hDC, hSavedBrush);
	SelectObject(hDC, hSavedPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);

	font_struct.lfHeight = TEXT_LINE_HEIGHT;
	HFONT hFont = CreateFontIndirect(&font_struct);
	HGDIOBJ hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, COLOR_OF_TEXT_FOLLOW);
	DrawText(hDC, isFriend? L"取消关注" : L"加关注", -1, &m_rcFollow, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);
}

void CM8WeiboUserWnd::OnDrawCounts(HDC hDC)
{
	SetBkMode(hDC,TRANSPARENT);
	LOGFONT font_struct={ 0, 0, 0, 0, 0, 0, 0, 0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"宋体"
	};

	wchar_t buffer[16];
	// 输出关注、微博、粉丝、收藏数字
	font_struct.lfHeight = TEXT_LINE_HEIGHT;
	font_struct.lfWeight = FW_MEDIUM;
	HFONT hFont = CreateFontIndirect(&font_struct);
	HGDIOBJ hSavedFont = SelectObject(hDC, hFont);
	HBRUSH hBrush = CreateSolidBrush(COLOR_OF_BG_BUTTON);
	SetTextColor(hDC, COLOR_OF_TEXT_CONTENT);

	RECT rcTemp = m_rcFriends;
	rcTemp.top += PAD_TOP_RECT;
	rcTemp.bottom = rcTemp.top+TEXT_LINE_HEIGHT;
	rcTemp.left += ROUND_WIDTH;
	rcTemp.right -= ROUND_WIDTH;
	wsprintf(buffer, L"%d", m_user.friends_count_);
	FillRect(hDC, &rcTemp, hBrush);
	DrawText(hDC, buffer, -1, &rcTemp, DT_SINGLELINE|DT_CENTER);

	rcTemp = m_rcStatuses;
	rcTemp.top += PAD_TOP_RECT;
	rcTemp.bottom = rcTemp.top+TEXT_LINE_HEIGHT;
	rcTemp.left += ROUND_WIDTH;
	rcTemp.right -= ROUND_WIDTH;
	wsprintf(buffer, L"%d", m_user.statuses_count_);
	FillRect(hDC, &rcTemp, hBrush);
	DrawText(hDC, buffer, -1, &rcTemp, DT_SINGLELINE|DT_CENTER);

	rcTemp = m_rcFollowers;
	rcTemp.top += PAD_TOP_RECT;
	rcTemp.bottom = rcTemp.top+TEXT_LINE_HEIGHT;
	rcTemp.left += ROUND_WIDTH;
	rcTemp.right -= ROUND_WIDTH;
	wsprintf(buffer, L"%d", m_user.followers_count_);
	FillRect(hDC, &rcTemp, hBrush);
	DrawText(hDC, buffer, -1, &rcTemp, DT_SINGLELINE|DT_CENTER);

	rcTemp = m_rcFavourites;
	rcTemp.top += PAD_TOP_RECT;
	rcTemp.bottom = rcTemp.top+TEXT_LINE_HEIGHT;
	rcTemp.left += ROUND_WIDTH;
	rcTemp.right -= ROUND_WIDTH;
	wsprintf(buffer, L"%d", m_user.favourites_count_);
	FillRect(hDC, &rcTemp, hBrush);
	DrawText(hDC, buffer, -1, &rcTemp, DT_SINGLELINE|DT_CENTER);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hBrush);
	DeleteObject(hFont);
}

void CM8WeiboUserWnd::RefreshCounts()
{
	M8::m8_wb_get_users_show user_info;
	if (theApp.GetUserInfoUnknown(m_hWnd, t_wb_uid::IDType_screenname, m_user.screen_name_, &user_info))
	{
		m_user.friends_count_ = user_info.user_.friends_count_;
		m_user.statuses_count_ = user_info.user_.statuses_count_;
		m_user.followers_count_ = user_info.user_.followers_count_;
		m_user.favourites_count_ = user_info.user_.favourites_count_;
		HDC hDC = m_scrool_canvas.m_dcCanvas.GetDC();
		OnDrawCounts(hDC);
	}
}

void CM8WeiboUserWnd::RefreshFriendships()
{
	M8::m8_wb_get_friendships_show friendships_exists;
	if (theApp.GetFriendshipsExists(m_hWnd, theApp.GetUserInfoSelf()->id_, m_user.id_, &friendships_exists))
	{
		isFriend = (friendships_exists.source_.following_[0] == '1');
		isFollower = (friendships_exists.source_.followed_by_[0] == '1');
		PostMessage(MZ_WM_COMMAND, MZ_IDC_USER_DRAW_FOLLOW, 0);
	}
}
