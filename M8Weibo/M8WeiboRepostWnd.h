#pragma once

#include <mzfc_inc.h>
#include "M8Struct.h"

#define MZ_IDC_REPOST_CAPTION      101
#define MZ_IDC_REPOST_CANCEL       102
#define MZ_IDC_REPOST_SEND         103
#define MZ_IDC_REPOST_EDIT         104
#define MZ_IDC_REPOST_TOOLBAR      105
#define MZ_IDC_REPOST_COMMENT      106

class CM8WeiboRepostWnd : public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CM8WeiboRepostWnd);
public:
	CM8WeiboRepostWnd(void);
	~CM8WeiboRepostWnd(void);

	M8::m8_status *m_statuses;

	UiCaption m_ctlCaption;
	//UiButton m_btnCancel;
	//UiButton m_btnSend;
	UiEdit m_ctlEdit;
	UiButton m_btnComment;
	UiStatic m_staticComment;
	UiToolBarPro m_toolbar;

protected:
	virtual BOOL OnInitDialog();
	virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);
};
