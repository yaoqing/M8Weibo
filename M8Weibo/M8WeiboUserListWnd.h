#pragma once

#include <mzfc_inc.h>
#include "M8Struct.h"
#include "M8WeiboToolbar.h"
#include "M8WeiboMoreCanvas.h"
#include <list>

#define MZ_IDC_USER_LIST_CAPTION                101
#define MZ_IDC_USER_LIST_GOBACK                 102
#define MZ_IDC_USER_LIST_GOHOME                 103
#define MZ_IDC_USER_LIST_SCROOLWIN              104
#define MZ_IDC_USER_LIST_CANVAS                 105
#define MZ_IDC_USER_LIST_TOOLBAR                110
#define MZ_IDC_USER_LIST_WORKING                120
#define MZ_IDC_USER_LIST_CLICK_DOWN             121
#define MZ_IDC_USER_LIST_CLICK_UP               122
#define MZ_IDC_USER_LIST_MORE                   999
#define MZ_IDC_USER_LIST_USERS                  1000

#define MZ_ID_USER_LIST_TIMER                   200

class CM8WeiboUserListCanvas: public UiCanvas
{
public:
	M8::m8_user_and_status *m_user_and_status; // 用于设置定时器ID
	RECT m_rcUser;
	HWND m_hWnd;
	DWORD m_color_bg;
	CM8WeiboUserListCanvas() { init(); }
	~CM8WeiboUserListCanvas() { init(); }
	void init();
	virtual int OnLButtonDown(UINT fwKeys, int xPos, int yPos);
	virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboUserListScroolWin : public UiScrollWin
{
public:
	HWND m_hWnd;
	DWORD m_dwTimerID;
	virtual int OnLButtonDown(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboUserListWnd: public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CM8WeiboUserListWnd);
public:
	CM8WeiboUserListWnd(void);
	~CM8WeiboUserListWnd(void);

	M8::m8_user *m_user;
	int m_user_type;

	CMzString m_search_name;

	UiCaption m_ctlCaption;
	UiButton_Image m_button_goback;
	UiButton_Image m_button_gohome;
	ImageContainer m_image_container;

	CM8WeiboToolbar m_toolbar;

	int m_size_of_user;
	CM8WeiboUserListScroolWin m_scrool_win;
	CM8WeiboUserListCanvas * m_users_canvas;
	CM8WeiboUserListMoreCanvas m_more_canvas;
	
	struct USERS
	{
		int size_of_user_;
		M8::m8_user_and_status *users_;
		int prev_cursor_;
		int next_cursor_;
	};
	
	enum
	{
		FRIEND = 0,
		FOLLOWER,
		USER_SEARCH,
	};

protected:
	virtual BOOL OnInitDialog();
	virtual void OnTimer(UINT_PTR nIDEvent);
	virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);

private:
	std::list<USERS> m_list_users;
	void RemoveAllUsers(int opt_type);
	bool RefreshFriends(int opt_type, M8::m8_wb_get_statuses_friends*& pUsers);
	bool RefreshFollowers(int opt_type, M8::m8_wb_get_statuses_followers*& pUsers);
	bool RefreshUserSearchs(int opt_type, M8::m8_wb_users_search*& pUsers);
	void AddFriends(int opt_type, M8::m8_wb_get_statuses_friends* pUsers);
	void AddFollowers(int opt_type, M8::m8_wb_get_statuses_followers* pUsers);
	void AddUserSearchs(int opt_type, M8::m8_wb_users_search* pUsers);
	int GetUserArray(M8::m8_user_and_status **&pUserArray);
	void DrawAllUsers(M8::m8_user_and_status **pUserArray);
	void MoveMoreButton();
	void OnDrawUser(HDC hDC, RECT rcUser, DWORD bg_color, bool pressed, M8::m8_user_and_status * pUser, wchar_t * text);
	void OnUserClick(CM8WeiboUserListCanvas * p_canvas, bool pressed);

private:
	ImagingHelper* imgNormalGoback;
	ImagingHelper* imgPressedGoback;
	ImagingHelper* imgNormalGohome;
	ImagingHelper* imgPressedGohome;
	ImagingHelper* imgArrowGray;
	ImagingHelper* imgArrowWhite;
	ImagingHelper* imgTokenBoy;
	ImagingHelper* imgTokenGirl;

	int MZ_STATUS_COUNT_PER_LINE;

	// 定义各种字体的颜色
	DWORD COLOR_OF_BG_PRESSED;
	DWORD COLOR_OF_FG_PRESSED;
	DWORD COLOR_OF_BG_STATUS_0;
	DWORD COLOR_OF_BG_STATUS_1;
	DWORD COLOR_OF_AUTHOR;
	DWORD COLOR_OF_STATUS;

	// 定义各种字体的行高
	int LINE_HEIGHT_AUTHOR;
	int LINE_HEIGHT_STATUS;
	int SPACE_AUTHOR_AND_STATUS;

	// 定义单条微博的内空白
	int PAD_TOP_USER;
	int PAD_BOTTOM_USER;
	int PAD_LEFT_USER;
	int PAD_RIGHT_USER;

	// 定义微博宽度和高度
	int FULL_WIDTH_OF_CANVAS;
	int FULL_HEIGHT_OF_CANVAS;
};
