#pragma once

#include <mzfc_inc.h>
#include "M8Struct.h"

#define MZ_IDC_COMMENT_CAPTION      101
#define MZ_IDC_COMMENT_CANCEL       102
#define MZ_IDC_COMMENT_SEND         103
#define MZ_IDC_COMMENT_EDIT         104
#define MZ_IDC_COMMENT_TOOLBAR      105
#define MZ_IDC_COMMENT_REPOST       106

class CM8WeiboCommentViewWnd;
class CM8WeiboCommentWnd : public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CM8WeiboCommentWnd);
public:
	CM8WeiboCommentWnd(void);
	~CM8WeiboCommentWnd(void);

	M8::m8_status *m_statuses;
	M8::m8_comment *m_comment;
	CM8WeiboCommentViewWnd *m_pCommentViewWnd;

	UiCaption m_ctlCaption;
	//UiButton m_btnCancel;
	//UiButton m_btnSend;
	UiEdit m_ctlEdit;
	UiButton m_btnRepost;
	UiStatic m_staticRepost;
	UiToolBarPro m_toolbar;

protected:
	virtual BOOL OnInitDialog();
	virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);
};
