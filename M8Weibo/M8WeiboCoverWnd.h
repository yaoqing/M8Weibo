#pragma once

#include <mzfc_inc.h>

#define MZ_ID_COVER_TIMER              200

class CM8WeiboCoverCanvas : public UiCanvas
{
public:
	CM8WeiboCoverCanvas(void);
	~CM8WeiboCoverCanvas(void);
	ImageContainer m_image_container;
	ImagingHelper* imgCover;
	virtual void PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate);
};

class CM8WeiboCoverWnd : public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CM8WeiboCoverWnd);
public:
	CM8WeiboCoverWnd(void);
	~CM8WeiboCoverWnd(void);
	CM8WeiboCoverCanvas cover;

protected:
	virtual BOOL OnInitDialog();
	virtual void OnTimer(UINT_PTR nIDEvent);
	virtual int OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);
};
