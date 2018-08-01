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

	// ������΢�������ۼ�������λ��
	RECT m_rc_rt_comments;
	RECT m_rc_rt_links;
	// ������΢�������ۼ��������߿�λ��
	RECT m_rc_rt_comments_box;
	RECT m_rc_rt_links_box;
	// ΢�����ĵ����ۼ�������λ��
	RECT m_rc_comments;
	RECT m_rc_links;
	// ΢�����ĵ����ۼ��������߿�λ��
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

	// ��������������ɫ
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

	// �������������и�
	int TEXT_LINE_HEIGHT;
	int AUTHOR_LINE_HEIGHT;
	int CREATE_SOURCE_LINE_HEIGHT;
	int COUNTS_LINE_HEIGHT;
	int RT_COUNTS_LINE_HEIGHT;
	int SPACE_LINE_HEIGHT;
	int RT_TEXT_LINE_HEIGHT;
	int RT_SPACE_LINE_HEIGHT;

	// �����������εı߳������������ƫ������Բ�ǰ뾶
	int DELTA_LENGTH;
	int DELTA_OFFSET;
	int ROUND_WIDTH;
	// ����ͷ��ı߳�
	int TOKEN_LENGTH;
	// ����΢�������ܸ߶�
	int HEIGHT_OF_AUTHOR;
	// ����ͷ��������֮��Ŀհ�
	int PAD_SPACE_OF_TOKEN_AND_AUTHOR;

	// ����΢�������ڿհ�
	int PAD_LEFT_AUTHOR;
	int PAD_TOP_AUTHOR;
	int PAD_RIGHT_AUTHOR;
	int PAD_BOTTOM_AUTHOR;	

	// ����΢�������ڿհ�
	int PAD_LEFT_CONTENT;
	int PAD_TOP_CONTENT;
	int PAD_RIGHT_CONTENT;
	int PAD_BOTTOM_CONTENT;

	// ����΢�����ĵز��ڿհ׵Ĵ�����Դ����������۰�ť�ĸ߶�
	int CREATE_SOURCE_HEIGHT;
	int LINK_COMMENTS_HEIGHT;

	// ��������΢������հ�
	int MARGIN_LEFT_RT;
	int MARGIN_TOP_RT;
	int MARGIN_RIGHT_RT;
	int MARGIN_BOTTOM_RT;

	// ��������΢�����ڿհ�
	int PAD_LEFT_RT;
	int PAD_TOP_RT;
	int PAD_RIGHT_RT;
	int PAD_BOTTOM_RT;

	// ��������΢����(���ú����۵��ҿհ׺Ϳ�ȡ�ͼ�������֮��ļ��)
	int RT_MARGIN_RIGHT_OTHER;
	int RT_WIDTH_OF_OTHER;
	int RT_SPACE_OF_ICON_AND_COUNTS;

	// ���崴��ʱ�����Դ����հ�
	int MARGIN_LEFT_CREATE_SOURCE;

	// ����΢�������ú����۰�ť�����С��ҡ��ϡ��¿հ׼���ť��ͼ�������߿�Ŀ��
	int SPACE_LEFT_COUNTS;
	int SPACE_MID_COUNTS;
	int SPACE_RIGHT_COUNTS;
	int SPACE_TOP_COUNTS;
	int SPACE_BOTTOM_COUNTS;
	int SPACE_LEFT_IN_BUTTON;

	// ��������ͼ���Ͽհס��¿հס���հס��ҿհ�
	int MIDDLE_TOP_HEIGHT;
	int MIDDLE_BOTTOM_HEIGHT;
	int MIDDLE_LEFT_PAD;
	int MIDDLE_RIGHT_PAD;

	// ����΢����Ⱥ͸߶�
	int FULL_WIDTH_OF_STATUS;
	int FULL_HEIGHT_OF_STATUS;
};
