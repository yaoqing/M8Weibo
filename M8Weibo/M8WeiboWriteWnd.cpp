#include "M8WeiboWriteWnd.h"
#include "M8WeiboApp.h"
#include "resource.h"
#include <UsbNotifyApi.h>
#include <time.h>

#include <initguid.h>
#include <IMzUnknown.h>
#include <IMzUnknown_IID.h>
#include <ICamera_GUID.h>
#include <ICameraApp.h>

#include <InitGuid.h>
#include <IMzUnknown.h>
#include <IMzUnknown_IID.h>
#include <IPhotoViewer.h>
#include <IPhotoViewer_GUID.h>

void CM8WeiboWriteCanvas::init()
{
	memset(&m_rcPhoto, 0, sizeof(RECT));
	m_srcDC = NULL;
	m_photo_name = L"";
	m_photo_width = m_photo_height = 0;
}

void CM8WeiboWriteCanvas::PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(247,247,247));//(RGB(220,230,242));
	FillRect(hdcDst, prcWin, hBrush);
	DeleteObject(hBrush);
	if (m_srcDC)
	{
		RECT rcTemp = m_rcPhoto;
		OffsetRect(&rcTemp, prcWin->left, prcWin->top);		
		::StretchBlt(hdcDst, rcTemp.left, rcTemp.top, rcTemp.right-rcTemp.left, rcTemp.bottom-rcTemp.top, m_srcDC, 0, 0, m_photo_width, m_photo_height, SRCCOPY);;
	}
}

MZ_IMPLEMENT_DYNAMIC(CM8WeiboWriteWnd)

CM8WeiboWriteWnd::CM8WeiboWriteWnd(void)
{
	imgNormalCamera = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_CAMERA_BLUE, true);
	imgPressedCamera = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_CAMERA_GREEN, true);
}

CM8WeiboWriteWnd::~CM8WeiboWriteWnd(void)
{
}

BOOL CM8WeiboWriteWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
	{
		return FALSE;
	}

	int width = GetWidth();

	m_ctlCaption.SetID(MZ_IDC_WRITE_CAPTION);
	m_ctlCaption.SetPos(0, 0, width, MZM_HEIGHT_TEXT_TOOLBAR);
	m_ctlCaption.SetTextColor(RGB(89,89,89));
	m_ctlCaption.SetTextSize(30);
	m_ctlCaption.SetTextWeight(FW_MEDIUM);
	m_ctlCaption.SetText(L"发表新微博");
	AddUiWin(&m_ctlCaption);


	m_button_camera.SetID(MZ_IDC_WRITE_CAMERA);
	m_button_camera.SetPos(24, 10, 48, 48);
	m_button_camera.SetButtonType(MZC_BUTTON_NONE);
	m_button_camera.SetImage_Normal(imgNormalCamera);
	m_button_camera.SetImage_Pressed(imgPressedCamera);
	m_ctlCaption.AddChild(&m_button_camera);


	//m_btnCancel.SetID(MZ_IDC_WRITE_CANCEL);
	//m_btnCancel.SetPos(20, 15, 100, 40);
	//m_btnCancel.SetButtonType(MZC_BUTTON_ROUND_WHITE);
	//m_btnCancel.SetText(L"取消");
	//m_btnCancel.SetTextColor(0X59BB9B);
	//m_ctlCaption.AddChild(&m_btnCancel);

	//m_btnSend.SetID(MZ_IDC_WRITE_SEND);
	//m_btnSend.SetPos(w-20-100, 15, 100, 40);
	//m_btnSend.SetButtonType(MZC_BUTTON_ROUND_WHITE);
	//m_btnSend.SetText(L"发送");
	//m_btnSend.SetTextColor(0X59BB9B);
	//m_ctlCaption.AddChild(&m_btnSend);

	int edit_height = 176;
	m_ctlEdit.SetID(MZ_IDC_WRITE_EDIT);
	m_ctlEdit.SetPos(0, MZM_HEIGHT_TEXT_TOOLBAR, width, edit_height);
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
	
	if (m_strPrefix.Length() > 0)
	{
		m_ctlEdit.SetText(m_strPrefix);
	}

	m_toolbar.SetID(MZ_IDC_WRITE_TOOLBAR);
	m_toolbar.SetPos(0,GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR-3,GetWidth(),MZM_HEIGHT_TEXT_TOOLBAR+3);
	m_toolbar.SetButton(TOOLBARPRO_LEFT_TEXTBUTTON, TRUE, TRUE, L"取消");
	m_toolbar.SetButton(TOOLBARPRO_RIGHT_TEXTBUTTON, TRUE, TRUE, L"发送");
	AddUiWin(&m_toolbar);

	int photo_top_space = 1;
	int photo_bottom_space = 0;
	m_rc_photo.left = 0;
	m_rc_photo.right = width;
	m_rc_photo.top = 0;
	m_rc_photo.bottom = GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR-edit_height-MZM_HEIGHT_TEXT_TOOLBAR-3-photo_top_space-photo_bottom_space;
	int photo_width = m_rc_photo.right-m_rc_photo.left;
	int photo_height = m_rc_photo.bottom-m_rc_photo.top;

	m_photo_canvas.SetID(MZ_IDC_WRITE_CAMERA);
	m_photo_canvas.SetPos(0, MZM_HEIGHT_TEXT_TOOLBAR+edit_height+photo_top_space, photo_width, photo_height);
	m_photo_canvas.m_dcCanvas.Create(photo_width, photo_height);
	AddUiWin(&m_photo_canvas);

	//m_ctlEdit.SetFocus(true);
	//MzOpenSip();

	return TRUE;
}

void CM8WeiboWriteWnd::OnMzCommand( WPARAM wParam, LPARAM lParam )
{
	UINT_PTR id = LOWORD(wParam);
	switch(id)
	{
	case MZ_IDC_WRITE_CAMERA:
		{
			if (GetUsbConnectType() == USB_MASSSTORAGE_ATTACH)
			{
				MzMessageBox(m_hWnd, L"U盘模式下不能拍照及浏览图片！", NULL, MB_OK, 1000, TRUE);
				break;
			}

			MzCloseSip();
			img_helper.Unload();
			m_photo_canvas.init();
			CMzString camera_photo_name;

			MzPopupMenu menu;
			menu.AppendMenuItem(MZV2_MID_MIN+1, L"现在拍照");
			menu.AppendMenuItem(MZV2_MID_MIN+2, L"相机照片");
			int result = menu.MzTrackPopupMenu(m_hWnd, TRUE, NULL, TRUE);
			switch (result)
			{
			case MZV2_MID_MIN+1:
				{
					IMzOpen *pMzOpen = NULL;
					ICameraApp *pCameraApp = NULL;
					if (SUCCEEDED(CoCreateInstance(CLSID_Camera, NULL,CLSCTX_INPROC_SERVER, IID_MZ_Open, (void **)&pMzOpen)))
					{
						if (SUCCEEDED(pMzOpen->QueryInterface(IID_MZ_CameraApp, (void**)&pCameraApp)))
						{
							// 创建目录
							CMzString photo_path = L"\\Disk\\Photo\\Camera Album\\";
							theApp.CreateDirectory(photo_path);

							// 生成文件名
							time_t timer;
							::_time64(&timer);
							struct tm now;
							_localtime64_s(&now, &timer);
							wchar_t photo_name[24];
							wsprintf(photo_name, L"%04d%02d%02d%02d%02d%02d.png", 1900+now.tm_year, 
								now.tm_mon, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);

							pCameraApp->SetMainWindowHandle(m_hWnd);
							pCameraApp->SetPhotoPath(photo_path+photo_name);

							if (pMzOpen->Invoke()) camera_photo_name = pCameraApp->GetPhotoPath();
							if (pCameraApp) { pCameraApp->Release(); pCameraApp = NULL; }
						}
						if (pMzOpen) { pMzOpen->Release(); pMzOpen = NULL; }
					}
				}
				break;
			case MZV2_MID_MIN+2:
				{
					IMzSelect *pSelect = NULL;  
					IPhotoViewer *pImage = NULL;
					if (SUCCEEDED(CoCreateInstance(CLSID_PhotoView, NULL,CLSCTX_INPROC_SERVER, IID_MZ_PhotoViewer, (void **)&pImage)))
					{  
						if (SUCCEEDED(pImage->QueryInterface(IID_MZ_Select, (void**)&pSelect)))
						{
							// 创建目录
							CMzString photo_path = L"\\Disk\\Photo\\Camera Album\\";
							theApp.CreateDirectory(photo_path);
							pImage->SetParentWnd(m_hWnd);
							pImage->SetSelectPhotoMode(SELECT_NORMAL);
							CMzString szModuleName = theApp.GetModuleFileName(NULL);
							int pos = szModuleName.FindReverse(L'\\');
							CMzString szModulePath = szModuleName.SubStr(0, pos+1);
							pImage->SetCurrentImageFileName(szModulePath + L"Camera.png");
							if (pSelect->Invoke()) camera_photo_name = pImage->GetImageFileName();
							if (pSelect) { pSelect->Release(); pSelect = NULL; }
						}  
						if (pImage) { pImage->Release(); pImage = NULL; }
					}
				}
				break;
			}

			if (camera_photo_name.Length())
			{
				m_photo_canvas.m_photo_name = camera_photo_name;
				img_helper.LoadImage(camera_photo_name.C_Str(), true, true);
				int pic_width = img_helper.GetImageWidth();
				int pic_height = img_helper.GetImageHeight();
				m_photo_canvas.m_photo_width = pic_width;
				m_photo_canvas.m_photo_height = pic_height;
				float rate_x = (pic_width*1.0)/(m_rc_photo.right-m_rc_photo.left);
				float rate_y = (pic_height*1.0)/(m_rc_photo.bottom-m_rc_photo.top);
				float max_rate = max(rate_x, rate_y);
				pic_width /= max_rate;
				pic_height /= max_rate;
				RECT rcTemp = m_rc_photo;
				rcTemp.left += (m_rc_photo.right-m_rc_photo.left-pic_width)/2;
				rcTemp.right = rcTemp.left + pic_width;
				rcTemp.top += (m_rc_photo.bottom-m_rc_photo.top-pic_height)/2;
				rcTemp.bottom = rcTemp.top + pic_height;
				m_photo_canvas.m_rcPhoto = rcTemp;
				m_photo_canvas.m_srcDC = img_helper.GetDC();

				if (m_ctlEdit.GetText().Length() == 0) m_ctlEdit.SetText(L"图片分享");
			}
		}
		break;
	case MZ_IDC_WRITE_TOOLBAR:
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
					MzMessageBox(m_hWnd, L"发送内容为空！", NULL, MB_OK, 1000, TRUE);
					break;
				}
				if (!theApp.IsValid())
				{
					MzMessageBox(m_hWnd, L"试用版不能发送微博！", NULL, MB_OK, 1000, TRUE);
					break;
				}
				if (MzIsSipOpen()) MzCloseSip();
				if (m_photo_canvas.m_photo_name.Length() > 0)
				{
					if (theApp.PutStatusesUpload(m_hWnd, m_photo_canvas.m_photo_name, m_ctlEdit.GetText()))
					{
						MzMessageBox(m_hWnd, L"成功发送新微博！", NULL, MB_OK, 1000, TRUE);
					}
					else
					{
						break;
					}
				}
				else
				{
					if (theApp.PutStatusesUpdate(m_hWnd, m_ctlEdit.GetText()))
					{
						MzMessageBox(m_hWnd, L"成功发送新微博！", NULL, MB_OK, 1000, TRUE);
					}
					else
					{
						break;
					}
				}
				DestroyWindow();
				delete this;
				theApp.PopWnd(this);
			}
		}
		break;
	}
}

int CM8WeiboWriteWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	 /*switch(message)
	 {
	 }*/
	 return SHK_RET_APPNOEXIT_SHELLTOP;
}
