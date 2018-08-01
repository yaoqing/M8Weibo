#pragma once

#include <mzfc_inc.h>
#include "M8Struct.h"
#include "M8WeiboMoreCanvas.h"
#include <list>

#define MZ_IDC_COMMENT_VIEW_CAPTION                101
#define MZ_IDC_COMMENT_VIEW_GOBACK                 102
#define MZ_IDC_COMMENT_VIEW_COMMENT                103
#define MZ_IDC_COMMENT_VIEW_REPLY                  104
#define MZ_IDC_COMMENT_VIEW_SCROOLWIN              105
#define MZ_IDC_COMMENT_VIEW_TOOLBAR                106
#define MZ_IDC_COMMENT_VIEW_CANVAS                 107
#define MZ_IDC_COMMENT_VIEW_WORKING                120
#define MZ_IDC_COMMENT_VIEW_CLICK_DOWN             121
#define MZ_IDC_COMMENT_VIEW_CLICK_UP               122
#define MZ_IDC_COMMENT_VIEW_MORE                   999
#define MZ_IDC_COMMENT_VIEW_COMMENTES              1000

#define MZ_ID_COMMENT_VIEW_TIMER                   200

class CM8WeiboCommentViewCanvas: public UiCanvas
{
public:
	M8::m8_comment *m_comment; // 用于设置定时器ID
	RECT m_rcComment;
	HWND m_hWnd;
	DWORD m_color_bg;
	int m_right_margin;
	CM8WeiboCommentViewCanvas() { init(); }
	~CM8WeiboCommentViewCanvas() { init(); }
	void init();
	virtual int OnLButtonDown(UINT fwKeys, int xPos, int yPos);
	virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboCommentViewScroolWin : public UiScrollWin
{
public:
	HWND m_hWnd;
	DWORD m_dwTimerID;
	virtual int OnLButtonDown(UINT fwKeys, int xPos, int yPos);
	virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboCommentViewWnd: public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CM8WeiboCommentViewWnd);
public:
	CM8WeiboCommentViewWnd(void);
	~CM8WeiboCommentViewWnd(void);

	M8::m8_status *m_status;
	//M8::m8_wb_get_statuses_comments_list *m_comment_list;

	UiCaption m_ctlCaption;
	UiButton_Image m_button_goback;
	UiButton_Image m_button_comment;
	ImageContainer m_image_container;

	int m_size_of_comment;
	CM8WeiboCommentViewScroolWin m_scrool_win;
	CM8WeiboCommentViewCanvas * m_comments_canvas;
	CM8WeiboCommentViewMoreCanvas m_more_canvas;

protected:
	virtual BOOL OnInitDialog();
	virtual void OnTimer(UINT_PTR nIDEvent);
	virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);

private:
	std::list<M8::m8_wb_get_statuses_comments_list *> m_list_comments;
	void MoveMoreButton();
	bool RefreshComment(int opt_type, M8::m8_wb_get_statuses_comments_list*& p_comments_list);
	void RemoveAllComment(int opt_type);
	void DrawAllComment(M8::m8_comment **p_comment_list);
	void AddComment(int opt_type, M8::m8_wb_get_statuses_comments_list* p_comments_list);
	int GetCommentArray(M8::m8_comment **&p_comment_list);
	void OnDrawComment(HDC hDC, RECT rcComment, DWORD bg_color, bool pressed, M8::m8_comment * p_comment, int lines, CMzString * p_lines);
	void OnCommentClick(CM8WeiboCommentViewCanvas * p_canvas, bool pressed);
	void OnCommentReply(M8::m8_comment *p_comment);

private:
	ImagingHelper* imgNormalGoback;
	ImagingHelper* imgPressedGoback;
	ImagingHelper* imgNormalComment;
	ImagingHelper* imgPressedComment;

	int MZ_COMMENT_COUNT_PER_LINE;

	// 定义各种字体的颜色
	DWORD COLOR_OF_BG_COMMENT_P;
	DWORD COLOR_OF_FG_CONTENT_P;
	DWORD COLOR_OF_BG_COMMENT_0;
	DWORD COLOR_OF_BG_COMMENT_1;
	DWORD COLOR_OF_TEXT_CONTENT;
	DWORD COLOR_OF_TEXT_AUTHOR;
	DWORD COLOR_OF_TEXT_CREATE_AT;

	// 定义各种字体的行高
	int TEXT_LINE_HEIGHT;
	int AUTHOR_LINE_HEIGHT;
	int CREATE_AT_LINE_HEIGHT;
	int SPACE_LINE_HEIGHT;

	// 定义单条微博的内空白
	int PAD_TOP_COMMENT;
	int PAD_BOTTOM_COMMENT;
	int PAD_LEFT_COMMENT;
	int PAD_RIGHT_COMMENT;

	// 定义作者的左空白、创建时间右空白和宽度
	int MARGIN_LEFT_AUTHOR;
	int MARGIN_RIGHT_CREATED_AT;
	int WIDTH_OF_CREATED_AT;

	// 定义微博宽度和高度
	int FULL_WIDTH_OF_COMMENT;
	int FULL_HEIGHT_OF_COMMENT;
};
