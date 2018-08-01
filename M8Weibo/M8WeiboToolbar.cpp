#include "M8WeiboToolbar.h"
#include "resource.h"

CM8WeiboToolbar::CM8WeiboToolbar(void)
{
	COLOR_OF_BG_TOOLBAR = RGB(211,211,211);
	PAD_LEFT_OF_TOOLBAR = 24;
	PAD_RIGHT_OF_TOOLBAR = 24;

	for (int i = 0; i < M8_BUTTON_COUNT_OF_TOOLBAR; i++)
	{
		m_images_normal[i] = NULL;
		m_images_pressed[i] = NULL;
	}

	m_image_background = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_BG_TOOLBAR, true);
}

CM8WeiboToolbar::~CM8WeiboToolbar(void)
{
	m_image_container.RemoveImage(MzGetInstanceHandle(), IDB_PNG_BG_TOOLBAR, true);
}

void CM8WeiboToolbar::PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate)
{
	/*SetBkMode(hdcDst,TRANSPARENT);
	HBRUSH hBrush = CreateSolidBrush(COLOR_OF_BG_TOOLBAR);
	FillRect(hdcDst, prcWin, hBrush);
	DeleteObject(hBrush);*/

	//m_image_background->Draw(hdcDst, prcWin, true);
	int width = prcWin->right-prcWin->left;
	int img_width = m_image_background->GetImageWidth();
	RECT rcTemp = *prcWin;
	rcTemp.right = img_width;
	while (rcTemp.right <= width)
	{
		m_image_background->Draw(hdcDst, &rcTemp);
		rcTemp.left += img_width;
		rcTemp.right += img_width;
	}
	
	return;
}

bool CM8WeiboToolbar::SetButton(int index, bool isDefault, int idResNormal, int idResPressed)
{
	if (index < 0 || index >= M8_BUTTON_COUNT_OF_TOOLBAR)
	{
		return false;
	}

	int id = GetID();
	int width = GetWidth();
	int height = GetHeight();
	int space = (width-PAD_LEFT_OF_TOOLBAR-PAD_RIGHT_OF_TOOLBAR-M8_BUTTON_COUNT_OF_TOOLBAR*M8_BUTTON_WIDTH_OF_TOOLBAR)/4;
	int offsetX = PAD_LEFT_OF_TOOLBAR;
	int offsetY = 0;//(height-M8_BUTTON_WIDTH_OF_TOOLBAR)/2;
	
	m_images_normal[index] = m_image_container.LoadImage(MzGetInstanceHandle(), idResNormal, true);
	m_images_pressed[index] = m_image_container.LoadImage(MzGetInstanceHandle(), idResPressed, true);
	
	m_buttons[index].SetID(id+index+1);
	m_buttons[index].SetPos(offsetX+(M8_BUTTON_WIDTH_OF_TOOLBAR+space)*index-space/2, offsetY, M8_BUTTON_WIDTH_OF_TOOLBAR+space, height);
	m_buttons[index].SetButtonType(MZC_BUTTON_NONE);
	m_buttons[index].SetImage_Normal(isDefault? m_images_pressed[index]: m_images_normal[index]);
	m_buttons[index].SetImage_Pressed(isDefault? m_images_normal[index]: m_images_pressed[index]);

	AddChild(&m_buttons[index]);
	return true;
}
