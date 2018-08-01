#pragma once

#include <mzfc_inc.h>
#include "M8Struct.h"
#include "M8WeiboToolbar.h"

#define MZ_IDC_USER_CAPTION                101
#define MZ_IDC_USER_GOBACK                 102
#define MZ_IDC_USER_GOHOME                 103
#define MZ_IDC_USER_SCROOLWIN              104
#define MZ_IDC_USER_CANVAS                 105
#define MZ_IDC_USER_SCANVAS                106
#define MZ_IDC_USER_DRAW_FOLLOW            107
#define MZ_IDC_USER_DRAW_TOKEN             108
#define MZ_IDC_USER_TOOLBAR                110

#define MZ_ID_USER_TIMER                   200

class CM8WeiboUserWnd;
class CM8WeiboUserCanvas: public UiCanvas
{
public:
	CM8WeiboUserCanvas();
	~CM8WeiboUserCanvas();
	CM8WeiboUserWnd * m_parentWnd;
	HWND m_hWnd;
	char *m_uid;
	virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboUserWnd;
class CM8WeiboUserScroolCanvas: public UiCanvas
{
public:
	CM8WeiboUserScroolCanvas(void);
	~CM8WeiboUserScroolCanvas(void);
	HWND m_hWnd;
	CM8WeiboUserWnd *m_parentWnd;
	virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboUserWnd : public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CM8WeiboUserWnd);
public:
	CM8WeiboUserWnd(void);
	~CM8WeiboUserWnd(void);
	void SetUserInfo(M8::m8_user *p_user);
	M8::m8_user * GetUserInfo() { return &m_user; }
	bool IsSelf() const { return isSelf; }
	bool IsFriend() const { return isFriend; }
	void SetFriend(bool val) { isFriend = val; }
	bool IsFollower() const { return isFollower; }
	void SetFollower(bool val) { isFollower = val; }
	void RefreshCounts();
	void RefreshFriendships();

	UiCaption m_ctlCaption;
	UiButton_Image m_button_goback;
	UiButton_Image m_button_gohome;
	ImageContainer m_image_container;
	UiButton m_button_follow;
	RECT m_rcFollow;
	RECT m_rcToken;

	CM8WeiboUserCanvas m_canvas;
	UiScrollWin m_scrool_win;
	CM8WeiboUserScroolCanvas m_scrool_canvas;
	CM8WeiboToolbar m_toolbar;

	RECT m_rcFriends;
	RECT m_rcStatuses;
	RECT m_rcFollowers;
	RECT m_rcFavourites;
protected:
	virtual BOOL OnInitDialog();
	virtual void OnTimer(UINT_PTR nIDEvent);
	virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);

	void OnDrawFollow(HDC hDC, bool first);
	void OnDrawCounts(HDC hDC);

private:
	bool isSelf;
	bool isFriend;
	bool isFollower;
	M8::m8_user m_user;

	ImagingHelper* imgNormalGoback;
	ImagingHelper* imgPressedGoback;
	ImagingHelper* imgNormalGohome;
	ImagingHelper* imgPressedGohome;
	ImagingHelper* imgNormalRefresh;
	ImagingHelper* imgPressedRefresh;
	ImagingHelper* imgTokenBoy;
	ImagingHelper* imgTokenGirl;
	ImagingHelper* imgMale;
	ImagingHelper* imgFemale;

	int MZ_STATUS_COUNT_PER_LINE;

	// 定义各种字体的颜色
	DWORD COLOR_OF_BG_AUTHOR;
	DWORD COLOR_OF_BG_CONTENT;
	DWORD COLOR_OF_TEXT_CONTENT;
	DWORD COLOR_OF_TEXT_LABLE;
	DWORD COLOR_OF_TEXT_AUTHOR;
	DWORD COLOR_OF_SPE_LINE;
	DWORD COLOR_OF_EDG_BUTTON;
	DWORD COLOR_OF_BG_BUTTON;
	DWORD COLOR_OF_BG_FOLLOW_1;
	DWORD COLOR_OF_BG_FOLLOW_2;
	DWORD COLOR_OF_TEXT_FOLLOW;

	// 定义各种字体的行高
	int TEXT_LINE_HEIGHT;
	int AUTHOR_LINE_HEIGHT;
	int SPACE_LINE_HEIGHT;

	// 定义头像的边长
	int TOKEN_LENGTH;
	// 定义微博作者总高度
	int HEIGHT_OF_AUTHOR;
	// 定义头像与作者之间的空白
	int PAD_SPACE_OF_TOKEN_AND_AUTHOR;

	// 定义微博作者内空白
	int PAD_LEFT_AUTHOR;
	int PAD_TOP_AUTHOR;
	int PAD_RIGHT_AUTHOR;
	int PAD_BOTTOM_AUTHOR;

	// 定义关注按钮的外空白
	int MARGIN_LEFT_FOLLOW;
	int MARGIN_TOP_FOLLOW;
	int MARGIN_RIGHT_FOLLOW;
	int MARGIN_BOTTOM_FOLLOW;

	// 定义用户信息内空白
	int PAD_LEFT_CONTENT;
	int PAD_TOP_CONTENT;
	int PAD_RIGHT_CONTENT;
	int PAD_BOTTOM_CONTENT;

	// 矩形之间的垂直间距
	int V_SPACE_IN_RECT;
	int PAD_TOP_RECT;
	int PAD_BOTTOM_RECT;
	int WIDTH_OF_LABLE;

	// 定义圆角半径
	int ROUND_WIDTH;

	// 定义微博宽度和高度
	int FULL_WIDTH_OF_STATUS;
	int FULL_HEIGHT_OF_STATUS;
};
