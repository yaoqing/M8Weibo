#pragma once

#include <mzfc_inc.h>

#define M8_BUTTON_COUNT_OF_TOOLBAR       5
#define M8_BUTTON_WIDTH_OF_TOOLBAR       48
class CM8WeiboToolbar : public UiWin
{
public:
	CM8WeiboToolbar(void);
	~CM8WeiboToolbar(void);

	virtual void PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate);
	bool SetButton(int index, bool isDefault, int idResNormal, int idResPressed);

private:
	ImageContainer m_image_container;
	ImagingHelper *m_image_background;
	ImagingHelper *m_images_normal[M8_BUTTON_COUNT_OF_TOOLBAR];
	ImagingHelper *m_images_pressed[M8_BUTTON_COUNT_OF_TOOLBAR];
	UiButton_Image m_buttons[M8_BUTTON_COUNT_OF_TOOLBAR];

	DWORD COLOR_OF_BG_TOOLBAR;

	int PAD_LEFT_OF_TOOLBAR;
	int PAD_RIGHT_OF_TOOLBAR;
};
