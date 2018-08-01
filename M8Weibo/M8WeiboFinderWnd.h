#pragma once

#include <mzfc_inc.h>
#include "M8Struct.h"

#define MZ_IDC_FINDER_CAPTION      101
#define MZ_IDC_FINDER_CANCEL       102
#define MZ_IDC_FINDER_SEND         103
#define MZ_IDC_FINDER_EDIT         104
#define MZ_IDC_FINDER_TOOLBAR      105
#define MZ_IDC_FINDER_SCROOLWIN    106
#define MZ_IDC_FINDER_USER         107
#define MZ_IDC_FINDER_WEIBO        108

class CM8WeiboFinderWnd : public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CM8WeiboFinderWnd);
public:
	CM8WeiboFinderWnd(void);
	~CM8WeiboFinderWnd(void);

	UiCaption m_ctlCaption;
	//UiButton m_btnCancel;
	//UiButton m_btnSend;
	UiSingleLineEdit m_ctlEdit;
	UiScrollWin m_scrool_win;
	UiToolBarPro m_toolbar;

	UiButton m_btnUser;
	UiStatic m_staticUser;
	UiButton m_btnWeibo;
	UiStatic m_staticWeibo;

protected:
	virtual BOOL OnInitDialog();
	virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);
};
