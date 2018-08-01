#include "M8WeiboRepostWnd.h"
#include "M8WeiboApp.h"
#include "StringW2A.h"
#include "StringA2W.h"

MZ_IMPLEMENT_DYNAMIC(CM8WeiboRepostWnd)

CM8WeiboRepostWnd::CM8WeiboRepostWnd(void)
{
}

CM8WeiboRepostWnd::~CM8WeiboRepostWnd(void)
{
}

BOOL CM8WeiboRepostWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
	{
		return FALSE;
	}

	int width = GetWidth();

	m_ctlCaption.SetID(MZ_IDC_REPOST_CAPTION);
	m_ctlCaption.SetPos(0, 0, width, MZM_HEIGHT_TEXT_TOOLBAR);
	m_ctlCaption.SetTextColor(RGB(89,89,89));
	m_ctlCaption.SetTextSize(30);
	m_ctlCaption.SetTextWeight(FW_MEDIUM);
	m_ctlCaption.SetText(L"转发微博");
	AddUiWin(&m_ctlCaption);

	//m_btnCancel.SetID(MZ_IDC_REPOST_CANCEL);
	//m_btnCancel.SetPos(20, 15, 100, 40);
	//m_btnCancel.SetButtonType(MZC_BUTTON_ROUND_WHITE);
	//m_btnCancel.SetText(L"取消");
	//m_btnCancel.SetTextColor(0X59BB9B);
	//m_ctlCaption.AddChild(&m_btnCancel);

	//m_btnSend.SetID(MZ_IDC_REPOST_SEND);
	//m_btnSend.SetPos(w-20-100, 15, 100, 40);
	//m_btnSend.SetButtonType(MZC_BUTTON_ROUND_WHITE);
	//m_btnSend.SetText(L"发送");
	//m_btnSend.SetTextColor(0X59BB9B);
	//m_ctlCaption.AddChild(&m_btnSend);

	int height_of_edit = 180;
	m_ctlEdit.SetID(MZ_IDC_REPOST_EDIT);
	m_ctlEdit.SetPos(0, MZM_HEIGHT_TEXT_TOOLBAR, width, height_of_edit);
	//设置编辑器是否支持表情识别
	m_ctlEdit.OpenFace(true);
	//设置字体大小，详见MzMetrics.h
	m_ctlEdit.SetFontSize(MZFS_NORMAL);
	//启用、关闭列表线的绘制
	m_ctlEdit.EnableGridlines(false);
	//每个文字使用自己的字体,颜色等
	m_ctlEdit.EnableRichTextFormat(true);
	//控件文本可以读写
	m_ctlEdit.SetReadOnly(false);
	m_ctlEdit.EnableZoomIn(false);
	//设置行距为10
	m_ctlEdit.SetLineSpace(10);
	//设置编辑器右边的无效区域宽度为10
	m_ctlEdit.SetRightInvalid(10);
	//启用列表线的绘制
	//m_ctlEdit.EnableGridlines(true);
	//设置控件内部的文字可以滚动
	m_ctlEdit.EnableInsideScroll(true);
	//编辑框的最小高度为100
	m_ctlEdit.SetInitHeight(100);
	//设置编辑器背景类型,类型详见UiEdit.h
	m_ctlEdit.SetEditBgType(UI_EDIT_BGTYPE_FILL_WHITE_AND_TOPSHADOW);
	//设置字体颜色
	m_ctlEdit.SetFontColor(RGB(64,64,64));
	//设置编辑器输入法模式,模式种类详见SipHelper.h
	m_ctlEdit.SetSipMode(IM_SIP_MODE_GEL_PY);
	//启用Redo,Undo功能
	m_ctlEdit.EnableUndo(true);

	if (m_statuses->retweeted_status_)
	{
		char buffer[WB_REAL_LEN(WB_INFO)*2];
		sprintf(buffer, "//@%s:%s", m_statuses->user_->screen_name_, m_statuses->text_);
		m_ctlEdit.SetText(CStringA2W(buffer).ptr());
		m_ctlEdit.SetCaretPos(0, 0);
	}

	AddUiWin(&m_ctlEdit);

	m_btnComment.SetID(MZ_IDC_REPOST_COMMENT);
	m_btnComment.SetPos(0,MZM_HEIGHT_TEXT_TOOLBAR+height_of_edit, 50, 64);
	m_btnComment.SetButtonType(MZCV2_CHECKBOX_SELECT);
	m_btnComment.SetButtonMode(MZC_BUTTON_MODE_HOLD);
	AddUiWin(&m_btnComment);

	m_staticComment.SetPos(64,MZM_HEIGHT_TEXT_TOOLBAR+height_of_edit,200,64);
	m_staticComment.SetText(L"同时评论原始微博");
	AddUiWin(&m_staticComment);

	m_toolbar.SetID(MZ_IDC_REPOST_TOOLBAR);
	m_toolbar.SetPos(0,GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR-3,GetWidth(),MZM_HEIGHT_TEXT_TOOLBAR+3);
	m_toolbar.SetButton(TOOLBARPRO_LEFT_TEXTBUTTON, TRUE, TRUE, L"取消");
	m_toolbar.SetButton(TOOLBARPRO_RIGHT_TEXTBUTTON, TRUE, TRUE, L"发送");
	AddUiWin(&m_toolbar);

	m_ctlEdit.SetFocus(true);
	MzOpenSip();

	return TRUE;
}

void CM8WeiboRepostWnd::OnMzCommand( WPARAM wParam, LPARAM lParam )
{
	UINT_PTR id = LOWORD(wParam);
	switch(id)
	{
	case MZ_IDC_REPOST_TOOLBAR:
		{
			int nIndex = lParam;
			if (nIndex==TOOLBARPRO_LEFT_TEXTBUTTON)
			{
				if (MzIsSipOpen()) MzCloseSip();
				DestroyWindow();
				delete this;
				theApp.PopWnd(this);
			}
			else if (nIndex==TOOLBARPRO_RIGHT_TEXTBUTTON)
			{
				CMzString szText = m_ctlEdit.GetText();
				if (szText.Length() == 0)
				{
					MzMessageBox(m_hWnd, L"转发内容为空！", NULL, MB_OK, 1000, TRUE);
					break;
				}
				if (!theApp.IsValid())
				{
					MzMessageBox(m_hWnd, L"试用版不能转发微博！", NULL, MB_OK, 1000, TRUE);
					break;
				}
				if (MzIsSipOpen()) MzCloseSip();
				if (theApp.PutStatusesRepost(m_hWnd, m_statuses->id_, m_btnComment.GetState(), m_ctlEdit.GetText()))
				{
					MzMessageBox(m_hWnd, L"发送成功！", NULL, MB_OK, 1000, TRUE);
				}
				DestroyWindow();
				delete this;
				theApp.PopWnd(this);
			}
		}
		break;
	}
}

int CM8WeiboRepostWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	 /*switch(message)
	 {
	 }*/
	 return SHK_RET_APPNOEXIT_SHELLTOP;
}
