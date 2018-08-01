#include "M8WeiboHomeWnd.h"
#include "M8WeiboApp.h"
#include "M8WeiboFinderWnd.h"
#include "M8WeiboStatusesWnd.h"
#include "M8WeiboUserListWnd.h"
#include "M8WeiboUserWnd.h"
#include "M8WeiboWriteWnd.h"
#include "M8WeiboSettingsWnd.h"
#include "M8WeiboParameter.h"
#include "resource.h"
#include "StringW2A.h"
#include "StringA2W.h"

void CM8WeiboHomeCanvas::init()
{
	m_status = NULL;
	m_hWnd = NULL;
	memset(&m_rcStatus, 0, sizeof(RECT));
}

int CM8WeiboHomeCanvas::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	SetTimer(m_hWnd, (UINT)m_status, 100, NULL);
	CM8WeiboHomeScroolWin *pWnd = (CM8WeiboHomeScroolWin *)GetParent();
	pWnd->m_dwTimerID = (UINT)m_status;
	return UiCanvas::OnLButtonUp(fwKeys, xPos, yPos);
}

int CM8WeiboHomeScroolWin::OnLButtonDown(UINT fwKeys, int xPos, int yPos)
{
	KillTimer(m_hWnd, m_dwTimerID);
	return UiScrollWin::OnLButtonDown(fwKeys, xPos, yPos);
}

MZ_IMPLEMENT_DYNAMIC(CM8WeiboHomeWnd)

CM8WeiboHomeWnd::CM8WeiboHomeWnd(void)
{
	m_user = NULL;
	m_user_type = ALL_FRIENDS;
	MZ_STATUS_COUNT_PER_LINE = 15;

	COLOR_OF_BG_STATUS = RGB(239,239,239);
	COLOR_OF_BG_CONTENT = RGB(255,255,255);
	COLOR_OF_BG_CONTENT_SELF = RGB(239,243,255);
	COLOR_OF_EDG_CONTENT = RGB(208,208,208);//(189,228,215);
	COLOR_OF_EDG_CONTENT_SELF = RGB(181,198,222);
	COLOR_OF_TEXT_CONTENT = RGB(89,89,89);
	COLOR_OF_TEXT_AUTHOR = RGB(84,141,212);
	COLOR_OF_TEXT_CREATE_AT = RGB(212,141,84);
	COLOR_OF_TEXT_SOURCE = RGB(196,189,151);
	COLOR_OF_TEXT_COUNTS = RGB(216,216,216);
	COLOR_OF_RT_TEXT_CONTENT = RGB(127,127,127);
	COLOR_OF_RT_LINE = RGB(216,216,216);

	// 定义各种字体的行高
	TEXT_LINE_HEIGHT = 26;
	AUTHOR_LINE_HEIGHT = 26;
	CREATE_AT_LINE_HEIGHT = 22;
	SOURCE_LINE_HEIGHT = 22;
	COUNTS_LINE_HEIGHT = 22;
	SPACE_LINE_HEIGHT = 3;
	RT_TEXT_LINE_HEIGHT = 25;
	RT_SPACE_LINE_HEIGHT = 2;

	// 定义左三角形的边长
	DELTA_LENGTH = 28;
	// 定义头像的边长
	TOKEN_LENGTH = 50;

	// 定义单条微博的外空白
	MARGIN_TOP_STATUS = 3;
	MARGIN_BOTTOM_STATUS = 3;
	MARGIN_LEFT_STATUS = 0;
	MARGIN_RIGHT_STATUS = 0;

	// 定义单条微博的内空白
	PAD_TOP_STATUS = 0;
	PAD_BOTTOM_STATUS = 0;
	PAD_LEFT_STATUS = 64;
	PAD_RIGHT_STATUS = 0;

	// 定义微博正文的外空白
	MARGIN_TOP_CONTENT = 3;
	MARGIN_BOTTOM_CONTENT = 1;
	MARGIN_LEFT_CONTENT = 16;
	MARGIN_RIGHT_CONTENT = 9;

	// 定义微博正文的内空白
	PAD_TOP_CONTENT = AUTHOR_LINE_HEIGHT*2;
	PAD_BOTTOM_CONTENT = SOURCE_LINE_HEIGHT*1.5;
	PAD_LEFT_CONTENT = 16;
	PAD_RIGHT_CONTENT = 0;

	// 定义作者的左空白、创建时间右空白和宽度、图标和创建时间之间的间隔
	MARGIN_LEFT_AUTHOR = 16;
	MARGIN_RIGHT_CREATED_AT = 12;
	WIDTH_OF_CREATED_AT = 108;
	SPACE_OF_ICON_AND_CREATED_AT = 3;

	// 定义来源的左空白、引用和评论的右空白和宽度、图标和统计数之间的间隔
	MARGIN_LEFT_SORUCE = 16;
	MARGIN_RIGHT_OTHER = 12;
	WIDTH_OF_OTHER = 136;
	SPACE_OF_ICON_AND_COUNTS = 3;

	// 定义引用微博的左缩进、上空白和下空白（为简化代码，不设置内衬大小）
	RT_TOP_HEIGHT = 18;
	RT_BOTTOM_HEIGHT = 3;
	RT_TAB_WIDTH = 16;

	// 定义缩略图的上空白、下空白、左空白、固定宽度和高度
	THUMB_TOP_HEIGHT = 10;
	THUMB_BOTTOM_HEIGHT = 10;
	THUMB_LEFT_PAD = 20;
	THUMB_FIX_WIDTH = 100;
	THUMB_FIX_HEIGHT = 100;

	imgNormalWrite = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_WRITE_BLUE, true);
	imgNormalRefresh = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_REFRESH_BLUE, true);
	imgPressedWrite = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_WRITE_GREEN, true);
	imgPressedRefresh = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_REFRESH_GREEN, true);
	imgNormalGoback = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOBACK_BLUE, true);
	imgPressedGoback = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOBACK_GREEN, true);
	imgNormalGohome = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOHOME_BLUE, true);
	imgPressedGohome = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOHOME_GREEN, true);
	imgTokenBoy = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_TOKEN_BOY, true);
	imgTokenGirl = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_TOKEN_GIRL, true);
	imgSmallPicture = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_SMALL_PICTURE, true);
	imgSmallComment = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_SMALL_COMMENT_BLUE, true);
	imgSmallLink = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_SMALL_REPOST_BLUE, true);
	imgNormalAtme = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_ATME_BLUE, true);
	imgNormalTome = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_TOME_BLUE, true);
	imgPressedAtme = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_ATME_GREEN, true);
	imgPressedTome = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_TOME_GREEN, true);

}

CM8WeiboHomeWnd::~CM8WeiboHomeWnd(void)
{
	RemoveAllStatus(t_wb_option_params::WBOPTType_unk);
}

BOOL CM8WeiboHomeWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
	{
		return FALSE;
	}

	int width = GetWidth();
	m_toolbar.SetID(MZ_IDC_HOME_TOOLBAR);
	m_toolbar.SetPos(0, GetHeight()-MZM_HEIGHT_ICON_TOOLBAR, width, MZM_HEIGHT_ICON_TOOLBAR);
	if (m_user_type == ALL_FRIENDS || m_user_type == MENTIONS || m_user_type == COMMENT_TOME)
	{
		m_toolbar.SetButton(0, false, IDB_PNG_HOME_BLUE, IDB_PNG_HOME_GREEN);
		if (m_user_type == MENTIONS || m_user_type == COMMENT_TOME)
			m_toolbar.SetButton(1, true, IDB_PNG_MESSAGE_BLUE, IDB_PNG_MESSAGE_GREEN);
		else
			m_toolbar.SetButton(1, false, IDB_PNG_MESSAGE_BLUE, IDB_PNG_MESSAGE_GREEN);
		m_toolbar.SetButton(2, false, IDB_PNG_USER_BLUE, IDB_PNG_USER_GREEN);
		m_toolbar.SetButton(3, false, IDB_PNG_MORE_BLUE, IDB_PNG_MORE_GREEN);
		m_toolbar.SetButton(4, false, IDB_PNG_SETTINGS_BLUE, IDB_PNG_SETTINGS_GREEN);
		AddUiWin(&m_toolbar);
	}
	else if (m_user_type == THE_FRIEND || m_user_type == FAVOURITE)
	{
		m_toolbar.SetButton(0, true, IDB_PNG_BOOK_BLUE, IDB_PNG_BOOK_GREEN);
		m_toolbar.SetButton(2, false, IDB_PNG_HEART_BLUE, IDB_PNG_HEART_GREEN);
		m_toolbar.SetButton(4, false, IDB_PNG_PUZZLE_BLUE, IDB_PNG_PUZZLE_GREEN);
		AddUiWin(&m_toolbar);
	}
	else if (m_user_type == WB_SEARCH)
	{
		;
	}

	m_scrool_win.SetID(MZ_IDC_HOME_SCROOLWIN);
	if (m_user_type == WB_SEARCH)
		m_scrool_win.SetPos(0, MZM_HEIGHT_TEXT_TOOLBAR, width, GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR);
	else
		m_scrool_win.SetPos(0, MZM_HEIGHT_TEXT_TOOLBAR, width, GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR-MZM_HEIGHT_ICON_TOOLBAR);
	m_scrool_win.EnableScrollBarV(true);
	AddUiWin(&m_scrool_win);

	m_more_canvas.SetID(MZ_IDC_HOME_MORE);
	m_more_canvas.SetPos(0,0,0,0);
	m_more_canvas.m_hWnd = m_hWnd;
	m_scrool_win.AddChild(&m_more_canvas);

	m_ctlCaption.SetID(MZ_IDC_HOME_CAPTION);
	m_ctlCaption.SetPos(0, 0, width, MZM_HEIGHT_TEXT_TOOLBAR);
	m_ctlCaption.SetTextColor(RGB(89,89,89));
	m_ctlCaption.SetTextSize(30);
	m_ctlCaption.SetTextWeight(FW_MEDIUM);
	if (m_user_type == ALL_FRIENDS)
	{
		m_ctlCaption.SetText(CStringA2W(m_user->screen_name_).ptr());
	}
	else if (m_user_type == THE_FRIEND)
	{
		m_ctlCaption.SetText(L"微博");
	}
	else if (m_user_type == FAVOURITE)
	{
		m_ctlCaption.SetText(L"收藏");
	}
	else if (m_user_type == MENTIONS)
	{
		m_ctlCaption.SetText(L"@我");
	}
	else if (m_user_type == COMMENT_TOME)
	{
		m_ctlCaption.SetText(L"回复我");
	}
	else if (m_user_type == WB_SEARCH)
	{
		m_ctlCaption.SetText(L"微博搜索");
	}
	AddUiWin(&m_ctlCaption);

	m_button_write.SetID(MZ_IDC_HOME_WRITE);
	m_button_write.SetPos(24, 10, 48, 48);
	m_button_write.SetButtonType(MZC_BUTTON_NONE);
	if (m_user_type == ALL_FRIENDS)
	{
		m_button_write.SetImage_Normal(imgNormalWrite);
		m_button_write.SetImage_Pressed(imgPressedWrite);
	}
	else if (m_user_type == THE_FRIEND || m_user_type == FAVOURITE || m_user_type == WB_SEARCH)
	{
		m_button_write.SetImage_Normal(imgNormalGoback);
		m_button_write.SetImage_Pressed(imgPressedGoback);
	}
	else if (m_user_type == MENTIONS)
	{
		m_button_write.SetImage_Normal(imgPressedAtme);
		m_button_write.SetImage_Pressed(imgNormalAtme);
	}
	else if (m_user_type == COMMENT_TOME)
	{
		m_button_write.SetImage_Normal(imgNormalAtme);
		m_button_write.SetImage_Pressed(imgPressedAtme);
	}
	m_ctlCaption.AddChild(&m_button_write);

	m_button_refresh.SetID(MZ_IDC_HOME_REFRESH);
	m_button_refresh.SetPos(width-24-48, 10, 48, 48);
	m_button_refresh.SetButtonType(MZC_BUTTON_NONE);
	if (m_user_type == ALL_FRIENDS)
	{
		m_button_refresh.SetImage_Normal(imgNormalRefresh);
		m_button_refresh.SetImage_Pressed(imgPressedRefresh);
	}
	else if (m_user_type == THE_FRIEND || m_user_type == FAVOURITE || m_user_type == WB_SEARCH)
	{
		m_button_refresh.SetImage_Normal(imgNormalGohome);
		m_button_refresh.SetImage_Pressed(imgPressedGohome);
	}
	else if (m_user_type == MENTIONS)
	{
		m_button_refresh.SetImage_Normal(imgNormalTome);
		m_button_refresh.SetImage_Pressed(imgPressedTome);
	}
	else if (m_user_type == COMMENT_TOME)
	{
		m_button_refresh.SetImage_Normal(imgPressedTome);
		m_button_refresh.SetImage_Pressed(imgNormalTome);
	}
	m_ctlCaption.AddChild(&m_button_refresh);

	m_scrool_win.m_hWnd = m_hWnd;
	SetTimer(m_hWnd, MZ_ID_HOME_TIMER, t_wb_option_params::WBOPTType_unk, NULL);

	return TRUE;
}

void CM8WeiboHomeWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == MZ_ID_HOME_TIMER)
	{
		KillTimer(m_hWnd, MZ_ID_HOME_TIMER);

		// 从文件中反序列化
		int size = 0;
		M8::m8_status **p_status_list = NULL;
		if (m_user_type == ALL_FRIENDS && theApp.UnSerial(theApp.GetUserInfoSelf()->id_, size, p_status_list))
		{
			MzBeginWaitDlg(m_hWnd, NULL, true);

			STATUSES statuses = {size, *p_status_list, 0};
			m_list_statuses.push_back(statuses);
			
			// 生成微博及更多按钮
			DrawAllStatuses(p_status_list, size);
			MoveMoreButton();

			// 回收内存
			delete [] p_status_list;

			MzEndWaitDlg();
		}
		else
		{
			PostMessage(MZ_WM_COMMAND, MZ_IDC_HOME_WORKING, t_wb_option_params::WBOPTType_unk);
		}
	}
	else if (nIDEvent == m_scrool_win.m_dwTimerID)
	{
		KillTimer(m_hWnd, m_scrool_win.m_dwTimerID);

		RECT rcWork = MzGetWorkArea();
		CM8WeiboStatusesWnd *pStatusesWnd = new CM8WeiboStatusesWnd();
		if (m_user_type == COMMENT_TOME)
			pStatusesWnd->m_status = ((M8::m8_status *)nIDEvent)->retweeted_status_;
		else
			pStatusesWnd->m_status = (M8::m8_status *)nIDEvent;
		pStatusesWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
		pStatusesWnd->Show();
		theApp.m_stack.push(pStatusesWnd);
	}
}

void CM8WeiboHomeWnd::OnMzCommand( WPARAM wParam, LPARAM lParam )
{
 	UINT_PTR id = LOWORD(wParam);
 	switch(id)
 	{
	case MZ_IDC_HOME_WRITE:
		{
			if (m_user_type == ALL_FRIENDS)
			{
				RECT rcWork = MzGetWorkArea();
				CM8WeiboWriteWnd *pWriteWnd = new CM8WeiboWriteWnd();
				pWriteWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pWriteWnd->Show();
				theApp.m_stack.push(pWriteWnd);
			}
			else if (m_user_type == THE_FRIEND || m_user_type == FAVOURITE || m_user_type == WB_SEARCH)
			{
				DestroyWindow();
				delete this;
				theApp.PopWnd(this);
			}
			else if (m_user_type == MENTIONS)
			{
			}
			else if (m_user_type == COMMENT_TOME)
			{
				DestroyWindow();
				delete this;
				theApp.PopWnd(this);

				RECT rcWork = MzGetWorkArea();
				CM8WeiboHomeWnd *pHomeWnd = new CM8WeiboHomeWnd();
				pHomeWnd->m_user = theApp.GetUserInfoSelf();
				pHomeWnd->m_user_type = CM8WeiboHomeWnd::MENTIONS;
				pHomeWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pHomeWnd->Show();
				theApp.m_stack.push(pHomeWnd);
			}
		}
		break;
	case MZ_IDC_HOME_REFRESH:
		{
			if (m_user_type == ALL_FRIENDS)
			{
				PostMessage(MZ_WM_COMMAND, MZ_IDC_HOME_WORKING, t_wb_option_params::WBOPTType_since_id);
			}
			else if (m_user_type == THE_FRIEND || m_user_type == FAVOURITE || m_user_type == WB_SEARCH)
			{
				theApp.GoHome();
			}
			else if (m_user_type == MENTIONS)
			{
				DestroyWindow();
				delete this;
				theApp.PopWnd(this);

				RECT rcWork = MzGetWorkArea();
				CM8WeiboHomeWnd *pHomeWnd = new CM8WeiboHomeWnd();
				pHomeWnd->m_user = theApp.GetUserInfoSelf();
				pHomeWnd->m_user_type = CM8WeiboHomeWnd::COMMENT_TOME;
				pHomeWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pHomeWnd->Show();
				theApp.m_stack.push(pHomeWnd);
			}
			else if (m_user_type == COMMENT_TOME)
			{
			}
		}
		break;
	case MZ_IDC_HOME_WORKING:
		{
			//M8::m8_wb_get_users_show tmp_user_info;
			//theApp.QueryUserInfoUnknown(m_hWnd, t_wb_uid::IDType_screenname, "", &tmp_user_info);

			int opt_type = lParam;
			M8::m8_wb_get_statuses_friends_timeline *p_statuses_friends_timeline = NULL;
			M8::m8_wb_get_statuses_user_timeline *p_statuses_user_timeline = NULL;
			M8::m8_wb_get_favorites *p_favorites = NULL;
			M8::m8_wb_get_statuses_mentions *p_mentions = NULL;
			M8::m8_wb_get_statuses_comments_tome *p_comments_tome = NULL;
			M8::m8_wb_search *p_weibo_search = NULL;
			if (m_user_type == ALL_FRIENDS) if (!RefreshFriendsStatus(opt_type, p_statuses_friends_timeline)) break;
			if (m_user_type == THE_FRIEND) if (!RefreshUserStatus(opt_type, p_statuses_user_timeline)) break;
			if (m_user_type == FAVOURITE) if (!RefreshFavorites(opt_type, p_favorites)) break;
			if (m_user_type == MENTIONS) if (!RefreshMentions(opt_type, p_mentions)) break;
			if (m_user_type == COMMENT_TOME) if (!RefreshCommentsToMe(opt_type, p_comments_tome)) break;
			if (m_user_type == WB_SEARCH) if (!RefreshWeiboSearch(opt_type, p_weibo_search)) break;

			MzBeginWaitDlg(m_hWnd, NULL, true);
			m_scrool_win.ScrollTo();
			RemoveAllStatus(opt_type);

			if (m_user_type == ALL_FRIENDS) AddFriendsStatus(opt_type, p_statuses_friends_timeline);
			if (m_user_type == THE_FRIEND) AddUserStatus(opt_type, p_statuses_user_timeline);
			if (m_user_type == FAVOURITE) AddFavorites(opt_type, p_favorites);
			if (m_user_type == MENTIONS) AddMentions(opt_type, p_mentions);
			if (m_user_type == COMMENT_TOME) AddCommentsToMe(opt_type, p_comments_tome);
			if (m_user_type == WB_SEARCH) AddWeiboSearch(opt_type, p_weibo_search);
			
			// 计算微博总数
			M8::m8_status **p_status_list = NULL;
			int size_of_status = GetStatusArray(p_status_list);

			// 生成微博及更多按钮
			DrawAllStatuses(p_status_list, size_of_status);
			MoveMoreButton();

			// 回收内存
			delete [] p_status_list;

			MzEndWaitDlg();
		}
		break;
	case MZ_IDC_HOME_TOOLBAR+1:
		{
			if (m_user_type == ALL_FRIENDS || m_user_type == THE_FRIEND || m_user_type == FAVOURITE)
			{
				PostMessage(MZ_WM_COMMAND,  MZ_IDC_HOME_WORKING, t_wb_option_params::WBOPTType_since_id);
			}
			else if (m_user_type == MENTIONS || m_user_type == COMMENT_TOME)
			{
				theApp.GoHome();
			}
		}
		break;
	case MZ_IDC_HOME_TOOLBAR+2:
		{
			if (m_user_type == ALL_FRIENDS)
			{
				RECT rcWork = MzGetWorkArea();
				CM8WeiboHomeWnd *pHomeWnd = new CM8WeiboHomeWnd();
				pHomeWnd->m_user = theApp.GetUserInfoSelf();
				pHomeWnd->m_user_type = CM8WeiboHomeWnd::MENTIONS;
				pHomeWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pHomeWnd->Show();
				theApp.m_stack.push(pHomeWnd);
			}
			else if (m_user_type == THE_FRIEND)
			{
			}
			else if (m_user_type == FAVOURITE)
			{
			}
			else if (m_user_type == MENTIONS)
			{
			}
			else if (m_user_type == COMMENT_TOME)
			{
			}
		}
		break;
	case MZ_IDC_HOME_TOOLBAR+3:
		{
			if (m_user_type == ALL_FRIENDS || m_user_type == MENTIONS || m_user_type == COMMENT_TOME)
			{
				RECT rcWork = MzGetWorkArea();
				CM8WeiboUserWnd *pUserWnd = new CM8WeiboUserWnd();
				pUserWnd->SetUserInfo(theApp.GetUserInfoSelf());
				pUserWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pUserWnd->Show();
				if (m_user_type == MENTIONS) 
				{
					DestroyWindow();
					delete this;
					theApp.PopWnd(this);
				}
				theApp.m_stack.push(pUserWnd);
			}
			else if (m_user_type == THE_FRIEND || m_user_type == FAVOURITE)
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
		}
		break;
	case MZ_IDC_HOME_TOOLBAR+4:
		{
			if (m_user_type == ALL_FRIENDS || m_user_type == MENTIONS || m_user_type == COMMENT_TOME)
			{
				RECT rcWork = MzGetWorkArea();
				CM8WeiboFinderWnd *pFinderWnd = new CM8WeiboFinderWnd();
				pFinderWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pFinderWnd->Show();
				if (m_user_type == MENTIONS)
				{
					DestroyWindow();
					delete this;
					theApp.PopWnd(this);
				}
				theApp.m_stack.push(pFinderWnd);
			}
			else if (m_user_type == THE_FRIEND || m_user_type == FAVOURITE)
			{
			}
		}
		break;
	case MZ_IDC_HOME_TOOLBAR+5:
		{
			if (m_user_type == ALL_FRIENDS || m_user_type == MENTIONS || m_user_type == COMMENT_TOME)
			{
				RECT rcWork = MzGetWorkArea();
				CM8WeiboSettingsWnd *pSettingsWnd = new CM8WeiboSettingsWnd();
				pSettingsWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pSettingsWnd->Show();
				theApp.m_stack.push(pSettingsWnd);
			}
			else if (m_user_type == THE_FRIEND || m_user_type == FAVOURITE)
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
		}
		break;
	}
}

void CM8WeiboHomeWnd::DrawRetweetedStatus(M8::m8_status *retweeted_status, HDC hDC, RECT rcOrigin, int offsetLeft, int offsetTop, int offsetRight)
{
	 // 按每行制定字符个数换行
	 char *p_text = new char[WB_REAL_LEN(SCREEN_NAME) + WB_REAL_LEN(WB_INFO)];
	 sprintf(p_text, "%s：%s", retweeted_status->user_->screen_name_, retweeted_status->text_);
	 CStringA2W text(p_text);
	 int lines = theApp.SplitString(text.ptr(), MZ_STATUS_COUNT_PER_LINE);
	 delete [] p_text;
	 CMzString *p_lines =  new CMzString[lines];
	 theApp.SplitString(lines, p_lines, text.ptr(), MZ_STATUS_COUNT_PER_LINE);

	 LOGFONT font_struct={ 0, 0, 0, 0, 0, 0, 0, 0,
		 ANSI_CHARSET,
		 OUT_DEFAULT_PRECIS,
		 CLIP_DEFAULT_PRECIS,
		 DEFAULT_QUALITY,
		 DEFAULT_PITCH,
		 L"宋体"
	 };

	 // 输出引用换行占位符
	 HPEN hPen = CreatePen(PS_SOLID, 2, COLOR_OF_RT_LINE);
	 HGDIOBJ hSavedPen = SelectObject(hDC, hPen);
	 MoveToEx(hDC, rcOrigin.left+offsetLeft-RT_TAB_WIDTH, rcOrigin.top+offsetTop+RT_TOP_HEIGHT/2, NULL);
	 LineTo(hDC, rcOrigin.right-offsetRight-RT_TAB_WIDTH, rcOrigin.top+offsetTop+RT_TOP_HEIGHT/2);
	 SelectObject(hDC, hSavedPen);
	 DeleteObject(hPen);

	 offsetTop += RT_TOP_HEIGHT;

	 // 引用正文所占块位置
	 RECT rcText;
	 rcText.left = rcOrigin.left+offsetLeft;
	 rcText.right = rcOrigin.right-offsetRight;
	 rcText.top = rcOrigin.top+offsetTop;
	 rcText.bottom = rcText.top+RT_TEXT_LINE_HEIGHT;

	 // 输出内容
	 font_struct.lfHeight = RT_TEXT_LINE_HEIGHT;
	 HFONT hFont = CreateFontIndirect(&font_struct);
	 HGDIOBJ hSavedFont = SelectObject(hDC, hFont);
	 SetTextColor(hDC, COLOR_OF_RT_TEXT_CONTENT);
	 for (int i = 0; i < lines; i++)
	 {
		 DrawText(hDC, p_lines[i].C_Str(), -1, &rcText, DT_SINGLELINE|DT_LEFT);
		 rcText.top += RT_TEXT_LINE_HEIGHT+RT_SPACE_LINE_HEIGHT;
		 rcText.bottom = rcText.top+RT_TEXT_LINE_HEIGHT;
		 offsetTop += RT_TEXT_LINE_HEIGHT+RT_SPACE_LINE_HEIGHT;
	 }
	 offsetTop -= RT_SPACE_LINE_HEIGHT;
	 SelectObject(hDC, hSavedFont);
	 DeleteObject(hFont);

	 delete [] p_lines;

	 // 画缩略图
	 if (theApp.m_parameter->getAutoDownloadSmallPicture() && *retweeted_status->thumbnail_pic_ && !theApp.IsGif(retweeted_status->thumbnail_pic_))
	 {
		 RECT rcThumb;
		 rcThumb.left = rcText.left+THUMB_LEFT_PAD;
		 rcThumb.right = rcThumb.left+THUMB_FIX_WIDTH;
		 rcThumb.top = rcText.bottom+THUMB_TOP_HEIGHT-RT_TEXT_LINE_HEIGHT-RT_SPACE_LINE_HEIGHT;
		 rcThumb.bottom = rcThumb.top+THUMB_FIX_HEIGHT;

		 CMzString small_picture_path;
		 CMzString picture_token_name = theApp.UpdateSmallPictureTokenName(retweeted_status->user_->id_, retweeted_status->thumbnail_pic_, small_picture_path);
		 CMzString small_picure_name = theApp.FindTokenFileName(small_picture_path, picture_token_name);
		 if (small_picure_name.IsEmpty())
		 {
			 if(theApp.DownloadTokenFile(m_hWnd, retweeted_status->user_->id_, false, retweeted_status->thumbnail_pic_, small_picture_path, picture_token_name))
			 {
				 small_picure_name = small_picture_path+picture_token_name;
			 }
		 }
		 ImagingHelper img_helper;
		 img_helper.LoadImage(small_picure_name.C_Str(), false, false, true);
		 img_helper.Draw(hDC, &rcThumb, true, false);
		 img_helper.UnloadImage();
	 }
/*
	 if (retweeted_status->retweeted_status_)
	 {
		 DrawRetweetedStatus(retweeted_status->retweeted_status_, hDC, rcOrigin, offsetLeft, offsetTop, offsetRight);
	 }

	 int height = lines*RT_TEXT_LINE_HEIGHT+RT_TOP_HEIGHT+RT_BOTTOM_HEIGHT;
	 return (retweeted_status->retweeted_status_)? height+DrawRetweetedStatus(retweeted_status->retweeted_status_, offsetLeft, offsetTop, offsetRight): height;
*/
}

LRESULT  CM8WeiboHomeWnd::MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam)
 {
	 switch(message)
	 {
	 case WM_COPYDATA:
		 {
			 if (wParam == -1) theApp.ShowAll();
		 }
		 break;
	 case MZ_WM_TASKMAN_ONOFF:
		 {
			 if (LOWORD(lParam) == TASKMAN_EXEC_PROG) theApp.ShowAll();
		 }
		 break;
	 }
	 return CMzWndEx::MzDefWndProc(message,wParam,lParam);
 }

int CM8WeiboHomeWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	 /*switch(message)
	 {
	 }*/
	 return SHK_RET_APPNOEXIT_SHELLTOP;
}

void CM8WeiboHomeWnd::RemoveAllStatus(int opt_type)
{
	// 下一页或者退出时清除微博及画布数据
	if (opt_type != t_wb_option_params::WBOPTType_since_id)
	{
		std::list<STATUSES>::iterator iter1;
		for (iter1 = m_list_statuses.begin(); iter1 != m_list_statuses.end(); iter1++)
		{
			delete [] iter1->statuses_;
		}
		m_list_statuses.clear();

		std::list<CM8WeiboHomeCanvas *>::iterator iter2;
		for (iter2 = m_list_canvas.begin(); iter2 != m_list_canvas.end(); iter2++)
		{
			m_scrool_win.RemoveChild(*iter2);
			delete *iter2;
		}
		m_list_canvas.clear();
	}
}

bool CM8WeiboHomeWnd::RefreshFriendsStatus(int opt_type, M8::m8_wb_get_statuses_friends_timeline *&pStatuses)
{
	// 根据OPT_TYPE刷新微博数据
	char *wb_id = NULL;
	pStatuses = NULL;
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		if (m_list_statuses.size() > 0)
		{
			STATUSES front = m_list_statuses.front();
			if (front.size_of_status_ > 0) wb_id = front.statuses_[0].id_;
		}
	}
	else if (opt_type == t_wb_option_params::WBOPTType_max_id)
	{
		if (m_list_statuses.size() > 0)
		{
			STATUSES back = m_list_statuses.back();
			if (back.size_of_status_ > 0) wb_id = back.statuses_[back.size_of_status_-1].id_;
		}
	}
	if (!theApp.GetStatusesFriendsTimeline(m_hWnd, opt_type, wb_id, pStatuses))
	{
		delete pStatuses;
		return false;
	}
	if (pStatuses->size_of_status_ == 0)
	{
		delete pStatuses;
		return false;
	}
	return true;
}

bool CM8WeiboHomeWnd::RefreshUserStatus(int opt_type, M8::m8_wb_get_statuses_user_timeline *&pStatuses)
{
	// 根据OPT_TYPE刷新微博数据
	char *wb_id = NULL;
	pStatuses = NULL;
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		if (m_list_statuses.size() > 0)
		{
			STATUSES front = m_list_statuses.front();
			if (front.size_of_status_ > 0) wb_id = front.statuses_[0].id_;
		}
	}
	else if (opt_type == t_wb_option_params::WBOPTType_max_id)
	{
		if (m_list_statuses.size() > 0)
		{
			STATUSES back = m_list_statuses.back();
			if (back.size_of_status_ > 0) wb_id = back.statuses_[back.size_of_status_-1].id_;
		}
	}
	if (!theApp.GetStatusesUserTimeline(m_hWnd, m_user->id_, opt_type, wb_id, pStatuses))
	{
		delete pStatuses;
		return false;
	}
	if (pStatuses->size_of_status_ == 0)
	{
		delete pStatuses;
		return false;
	}
	return true;
}

bool CM8WeiboHomeWnd::RefreshFavorites(int opt_type, M8::m8_wb_get_favorites *&pStatuses)
{
	// 根据OPT_TYPE刷新微博数据
	int page = 1;
	pStatuses = NULL;
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		if (m_list_statuses.size() > 0)
		{
			STATUSES front = m_list_statuses.front();
			if (front.size_of_status_ > 0) page = front.page_-1;
			if (page < 1) page = 1;
		}
	}
	else if (opt_type == t_wb_option_params::WBOPTType_max_id)
	{
		if (m_list_statuses.size() > 0)
		{
			STATUSES back = m_list_statuses.back();
			if (back.size_of_status_ > 0) page = back.page_+1;
		}
	}
	if (!theApp.GetFavorites(m_hWnd, page, pStatuses))
	{
		delete pStatuses;
		return false;
	}
	if (pStatuses->size_of_status_ == 0)
	{
		delete pStatuses;
		return false;
	}
	return true;
}

bool CM8WeiboHomeWnd::RefreshMentions(int opt_type, M8::m8_wb_get_statuses_mentions *&pStatuses)
{
	// 根据OPT_TYPE刷新微博数据
	char *wb_id = NULL;
	pStatuses = NULL;
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		if (m_list_statuses.size() > 0)
		{
			STATUSES front = m_list_statuses.front();
			if (front.size_of_status_ > 0) wb_id = front.statuses_[0].id_;
		}
	}
	else if (opt_type == t_wb_option_params::WBOPTType_max_id)
	{
		if (m_list_statuses.size() > 0)
		{
			STATUSES back = m_list_statuses.back();
			if (back.size_of_status_ > 0) wb_id = back.statuses_[back.size_of_status_-1].id_;
		}
	}
	if (!theApp.GetStatusesMentions(m_hWnd, opt_type, wb_id, pStatuses))
	{
		delete pStatuses;
		return false;
	}
	if (pStatuses->size_of_status_ == 0)
	{
		delete pStatuses;
		return false;
	}
	return true;
}

bool CM8WeiboHomeWnd::RefreshCommentsToMe(int opt_type, M8::m8_wb_get_statuses_comments_tome *&pStatuses)
{
	// 根据OPT_TYPE刷新微博数据
	char *wb_id = NULL;
	pStatuses = NULL;
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		if (m_list_statuses.size() > 0)
		{
			STATUSES front = m_list_statuses.front();
			if (front.size_of_status_ > 0) wb_id = front.statuses_[0].id_;
		}
	}
	else if (opt_type == t_wb_option_params::WBOPTType_max_id)
	{
		if (m_list_statuses.size() > 0)
		{
			STATUSES back = m_list_statuses.back();
			if (back.size_of_status_ > 0) wb_id = back.statuses_[back.size_of_status_-1].id_;
		}
	}
	if (!theApp.GetStatusesCommentsToMe(m_hWnd, opt_type, wb_id, pStatuses))
	{
		delete pStatuses;
		return false;
	}
	if (pStatuses->size_of_comment_ == 0)
	{
		delete pStatuses;
		return false;
	}
	return true;
}

bool CM8WeiboHomeWnd::RefreshWeiboSearch(int opt_type, M8::m8_wb_search *&pStatuses)
{
	// 根据OPT_TYPE刷新微博数据
	int page = 1;
	pStatuses = NULL;
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		STATUSES front = m_list_statuses.front();
		if (front.size_of_status_ > 0) page = front.page_-1;
		if (page < 1) page = 1;
	}
	else if (opt_type == t_wb_option_params::WBOPTType_max_id)
	{
		if (m_list_statuses.size() > 0)
		{
			STATUSES back = m_list_statuses.back();
			if (back.size_of_status_ > 0) page = back.page_+1;
		}
	}
	if (!theApp.GetWeiboSearch(m_hWnd, CStringW2A(m_search_name).ptr(), page, pStatuses))
	{
		delete pStatuses;
		return false;
	}
	if (pStatuses->size_of_status_ == 0)
	{
		delete pStatuses;
		return false;
	}
	return true;
}

void CM8WeiboHomeWnd::AddFriendsStatus(int opt_type, M8::m8_wb_get_statuses_friends_timeline * pStatuses)
{
	STATUSES statuses = {pStatuses->size_of_status_, pStatuses->statuses_, 0};
	pStatuses->size_of_status_ = 0;
	pStatuses->statuses_ = NULL;
	delete pStatuses;

	// 根据OPT_TYPE添加微博数据
	// 不是查询最新微博，需要删除现有保存的数据，防止页面过长
	// 是最新微博，将新数据保存在现有数据前面
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		m_list_statuses.push_front(statuses);
	}
	else
	{
		m_list_statuses.push_back(statuses);
	}
}

void CM8WeiboHomeWnd::AddMentions(int opt_type, M8::m8_wb_get_statuses_mentions * pStatuses)
{
	STATUSES statuses = {pStatuses->size_of_status_, pStatuses->statuses_, 0};
	pStatuses->size_of_status_ = 0;
	pStatuses->statuses_ = NULL;
	delete pStatuses;

	// 根据OPT_TYPE添加微博数据
	// 不是查询最新微博，需要删除现有保存的数据，防止页面过长
	// 是最新微博，将新数据保存在现有数据前面
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		m_list_statuses.push_front(statuses);
	}
	else
	{
		m_list_statuses.push_back(statuses);
	}
}

void CM8WeiboHomeWnd::AddCommentsToMe(int opt_type, M8::m8_wb_get_statuses_comments_tome * pStatuses)
{
	M8::m8_status *status = new M8::m8_status[pStatuses->size_of_comment_];
	for (int i = 0; i < pStatuses->size_of_comment_; i++)
	{
		status[i].retweeted_status_ = new M8::m8_status;
		memcpy(status[i].retweeted_status_, &(pStatuses->comments_[i].status_), sizeof(M8::m8_status));
		status[i].retweeted_status_->retweeted_status_ = NULL;
		if (pStatuses->comments_[i].status_.user_)
		{
			status[i].retweeted_status_->user_ = new M8::m8_user;
			memcpy(status[i].retweeted_status_->user_, pStatuses->comments_[i].status_.user_, sizeof(M8::m8_user));
		}
		else
		{
			status[i].retweeted_status_->user_ = NULL;
		}
		strcpy(status[i].created_at_, pStatuses->comments_[i].created_at_);
		strcpy(status[i].id_, pStatuses->comments_[i].id_);
		strcpy(status[i].text_, pStatuses->comments_[i].text_);
		strcpy(status[i].source_, pStatuses->comments_[i].source_);
		strcpy(status[i].mid_, pStatuses->comments_[i].mid_);
		status[i].user_ = new M8::m8_user;
		memcpy(status[i].user_, &(pStatuses->comments_[i].user_), sizeof(M8::m8_user));
	}

	STATUSES statuses = {pStatuses->size_of_comment_, status, 0};
	delete pStatuses;

	// 根据OPT_TYPE添加微博数据
	// 不是查询最新微博，需要删除现有保存的数据，防止页面过长
	// 是最新微博，将新数据保存在现有数据前面
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		m_list_statuses.push_front(statuses);
	}
	else
	{
		m_list_statuses.push_back(statuses);
	}
}

void CM8WeiboHomeWnd::AddWeiboSearch(int opt_type, M8::m8_wb_search *&pStatuses)
{
	STATUSES statuses = {pStatuses->size_of_status_, pStatuses->statuses_, pStatuses->page_};
	pStatuses->size_of_status_ = 0;
	pStatuses->statuses_ = NULL;
	delete pStatuses;

	// 根据OPT_TYPE添加微博数据
	// 不是查询最新微博，需要删除现有保存的数据，防止页面过长
	// 是最新微博，将新数据保存在现有数据前面
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		m_list_statuses.push_front(statuses);
	}
	else
	{
		m_list_statuses.push_back(statuses);
	}
}

void CM8WeiboHomeWnd::AddUserStatus(int opt_type, M8::m8_wb_get_statuses_user_timeline * pStatuses)
{
	STATUSES statuses = {pStatuses->size_of_status_, pStatuses->statuses_, 0};
	pStatuses->size_of_status_ = 0;
	pStatuses->statuses_ = NULL;
	delete pStatuses;

	// 根据OPT_TYPE添加微博数据
	// 不是查询最新微博，需要删除现有保存的数据，防止页面过长
	// 是最新微博，将新数据保存在现有数据前面
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		m_list_statuses.push_front(statuses);
	}
	else
	{
		m_list_statuses.push_back(statuses);
	}
}

void CM8WeiboHomeWnd::AddFavorites(int opt_type, M8::m8_wb_get_favorites * pStatuses)
{
	STATUSES statuses = {pStatuses->size_of_status_, pStatuses->statuses_, pStatuses->page_};
	pStatuses->size_of_status_ = 0;
	pStatuses->statuses_ = NULL;
	delete pStatuses;

	// 根据OPT_TYPE添加微博数据
	// 不是查询最新微博，需要删除现有保存的数据，防止页面过长
	// 是最新微博，将新数据保存在现有数据前面
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		m_list_statuses.push_front(statuses);
	}
	else
	{
		m_list_statuses.push_back(statuses);
	}
}

int CM8WeiboHomeWnd::GetStatusArray(M8::m8_status **&p_status_list)
{
	int size = 0;
	int index = 0;
	int outofmax = 0;
	int MAX_STATUSES = 20;
	p_status_list = NULL;
	std::list<STATUSES>::iterator iter;
	for (iter = m_list_statuses.begin(); iter != m_list_statuses.end(); iter++)
	{
		if (size < MAX_STATUSES) size += iter->size_of_status_; else outofmax++;
	}

	while (outofmax-- > 0)
	{
		STATUSES back = m_list_statuses.back();
		m_list_statuses.pop_back();
		delete [] back.statuses_;
	}

	p_status_list = new M8::m8_status*[size];

	for (iter = m_list_statuses.begin(); iter != m_list_statuses.end(); iter++)
	{
		for (int i = 0; i < iter->size_of_status_; i++)
		{
			p_status_list[index++] = iter->statuses_+i;
		}
	}

	// 序列化操作
	if (m_user_type == ALL_FRIENDS) theApp.Serial(theApp.GetUserInfoSelf()->id_, size, p_status_list);
	return size;
}

void CM8WeiboHomeWnd::DrawAllStatuses(M8::m8_status ** p_status_list, int size_of_status)
{
	std::list<CM8WeiboHomeCanvas *> tmp_list_canvas;
	for (int x = 0; x < size_of_status; x++)
	{
		M8::m8_status *p_status = p_status_list[x];
		if (m_list_canvas.size() > 0 && p_status == m_list_canvas.front()->m_status) break;
		bool isMyStatus = p_status->user_ && theApp.isSelf(p_status->user_->id_);

		// 按每行制定字符个数换行
		CStringA2W text(p_status->text_);
		int lines = theApp.SplitString(text.ptr(), MZ_STATUS_COUNT_PER_LINE);
		CMzString *p_lines = new CMzString[lines];
		theApp.SplitString(lines, p_lines, text.ptr(), MZ_STATUS_COUNT_PER_LINE);

		int width = GetWidth();

		// 定义微博宽度和高度
		FULL_WIDTH_OF_STATUS = width;
		FULL_HEIGHT_OF_STATUS = lines*(TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT)-SPACE_LINE_HEIGHT
			+PAD_TOP_STATUS+PAD_BOTTOM_STATUS
			+MARGIN_TOP_CONTENT+MARGIN_BOTTOM_CONTENT
			+PAD_TOP_CONTENT+PAD_BOTTOM_CONTENT;

		// 增加转发微博的高度
		if (p_status->retweeted_status_)
		{
			char *p_text = new char[WB_REAL_LEN(SCREEN_NAME) + WB_REAL_LEN(WB_INFO)];
			M8::m8_status *p_retweeted_status_ = p_status->retweeted_status_;
			while (p_retweeted_status_)
			{
				sprintf(p_text, "%s：%s", p_retweeted_status_->user_->screen_name_, p_retweeted_status_->text_);
				int lines = theApp.SplitString(CStringA2W(p_text).ptr(), MZ_STATUS_COUNT_PER_LINE);
				FULL_HEIGHT_OF_STATUS += lines*(RT_TEXT_LINE_HEIGHT+RT_SPACE_LINE_HEIGHT)
					-RT_SPACE_LINE_HEIGHT+RT_TOP_HEIGHT+RT_BOTTOM_HEIGHT;
				p_retweeted_status_ = p_retweeted_status_->retweeted_status_;
			}
			delete [] p_text;
		}

		// 增加缩略图的高度
		if (theApp.m_parameter->getAutoDownloadSmallPicture() && *p_status->thumbnail_pic_ && !theApp.IsGif(p_status->thumbnail_pic_))
		{
			FULL_HEIGHT_OF_STATUS += THUMB_FIX_HEIGHT+THUMB_TOP_HEIGHT+THUMB_BOTTOM_HEIGHT;
		}
		if (theApp.m_parameter->getAutoDownloadSmallPicture() && p_status->retweeted_status_ && *p_status->retweeted_status_->thumbnail_pic_ && !theApp.IsGif(p_status->retweeted_status_->thumbnail_pic_))
		{
			FULL_HEIGHT_OF_STATUS += THUMB_FIX_HEIGHT+THUMB_TOP_HEIGHT+THUMB_BOTTOM_HEIGHT;
		}

		LOGFONT font_struct={ 0, 0, 0, 0, 0, 0, 0, 0,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH,
			L"宋体"
		};

		// 微博所占块位置
		RECT rcStatus;
		if (x == 0)
		{
			rcStatus.left = MARGIN_LEFT_STATUS;
			rcStatus.top = MARGIN_TOP_STATUS;
			rcStatus.right = FULL_WIDTH_OF_STATUS-MARGIN_RIGHT_STATUS;
			rcStatus.bottom = FULL_HEIGHT_OF_STATUS+rcStatus.top;
		}
		else
		{
			CM8WeiboHomeCanvas *last_status = tmp_list_canvas.back();
			rcStatus = last_status->m_rcStatus;
			rcStatus.top = rcStatus.bottom+MARGIN_TOP_STATUS+MARGIN_BOTTOM_STATUS;
			rcStatus.bottom = FULL_HEIGHT_OF_STATUS+rcStatus.top;
		}
		CM8WeiboHomeCanvas * tmp_cavas = new CM8WeiboHomeCanvas();
		tmp_cavas->m_hWnd = m_hWnd;
		tmp_cavas->m_rcStatus = rcStatus;
		tmp_cavas->m_status = p_status;
		tmp_list_canvas.push_back(tmp_cavas);

		// 创建微博块
		tmp_cavas->SetID(MZ_IDC_HOME_STATUSES+x);
		tmp_cavas->SetPos(rcStatus.left, rcStatus.top, rcStatus.right -rcStatus.left, rcStatus.bottom-rcStatus.top);
		tmp_cavas->m_dcCanvas.Create(rcStatus.right-rcStatus.left, rcStatus.bottom-rcStatus.top);

		// 填充微博背景
		RECT rcOrigin = rcStatus;
		rcOrigin.right -= rcOrigin.left;
		rcOrigin.bottom -= rcOrigin.top;
		rcOrigin.left = 0;
		rcOrigin.top = 0;
		HDC hDC = tmp_cavas->m_dcCanvas.GetDC();
		SetBkMode(hDC,TRANSPARENT);
		HBRUSH hBrush = CreateSolidBrush(COLOR_OF_BG_STATUS);
		FillRect(hDC, &rcOrigin, hBrush);
		DeleteObject(hBrush);

		// 初始化偏移量
		int offsetLeft = 0;
		int offsetTop = 0;
		int offsetRight = 0;
		int offsetBottom = 0;

		// 画微博作者头像
		RECT rcToken;
		rcToken.left = rcOrigin.left+offsetLeft+(PAD_LEFT_STATUS-TOKEN_LENGTH)/2;
		rcToken.right = rcToken.left+TOKEN_LENGTH;
		rcToken.top = rcOrigin.top+offsetTop+PAD_TOP_STATUS+MARGIN_TOP_CONTENT;
		rcToken.bottom = rcToken.top+TOKEN_LENGTH;

		CMzString profile_image_path;
		CMzString profile_token_name = theApp.UpdateProfileTokenName(p_status->user_->id_, p_status->user_->profile_image_url_, profile_image_path);
		CMzString profile_image_name = theApp.FindTokenFileName(profile_image_path, profile_token_name);
		if (profile_image_name.Length() > 0 || theApp.m_parameter->getAutoDownloadProfileImage())
		{
			if (profile_image_name.Length() == 0)
			{
				theApp.DownloadTokenFile(m_hWnd, p_status->user_->id_, true, p_status->user_->profile_image_url_, profile_image_path, profile_token_name);
				profile_image_name = profile_image_path + profile_token_name;
			}
			ImagingHelper::DrawImage(hDC, &rcToken, profile_image_name.C_Str());
		}
		else
		{
			if (p_status->user_->gender_[0] == 'm')
				imgTokenBoy->Draw(hDC, &rcToken);
			else
				imgTokenGirl->Draw(hDC, &rcToken);
		}

		// 保留偏移量
		offsetLeft += PAD_LEFT_STATUS+MARGIN_LEFT_CONTENT;
		offsetTop += PAD_TOP_STATUS+MARGIN_TOP_CONTENT;
		offsetRight += PAD_RIGHT_STATUS+MARGIN_RIGHT_CONTENT;
		offsetBottom += PAD_BOTTOM_STATUS+MARGIN_BOTTOM_CONTENT;

		// 内容所占块位置
		RECT rcContent = rcOrigin;
		rcContent.left += offsetLeft;
		rcContent.top += offsetTop;
		rcContent.right -= offsetRight;
		rcContent.bottom -= offsetBottom;

		// 描绘内容边框
		HPEN hPen = CreatePen(PS_SOLID, 1, isMyStatus?COLOR_OF_EDG_CONTENT_SELF:COLOR_OF_EDG_CONTENT);
		hBrush = CreateSolidBrush(isMyStatus?COLOR_OF_BG_CONTENT_SELF:COLOR_OF_BG_CONTENT);
		HGDIOBJ hSavedPen = SelectObject(hDC, hPen);
		HGDIOBJ hSavedBrush = SelectObject(hDC, hBrush);
		//RoundRect(hDC, 10, 10, width-10, 100-10, 9, 9);
		Rectangle(hDC, rcContent.left, rcContent.top, rcContent.right, rcContent.bottom);
		SelectObject(hDC, hSavedBrush);
		SelectObject(hDC, hSavedPen);
		DeleteObject(hBrush);
		DeleteObject(hPen);

		// 画边框的左三角
		int a = rcContent.left;
		int b = rcContent.top+(TOKEN_LENGTH-DELTA_LENGTH)/2;
		for (int c = 0; c < DELTA_LENGTH/2; c++)
		{
			SetPixel(hDC, a--, b++, isMyStatus?COLOR_OF_EDG_CONTENT_SELF:COLOR_OF_EDG_CONTENT);
			int pos = a+1;
			while (rcContent.left-pos>=0) SetPixel(hDC, pos++, b, isMyStatus?COLOR_OF_BG_CONTENT_SELF:COLOR_OF_BG_CONTENT);
		}
		for (int c = 0; c < DELTA_LENGTH/2; c++)
		{
			SetPixel(hDC, ++a, b++, isMyStatus?COLOR_OF_EDG_CONTENT_SELF:COLOR_OF_EDG_CONTENT);
			int pos = a+1;
			while (rcContent.left-pos>=0) SetPixel(hDC, pos++, b, isMyStatus?COLOR_OF_BG_CONTENT_SELF:COLOR_OF_BG_CONTENT);
		}

		// 作者所占块位置
		RECT rcAuthor;
		rcAuthor.left = rcOrigin.left+offsetLeft+MARGIN_LEFT_AUTHOR;
		rcAuthor.top = rcOrigin.top+offsetTop;
		rcAuthor.right = rcOrigin.right-MARGIN_RIGHT_CREATED_AT-WIDTH_OF_CREATED_AT;
		rcAuthor.bottom = rcAuthor.top+PAD_TOP_CONTENT;

		// 输出作者
		font_struct.lfHeight = AUTHOR_LINE_HEIGHT;
		HFONT hFont = CreateFontIndirect(&font_struct);
		HGDIOBJ hSavedFont = SelectObject(hDC, hFont);
		SetTextColor(hDC, COLOR_OF_TEXT_AUTHOR);
		CStringA2W info(p_status->user_->screen_name_);
		DrawText(hDC, info.ptr(), -1, &rcAuthor, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		SelectObject(hDC, hSavedFont);
		DeleteObject(hFont);

		// 时间所占块位置
		RECT rcCreateAt=rcAuthor;
		rcCreateAt.right = rcOrigin.right-offsetRight-MARGIN_RIGHT_CREATED_AT;
		rcCreateAt.left = rcCreateAt.right-WIDTH_OF_CREATED_AT;

		// 输出时间
		font_struct.lfHeight = CREATE_AT_LINE_HEIGHT;
		hFont = CreateFontIndirect(&font_struct);
		hSavedFont = SelectObject(hDC, hFont);
		SetTextColor(hDC, COLOR_OF_TEXT_CREATE_AT);
		CStringA2W create_at(p_status->created_at_);
		wchar_t etc[6][16];
		wmemset((wchar_t *)etc, 0, 96);
		theApp.FormatCreateAt(create_at.ptr(), etc);
		wchar_t str_time[16];
		swprintf(str_time, L"%s-%s %s", etc[1], etc[2], etc[3]);
		DrawText(hDC, str_time, -1, &rcCreateAt, DT_SINGLELINE|DT_RIGHT|DT_VCENTER);
		SelectObject(hDC, hSavedFont);
		DeleteObject(hFont);

		// 判断是否包含图像
		if (*p_status->thumbnail_pic_ || (p_status->retweeted_status_ && *p_status->retweeted_status_->thumbnail_pic_))
		{
			RECT rcPicture = rcCreateAt;
			rcPicture.right = rcPicture.left-SPACE_OF_ICON_AND_CREATED_AT;
			rcPicture.left = rcPicture.right-16;
			rcPicture.top += (rcPicture.bottom-rcPicture.top-16)/2;
			rcPicture.bottom = rcPicture.top+16;
			imgSmallPicture->Draw(hDC, &rcPicture);
		}

		// 偏移量进入TEXT范围
		offsetLeft += PAD_LEFT_CONTENT;
		offsetTop += PAD_TOP_CONTENT;
		offsetRight += PAD_RIGHT_CONTENT;
		offsetBottom += PAD_BOTTOM_CONTENT;

		// 内容所占块位置
		RECT rcText;
		rcText.left = rcOrigin.left+offsetLeft;
		rcText.top = rcOrigin.top+offsetTop;
		rcText.right = rcOrigin.right-offsetRight;
		rcText.bottom = rcText.top+TEXT_LINE_HEIGHT;

		// 输出内容
		font_struct.lfHeight = TEXT_LINE_HEIGHT;
		hFont = CreateFontIndirect(&font_struct);
		hSavedFont = SelectObject(hDC, hFont);
		SetTextColor(hDC, COLOR_OF_TEXT_CONTENT);
		for (int i = 0; i < lines; i++)
		{
			DrawText(hDC, p_lines[i].C_Str(), -1, &rcText, DT_SINGLELINE|DT_LEFT);
			rcText.top += TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT;
			rcText.bottom = rcText.top+TEXT_LINE_HEIGHT;
		}
		SelectObject(hDC, hSavedFont);
		DeleteObject(hFont);
		delete [] p_lines;

		if (p_status->retweeted_status_)
		{
			DrawRetweetedStatus(p_status->retweeted_status_, hDC, rcOrigin, offsetLeft+RT_TAB_WIDTH, 
				offsetTop+lines*(TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT)-SPACE_LINE_HEIGHT, offsetRight);
		}

		// 画缩略图
		if (theApp.m_parameter->getAutoDownloadSmallPicture() && *p_status->thumbnail_pic_ && !theApp.IsGif(p_status->thumbnail_pic_))
		{
			RECT rcThumb;
			rcThumb.left = rcOrigin.left+offsetLeft+THUMB_LEFT_PAD;
			rcThumb.right = rcThumb.left+THUMB_FIX_WIDTH;
			rcThumb.bottom = rcOrigin.bottom-offsetBottom-THUMB_BOTTOM_HEIGHT;
			rcThumb.top = rcThumb.bottom-THUMB_FIX_HEIGHT;

			CMzString small_picture_path;
			CMzString picture_token_name = theApp.UpdateSmallPictureTokenName(p_status->user_->id_, p_status->thumbnail_pic_, small_picture_path);
			CMzString small_picure_name = theApp.FindTokenFileName(small_picture_path, picture_token_name);
			if (small_picure_name.IsEmpty())
			{
				if(theApp.DownloadTokenFile(m_hWnd, p_status->user_->id_, false, p_status->thumbnail_pic_, small_picture_path, picture_token_name))
				{
					small_picure_name = small_picture_path+picture_token_name;
				}
			}
			ImagingHelper img_helper;
			img_helper.LoadImage(small_picure_name.C_Str(), false, false, true);
			img_helper.Draw(hDC, &rcThumb, true, false);
			img_helper.UnloadImage();
		}

		// 偏移量退出TEXT范围
		offsetLeft -= PAD_LEFT_CONTENT;
		offsetTop -= PAD_TOP_CONTENT;
		offsetRight -= PAD_RIGHT_CONTENT;
		offsetBottom -= PAD_BOTTOM_CONTENT;

		// 来源所占块位置
		RECT rcSource;
		rcSource.left = rcOrigin.left+offsetLeft+MARGIN_LEFT_SORUCE;
		rcSource.right = rcOrigin.right-MARGIN_RIGHT_OTHER-WIDTH_OF_OTHER;
		rcSource.bottom = rcOrigin.bottom-offsetBottom;
		rcSource.top = rcSource.bottom-PAD_BOTTOM_CONTENT;

		// 输出来源
		font_struct.lfHeight = SOURCE_LINE_HEIGHT;
		hFont = CreateFontIndirect(&font_struct);
		hSavedFont = SelectObject(hDC, hFont);
		SetTextColor(hDC, COLOR_OF_TEXT_SOURCE);
		CStringA2W source(p_status->source_);
		wchar_t buffer[64];
		wmemset(buffer, 0, 64);
		theApp.FormatSource(source.ptr(), buffer);
		DrawText(hDC, buffer, -1, &rcSource, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		SelectObject(hDC, hSavedFont);
		DeleteObject(hFont);

		// 引用及评论所占块位置
		RECT rcCounts=rcSource;
		rcCounts.right = rcOrigin.right-offsetRight-MARGIN_RIGHT_OTHER;
		rcCounts.left = rcCounts.right-WIDTH_OF_OTHER;

		// 画引用图标
		RECT rcLink = rcCounts;
		rcLink.top = rcLink.top+(rcCounts.bottom-rcCounts.top-16)/2;
		rcLink.bottom = rcLink.top+16;
		rcLink.right = rcLink.left+16;
		if (p_status->m_links_) imgSmallLink->Draw(hDC, &rcLink);

		// 画评论图标
		RECT rcComment = rcLink;
		rcComment.left = rcLink.left+WIDTH_OF_OTHER/2;
		rcComment.right = rcComment.left+16;
		if (p_status->m_comments_) imgSmallComment->Draw(hDC, &rcComment);

		rcLink.left = rcLink.right+SPACE_OF_ICON_AND_COUNTS;
		rcLink.right = rcComment.left;

		rcComment.left = rcComment.right+SPACE_OF_ICON_AND_COUNTS;
		rcComment.right = rcCounts.right;

		// 输出引用数及评论数
		font_struct.lfHeight = COUNTS_LINE_HEIGHT;
		hFont = CreateFontIndirect(&font_struct);
		hSavedFont = SelectObject(hDC, hFont);
		SetTextColor(hDC, COLOR_OF_TEXT_COUNTS);
		wsprintf(buffer, L"%d", p_status->m_links_);
		if (p_status->m_links_) DrawText(hDC, buffer, -1, &rcLink, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		wsprintf(buffer, L"%d", p_status->m_comments_);
		if (p_status->m_comments_) DrawText(hDC, buffer, -1, &rcComment, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		SelectObject(hDC, hSavedFont);
		DeleteObject(hFont);

		m_scrool_win.AddChild(tmp_cavas);
	}

	RECT rcTemp = tmp_list_canvas.back()->m_rcStatus;
	std::list<CM8WeiboHomeCanvas *>::iterator iter;
	for (iter = m_list_canvas.begin(); iter != m_list_canvas.end(); iter++)
	{
		(*iter)->Move(0, rcTemp.bottom+MARGIN_BOTTOM_STATUS);
		(*iter)->m_rcStatus.top += rcTemp.bottom+MARGIN_BOTTOM_STATUS;
		(*iter)->m_rcStatus.bottom += rcTemp.bottom+MARGIN_BOTTOM_STATUS;
	}

	while (tmp_list_canvas.size() > 0)
	{
		CM8WeiboHomeCanvas * back = tmp_list_canvas.back();
		m_list_canvas.push_front(back);
		tmp_list_canvas.pop_back();
	}

	while (m_list_canvas.size()-size_of_status > 0)
	{
		CM8WeiboHomeCanvas * back = m_list_canvas.back();
		m_list_canvas.pop_back();
		m_scrool_win.RemoveChild(back);
		delete back;
	}
}

void CM8WeiboHomeWnd::MoveMoreButton()
{
	// 画更多按钮
	m_scrool_win.RemoveChild(&m_more_canvas);
	RECT rcLast;
	if (m_list_canvas.size() > 0)
	{
		rcLast = m_list_canvas.back()->m_rcStatus;
		rcLast.top = rcLast.bottom+MARGIN_TOP_STATUS+MARGIN_BOTTOM_STATUS;
		rcLast.bottom = rcLast.top+64;
	}
	else
	{
		rcLast.left = 0;
		rcLast.right = GetWidth();
		rcLast.top = MARGIN_TOP_CONTENT+MARGIN_TOP_STATUS;
		rcLast.bottom = rcLast.top+64;
	}
	m_more_canvas.SetID(MZ_IDC_HOME_MORE);
	m_more_canvas.SetPos(rcLast.left, rcLast.top, rcLast.right-rcLast.left, rcLast.bottom-rcLast.top);
	m_more_canvas.m_hWnd = m_hWnd;
	m_scrool_win.AddChild(&m_more_canvas);
}
