#pragma once

#include <mzfc_inc.h>

#define MZ_IDC_SETTINGS_CAPTION                101
#define MZ_IDC_SETTINGS_GOBACK                 102
#define MZ_IDC_SETTINGS_GOHOME                 103
#define MZ_IDC_SETTINGS_SCROOLWIN              104
#define MZ_IDC_SETTINGS_PROFILE_IMAGE          105
#define MZ_IDC_SETTINGS_THUMB_IMAGE            106
#define MZ_IDC_SETTINGS_MIDDLE_IMAGE           107
#define MZ_IDC_SETTINGS_NETENV                 108
#define MZ_IDC_SETTINGS_ACCOUNT                109
#define MZ_IDC_SETTINGS_EXIT                   110

#define MZ_ID_SETTINGS_TIMER                   200

class CM8WeiboSettingsButton: public UiCanvas
{
public:
	CMzString m_text;
	HWND m_hWnd;
	bool m_pressed;
	CM8WeiboSettingsButton() { init(); }
	~CM8WeiboSettingsButton() { init(); }
	void init();
	virtual int OnLButtonDown(UINT fwKeys, int xPos, int yPos);
	virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
	void OnPaintWin();

	DWORD COLOR_OF_BG;
	DWORD COLOR_OF_FG;
	int LINE_HEIGHT_OF_TEXT;
	int LEFT_PAD_OF_TEXT;
};

class CM8WeiboSettingsScroolWin : public UiScrollWin
{
public:
	HWND m_hWnd;
	DWORD m_dwTimerID;
	virtual int OnLButtonDown(UINT fwKeys, int xPos, int yPos);
};

class CM8WeiboSettingsCanvas: public UiCanvas
{
public:
	CM8WeiboSettingsCanvas();
	~CM8WeiboSettingsCanvas();
	void OnPaintWin();

	int LINE_HEIGHT_OF_TEXT;
	int SPACE_OF_LINE;
	int LEFT_PAD_TEXT;
	DWORD COLOR_OF_TEXT;
	DWORD COLOR_OF_BACKGROUND;
};

class CM8WeiboSettingsWnd: public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CM8WeiboSettingsWnd);
public:
	CM8WeiboSettingsWnd(void);
	~CM8WeiboSettingsWnd(void);

	UiStatic m_static_profile;
	UiStatic m_static_thumb;
	UiStatic m_static_middle;

	UiButton m_switch_profile;
	UiButton m_switch_thumb;
	UiButton m_switch_middle;
	CM8WeiboSettingsScroolWin m_scrool_win;
	UiButton m_lines[7];

	CM8WeiboSettingsButton m_btnNetEnv;
	CM8WeiboSettingsButton m_btnAccount;
	CM8WeiboSettingsButton m_btnExit;
	CM8WeiboSettingsCanvas m_canvas;

	UiCaption m_ctlCaption;
	UiButton_Image m_button_goback;
	UiButton_Image m_button_gohome;
	ImageContainer m_image_container;

protected:
	virtual BOOL OnInitDialog();
	virtual void OnTimer(UINT_PTR nIDEvent);
	virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);

private:
	ImagingHelper* imgNormalGoback;
	ImagingHelper* imgPressedGoback;
	ImagingHelper* imgNormalGohome;
	ImagingHelper* imgPressedGohome;

	int SPACE_CONTROL;
	int LINE_HEIGHT;
	int HEIGHT_CONTROL;

	int SWITCH_WIDTH;
	int STATIC_WIDTH;
};
