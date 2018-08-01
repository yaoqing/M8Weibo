#pragma once

#include <mzfc_inc.h>
#include "M8Struct.h"
#include "M8WeiboToolbar.h"

#define MZ_IDC_STATUS_CAPTION                101
#define MZ_IDC_STATUS_GOBACK                 102
#define MZ_IDC_STATUS_GOHOME                 103
#define MZ_IDC_STATUS_SCROOLWIN              104
#define MZ_IDC_STATUS_CANVAS                 105
#define MZ_IDC_STATUS_SCANVAS                106
#define MZ_IDC_STATUS_TOOLBAR                110

#define MZ_ID_STATUS_TIMER                   200

class CM8WeiboTokenCanvas: public UiCanvas
{
public:
	CM8WeiboTokenCanvas()
	{
		m_user = NULL;
	}
	~CM8WeiboTokenCanvas()
	{
		m_user = NULL;
	}
	M8::m8_user * m_user;
	virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboStatusesWnd;
class CM8WeiboStatusesCanvas: public UiCanvas
{
public:
	CM8WeiboStatusesCanvas(void);
	~CM8WeiboStatusesCanvas(void);
	HWND m_hWnd;
	CM8WeiboStatusesWnd *m_parentWnd;
	virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboStatusesWnd : public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CM8WeiboStatusesWnd);
public:
	CM8WeiboStatusesWnd(void);
	~CM8WeiboStatusesWnd(void);

	M8::m8_status *m_status;

	UiCaption m_ctlCaption;
	UiButton_Image m_button_goback;
	UiButton_Image m_button_gohome;
	ImageContainer m_image_container;

	CM8WeiboTokenCanvas m_canvas;
	UiScrollWin m_scrool_win;
	CM8WeiboStatusesCanvas m_scrool_canvas;
	CM8WeiboToolbar m_toolbar;

	unsigned long m_rt_comments;
	unsigned long m_rt_links;
	int m_favorite_flag;

	// 被引用微博的评论及引用数位置
	RECT m_rc_rt_comments;
	RECT m_rc_rt_links;
	// 被引用微博的评论及引用数边框位置
	RECT m_rc_rt_comments_box;
	RECT m_rc_rt_links_box;
	// 微博正文的评论及引用数位置
	RECT m_rc_comments;
	RECT m_rc_links;
	// 微博正文的评论及引用数边框位置
	RECT m_rc_comments_box;
	RECT m_rc_links_box;

protected:
	virtual BOOL OnInitDialog();
	virtual void OnTimer(UINT_PTR nIDEvent);
	virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);

private:
	ImagingHelper* imgNormalGoback;
	ImagingHelper* imgNormalGohome;
	ImagingHelper* imgPressedGoback;
	ImagingHelper* imgPressedGohome;
	ImagingHelper* imgTokenBoy;
	ImagingHelper* imgTokenGirl;
	ImagingHelper* imgArrowGray;
	ImagingHelper* imgSmallComment;
	ImagingHelper* imgSmallLink;
	ImagingHelper* imgNormalComment;
	ImagingHelper* imgNormalLink;

	int MZ_STATUS_COUNT_PER_LINE;

	// 定义各种字体的颜色
	DWORD COLOR_OF_BG_AUTHOR;
	DWORD COLOR_OF_BG_CONTENT;
	DWORD COLOR_OF_BG_RT;
	DWORD COLOR_OF_EDG_RT;
	DWORD COLOR_OF_TEXT_CONTENT;
	DWORD COLOR_OF_TEXT_AUTHOR;
	DWORD COLOR_OF_ARROR_AUTHOR;
	DWORD COLOR_OF_TEXT_COUNTS;
	DWORD COLOR_OF_TEXT_CREATE_SOURCE;
	DWORD COLOR_OF_RT_TEXT_COUNTS;
	DWORD COLOR_OF_RT_TEXT_CONTENT;
	DWORD COLOR_OF_RT_LINE;
	DWORD COLOR_OF_EDG_BUTTON;
	DWORD COLOR_OF_BG_BUTTON;

	// 定义各种字体的行高
	int TEXT_LINE_HEIGHT;
	int AUTHOR_LINE_HEIGHT;
	int CREATE_SOURCE_LINE_HEIGHT;
	int COUNTS_LINE_HEIGHT;
	int RT_COUNTS_LINE_HEIGHT;
	int SPACE_LINE_HEIGHT;
	int RT_TEXT_LINE_HEIGHT;
	int RT_SPACE_LINE_HEIGHT;

	// 定义上三角形的边长、三角形左边偏移量、圆角半径
	int DELTA_LENGTH;
	int DELTA_OFFSET;
	int ROUND_WIDTH;
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

	// 定义微博正文内空白
	int PAD_LEFT_CONTENT;
	int PAD_TOP_CONTENT;
	int PAD_RIGHT_CONTENT;
	int PAD_BOTTOM_CONTENT;

	// 定义微博正文地步内空白的创建来源块和引用评论按钮的高度
	int CREATE_SOURCE_HEIGHT;
	int LINK_COMMENTS_HEIGHT;

	// 定义引用微博的外空白
	int MARGIN_LEFT_RT;
	int MARGIN_TOP_RT;
	int MARGIN_RIGHT_RT;
	int MARGIN_BOTTOM_RT;

	// 定义引用微博的内空白
	int PAD_LEFT_RT;
	int PAD_TOP_RT;
	int PAD_RIGHT_RT;
	int PAD_BOTTOM_RT;

	// 定义引用微博的(引用和评论的右空白和宽度、图标和数字之间的间隔)
	int RT_MARGIN_RIGHT_OTHER;
	int RT_WIDTH_OF_OTHER;
	int RT_SPACE_OF_ICON_AND_COUNTS;

	// 定义创建时间和来源的左空白
	int MARGIN_LEFT_CREATE_SOURCE;

	// 定义微博的引用和评论按钮的左、中、右、上、下空白及按钮中图标距离左边框的宽度
	int SPACE_LEFT_COUNTS;
	int SPACE_MID_COUNTS;
	int SPACE_RIGHT_COUNTS;
	int SPACE_TOP_COUNTS;
	int SPACE_BOTTOM_COUNTS;
	int SPACE_LEFT_IN_BUTTON;

	// 定义缩略图的上空白、下空白、左空白、右空白
	int MIDDLE_TOP_HEIGHT;
	int MIDDLE_BOTTOM_HEIGHT;
	int MIDDLE_LEFT_PAD;
	int MIDDLE_RIGHT_PAD;

	// 定义微博宽度和高度
	int FULL_WIDTH_OF_STATUS;
	int FULL_HEIGHT_OF_STATUS;
};
