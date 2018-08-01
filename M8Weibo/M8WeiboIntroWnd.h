#pragma once

#include <mzfc_inc.h>

class CM8WeiboIntroCanvas : public UiCanvas
{
public:
	CM8WeiboIntroCanvas(void);
	~CM8WeiboIntroCanvas(void);
	HWND m_hWnd;
	CMzWndEx * m_parentWnd;
	int m_img_index;
	int IMG_TOTAL_COUNT;
	virtual void PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate);
	virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboIntroWnd : public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CM8WeiboIntroWnd);
public:
	CM8WeiboIntroWnd(void);
	~CM8WeiboIntroWnd(void);
	CM8WeiboIntroCanvas m_canvas;

protected:
	virtual BOOL OnInitDialog();
};
