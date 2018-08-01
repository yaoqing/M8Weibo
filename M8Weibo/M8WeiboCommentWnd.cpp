#include "M8WeiboCommentWnd.h"
#include "M8WeiboApp.h"
#include "StringW2A.h"
#include "StringA2W.h"
#include "M8WeiboCommentViewWnd.h"

MZ_IMPLEMENT_DYNAMIC(CM8WeiboCommentWnd)

CM8WeiboCommentWnd::CM8WeiboCommentWnd(void)
{
	m_statuses = NULL;
	m_comment = NULL;
	m_pCommentViewWnd = NULL;
}

CM8WeiboCommentWnd::~CM8WeiboCommentWnd(void)
{
}

BOOL CM8WeiboCommentWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
	{
		return FALSE;
	}

	int width = GetWidth();

	m_ctlCaption.SetID(MZ_IDC_COMMENT_CAPTION);
	m_ctlCaption.SetPos(0, 0, width, MZM_HEIGHT_TEXT_TOOLBAR);
	m_ctlCaption.SetTextColor(RGB(89,89,89));
	m_ctlCaption.SetTextSize(30);
	m_ctlCaption.SetTextWeight(FW_MEDIUM);
	m_ctlCaption.SetText(m_comment? L"回复评论": L"发表评论");
	AddUiWin(&m_ctlCaption);

	//m_btnCancel.SetID(MZ_IDC_COMMENT_CANCEL);
	//m_btnCancel.SetPos(20, 15, 100, 40);
	//m_btnCancel.SetButtonType(MZC_BUTTON_ROUND_WHITE);
	//m_btnCancel.SetText(L"取消");
	//m_btnCancel.SetTextColor(0X59BB9B);
	//m_ctlCaption.AddChild(&m_btnCancel);

	//m_btnSend.SetID(MZ_IDC_COMMENT_SEND);
	//m_btnSend.SetPos(w-20-100, 15, 100, 40);
	//m_btnSend.SetButtonType(MZC_BUTTON_ROUND_WHITE);
	//m_btnSend.SetText(L"发送");
	//m_btnSend.SetTextColor(0X59BB9B);
	//m_ctlCaption.AddChild(&m_btnSend);

	int height_of_edit = 180;
	m_ctlEdit.SetID(MZ_IDC_COMMENT_EDIT);
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
	AddUiWin(&m_ctlEdit);

	m_btnRepost.SetID(MZ_IDC_COMMENT_REPOST);
	m_btnRepost.SetPos(0,MZM_HEIGHT_TEXT_TOOLBAR+height_of_edit, 50, 64);
	m_btnRepost.SetButtonType(MZCV2_CHECKBOX_SELECT);
	m_btnRepost.SetButtonMode(MZC_BUTTON_MODE_HOLD);
	AddUiWin(&m_btnRepost);

	m_staticRepost.SetPos(64,MZM_HEIGHT_TEXT_TOOLBAR+height_of_edit,220,64);
	m_staticRepost.SetText(L"同时转发到我的微博");
	AddUiWin(&m_staticRepost);

	m_toolbar.SetID(MZ_IDC_COMMENT_TOOLBAR);
	m_toolbar.SetPos(0,GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR-3,GetWidth(),MZM_HEIGHT_TEXT_TOOLBAR+3);
	m_toolbar.SetButton(TOOLBARPRO_LEFT_TEXTBUTTON, TRUE, TRUE, L"取消");
	m_toolbar.SetButton(TOOLBARPRO_RIGHT_TEXTBUTTON, TRUE, TRUE, L"发送");
	AddUiWin(&m_toolbar);

	m_ctlEdit.SetFocus(true);
	MzOpenSip();

	return TRUE;
}

void CM8WeiboCommentWnd::OnMzCommand( WPARAM wParam, LPARAM lParam )
{
	UINT_PTR id = LOWORD(wParam);
	switch(id)
	{
	case MZ_IDC_COMMENT_TOOLBAR:
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
					MzMessageBox(m_hWnd, L"评论内容为空！", NULL, MB_OK, 1000, TRUE);
					break;
				}
				if (!theApp.IsValid())
				{
					MzMessageBox(m_hWnd, L"试用版不能评论！", NULL, MB_OK, 1000, TRUE);
					break;
				}
				if (MzIsSipOpen()) MzCloseSip();
				if (m_btnRepost.GetState())
				{
					if (m_comment)
					{
						M8::m8_wb_put_statuses_comment *p_status_comment = NULL;
						if (theApp.PutStatusesComment(m_hWnd, m_statuses->id_, m_comment->id_, m_ctlEdit.GetText(), p_status_comment))
						{
							if (theApp.PutStatusesRepost(m_hWnd, m_statuses->id_, 0, CMzString(CStringA2W(p_status_comment->status_.text_).ptr())))
							{
								MzMessageBox(m_hWnd, L"发送成功！", NULL, MB_OK, 1000, TRUE);
							}
							delete p_status_comment;
						}
					}
					else
					{
						if (theApp.PutStatusesRepost(m_hWnd, m_statuses->id_, 1, m_ctlEdit.GetText()))
						{
							MzMessageBox(m_hWnd, L"发送成功！", NULL, MB_OK, 1000, TRUE);
						}
					}
				}
				else
				{
					M8::m8_wb_put_statuses_comment *p_status_comment = NULL;
					if (theApp.PutStatusesComment(m_hWnd, m_statuses->id_, m_comment? m_comment->id_: NULL, m_ctlEdit.GetText(), p_status_comment))
					{
						delete p_status_comment;
						MzMessageBox(m_hWnd, L"发送成功！", NULL, MB_OK, 1000, TRUE);
					}
				}
				DestroyWindow();
				delete this;
				theApp.PopWnd(this);
				if (m_pCommentViewWnd) m_pCommentViewWnd->PostMessage(MZ_WM_COMMAND, MZ_IDC_COMMENT_VIEW_WORKING, t_wb_option_params::WBOPTType_since_id);
			}
		}
		break;
	}
}

int CM8WeiboCommentWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	 /*switch(message)
	 {
	 }*/
	 return SHK_RET_APPNOEXIT_SHELLTOP;
}
