#pragma once

#include <mzfc_inc.h>

#define MZ_IDC_WRITE_CAPTION      101
#define MZ_IDC_WRITE_CANCEL       102
#define MZ_IDC_WRITE_SEND         103
#define MZ_IDC_WRITE_CAMERA       104
#define MZ_IDC_WRITE_EDIT         105
#define MZ_IDC_WRITE_TOOLBAR      106
#define MZ_IDC_WRITE_CANVAS       107

class CM8WeiboWriteCanvas: public UiCanvas
{
public:
	CM8WeiboWriteCanvas() {init();}
	~CM8WeiboWriteCanvas() {init();}
	void init();
	void PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate);
	RECT m_rcPhoto;
	int m_photo_width;
	int m_photo_height;
	HDC m_srcDC;
	CMzString m_photo_name;
};


class CM8WeiboWriteWnd: public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CM8WeiboWriteWnd);
public:
	CM8WeiboWriteWnd(void);
	~CM8WeiboWriteWnd(void);

	UiCaption m_ctlCaption;
	//UiButton m_btnCancel;
	//UiButton m_btnSend;
	UiButton_Image m_button_camera;
	ImageContainer m_image_container;
	UiEdit m_ctlEdit;
	UiToolBarPro m_toolbar;
	CM8WeiboWriteCanvas m_photo_canvas;
	RECT m_rc_photo;

	CMzString m_strPrefix;

protected:
	virtual BOOL OnInitDialog();
	virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);

private:
	ImagingHelper img_helper;
	ImagingHelper* imgNormalCamera;
	ImagingHelper* imgPressedCamera;
};
