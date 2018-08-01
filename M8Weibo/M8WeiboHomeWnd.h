#pragma once

#include <mzfc_inc.h>
#include "M8Struct.h"
#include "M8WeiboToolbar.h"
#include "M8WeiboMoreCanvas.h"
#include <list>

#define MZ_IDC_HOME_CAPTION           101
#define MZ_IDC_HOME_WRITE             102
#define MZ_IDC_HOME_REFRESH           103
#define MZ_IDC_HOME_SCROOLWIN         104
#define MZ_IDC_HOME_TOOLBAR           105
#define MZ_IDC_HOME_WORKING           120
#define MZ_IDC_HOME_MORE              999
#define MZ_IDC_HOME_STATUSES          1000

#define MZ_ID_HOME_TIMER              200

class CM8WeiboHomeCanvas: public UiCanvas
{
public:
	M8::m8_status *m_status; // 用于设置定时器ID
	RECT m_rcStatus;
	HWND m_hWnd;
	CM8WeiboHomeCanvas() { init(); }
	~CM8WeiboHomeCanvas() { init(); }
	void init();
	virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboHomeScroolWin : public UiScrollWin
{
public:
	HWND m_hWnd;
	DWORD m_dwTimerID;
	virtual int OnLButtonDown(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboHomeWnd: public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CM8WeiboHomeWnd);
public:
	CM8WeiboHomeWnd(void);
	~CM8WeiboHomeWnd(void);

	UiCaption m_ctlCaption;
	UiButton_Image m_button_write;
	UiButton_Image m_button_refresh;
	ImageContainer m_image_container;

	CM8WeiboToolbar m_toolbar;
	CM8WeiboHomeScroolWin m_scrool_win;

	std::list<CM8WeiboHomeCanvas *> m_list_canvas;
	CM8WeiboHomeMoreCanvas m_more_canvas;

	M8::m8_user *m_user;
	int m_user_type;
	CMzString m_search_name;

	struct STATUSES
	{
		int size_of_status_;
		M8::m8_status *statuses_;
		int page_;
	};

	enum
	{
		ALL_FRIENDS = 0,
		THE_FRIEND,
		FAVOURITE,
		MENTIONS,
		COMMENT_TOME,
		WB_SEARCH,
	};

protected:
	virtual BOOL OnInitDialog();
	virtual void OnTimer(UINT_PTR nIDEvent);
	// Over write MZFC's OnMzCommand function to dispose command notify
	virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual int OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);

    //void CM8WeiboHomeWnd::OnCommond(WORD wNotifyCode, WORD wID, HWND hwndctl);
  
    //void CM8WeiboHomeWnd::OnLButtonDblClk(UINT fwKeys, int xPos, int yPos);
  
    //void CM8WeiboHomeWnd::OnLButtonDown(UINT fwKeys, int xPos, int yPos);
  
    //void CM8WeiboHomeWnd::OnLButtonUp(UINT fwKeys, int xPos, int yPos);
  
    //void CM8WeiboHomeWnd::OnMouseMove(UINT fwKeys, int xPos, int yPos);
  
    //void CM8WeiboHomeWnd::OnPaint(HDC hdc, LPPAINTSTRUCT ps);
  
    //void CM8WeiboHomeWnd::OnSettingChange(DWORD wFlag, LPCTSTR pszSectionName);
  
    //int CM8WeiboHomeWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);
  
    //void CM8WeiboHomeWnd::OnSize(int nWidth, int nHeight);
  
    //void CM8WeiboHomeWnd::OnTimer(UINT_PTR nIDEvent);

private:
	std::list<STATUSES> m_list_statuses;

	void DrawRetweetedStatus(M8::m8_status *retweeted_status, HDC hDC, RECT rcOrigin, int offsetLeft, int offsetTop, int offsetRight);

	void RemoveAllStatus(int opt_type);
	bool RefreshFriendsStatus(int opt_type, M8::m8_wb_get_statuses_friends_timeline *&pStatuses);
	bool RefreshUserStatus(int opt_type, M8::m8_wb_get_statuses_user_timeline *&pStatuses);
	bool RefreshFavorites(int opt_type, M8::m8_wb_get_favorites *&pStatuses);
	bool RefreshMentions(int opt_type, M8::m8_wb_get_statuses_mentions *&pStatuses);
	bool RefreshCommentsToMe(int opt_type, M8::m8_wb_get_statuses_comments_tome *&pStatuses);
	bool RefreshWeiboSearch(int opt_type, M8::m8_wb_search *&pStatuses);

	void AddFriendsStatus(int opt_type, M8::m8_wb_get_statuses_friends_timeline * pStatuses);
	void AddUserStatus(int opt_type, M8::m8_wb_get_statuses_user_timeline * pStatuses);
	void AddFavorites(int opt_type, M8::m8_wb_get_favorites * pStatuses);
	void AddMentions(int opt_type, M8::m8_wb_get_statuses_mentions * pStatuses);
	void AddCommentsToMe(int opt_type, M8::m8_wb_get_statuses_comments_tome * pStatuses);
	void AddWeiboSearch(int opt_type, M8::m8_wb_search *&pStatuses);

	int GetStatusArray(M8::m8_status **&p_status_list);
	void DrawAllStatuses(M8::m8_status ** p_status_list, int size_of_status);
	void MoveMoreButton();

private:
	ImagingHelper* imgNormalWrite;
	ImagingHelper* imgNormalRefresh;
	ImagingHelper* imgPressedWrite;
	ImagingHelper* imgPressedRefresh;
	ImagingHelper* imgNormalGoback;
	ImagingHelper* imgPressedGoback;
	ImagingHelper* imgNormalGohome;
	ImagingHelper* imgPressedGohome;
	ImagingHelper* imgTokenBoy;
	ImagingHelper* imgTokenGirl;
	ImagingHelper* imgSmallPicture;
	ImagingHelper* imgSmallComment;
	ImagingHelper* imgSmallLink;
	ImagingHelper* imgNormalAtme;
	ImagingHelper* imgNormalTome;
	ImagingHelper* imgPressedAtme;
	ImagingHelper* imgPressedTome;

	int MZ_STATUS_COUNT_PER_LINE;

	// 定义各种字体的颜色
	DWORD COLOR_OF_BG_STATUS;
	DWORD COLOR_OF_BG_CONTENT;
	DWORD COLOR_OF_BG_CONTENT_SELF;
	DWORD COLOR_OF_EDG_CONTENT;
	DWORD COLOR_OF_EDG_CONTENT_SELF;
	DWORD COLOR_OF_TEXT_CONTENT;
	DWORD COLOR_OF_TEXT_AUTHOR;
	DWORD COLOR_OF_TEXT_CREATE_AT;
	DWORD COLOR_OF_TEXT_SOURCE;
	DWORD COLOR_OF_TEXT_COUNTS;
	DWORD COLOR_OF_RT_TEXT_CONTENT;
	DWORD COLOR_OF_RT_LINE;

	// 定义各种字体的行高
	int TEXT_LINE_HEIGHT;
	int AUTHOR_LINE_HEIGHT;
	int CREATE_AT_LINE_HEIGHT;
	int SOURCE_LINE_HEIGHT;	
	int COUNTS_LINE_HEIGHT;
	int SPACE_LINE_HEIGHT;
	int RT_TEXT_LINE_HEIGHT;
	int RT_SPACE_LINE_HEIGHT;

	// 定义左三角形的边长
	int DELTA_LENGTH;
	// 定义头像的边长
	int TOKEN_LENGTH;

	// 定义单条微博的外空白
	int MARGIN_TOP_STATUS;
	int MARGIN_BOTTOM_STATUS;
	int MARGIN_LEFT_STATUS;
	int MARGIN_RIGHT_STATUS;

	// 定义单条微博的内空白
	int PAD_TOP_STATUS;
	int PAD_BOTTOM_STATUS;
	int PAD_LEFT_STATUS;
	int PAD_RIGHT_STATUS;

	// 定义微博正文的外空白
	int MARGIN_TOP_CONTENT;
	int MARGIN_BOTTOM_CONTENT;
	int MARGIN_LEFT_CONTENT;
	int MARGIN_RIGHT_CONTENT;

	// 定义微博正文的内空白
	int PAD_TOP_CONTENT;
	int PAD_BOTTOM_CONTENT;
	int PAD_LEFT_CONTENT;
	int PAD_RIGHT_CONTENT;

	// 定义作者的左空白、创建时间右空白和宽度、图标和创建时间之间的间隔
	int MARGIN_LEFT_AUTHOR;
	int MARGIN_RIGHT_CREATED_AT;
	int WIDTH_OF_CREATED_AT;
	int SPACE_OF_ICON_AND_CREATED_AT;

	// 定义来源的左空白、引用和评论的右空白和宽度、图标和数字之间的间隔
	int MARGIN_LEFT_SORUCE;
	int MARGIN_RIGHT_OTHER;
	int WIDTH_OF_OTHER;
	int SPACE_OF_ICON_AND_COUNTS;

	// 定义引用微博的左缩进、上空白和下空白（为简化代码，不设置内衬大小）
	int RT_TOP_HEIGHT;
	int RT_BOTTOM_HEIGHT;
	int RT_TAB_WIDTH;

	// 定义缩略图的上空白、下空白、左空白、固定宽度和高度
	int THUMB_TOP_HEIGHT;
	int THUMB_BOTTOM_HEIGHT;
	int THUMB_LEFT_PAD;
	int THUMB_FIX_WIDTH;
	int THUMB_FIX_HEIGHT;

	// 定义微博宽度和高度
	int FULL_WIDTH_OF_STATUS;
	int FULL_HEIGHT_OF_STATUS;
  };
