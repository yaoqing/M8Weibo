#pragma once

#include <mzfc_inc.h>

class CM8WeiboMoreCanvas : public UiCanvas
{
public:
	CM8WeiboMoreCanvas(void);
	~CM8WeiboMoreCanvas(void);
	virtual void PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate);
	HWND m_hWnd;

private:
	DWORD COLOR_OF_BACKGROUND;
	DWORD COLOR_OF_TEXT;

	int LINE_HEIGHT_OF_TEXT;
};

class CM8WeiboHomeMoreCanvas : public CM8WeiboMoreCanvas
{
public:
	virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboCommentViewMoreCanvas : public CM8WeiboMoreCanvas
{
public:
	virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboUserListMoreCanvas : public CM8WeiboMoreCanvas
{
public:
	virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
};
