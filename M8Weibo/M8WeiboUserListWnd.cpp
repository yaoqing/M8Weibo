#include "M8WeiboUserListWnd.h"
#include "M8WeiboApp.h"
#include "M8WeiboHomeWnd.h"
#include "M8WeiboUserWnd.h"
#include "M8WeiboParameter.h"
#include "resource.h"
#include "StringW2A.h"
#include "StringA2W.h"

void CM8WeiboUserListCanvas::init()
{
	m_user_and_status = NULL;
	m_hWnd = NULL;
	m_color_bg = 0;
	memset(&m_rcUser, 0, sizeof(RECT));
}

int CM8WeiboUserListCanvas::OnLButtonDown(UINT fwKeys, int xPos, int yPos)
{
	::SendMessage(m_hWnd, MZ_WM_COMMAND, MZ_IDC_USER_LIST_CLICK_DOWN, (LPARAM)this);
	return UiCanvas::OnLButtonDown(fwKeys, xPos, yPos);
}

int CM8WeiboUserListCanvas::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	::SendMessage(m_hWnd, MZ_WM_COMMAND, MZ_IDC_USER_LIST_CLICK_UP, (LPARAM)this);
	SetTimer(m_hWnd, (UINT)m_user_and_status, 100, NULL);
	CM8WeiboUserListScroolWin *pWnd = (CM8WeiboUserListScroolWin *)GetParent();
	pWnd->m_dwTimerID = (UINT)m_user_and_status;
	return UiCanvas::OnLButtonUp(fwKeys, xPos, yPos);
}

int CM8WeiboUserListScroolWin::OnLButtonDown(UINT fwKeys, int xPos, int yPos)
{
	KillTimer(m_hWnd, m_dwTimerID);
	return UiScrollWin::OnLButtonDown(fwKeys, xPos, yPos);
}

MZ_IMPLEMENT_DYNAMIC(CM8WeiboUserListWnd)

CM8WeiboUserListWnd::CM8WeiboUserListWnd(void)
{
	m_user = NULL;
	m_user_type = FRIEND;

	m_size_of_user = 0;
	m_users_canvas = NULL;

	MZ_STATUS_COUNT_PER_LINE = 14;

	// 定义各种字体的颜色
	COLOR_OF_BG_PRESSED = RGB(96,152,214);
	COLOR_OF_FG_PRESSED = RGB(255,255,255);
	COLOR_OF_BG_STATUS_0 = RGB(239,243,255);
	COLOR_OF_BG_STATUS_1 = RGB(255,255,255);
	COLOR_OF_AUTHOR = RGB(84,141,212);
	COLOR_OF_STATUS = RGB(128,128,128);

	// 定义各种字体的行高
	LINE_HEIGHT_AUTHOR = 28;
	LINE_HEIGHT_STATUS = 26;
	SPACE_AUTHOR_AND_STATUS = 16;

	// 定义单个用户的内空白
	PAD_TOP_USER = 10;
	PAD_BOTTOM_USER = 10;
	PAD_LEFT_USER = 80;
	PAD_RIGHT_USER = 48;

	imgNormalGoback = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOBACK_BLUE, true);
	imgPressedGoback = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOBACK_GREEN, true);
	imgNormalGohome = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOHOME_BLUE, true);
	imgPressedGohome = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOHOME_GREEN, true);
	imgArrowGray = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_ARROW_GRAY, true);
	imgArrowWhite = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_ARROW_WHITE, true);
	imgTokenBoy = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_TOKEN_BOY, true);
	imgTokenGirl = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_TOKEN_GIRL, true);
}

CM8WeiboUserListWnd::~CM8WeiboUserListWnd(void)
{
	m_image_container.RemoveAll();
	RemoveAllUsers(t_wb_option_params::WBOPTType_unk);
}

BOOL CM8WeiboUserListWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
	{
		return FALSE;
	}

	int width = GetWidth();

	m_ctlCaption.SetID(MZ_IDC_USER_LIST_CAPTION);
	m_ctlCaption.SetPos(0, 0, width, MZM_HEIGHT_TEXT_TOOLBAR);
	m_ctlCaption.SetTextColor(RGB(89,89,89));
	m_ctlCaption.SetTextSize(30);
	m_ctlCaption.SetTextWeight(FW_MEDIUM);
	if (m_user_type == FRIEND) m_ctlCaption.SetText(L"关注");
	if (m_user_type == FOLLOWER) m_ctlCaption.SetText(L"粉丝");
	if (m_user_type == USER_SEARCH) m_ctlCaption.SetText(L"用户搜索");
	AddUiWin(&m_ctlCaption);

	m_button_goback.SetID(MZ_IDC_USER_LIST_GOBACK);
	m_button_goback.SetPos(24, 10, 48, 48);
	m_button_goback.SetButtonType(MZC_BUTTON_NONE);
	m_button_goback.SetImage_Normal(imgNormalGoback);
	m_button_goback.SetImage_Pressed(imgPressedGoback);
	m_ctlCaption.AddChild(&m_button_goback);

	m_button_gohome.SetID(MZ_IDC_USER_LIST_GOHOME);
	m_button_gohome.SetPos(width-24-48, 10, 48, 48);
	m_button_gohome.SetButtonType(MZC_BUTTON_NONE);
	m_button_gohome.SetImage_Normal(imgNormalGohome);
	m_button_gohome.SetImage_Pressed(imgPressedGohome);
	m_ctlCaption.AddChild(&m_button_gohome);

	m_scrool_win.SetID(MZ_IDC_USER_LIST_SCROOLWIN);
	if (m_user_type == USER_SEARCH)
		m_scrool_win.SetPos(0, MZM_HEIGHT_TEXT_TOOLBAR, width, GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR);
	else
		m_scrool_win.SetPos(0, MZM_HEIGHT_TEXT_TOOLBAR, width, GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR-MZM_HEIGHT_ICON_TOOLBAR);
	m_scrool_win.EnableScrollBarV(true);
	AddUiWin(&m_scrool_win);

	m_more_canvas.SetID(MZ_IDC_USER_LIST_MORE);
	m_more_canvas.SetPos(0,0,0,0);
	m_more_canvas.m_hWnd = m_hWnd;
	m_scrool_win.AddChild(&m_more_canvas);

	m_toolbar.SetID(MZ_IDC_USER_LIST_TOOLBAR);
	m_toolbar.SetPos(0, GetHeight()-MZM_HEIGHT_ICON_TOOLBAR, width, MZM_HEIGHT_ICON_TOOLBAR);
	m_toolbar.SetButton(0, false, IDB_PNG_BOOK_BLUE, IDB_PNG_BOOK_GREEN);
	if (m_user_type == FRIEND)
		m_toolbar.SetButton(2, false, IDB_PNG_HEART_BLUE, IDB_PNG_HEART_GREEN);
	else
		m_toolbar.SetButton(2, true, IDB_PNG_HEART_BLUE, IDB_PNG_HEART_GREEN);
	if (m_user_type == FOLLOWER)
		m_toolbar.SetButton(4, false, IDB_PNG_PUZZLE_BLUE, IDB_PNG_PUZZLE_GREEN);
	else
		m_toolbar.SetButton(4, true, IDB_PNG_PUZZLE_BLUE, IDB_PNG_PUZZLE_GREEN);
	if (m_user_type != USER_SEARCH) AddUiWin(&m_toolbar);

	m_scrool_win.m_hWnd = m_hWnd;
	SetTimer(m_hWnd, MZ_ID_USER_LIST_TIMER, t_wb_option_params::WBOPTType_unk, NULL);
	return TRUE;
}


void CM8WeiboUserListWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == MZ_ID_USER_LIST_TIMER)
	{
		KillTimer(m_hWnd, MZ_ID_USER_LIST_TIMER);
		PostMessage(MZ_WM_COMMAND, MZ_IDC_USER_LIST_WORKING, t_wb_option_params::WBOPTType_unk);
	}
	else if (nIDEvent == m_scrool_win.m_dwTimerID)
	{
		KillTimer(m_hWnd, m_scrool_win.m_dwTimerID);

		RECT rcWork = MzGetWorkArea();
		CM8WeiboUserWnd *pUserWnd = new CM8WeiboUserWnd();
		pUserWnd->SetUserInfo(&((M8::m8_user_and_status *)nIDEvent)->user_);
		pUserWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
		pUserWnd->Show();
		theApp.m_stack.push(pUserWnd);
	}
}

void CM8WeiboUserListWnd::OnMzCommand(WPARAM wParam, LPARAM lParam)
{
	UINT_PTR id = LOWORD(wParam);
	switch(id)
	{
	case MZ_IDC_USER_LIST_GOBACK:
		{
			DestroyWindow();
			delete this;
			theApp.PopWnd(this);
		}
		break;
	case MZ_IDC_USER_LIST_GOHOME:
		{
			theApp.GoHome();
		}
		break;
	case MZ_IDC_USER_LIST_WORKING:
		{
			int opt_type = lParam;
			M8::m8_wb_get_statuses_friends* pFriends = NULL;
			M8::m8_wb_get_statuses_followers* pFollowers = NULL;
			M8::m8_wb_users_search* pUserSearchs = NULL;
			if (m_user_type == FRIEND) if (!RefreshFriends(opt_type, pFriends)) break;
			if (m_user_type == FOLLOWER) if (!RefreshFollowers(opt_type, pFollowers)) break;
			if (m_user_type == USER_SEARCH) if (!RefreshUserSearchs(opt_type, pUserSearchs)) break;

			MzBeginWaitDlg(m_hWnd, NULL, true);
			m_scrool_win.ScrollTo();
			RemoveAllUsers(opt_type);
			if (m_user_type == FRIEND) AddFriends(opt_type, pFriends);
			if (m_user_type == FOLLOWER) AddFollowers(opt_type, pFollowers);
			if (m_user_type == USER_SEARCH) AddUserSearchs(opt_type, pUserSearchs);
			M8::m8_user_and_status **pUserArray = NULL;
			m_size_of_user = GetUserArray(pUserArray);
			DrawAllUsers(pUserArray);
			MoveMoreButton();
			delete [] pUserArray;
			MzEndWaitDlg();
		}
		break;
	case MZ_IDC_USER_LIST_CLICK_DOWN:
		{
			CM8WeiboUserListCanvas *p_canvas = (CM8WeiboUserListCanvas*)lParam;
			OnUserClick(p_canvas, true);
			Invalidate();
			UpdateWindow();
		}
		break;
	case MZ_IDC_USER_LIST_CLICK_UP:
		{
			CM8WeiboUserListCanvas *p_canvas = (CM8WeiboUserListCanvas*)lParam;
			OnUserClick(p_canvas, false);
			Invalidate();
			UpdateWindow();
		}
		break;
	case MZ_IDC_USER_TOOLBAR+1:
		{
			RECT rcWork = MzGetWorkArea();
			CM8WeiboHomeWnd *pHomeWnd = new CM8WeiboHomeWnd();
			pHomeWnd->m_user = m_user;
			pHomeWnd->m_user_type = CM8WeiboHomeWnd::THE_FRIEND;
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
			if (m_user_type == FRIEND)
			{
				RECT rcWork = MzGetWorkArea();
				CM8WeiboUserListWnd *pUserListWnd = new CM8WeiboUserListWnd();
				pUserListWnd->m_user = m_user;
				pUserListWnd->m_user_type = CM8WeiboUserListWnd::FOLLOWER;
				pUserListWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pUserListWnd->Show();
				DestroyWindow();
				delete this;
				theApp.PopWnd(this);
				theApp.m_stack.push(pUserListWnd);
			}
			else
			{
				PostMessage(MZ_WM_COMMAND, MZ_IDC_USER_LIST_WORKING, t_wb_option_params::WBOPTType_unk);
			}
		}
		break;
	case MZ_IDC_USER_TOOLBAR+5:
		{
			if (m_user_type == FOLLOWER)
			{
				RECT rcWork = MzGetWorkArea();
				CM8WeiboUserListWnd *pUserListWnd = new CM8WeiboUserListWnd();
				pUserListWnd->m_user = m_user;
				pUserListWnd->m_user_type = CM8WeiboUserListWnd::FRIEND;
				pUserListWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pUserListWnd->Show();
				DestroyWindow();
				delete this;
				theApp.PopWnd(this);
				theApp.m_stack.push(pUserListWnd);
			}
			else
			{
				PostMessage(MZ_WM_COMMAND, MZ_IDC_USER_LIST_WORKING, t_wb_option_params::WBOPTType_unk);
			}
		}
		break;
	}
}

int CM8WeiboUserListWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	/*switch(message)
	 {
	 }*/
	 return SHK_RET_APPNOEXIT_SHELLTOP;
}

bool CM8WeiboUserListWnd::RefreshFriends(int opt_type, M8::m8_wb_get_statuses_friends*& pUsers)
{
	int cursor = 0;
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		USERS front = m_list_users.front();
		if (front.size_of_user_ > 0) cursor = front.next_cursor_;
	}
	else if (opt_type == t_wb_option_params::WBOPTType_max_id)
	{
		if (m_list_users.size() > 0)
		{
			USERS back = m_list_users.back();
			if (back.size_of_user_ > 0) cursor = back.next_cursor_;
		}
	}
	if (!theApp.GetFriendsList(m_hWnd, m_user->id_, cursor, pUsers))
	{
		delete pUsers;
		return false;
	}
	if (pUsers->size_of_friend_ == 0)
	{
		delete pUsers;
		return false;
	}
	return true;
}

bool CM8WeiboUserListWnd::RefreshFollowers(int opt_type, M8::m8_wb_get_statuses_followers*& pUsers)
{
	int cursor = 0;
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		USERS front = m_list_users.front();
		if (front.size_of_user_ > 0) cursor = front.next_cursor_;
	}
	else if (opt_type == t_wb_option_params::WBOPTType_max_id)
	{
		if (m_list_users.size() > 0)
		{
			USERS back = m_list_users.back();
			if (back.size_of_user_ > 0) cursor = back.next_cursor_;
		}
	}
	if (!theApp.GetFollowersList(m_hWnd, m_user->id_, cursor, pUsers))
	{
		delete pUsers;
		return false;
	}
	if (pUsers->size_of_follower_ == 0)
	{
		delete pUsers;
		return false;
	}
	return true;
}

bool CM8WeiboUserListWnd::RefreshUserSearchs(int opt_type, M8::m8_wb_users_search*& pUsers)
{
	int page = 1;
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		USERS front = m_list_users.front();
		if (front.size_of_user_ > 0) page = front.prev_cursor_;
	}
	else if (opt_type == t_wb_option_params::WBOPTType_max_id)
	{
		if (m_list_users.size() > 0)
		{
			USERS back = m_list_users.back();
			if (back.size_of_user_ > 0) page = back.next_cursor_;
		}
	}
	if (!theApp.GetUserSearch(m_hWnd, CStringW2A(m_search_name).ptr(), page, pUsers))
	{
		delete pUsers;
		return false;
	}
	if (pUsers->size_of_user_ == 0)
	{
		delete pUsers;
		return false;
	}
	return true;
}

void CM8WeiboUserListWnd::RemoveAllUsers(int opt_type)
{
	// 删除现有的用户画布数组
	for (int i = 0; i < m_size_of_user; i++)
	{
		m_scrool_win.RemoveChild(m_users_canvas+i);
	}

	delete [] m_users_canvas;
	m_users_canvas = NULL;
	m_size_of_user = 0;

	// 不是查询最新用户，需要删除现有保存的数据，防止页面过长
	// 是最新用户，将新数据保存在现有数据前面
	if (opt_type != t_wb_option_params::WBOPTType_since_id)
	{
		std::list<USERS>::iterator iter;
		for (iter = m_list_users.begin(); iter != m_list_users.end(); iter++)
		{
			delete [] iter->users_;
		}
		m_list_users.clear();
	}
}

void CM8WeiboUserListWnd::AddFriends(int opt_type, M8::m8_wb_get_statuses_friends* pUsers)
{
	USERS users = {pUsers->size_of_friend_, pUsers->friends_, pUsers->prev_cursor_, pUsers->next_cursor_};
	pUsers->size_of_friend_ = 0;
	pUsers->friends_ = NULL;
	delete pUsers;

	// 根据OPT_TYPE添加用户数据
	// 不是查询最新用户，需要删除现有保存的数据，防止页面过长
	// 是最新用户，将新数据保存在现有数据前面
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		m_list_users.push_front(users);
	}
	else
	{
		m_list_users.push_back(users);
	}
}

void CM8WeiboUserListWnd::AddFollowers(int opt_type, M8::m8_wb_get_statuses_followers* pUsers)
{
	USERS users = {pUsers->size_of_follower_, pUsers->followers_, pUsers->prev_cursor_, pUsers->next_cursor_};
	pUsers->size_of_follower_ = 0;
	pUsers->followers_ = NULL;
	delete pUsers;

	// 根据OPT_TYPE添加用户数据
	// 不是查询最新用户，需要删除现有保存的数据，防止页面过长
	// 是最新用户，将新数据保存在现有数据前面
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		m_list_users.push_front(users);
	}
	else
	{
		m_list_users.push_back(users);
	}
}

void CM8WeiboUserListWnd::AddUserSearchs(int opt_type, M8::m8_wb_users_search* pUsers)
{
	USERS users = {pUsers->size_of_user_, pUsers->users_, pUsers->prev_cursor_, pUsers->next_cursor_};
	pUsers->size_of_user_ = 0;
	pUsers->users_ = NULL;
	delete pUsers;

	// 根据OPT_TYPE添加用户数据
	// 不是查询最新用户，需要删除现有保存的数据，防止页面过长
	// 是最新用户，将新数据保存在现有数据前面
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		m_list_users.push_front(users);
	}
	else
	{
		m_list_users.push_back(users);
	}
}

int CM8WeiboUserListWnd::GetUserArray(M8::m8_user_and_status **&pUserArray)
{
	int size = 0;
	int index = 0;
	pUserArray = NULL;
	std::list<USERS>::iterator iter;
	for (iter = m_list_users.begin(); iter != m_list_users.end(); iter++)
	{
		size += (*iter).size_of_user_;
	}

	pUserArray = new M8::m8_user_and_status*[size];

	for (iter = m_list_users.begin(); iter != m_list_users.end(); iter++)
	{
		for (int i = 0; i <(*iter).size_of_user_; i++)
		{
			pUserArray[index++] = (*iter).users_+i;
		}
	}
	return size;
}

void CM8WeiboUserListWnd::DrawAllUsers(M8::m8_user_and_status **pUserArray)
{
	m_users_canvas = new CM8WeiboUserListCanvas[m_size_of_user];
	for (int x = 0; x < m_size_of_user; x++)
	{
		M8::m8_user_and_status *pUser = pUserArray[x];

		// 按每行制定字符个数换行
		CMzString text = CStringA2W(pUser->status_.text_).ptr();
		if (m_user_type == USER_SEARCH)
		{
			wchar_t temp[32];
			wsprintf(temp, L"粉丝：%d", pUser->user_.followers_count_);
			text = temp;
		}
		int lenght = wcslen(text.C_Str());
		bool is_trunc = false;
		if (lenght > MZ_STATUS_COUNT_PER_LINE)
		{
			lenght = MZ_STATUS_COUNT_PER_LINE;
			is_trunc = true;
		}
		wchar_t buffer[32];
		wmemset(buffer, 0, 32);
		wmemcpy(buffer, text.C_Str(), lenght);
		if (is_trunc) wcscat(buffer, L"...");

		int width = GetWidth();

		// 定义用户宽度和高度
		FULL_WIDTH_OF_CANVAS = width;
		FULL_HEIGHT_OF_CANVAS = PAD_TOP_USER+LINE_HEIGHT_AUTHOR+SPACE_AUTHOR_AND_STATUS+LINE_HEIGHT_STATUS+PAD_BOTTOM_USER;

		// 用户所占块位置
		RECT rcUser;
		if (x == 0)
		{
			rcUser.left = 0;
			rcUser.top = 0;
			rcUser.right = FULL_WIDTH_OF_CANVAS;
			rcUser.bottom = FULL_HEIGHT_OF_CANVAS+rcUser.top;
		}
		else
		{
			CM8WeiboUserListCanvas *last_status = m_users_canvas+x-1;
			rcUser = last_status->m_rcUser;
			rcUser.top = rcUser.bottom;
			rcUser.bottom = FULL_HEIGHT_OF_CANVAS+rcUser.top;
		}
		m_users_canvas[x].m_hWnd = m_hWnd;
		m_users_canvas[x].m_rcUser = rcUser;
		m_users_canvas[x].m_user_and_status = pUser;
		m_users_canvas[x].m_color_bg = x%2 ? COLOR_OF_BG_STATUS_1 : COLOR_OF_BG_STATUS_0;

		// 创建用户块
		m_users_canvas[x].SetID(MZ_IDC_USER_LIST_USERS+x);
		m_users_canvas[x].SetPos(rcUser.left, rcUser.top, rcUser.right -rcUser.left, rcUser.bottom-rcUser.top);
		m_users_canvas[x].m_dcCanvas.Create(rcUser.right-rcUser.left, rcUser.bottom-rcUser.top);
		HDC hDC = m_users_canvas[x].m_dcCanvas.GetDC();
		OnDrawUser(hDC, rcUser, m_users_canvas[x].m_color_bg, false, pUser, buffer);

		m_scrool_win.AddChild(m_users_canvas+x);
	}
}


void CM8WeiboUserListWnd::MoveMoreButton()
{
	// 画更多按钮
	m_scrool_win.RemoveChild(&m_more_canvas);
	RECT rcLast;
	if (m_size_of_user > 0)
	{
		rcLast = m_users_canvas[m_size_of_user-1].m_rcUser;
		rcLast.top = rcLast.bottom;
		rcLast.bottom = rcLast.top+64;
	}
	else
	{
		rcLast.left = 0;
		rcLast.right = GetWidth();
		rcLast.top = 0;
		rcLast.bottom = rcLast.top+64;
	}
	m_more_canvas.SetID(MZ_IDC_USER_LIST_MORE);
	m_more_canvas.SetPos(rcLast.left, rcLast.top, rcLast.right-rcLast.left, rcLast.bottom-rcLast.top);
	m_more_canvas.m_hWnd = m_hWnd;
	m_scrool_win.AddChild(&m_more_canvas);
}

void CM8WeiboUserListWnd::OnDrawUser(HDC hDC, RECT rcUser, DWORD bg_color, bool pressed, M8::m8_user_and_status * pUser, wchar_t * text)
{
	LOGFONT font_struct={ 0, 0, 0, 0, 0, 0, 0, 0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"宋体"
	};

	// 填充用户背景
	RECT rcOrigin = rcUser;
	rcOrigin.right -= rcOrigin.left;
	rcOrigin.bottom -= rcOrigin.top;
	rcOrigin.left = 0;
	rcOrigin.top = 0;
	SetBkMode(hDC,TRANSPARENT);
	HBRUSH hBrush = CreateSolidBrush(pressed? COLOR_OF_BG_PRESSED : bg_color);
	FillRect(hDC, &rcOrigin, hBrush);
	DeleteObject(hBrush);

	// 画用户作者头像
	RECT rcToken;
	rcToken.left = (PAD_LEFT_USER-50)/2;
	rcToken.right = rcToken.left+50;
	rcToken.top = (rcUser.bottom-rcUser.top-50)/2;
	rcToken.bottom = rcToken.top+50;

	CMzString profile_image_path;
	CMzString profile_token_name = theApp.UpdateProfileTokenName(pUser->user_.id_, pUser->user_.profile_image_url_, profile_image_path);
	CMzString profile_image_name = theApp.FindTokenFileName(profile_image_path, profile_token_name);
	if (profile_image_name.Length() > 0 || theApp.m_parameter->getAutoDownloadProfileImage())
	{
		if (profile_image_name.Length() == 0)
		{
			theApp.DownloadTokenFile(m_hWnd, pUser->user_.id_, true, pUser->user_.profile_image_url_, profile_image_path, profile_token_name);
			profile_image_name = profile_image_path + profile_token_name;
		}
		ImagingHelper::DrawImage(hDC, &rcToken, profile_image_name.C_Str());
	}
	else
	{
		if (pUser->user_.gender_[0] == 'm')
			imgTokenBoy->Draw(hDC, &rcToken);
		else
			imgTokenGirl->Draw(hDC, &rcToken);
	}

	// 初始化偏移量
	int offsetLeft = PAD_LEFT_USER;
	int offsetTop = PAD_TOP_USER;
	int offsetRight = PAD_RIGHT_USER;
	int offsetBottom = PAD_BOTTOM_USER;

	// 作者所占块位置
	RECT rcAuthor;
	rcAuthor.left = rcOrigin.left+offsetLeft;
	rcAuthor.top = rcOrigin.top+offsetTop;
	rcAuthor.right = rcOrigin.right-offsetRight;
	rcAuthor.bottom = rcAuthor.top+LINE_HEIGHT_AUTHOR;

	// 输出作者
	font_struct.lfHeight = LINE_HEIGHT_AUTHOR;
	HFONT hFont = CreateFontIndirect(&font_struct);
	HGDIOBJ hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, pressed? COLOR_OF_FG_PRESSED: COLOR_OF_AUTHOR);
	CStringA2W info(pUser->user_.screen_name_);
	DrawText(hDC, info.ptr(), -1, &rcAuthor, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	// 内容所占块位置
	RECT rcText = rcAuthor;
	rcText.top += LINE_HEIGHT_AUTHOR+SPACE_AUTHOR_AND_STATUS;
	rcText.bottom = rcText.top+LINE_HEIGHT_STATUS;

	// 输出内容
	font_struct.lfHeight = LINE_HEIGHT_STATUS;
	hFont = CreateFontIndirect(&font_struct);
	hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, pressed? COLOR_OF_FG_PRESSED: COLOR_OF_STATUS);
	DrawText(hDC, text, -1, &rcText, DT_SINGLELINE|DT_LEFT);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	if (m_user_type == USER_SEARCH) return;

	// 画右箭头
	RECT rcArrow=rcUser;
	OffsetRect(&rcArrow, -rcArrow.left, -rcArrow.top);
	rcArrow.top += (rcArrow.bottom-rcArrow.top-28)/2;
	rcArrow.bottom = rcArrow.top+28;
	rcArrow.right -= (PAD_RIGHT_USER-28)/2;
	rcArrow.left = rcArrow.right-28;
	if (pressed)
	{
		imgArrowWhite->Draw(hDC, &rcArrow);
	} 
	else
	{
		imgArrowGray->Draw(hDC, &rcArrow);
	}
}

void CM8WeiboUserListWnd::OnUserClick(CM8WeiboUserListCanvas * p_canvas, bool pressed)
{
	// 按每行制定字符个数换行
	CMzString text = CStringA2W(p_canvas->m_user_and_status->status_.text_).ptr();
	if (m_user_type == USER_SEARCH)
	{
		wchar_t temp[32];
		wsprintf(temp, L"粉丝：%d", p_canvas->m_user_and_status->user_.followers_count_);
		text = temp;
	}
	int lenght = wcslen(text.C_Str());
	bool is_trunc = false;
	if (lenght > MZ_STATUS_COUNT_PER_LINE)
	{
		lenght = MZ_STATUS_COUNT_PER_LINE;
		is_trunc = true;
	}
	wchar_t buffer[32];
	wmemset(buffer, 0, 32);
	wmemcpy(buffer, text.C_Str(), lenght);
	if (is_trunc) wcscat(buffer, L"...");

	HDC hDC = p_canvas->m_dcCanvas.GetDC();
	OnDrawUser(hDC, p_canvas->m_rcUser, p_canvas->m_color_bg, pressed, p_canvas->m_user_and_status, buffer);
}