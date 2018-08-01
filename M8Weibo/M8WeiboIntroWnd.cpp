#include "M8WeiboIntroWnd.h"
#include "M8WeiboCoverWnd.h"
#include "resource.h"

CM8WeiboIntroCanvas::CM8WeiboIntroCanvas(void)
{
	m_img_index = 0;
	IMG_TOTAL_COUNT = 4;
}

CM8WeiboIntroCanvas::~CM8WeiboIntroCanvas(void)
{
}

int CM8WeiboIntroCanvas::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	if (++m_img_index >= IMG_TOTAL_COUNT)
	{
		m_img_index = 0;
		RECT rcWork = MzGetWorkArea();
		CM8WeiboCoverWnd *pCoverWnd = new CM8WeiboCoverWnd();
		pCoverWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
		pCoverWnd->Show();
		DestroyWindow(m_hWnd);
		delete m_parentWnd;
	}
	else
	{
		m_parentWnd->Invalidate();
		m_parentWnd->UpdateWindow();
	}
	return UiCanvas::OnLButtonDown(fwKeys, xPos, yPos);
}

void CM8WeiboIntroCanvas::PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
	FillRect(hdcDst, prcWin, hBrush);
	DeleteObject(hBrush);

	if (m_img_index%IMG_TOTAL_COUNT == 0)
	{
		ImagingHelper::DrawImage(hdcDst, prcWin, MzGetInstanceHandle(), RT_RCDATA, MAKEINTRESOURCE(IDB_PNG_INTRO_0), false);
	}
	else if (m_img_index%IMG_TOTAL_COUNT == 1)
	{
		ImagingHelper::DrawImage(hdcDst, prcWin, MzGetInstanceHandle(), RT_RCDATA, MAKEINTRESOURCE(IDB_PNG_INTRO_1), false);
	}
	else if (m_img_index%IMG_TOTAL_COUNT == 2)
	{
		ImagingHelper::DrawImage(hdcDst, prcWin, MzGetInstanceHandle(), RT_RCDATA, MAKEINTRESOURCE(IDB_PNG_INTRO_2), false);
	}
	else if (m_img_index%IMG_TOTAL_COUNT == 3)
	{
		ImagingHelper::DrawImage(hdcDst, prcWin, MzGetInstanceHandle(), RT_RCDATA, MAKEINTRESOURCE(IDB_PNG_INTRO_3), false);
	}
}

MZ_IMPLEMENT_DYNAMIC(CM8WeiboIntroWnd)

CM8WeiboIntroWnd::CM8WeiboIntroWnd(void)
{
}

CM8WeiboIntroWnd::~CM8WeiboIntroWnd(void)
{
}

BOOL CM8WeiboIntroWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
	{
		return FALSE;
	}

	RECT rcWork = MzGetWorkArea();
	m_canvas.SetPos(0, 0, RECT_WIDTH(rcWork), RECT_HEIGHT(rcWork));
	m_canvas.m_dcCanvas.Create(RECT_WIDTH(rcWork), RECT_HEIGHT(rcWork));
	AddUiWin(&m_canvas);

	m_canvas.m_hWnd = m_hWnd;
	m_canvas.m_parentWnd = this;
	return TRUE;
}
