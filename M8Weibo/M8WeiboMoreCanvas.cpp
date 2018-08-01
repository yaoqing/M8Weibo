#include "M8WeiboMoreCanvas.h"
#include "M8WeiboApp.h"
#include "M8WeiboHomeWnd.h"
#include "M8WeiboCommentViewWnd.h"
#include "M8WeiboUserListWnd.h"
#include "M8Struct.h"

CM8WeiboMoreCanvas::CM8WeiboMoreCanvas(void)
{
	COLOR_OF_BACKGROUND = RGB(220,230,242);
	COLOR_OF_TEXT = RGB(89,89,89);
	LINE_HEIGHT_OF_TEXT = 26;
	m_hWnd = NULL;
}

CM8WeiboMoreCanvas::~CM8WeiboMoreCanvas(void)
{
}

void CM8WeiboMoreCanvas::PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate)
{
	HBRUSH hBrush = CreateSolidBrush(COLOR_OF_BACKGROUND);
	FillRect(hdcDst, prcWin, hBrush);
	DeleteObject(hBrush);

	// 输出更多
	LOGFONT font_struct={ 0, 0, 0, 0, 0, 0, 0, 0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"宋体"
	};
	font_struct.lfHeight = LINE_HEIGHT_OF_TEXT;
	HFONT hFont = CreateFontIndirect(&font_struct);
	HGDIOBJ hSavedFont = SelectObject(hdcDst, hFont);
	SetBkMode(hdcDst,TRANSPARENT);
	SetTextColor(COLOR_OF_TEXT);
	DrawText(hdcDst, L"更多...", -1, prcWin, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	SelectObject(hdcDst, hSavedFont);
	DeleteObject(hFont);
}

int CM8WeiboHomeMoreCanvas::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	if (theApp.IsValid())
	{
		::PostMessage(m_hWnd, MZ_WM_COMMAND, MZ_IDC_HOME_WORKING, t_wb_option_params::WBOPTType_max_id);
	}
	else
	{
		MzMessageBox(m_hWnd, L"试用版不能查看更多！", NULL, MB_OK, 1000, TRUE);
	}
	return UiCanvas::OnLButtonUp(fwKeys, xPos, yPos);
}

int CM8WeiboCommentViewMoreCanvas::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	if (theApp.IsValid())
	{
		::PostMessage(m_hWnd, MZ_WM_COMMAND, MZ_IDC_COMMENT_VIEW_WORKING, t_wb_option_params::WBOPTType_max_id);
	}
	else
	{
		MzMessageBox(m_hWnd, L"试用版不能查看更多！", NULL, MB_OK, 1000, TRUE);
	}
	return UiCanvas::OnLButtonUp(fwKeys, xPos, yPos);
}

int CM8WeiboUserListMoreCanvas::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	if (theApp.IsValid())
	{
		::PostMessage(m_hWnd, MZ_WM_COMMAND, MZ_IDC_USER_LIST_WORKING, t_wb_option_params::WBOPTType_max_id);
	}
	else
	{
		MzMessageBox(m_hWnd, L"试用版不能查看更多！", NULL, MB_OK, 1000, TRUE);
	}
	return UiCanvas::OnLButtonUp(fwKeys, xPos, yPos);
}