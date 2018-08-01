#include "M8WeiboStatusesWnd.h"
#include "M8WeiboApp.h"
#include "M8WeiboCommentViewWnd.h"
#include "M8WeiboCommentWnd.h"
#include "M8WeiboRepostWnd.h"
#include "M8WeiboRepostWnd.h"
#include "M8WeiboUserWnd.h"
#include "M8WeiboParameter.h"
#include "resource.h"
#include "StringW2A.h"
#include "StringA2W.h"

#include <InitGuid.h>
#include <IMzUnknown.h>
//#include <IMzUnknown_IID.h>
#include <IPhotoViewer.h>
//#include <IPhotoViewer_GUID.h>

int CM8WeiboTokenCanvas::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	RECT rcWork = MzGetWorkArea();
	CM8WeiboUserWnd *pUserWnd = new CM8WeiboUserWnd();
	pUserWnd->SetUserInfo(m_user);
	pUserWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
	pUserWnd->Show();
	theApp.m_stack.push(pUserWnd);
	return UiCanvas::OnLButtonUp(fwKeys, xPos, yPos);
}

CM8WeiboStatusesCanvas::CM8WeiboStatusesCanvas(void)
{
	m_hWnd = NULL;
	m_parentWnd = NULL;
}

CM8WeiboStatusesCanvas::~CM8WeiboStatusesCanvas(void)
{
	m_hWnd = NULL;
	m_parentWnd = NULL;
}

int CM8WeiboStatusesCanvas::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	POINT pos = {xPos, yPos};
	BOOL isInRect =PtInRect(&(m_parentWnd->m_rc_links_box), pos);
	if (isInRect)
	{
		RECT rcWork = MzGetWorkArea();
		CM8WeiboRepostWnd *pRepostWnd = new CM8WeiboRepostWnd();
		pRepostWnd->m_statuses = m_parentWnd->m_status;
		pRepostWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
		pRepostWnd->Show();
		theApp.m_stack.push(pRepostWnd);
	}
	isInRect =PtInRect(&(m_parentWnd->m_rc_comments_box), pos);
	if (isInRect)
	{
		RECT rcWork = MzGetWorkArea();
		CM8WeiboCommentViewWnd *pCommentViewWnd = new CM8WeiboCommentViewWnd();
		pCommentViewWnd->m_status = m_parentWnd->m_status;
		pCommentViewWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
		pCommentViewWnd->Show();
		theApp.m_stack.push(pCommentViewWnd);
	}
	RECT rcTemp = m_parentWnd->m_rc_rt_links_box;
	rcTemp.left -= 16;rcTemp.top -= 16;rcTemp.bottom += 16;
	isInRect =PtInRect(&rcTemp, pos);
	if (isInRect)
	{
		RECT rcWork = MzGetWorkArea();
		CM8WeiboRepostWnd *pRepostWnd = new CM8WeiboRepostWnd();
		pRepostWnd->m_statuses = m_parentWnd->m_status->retweeted_status_;
		pRepostWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
		pRepostWnd->Show();
		theApp.m_stack.push(pRepostWnd);
	}
	rcTemp = m_parentWnd->m_rc_rt_comments_box;
	rcTemp.right += 16;rcTemp.top -= 16;rcTemp.bottom += 16;
	isInRect =PtInRect(&rcTemp, pos);
	if (isInRect)
	{
		RECT rcWork = MzGetWorkArea();
		CM8WeiboCommentViewWnd *pCommentViewWnd = new CM8WeiboCommentViewWnd();
		pCommentViewWnd->m_status = m_parentWnd->m_status->retweeted_status_;
		pCommentViewWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
		pCommentViewWnd->Show();
		theApp.m_stack.push(pCommentViewWnd);
	}
	return UiCanvas::OnLButtonUp(fwKeys, xPos, yPos);
}

MZ_IMPLEMENT_DYNAMIC(CM8WeiboStatusesWnd)

CM8WeiboStatusesWnd::CM8WeiboStatusesWnd(void)
{
	m_rt_comments = 0;
	m_rt_links = 0;
	m_favorite_flag = 0;

	imgNormalGoback = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOBACK_BLUE, true);
	imgNormalGohome = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOHOME_BLUE, true);
	imgPressedGoback = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOBACK_GREEN, true);
	imgPressedGohome = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOHOME_GREEN, true);
	imgTokenBoy = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_TOKEN_BOY, true);
	imgTokenGirl = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_TOKEN_GIRL, true);
	imgArrowGray = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_ARROW_GRAY, true);
	imgSmallComment = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_SMALL_COMMENT_BLUE, true);
	imgSmallLink = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_SMALL_REPOST_BLUE, true);
	imgNormalComment = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_COMMENT_BLUE, true);
	imgNormalLink = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_REPOST_BLUE, true);

	MZ_STATUS_COUNT_PER_LINE = 16;

	// 定义各种字体的颜色
	COLOR_OF_BG_AUTHOR = RGB(249,249,249);
	COLOR_OF_BG_CONTENT = RGB(249,249,249);
	COLOR_OF_BG_RT = RGB(247,247,247);
	COLOR_OF_EDG_RT = RGB(225,225,225);
	COLOR_OF_TEXT_CONTENT = RGB(30,73,125);//RGB(89,89,89);
	COLOR_OF_TEXT_AUTHOR = RGB(0,0,0);//(63,63,63);
	COLOR_OF_ARROR_AUTHOR = RGB(127,127,127);
	COLOR_OF_TEXT_COUNTS = RGB(95,129,158);
	COLOR_OF_TEXT_CREATE_SOURCE = RGB(177,177,177);
	COLOR_OF_RT_TEXT_COUNTS = RGB(122,141,157);//(216,216,216);
	COLOR_OF_RT_TEXT_CONTENT = RGB(89,89,89);
	COLOR_OF_RT_LINE = RGB(216,216,216);
	COLOR_OF_EDG_BUTTON = RGB(180,183,187);
	COLOR_OF_BG_BUTTON = RGB(251,251,251);

	// 定义各种字体的行高
	TEXT_LINE_HEIGHT = 30;
	AUTHOR_LINE_HEIGHT = 28;
	CREATE_SOURCE_LINE_HEIGHT = 22;
	COUNTS_LINE_HEIGHT = 42;
	RT_COUNTS_LINE_HEIGHT = 22;
	SPACE_LINE_HEIGHT = 4;
	RT_TEXT_LINE_HEIGHT = 28;
	RT_SPACE_LINE_HEIGHT = 3;

	// 定义上三角形的边长、三角形左边偏移量、圆角半径
	DELTA_LENGTH = 28;
	DELTA_OFFSET = 24;
	ROUND_WIDTH = 16;
	// 定义头像的边长
	TOKEN_LENGTH = 50;
	// 定义微博作者总高度
	HEIGHT_OF_AUTHOR = 100;
	// 定义头像与作者之间的空白
	PAD_SPACE_OF_TOKEN_AND_AUTHOR = 16;

	// 定义微博作者内空白
	PAD_LEFT_AUTHOR = 16;
	PAD_TOP_AUTHOR = 16;
	PAD_RIGHT_AUTHOR = 16;
	PAD_BOTTOM_AUTHOR = 10;

	// 定义微博正文内空白
	PAD_LEFT_CONTENT = 24;
	PAD_TOP_CONTENT = 24;
	PAD_RIGHT_CONTENT = 16;
	PAD_BOTTOM_CONTENT = 0;

	// 定义微博正文地步内空白的创建来源块和引用评论按钮的高度
	CREATE_SOURCE_HEIGHT = 48;
	LINK_COMMENTS_HEIGHT = 96;
	PAD_BOTTOM_CONTENT = CREATE_SOURCE_HEIGHT+LINK_COMMENTS_HEIGHT;

	// 定义引用微博的外空白
	MARGIN_LEFT_RT = 0;
	MARGIN_TOP_RT = 24;
	MARGIN_RIGHT_RT = 0;
	MARGIN_BOTTOM_RT = 0;

	// 定义引用微博的内空白
	PAD_LEFT_RT = 18;
	PAD_TOP_RT = 16;
	PAD_RIGHT_RT = 18;
	PAD_BOTTOM_RT = 32;

	// 定义引用微博的(引用和评论的右空白和宽度、图标和数字之间的间隔)
	RT_MARGIN_RIGHT_OTHER = 0;
	RT_WIDTH_OF_OTHER = 136;
	RT_SPACE_OF_ICON_AND_COUNTS = 3;

	// 定义微博的引用和评论按钮的左、中、右、上、下空白及按钮中图标距离左边框的宽度
	SPACE_LEFT_COUNTS = 4;
	SPACE_MID_COUNTS = 16;
	SPACE_RIGHT_COUNTS = 4;
	SPACE_TOP_COUNTS = 12;
	SPACE_BOTTOM_COUNTS = 8;
	SPACE_LEFT_IN_BUTTON = 24;

	// 定义创建时间和来源的左空白
	MARGIN_LEFT_CREATE_SOURCE = 0;

	// 定义缩略图的上空白、下空白、左空白、右空白
	MIDDLE_TOP_HEIGHT = 16;
	MIDDLE_BOTTOM_HEIGHT = 4;
	MIDDLE_LEFT_PAD = 20;
	MIDDLE_RIGHT_PAD = 20;
}

CM8WeiboStatusesWnd::~CM8WeiboStatusesWnd(void)
{
}

BOOL CM8WeiboStatusesWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
	{
		return FALSE;
	}

	int width = GetWidth();

	m_ctlCaption.SetID(MZ_IDC_STATUS_CAPTION);
	m_ctlCaption.SetPos(0, 0, width, MZM_HEIGHT_TEXT_TOOLBAR);
	m_ctlCaption.SetTextColor(RGB(89,89,89));
	m_ctlCaption.SetTextSize(30);
	m_ctlCaption.SetTextWeight(FW_MEDIUM);
	m_ctlCaption.SetText(L"微博正文");
	AddUiWin(&m_ctlCaption);

	m_button_goback.SetID(MZ_IDC_STATUS_GOBACK);
	m_button_goback.SetPos(24, 10, 48, 48);
	m_button_goback.SetButtonType(MZC_BUTTON_NONE);
	m_button_goback.SetImage_Normal(imgNormalGoback);
	m_button_goback.SetImage_Pressed(imgPressedGoback);
	m_ctlCaption.AddChild(&m_button_goback);

	m_button_gohome.SetID(MZ_IDC_STATUS_GOHOME);
	m_button_gohome.SetPos(width-24-48, 10, 48, 48);
	m_button_gohome.SetButtonType(MZC_BUTTON_NONE);
	m_button_gohome.SetImage_Normal(imgNormalGohome);
	m_button_gohome.SetImage_Pressed(imgPressedGohome);
	m_ctlCaption.AddChild(&m_button_gohome);

	m_toolbar.SetID(MZ_IDC_STATUS_TOOLBAR);
	m_toolbar.SetPos(0, GetHeight()-MZM_HEIGHT_ICON_TOOLBAR, width, MZM_HEIGHT_ICON_TOOLBAR);
	m_toolbar.SetButton(0, false, IDB_PNG_REFRESH_BLUE, IDB_PNG_REFRESH_GREEN);
	m_toolbar.SetButton(1, false, IDB_PNG_REPOST_BLUE, IDB_PNG_REPOST_GREEN);
	m_toolbar.SetButton(2, false, IDB_PNG_COMMENT_BLUE, IDB_PNG_COMMENT_GREEN);
	m_toolbar.SetButton(3, false, IDB_PNG_STAR_BLUE, IDB_PNG_STAR_GREEN);
	m_toolbar.SetButton(4, false, IDB_PNG_REMOVE_BLUE, IDB_PNG_REMOVE_GREEN);
	AddUiWin(&m_toolbar);

	m_canvas.SetID(MZ_IDC_STATUS_CANVAS);
	m_canvas.SetPos(0, MZM_HEIGHT_TEXT_TOOLBAR, width, HEIGHT_OF_AUTHOR);
	m_canvas.m_dcCanvas.Create(width, HEIGHT_OF_AUTHOR);
	m_canvas.m_user = m_status->user_;

	// 填充作者背景
	RECT rcOrigin = {0,0,width,HEIGHT_OF_AUTHOR};
	HDC hDC = m_canvas.m_dcCanvas.GetDC();
	SetBkMode(hDC,TRANSPARENT);
	HBRUSH hBrush = CreateSolidBrush(COLOR_OF_BG_AUTHOR);
	FillRect(hDC, &rcOrigin, hBrush);
	DeleteObject(hBrush);

	// 画微博作者头像
	RECT rcToken;
	rcToken.left = rcOrigin.left+PAD_LEFT_AUTHOR;
	rcToken.right = rcToken.left+TOKEN_LENGTH;
	rcToken.top = rcOrigin.top+PAD_TOP_AUTHOR+(HEIGHT_OF_AUTHOR-TOKEN_LENGTH-PAD_TOP_AUTHOR-PAD_BOTTOM_AUTHOR)/2;
	rcToken.bottom = rcToken.top+TOKEN_LENGTH;

	CMzString profile_image_path;
	CMzString profile_token_name = theApp.UpdateProfileTokenName(m_status->user_->id_, m_status->user_->profile_image_url_, profile_image_path);
	CMzString profile_image_name = theApp.FindTokenFileName(profile_image_path, profile_token_name);
	if (profile_image_name.Length() > 0 || theApp.m_parameter->getAutoDownloadProfileImage())
	{
		if (profile_image_name.Length() == 0)
		{
			theApp.DownloadTokenFile(m_hWnd, m_status->user_->id_, true, m_status->user_->profile_image_url_, profile_image_path, profile_token_name);
			profile_image_name = profile_image_path + profile_token_name;
		}
		ImagingHelper::DrawImage(hDC, &rcToken, profile_image_name.C_Str());
	}
	else
	{
		if (m_status->user_->gender_[0] == 'm')
			imgTokenBoy->Draw(hDC, &rcToken);
		else
			imgTokenGirl->Draw(hDC, &rcToken);
	}

	// 作者所占块位置
	RECT rcAuthor = {PAD_LEFT_AUTHOR+TOKEN_LENGTH+PAD_SPACE_OF_TOKEN_AND_AUTHOR,
		PAD_TOP_AUTHOR,width-PAD_RIGHT_AUTHOR,
		HEIGHT_OF_AUTHOR-PAD_BOTTOM_AUTHOR};
	LOGFONT font_struct={ 0, 0, 0, 0, 0, 0, 0, 0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"FZLanTingHei-R-GBK"
	};

	// 输出作者
	font_struct.lfHeight = AUTHOR_LINE_HEIGHT;
	HFONT hFont = CreateFontIndirect(&font_struct);
	HGDIOBJ hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, COLOR_OF_TEXT_AUTHOR);
	CStringA2W author(m_status->user_->screen_name_);
	int len_author = wcslen(author.ptr());
	DrawText(hDC, author.ptr(), len_author, &rcAuthor, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	SetTextColor(hDC, COLOR_OF_ARROR_AUTHOR);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	AddUiWin(&m_canvas);

	// 画右箭头
	RECT rcArrow=rcAuthor;
	rcArrow.top += (rcArrow.bottom-rcArrow.top-28)/2;
	rcArrow.bottom = rcArrow.top+28;
	//rcArrow.right -= (PAD_RIGHT_USER-28)/2;
	rcArrow.left = rcArrow.right-28;
	imgArrowGray->Draw(hDC, &rcArrow);

	// 输出引用换行占位符
	HPEN hPen = CreatePen(PS_SOLID, 1, COLOR_OF_RT_LINE);
	HGDIOBJ hSavedPen = SelectObject(hDC, hPen);
	MoveToEx(hDC, 0, rcOrigin.top+HEIGHT_OF_AUTHOR-1, NULL);
	LineTo(hDC, width, rcOrigin.top+HEIGHT_OF_AUTHOR-1);
	SelectObject(hDC, hSavedPen);
	DeleteObject(hPen);

	// 创建滚动窗口
	RECT rcScrool = {0, MZM_HEIGHT_TEXT_TOOLBAR+HEIGHT_OF_AUTHOR, width, GetHeight()-MZM_HEIGHT_ICON_TOOLBAR};
	m_scrool_win.SetID(MZ_IDC_STATUS_SCROOLWIN);
	m_scrool_win.SetPos(rcScrool.left, rcScrool.top, rcScrool.right-rcScrool.left, rcScrool.bottom-rcScrool.top);
	m_scrool_win.EnableScrollBarV(true);
	AddUiWin(&m_scrool_win);

	// 按每行制定字符个数换行
	CStringA2W text(m_status->text_);
	int lines = theApp.SplitString(text.ptr(), MZ_STATUS_COUNT_PER_LINE);
	CMzString *p_lines =  new CMzString[lines];
	theApp.SplitString(lines, p_lines, text.ptr(), MZ_STATUS_COUNT_PER_LINE);

	// 设置微博正文的宽度和高度
	FULL_WIDTH_OF_STATUS = width;
	FULL_HEIGHT_OF_STATUS = lines*(TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT)-SPACE_LINE_HEIGHT
		+PAD_TOP_CONTENT+PAD_BOTTOM_CONTENT;

	// 增加转发微博的高度
	if (m_status->retweeted_status_)
	{
		char *p_text = new char[WB_REAL_LEN(SCREEN_NAME) + WB_REAL_LEN(WB_INFO)];
		M8::m8_status *p_retweeted_status_ = m_status->retweeted_status_;
		while (p_retweeted_status_)
		{
			sprintf(p_text, "%s：%s", p_retweeted_status_->user_->screen_name_, p_retweeted_status_->text_);
			int lines = theApp.SplitString(CStringA2W(p_text).ptr(), MZ_STATUS_COUNT_PER_LINE);
			FULL_HEIGHT_OF_STATUS += lines*(RT_TEXT_LINE_HEIGHT+RT_SPACE_LINE_HEIGHT)
				-RT_SPACE_LINE_HEIGHT+MARGIN_TOP_RT+MARGIN_BOTTOM_RT+PAD_TOP_RT+PAD_BOTTOM_RT;
			p_retweeted_status_ = p_retweeted_status_->retweeted_status_;
		}
		delete [] p_text;
	}

	// 增加缩略图的高度
	ImagingHelper img_helper;
	ImagingHelper img_helper_rt;
	int pic_width, pic_height;
	int pic_width_rt, pic_height_rt;
	pic_width = pic_height = pic_width_rt = pic_height_rt = 0;
	bool bStretch = false;
	bool bStretch_rt = false;
	if (theApp.m_parameter->getAutoDownloadMiddlePicture() && *m_status->bmiddle_pic_ && !theApp.IsGif(m_status->bmiddle_pic_))
	{
		CMzString middle_picture_path;
		CMzString picture_token_name = theApp.UpdateMiddlePictureTokenName(m_status->user_->id_, m_status->bmiddle_pic_, middle_picture_path);
		CMzString middle_picure_name = theApp.FindTokenFileName(middle_picture_path, picture_token_name);
		if (middle_picure_name.IsEmpty())
		{
			if(theApp.DownloadTokenFile(m_hWnd, m_status->user_->id_, false, m_status->bmiddle_pic_, middle_picture_path, picture_token_name))
			{
				middle_picure_name = middle_picture_path+picture_token_name;
			}
		}
		img_helper.LoadImage(middle_picure_name.C_Str(), false, false, true);
		pic_width = img_helper.GetImageWidth();
		pic_height = img_helper.GetImageHeight();
		int max_width = width-PAD_LEFT_CONTENT-PAD_RIGHT_CONTENT-MIDDLE_LEFT_PAD-MIDDLE_RIGHT_PAD;
		if (pic_width > max_width)
		{
			float rate = (max_width*1.0)/pic_width;
			pic_width *= rate;
			pic_height *= rate;
			bStretch = true;
		}
		int max_height = 2048 - FULL_HEIGHT_OF_STATUS - MIDDLE_TOP_HEIGHT - MIDDLE_BOTTOM_HEIGHT;
		if (pic_height > max_height)
		{
			float rate = (max_height*1.0)/pic_height;
			pic_width *= rate;
			pic_height *= rate;
			bStretch = true;
		}
		FULL_HEIGHT_OF_STATUS += pic_height+MIDDLE_TOP_HEIGHT+MIDDLE_BOTTOM_HEIGHT;
	}
	if (theApp.m_parameter->getAutoDownloadMiddlePicture() && m_status->retweeted_status_ && *m_status->retweeted_status_->bmiddle_pic_ && !theApp.IsGif(m_status->retweeted_status_->bmiddle_pic_))
	{
		CMzString middle_picture_path;
		CMzString picture_token_name = theApp.UpdateMiddlePictureTokenName(m_status->retweeted_status_->user_->id_, m_status->retweeted_status_->bmiddle_pic_, middle_picture_path);
		CMzString middle_picure_name = theApp.FindTokenFileName(middle_picture_path, picture_token_name);
		if (middle_picure_name.IsEmpty())
		{
			if(theApp.DownloadTokenFile(m_hWnd, m_status->retweeted_status_->user_->id_, false, m_status->retweeted_status_->bmiddle_pic_, middle_picture_path, picture_token_name))
			{
				middle_picure_name = middle_picture_path+picture_token_name;
			}
		}
		img_helper_rt.LoadImage(middle_picure_name.C_Str(), false, false, true);
		pic_width_rt = img_helper_rt.GetImageWidth();
		pic_height_rt = img_helper_rt.GetImageHeight();
		int max_width = width-PAD_LEFT_CONTENT-PAD_RIGHT_CONTENT-MIDDLE_LEFT_PAD-MIDDLE_RIGHT_PAD-MARGIN_LEFT_RT-MARGIN_RIGHT_RT-PAD_LEFT_RT-PAD_RIGHT_RT;
		if (pic_width_rt > max_width)
		{
			float rate = (max_width*1.0)/pic_width_rt;
			pic_width_rt *= rate;
			pic_height_rt *= rate;
			bStretch_rt = true;
		}
		int max_height = 2048 - FULL_HEIGHT_OF_STATUS - MIDDLE_TOP_HEIGHT - MIDDLE_BOTTOM_HEIGHT;
		if (pic_height_rt > max_height)
		{
			float rate = (max_height*1.0)/pic_height_rt;
			pic_width_rt *= rate;
			pic_height_rt *= rate;
			bStretch_rt = true;
		}
		if (pic_height_rt < 0){pic_height_rt = -(MIDDLE_TOP_HEIGHT+MIDDLE_BOTTOM_HEIGHT); pic_width_rt = 0;}
		FULL_HEIGHT_OF_STATUS += pic_height_rt+MIDDLE_TOP_HEIGHT+MIDDLE_BOTTOM_HEIGHT;
	}

	// 如果不满屏，扩展到满屏高度
	int pic_offset_height = 0;
	if (FULL_HEIGHT_OF_STATUS < rcScrool.bottom-rcScrool.top)
	{
		pic_offset_height = rcScrool.bottom-rcScrool.top - FULL_HEIGHT_OF_STATUS;
		FULL_HEIGHT_OF_STATUS = rcScrool.bottom-rcScrool.top;
	}

	// 添加微博子窗口
	m_scrool_canvas.SetID(MZ_IDC_STATUS_SCANVAS);
	m_scrool_canvas.SetPos(0, 0, FULL_WIDTH_OF_STATUS, FULL_HEIGHT_OF_STATUS);
	m_scrool_canvas.m_dcCanvas.Create(FULL_WIDTH_OF_STATUS, FULL_HEIGHT_OF_STATUS);
	m_scrool_win.AddChild(&m_scrool_canvas);
	m_scrool_canvas.m_hWnd = m_hWnd;
	m_scrool_canvas.m_parentWnd = this;
	hDC = m_scrool_canvas.m_dcCanvas.GetDC();
	SetBkMode(hDC,TRANSPARENT);

	// 填充微博正文背景
	rcOrigin.left = 0;
	rcOrigin.top = 0;
	rcOrigin.right = FULL_WIDTH_OF_STATUS;
	rcOrigin.bottom = FULL_HEIGHT_OF_STATUS;
	hBrush = CreateSolidBrush(COLOR_OF_BG_CONTENT);
	FillRect(hDC, &rcOrigin, hBrush);
	DeleteObject(hBrush);

	// 输出引用换行占位符
	hPen = CreatePen(PS_SOLID, 1, COLOR_OF_RT_LINE);
	hSavedPen = SelectObject(hDC, hPen);
	MoveToEx(hDC, 0, 0, NULL);
	LineTo(hDC, width, 0);
	SelectObject(hDC, hSavedPen);
	DeleteObject(hPen);

	// 初始化偏移量
	int offsetLeft = PAD_LEFT_CONTENT;
	int offsetTop = PAD_TOP_CONTENT;
	int offsetRight = PAD_RIGHT_CONTENT;
	int offsetBottom = PAD_BOTTOM_CONTENT;

	// 内容所占块位置
	RECT rcText;
	rcText.left = rcOrigin.left+offsetLeft;
	rcText.top = rcOrigin.top+offsetTop;
	rcText.right = rcOrigin.right-offsetRight;
	rcText.bottom = rcText.top+TEXT_LINE_HEIGHT;

	// 输出内容
	wcscpy(font_struct.lfFaceName, L"宋体");
	font_struct.lfHeight = TEXT_LINE_HEIGHT;
	hFont = CreateFontIndirect(&font_struct);
	hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, COLOR_OF_TEXT_CONTENT);
	for (int i = 0; i < lines; i++)
	{
		DrawText(hDC, p_lines[i].C_Str(), -1, &rcText, DT_SINGLELINE|DT_LEFT);
		rcText.top += TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT;
		rcText.bottom = rcText.top+TEXT_LINE_HEIGHT;
	}
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);
	delete [] p_lines;

	if (m_status->retweeted_status_)
	{
		// 偏移至引用边框处
		offsetLeft += MARGIN_LEFT_RT;
		offsetTop += lines*(TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT)-SPACE_LINE_HEIGHT+MARGIN_TOP_RT;
		offsetRight += MARGIN_RIGHT_RT;
		offsetBottom += MARGIN_BOTTOM_RT;

		// 内容所占块位置
		RECT rcRT = rcOrigin;
		rcRT.left += offsetLeft;
		rcRT.top += offsetTop;
		rcRT.right -= offsetRight;
		rcRT.bottom -= offsetBottom;

		// 描绘内容边框
		hPen = CreatePen(PS_SOLID, 1, COLOR_OF_EDG_RT);
		hBrush = CreateSolidBrush(COLOR_OF_BG_RT);
		hSavedPen = SelectObject(hDC, hPen);
		HGDIOBJ hSavedBrush = SelectObject(hDC, hBrush);
		//RoundRect(hDC, 10, 10, width-10, 100-10, 9, 9);
		RoundRect(hDC, rcRT.left, rcRT.top, rcRT.right, rcRT.bottom, ROUND_WIDTH, ROUND_WIDTH);
		SelectObject(hDC, hSavedBrush);
		SelectObject(hDC, hSavedPen);
		DeleteObject(hBrush);
		DeleteObject(hPen);

		// 画边框的上三角
		int a = rcRT.left+DELTA_OFFSET;
		int b = rcRT.top;
		for (int c = 0; c < DELTA_LENGTH/2; c++)
		{
			SetPixel(hDC, a++, b--, COLOR_OF_EDG_RT);
			int pos = b+1;
			while (rcRT.top-pos>=0) SetPixel(hDC, a, pos++, COLOR_OF_BG_RT);
		}
		for (int c = 0; c < DELTA_LENGTH/2; c++)
		{
			SetPixel(hDC, a++, ++b, COLOR_OF_EDG_RT);
			int pos = b+1;
			while (rcRT.top-pos>=0) SetPixel(hDC, a, pos++, COLOR_OF_BG_RT);
		}

		// 调整偏移量至引用微博内空白
		offsetLeft += PAD_LEFT_RT;
		offsetTop += PAD_TOP_RT;
		offsetRight += PAD_RIGHT_RT;
		offsetBottom += PAD_BOTTOM_RT;

		// 按每行制定字符个数换行
		char *p_text = new char[WB_REAL_LEN(SCREEN_NAME) + WB_REAL_LEN(WB_INFO)];
		sprintf(p_text, "%s：%s", m_status->retweeted_status_->user_->screen_name_, m_status->retweeted_status_->text_);
		CStringA2W text(p_text);
		int lines = theApp.SplitString(text.ptr(), MZ_STATUS_COUNT_PER_LINE);
		delete [] p_text;
		CMzString *p_lines =  new CMzString[lines];
		theApp.SplitString(lines, p_lines, text.ptr(), MZ_STATUS_COUNT_PER_LINE);

		// 引用正文所占块位置
		RECT rcText;
		rcText.left = rcOrigin.left+offsetLeft;
		rcText.right = rcOrigin.right-offsetRight;
		rcText.top = rcOrigin.top+offsetTop;
		rcText.bottom = rcText.top+RT_TEXT_LINE_HEIGHT;

		// 输出引用正文
		font_struct.lfHeight = RT_TEXT_LINE_HEIGHT;
		HFONT hFont = CreateFontIndirect(&font_struct);
		HGDIOBJ hSavedFont = SelectObject(hDC, hFont);
		SetTextColor(hDC, COLOR_OF_RT_TEXT_CONTENT);
		for (int i = 0; i < lines; i++)
		{
			DrawText(hDC, p_lines[i].C_Str(), -1, &rcText, DT_SINGLELINE|DT_LEFT);
			rcText.top += RT_TEXT_LINE_HEIGHT+RT_SPACE_LINE_HEIGHT;
			rcText.bottom = rcText.top+RT_TEXT_LINE_HEIGHT;
			offsetTop += RT_TEXT_LINE_HEIGHT+RT_SPACE_LINE_HEIGHT;
		}
		offsetTop -= RT_SPACE_LINE_HEIGHT;
		SelectObject(hDC, hSavedFont);
		DeleteObject(hFont);
		delete [] p_lines;

		// 画中缩略图
		if (theApp.m_parameter->getAutoDownloadMiddlePicture() && m_status->retweeted_status_ && *m_status->retweeted_status_->bmiddle_pic_ && !theApp.IsGif(m_status->retweeted_status_->bmiddle_pic_))
		{
			RECT rcMiddle;
			rcMiddle.left = rcText.left+MIDDLE_LEFT_PAD;
			rcMiddle.right = rcMiddle.left+pic_width_rt;
			rcMiddle.top = rcText.bottom+MIDDLE_TOP_HEIGHT-RT_TEXT_LINE_HEIGHT-RT_SPACE_LINE_HEIGHT;
			rcMiddle.bottom = rcMiddle.top+pic_height_rt;

			CMzString middle_picture_path;
			CMzString picture_token_name = theApp.UpdateMiddlePictureTokenName(m_status->retweeted_status_->user_->id_, m_status->retweeted_status_->bmiddle_pic_, middle_picture_path);
			CMzString middle_picure_name = theApp.FindTokenFileName(middle_picture_path, picture_token_name);
			if (middle_picure_name.IsEmpty())
			{
				if(theApp.DownloadTokenFile(m_hWnd, m_status->retweeted_status_->user_->id_, false, m_status->retweeted_status_->bmiddle_pic_, middle_picture_path, picture_token_name))
				{
					middle_picure_name = middle_picture_path+picture_token_name;
				}
			}
			img_helper_rt.Draw(hDC, &rcMiddle, bStretch_rt, true);
			img_helper_rt.UnloadImage();
		}

		// 引用及评论所占块位置
		RECT rcCounts;
		rcCounts.right = rcOrigin.right-offsetRight-RT_MARGIN_RIGHT_OTHER;
		rcCounts.left = rcCounts.right-RT_WIDTH_OF_OTHER;
		rcCounts.top = rcOrigin.top+offsetTop;
		if (theApp.m_parameter->getAutoDownloadMiddlePicture() && m_status->retweeted_status_ && *m_status->retweeted_status_->bmiddle_pic_ && !theApp.IsGif(m_status->retweeted_status_->bmiddle_pic_))
			rcCounts.top += pic_height_rt+MIDDLE_TOP_HEIGHT+MIDDLE_BOTTOM_HEIGHT;
		rcCounts.bottom = rcCounts.top+PAD_BOTTOM_RT;

		// 引用块外框位置
		m_rc_rt_links_box = rcCounts;
		m_rc_rt_links_box.right = rcCounts.left+RT_WIDTH_OF_OTHER/2-RT_SPACE_OF_ICON_AND_COUNTS;
		// 评论块外框位置
		m_rc_rt_comments_box = rcCounts;
		m_rc_rt_comments_box.left = rcCounts.left+RT_WIDTH_OF_OTHER/2;
		m_rc_rt_comments_box.right -= RT_SPACE_OF_ICON_AND_COUNTS;

		// 画引用图标
		RECT rcLink = rcCounts;
		rcLink.top = rcLink.top+(rcCounts.bottom-rcCounts.top-16)/2;
		rcLink.bottom = rcLink.top+16;
		rcLink.right = rcLink.left+16;
		imgSmallLink->Draw(hDC, &rcLink);

		// 画评论图标
		RECT rcComment = rcLink;
		rcComment.left = rcLink.left+RT_WIDTH_OF_OTHER/2;
		rcComment.right = rcComment.left+16;
		imgSmallComment->Draw(hDC, &rcComment);

		rcLink.left = rcLink.right+RT_SPACE_OF_ICON_AND_COUNTS;
		rcLink.right = rcComment.left-1;

		rcComment.left = rcComment.right+RT_SPACE_OF_ICON_AND_COUNTS;
		rcComment.right = rcCounts.right-1;

		m_rc_rt_comments = rcComment;
		m_rc_rt_links = rcLink;

		// 输出引用数及评论数
		m_rt_links = m_status->retweeted_status_->m_links_;
		m_rt_comments = m_status->retweeted_status_->m_comments_;
		font_struct.lfHeight = RT_COUNTS_LINE_HEIGHT;
		hFont = CreateFontIndirect(&font_struct);
		hSavedFont = SelectObject(hDC, hFont);
		SetTextColor(hDC, COLOR_OF_RT_TEXT_COUNTS);
		wchar_t temp[16];
		wsprintf(temp, L"%d", m_rt_links);
		DrawText(hDC, temp, -1, &rcLink, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		wsprintf(temp, L"%d", m_rt_comments);
		DrawText(hDC, temp, -1, &rcComment, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		SelectObject(hDC, hSavedFont);
		DeleteObject(hFont);

		// 调整偏移量至引用微博边框(不含外空白)
		offsetLeft -= PAD_LEFT_RT;
		offsetTop -= PAD_TOP_RT;
		offsetRight -= PAD_RIGHT_RT;
		offsetBottom -= PAD_BOTTOM_RT;

		// 调整偏移量至引用微博边框(包含外空白)
		offsetLeft -= MARGIN_LEFT_RT;
		offsetTop -= MARGIN_TOP_RT;
		offsetRight -= MARGIN_RIGHT_RT;
		offsetBottom -= MARGIN_BOTTOM_RT;
	}
	else
	{
		memset(&m_rc_rt_links, 0, sizeof(RECT));
		memset(&m_rc_rt_comments, 0, sizeof(RECT));
		memset(&m_rc_rt_links_box, 0, sizeof(RECT));
		memset(&m_rc_rt_comments_box, 0, sizeof(RECT));
	}

	// 画中缩略图
	if (theApp.m_parameter->getAutoDownloadMiddlePicture() && *m_status->bmiddle_pic_ && !theApp.IsGif(m_status->bmiddle_pic_))
	{
		RECT rcMiddle;
		rcMiddle.left = rcOrigin.left+offsetLeft+MIDDLE_LEFT_PAD;
		rcMiddle.right = rcMiddle.left+pic_width;
		rcMiddle.bottom = rcOrigin.bottom-MIDDLE_BOTTOM_HEIGHT-PAD_BOTTOM_CONTENT-pic_offset_height;
		rcMiddle.top = rcMiddle.bottom-pic_height;

		CMzString middle_picture_path;
		CMzString picture_token_name = theApp.UpdateMiddlePictureTokenName(m_status->user_->id_, m_status->bmiddle_pic_, middle_picture_path);
		CMzString middle_picure_name = theApp.FindTokenFileName(middle_picture_path, picture_token_name);
		if (middle_picure_name.IsEmpty())
		{
			if(theApp.DownloadTokenFile(m_hWnd, m_status->user_->id_, false, m_status->bmiddle_pic_, middle_picture_path, picture_token_name))
			{
				middle_picure_name = middle_picture_path+picture_token_name;
			}
		}
		img_helper.Draw(hDC, &rcMiddle, bStretch, true);
		img_helper.UnloadImage();
	}

	// 创建时间及来源所占块位置
	RECT rcCreateSource;
	rcCreateSource.left = rcOrigin.left+offsetLeft;
	rcCreateSource.right = rcOrigin.right;
	rcCreateSource.bottom = rcOrigin.bottom;
	rcCreateSource.top = rcCreateSource.bottom-CREATE_SOURCE_HEIGHT;

	// 输出创建时间及来源
	font_struct.lfHeight = CREATE_SOURCE_LINE_HEIGHT;
	hFont = CreateFontIndirect(&font_struct);
	hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, COLOR_OF_TEXT_CREATE_SOURCE);
	CStringA2W source(m_status->source_);
	wchar_t buffer[64];
	wmemset(buffer, 0, 64);
	theApp.FormatSource(source.ptr(), buffer);
	CStringA2W create_at(m_status->created_at_);
	wchar_t etc[6][16];
	wmemset((wchar_t *)etc, 0, 96);
	theApp.FormatCreateAt(create_at.ptr(), etc);
	wchar_t create_source[80];
	swprintf(create_source, L"%s-%s %s  %s", etc[1], etc[2], etc[3], buffer);
	DrawText(hDC, create_source, -1, &rcCreateSource, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	// 引用数及评论数按钮位置
	RECT rcLink;
	rcLink.left = rcOrigin.left+offsetLeft+SPACE_LEFT_COUNTS;
	rcLink.right = rcOrigin.right-offsetRight-SPACE_RIGHT_COUNTS;
	rcLink.top = rcOrigin.bottom-PAD_BOTTOM_CONTENT+SPACE_TOP_COUNTS;
	rcLink.bottom = rcLink.top+LINK_COMMENTS_HEIGHT-SPACE_BOTTOM_COUNTS;
	rcLink.top += SPACE_TOP_COUNTS;
	rcLink.bottom -= SPACE_BOTTOM_COUNTS;
	int counts_width = (rcLink.right-rcLink.left-SPACE_MID_COUNTS)/2;
	int counts_height = rcLink.bottom-rcLink.top;
	int counts_top = rcLink.top;
	rcLink.right = rcLink.left+counts_width;

	RECT rcComment = rcLink;
	rcComment.right = rcOrigin.right-offsetRight-SPACE_RIGHT_COUNTS;
	rcComment.left = rcComment.right-counts_width;

	// 描绘引用数及评论数按钮
	hPen = CreatePen(PS_SOLID, 1, COLOR_OF_EDG_BUTTON);
	hBrush = CreateSolidBrush(COLOR_OF_BG_BUTTON);
	hSavedPen = SelectObject(hDC, hPen);
	HGDIOBJ hSavedBrush = SelectObject(hDC, hBrush);
	RoundRect(hDC, rcLink.left, rcLink.top, rcLink.right, rcLink.bottom, ROUND_WIDTH, ROUND_WIDTH);
	RoundRect(hDC, rcComment.left, rcComment.top, rcComment.right, rcComment.bottom, ROUND_WIDTH, ROUND_WIDTH);
	SelectObject(hDC, hSavedBrush);
	SelectObject(hDC, hSavedPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);

	m_rc_comments_box = rcComment;
	m_rc_links_box = rcLink;

	// 画按钮中的引用图标
	rcLink.top += (counts_height-48)/2;
	rcLink.bottom = rcLink.top+48;
	rcLink.left += SPACE_LEFT_IN_BUTTON;
	rcLink.right = rcLink.left+48;
	imgNormalLink->Draw(hDC, &rcLink);

	// 画按钮中的评论图标
	rcComment.top += (counts_height-48)/2;
	rcComment.bottom = rcComment.top+48;
	rcComment.left += SPACE_LEFT_IN_BUTTON;
	rcComment.right = rcComment.left+48;
	imgNormalComment->Draw(hDC, &rcComment);

	// 画按钮中的引用数
	rcLink.top = counts_top+(counts_height-COUNTS_LINE_HEIGHT)/2;
	rcLink.bottom = rcLink.top+COUNTS_LINE_HEIGHT;
	rcLink.left = rcLink.left-SPACE_LEFT_IN_BUTTON+counts_width/2;
	rcLink.right = rcLink.left+counts_width/2-1;

	// 画按钮中的评论数
	rcComment.top = counts_top+(counts_height-COUNTS_LINE_HEIGHT)/2;
	rcComment.bottom = rcComment.top+COUNTS_LINE_HEIGHT;
	rcComment.left = rcComment.left-SPACE_LEFT_IN_BUTTON+counts_width/2;
	rcComment.right = rcComment.left+counts_width/2-1;

	m_rc_comments = rcComment;
	m_rc_links = rcLink;

	// 输出按钮中的引用数及评论数
	font_struct.lfHeight = COUNTS_LINE_HEIGHT;
	hFont = CreateFontIndirect(&font_struct);
	hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, COLOR_OF_TEXT_COUNTS);
	wchar_t temp[16];
	wsprintf(temp, L"%d", m_status->m_links_);
	DrawText(hDC, temp, -1, &rcLink, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	wsprintf(temp, L"%d", m_status->m_comments_);
	DrawText(hDC, temp, -1, &rcComment, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	if (m_status->retweeted_status_ && m_status->retweeted_status_->m_links_ == 0 &&
		m_status->retweeted_status_->m_comments_ == 0) SetTimer(m_hWnd, MZ_ID_STATUS_TIMER, 0, NULL);
	return TRUE;
}

void CM8WeiboStatusesWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == MZ_ID_STATUS_TIMER)
	{
		KillTimer(m_hWnd, MZ_ID_STATUS_TIMER);
		HDC hDC = m_scrool_canvas.m_dcCanvas.GetDC();
		LOGFONT font_struct={ 0, 0, 0, 0, 0, 0, 0, 0,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH,
			L"宋体"
		};
		M8::m8_counts counts[2];
		strcpy(counts[0].wbId_, m_status->id_);
		if (m_status->retweeted_status_)
		{
			strcpy(counts[1].wbId_, m_status->retweeted_status_->id_);
			if (!theApp.GetStatusesCounts(m_hWnd, 2, counts))
			{
				return;
			}
			else
			{
				m_rt_links = counts[1].rt_;
				m_rt_comments = counts[1].comments_;
				m_status->m_links_ = counts[0].rt_;
				m_status->m_comments_ = counts[0].comments_;
				m_status->retweeted_status_->m_links_ = counts[1].rt_;
				m_status->retweeted_status_->m_comments_ = counts[1].comments_;

				// 输出引用数及评论数
				font_struct.lfHeight = RT_COUNTS_LINE_HEIGHT;
				HFONT hFont = CreateFontIndirect(&font_struct);
				HGDIOBJ hSavedFont = SelectObject(hDC, hFont);
				HBRUSH hBrush = CreateSolidBrush(COLOR_OF_BG_RT);
				SetTextColor(hDC, COLOR_OF_RT_TEXT_COUNTS);
				wchar_t temp[16];
				wsprintf(temp, L"%d", m_rt_links);
				FillRect(hDC, &m_rc_rt_links, hBrush);
				DrawText(hDC, temp, -1, &m_rc_rt_links, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
				wsprintf(temp, L"%d", m_rt_comments);
				FillRect(hDC, &m_rc_rt_comments, hBrush);
				DrawText(hDC, temp, -1, &m_rc_rt_comments, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
				SelectObject(hDC, hSavedFont);
				DeleteObject(hBrush);
				DeleteObject(hFont);
			}
		}
		else
		{
			if (!theApp.GetStatusesCounts(m_hWnd, 1, counts))
			{
				return;
			}
			else
			{
				m_status->m_links_ = counts[0].rt_;
				m_status->m_comments_ = counts[0].comments_;
			}
		}

		// 输出引用数及评论数
		font_struct.lfHeight = COUNTS_LINE_HEIGHT;
		HFONT hFont = CreateFontIndirect(&font_struct);
		HGDIOBJ hSavedFont = SelectObject(hDC, hFont);
		HBRUSH hBrush = CreateSolidBrush(COLOR_OF_BG_BUTTON);
		SetTextColor(hDC, COLOR_OF_TEXT_COUNTS);
		wchar_t temp[16];
		wsprintf(temp, L"%d", m_status->m_links_);
		FillRect(hDC, &m_rc_links, hBrush);
		DrawText(hDC, temp, -1, &m_rc_links, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		wsprintf(temp, L"%d", m_status->m_comments_);
		FillRect(hDC, &m_rc_comments, hBrush);
		DrawText(hDC, temp, -1, &m_rc_comments, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		SelectObject(hDC, hSavedFont);
		DeleteObject(hBrush);
		DeleteObject(hFont);
	}
}

void CM8WeiboStatusesWnd::OnMzCommand(WPARAM wParam, LPARAM lParam)
{
	UINT_PTR id = LOWORD(wParam);
	switch(id)
	{
	case MZ_IDC_STATUS_GOBACK:
		{
			DestroyWindow();
			delete this;
			theApp.PopWnd(this);
		}
		break;
	case MZ_IDC_STATUS_GOHOME:
		{
			theApp.GoHome();
		}
		break;
	case MZ_IDC_STATUS_TOOLBAR+1:
		{
			SetTimer(m_hWnd, MZ_ID_STATUS_TIMER, 0, NULL);
		}
		break;
	case MZ_IDC_STATUS_TOOLBAR+2:
		{
			RECT rcWork = MzGetWorkArea();
			CM8WeiboRepostWnd *pRepostWnd = new CM8WeiboRepostWnd();
			pRepostWnd->m_statuses = m_status;
			pRepostWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
			pRepostWnd->Show();
			theApp.m_stack.push(pRepostWnd);
		}
		break;
	case MZ_IDC_STATUS_TOOLBAR+3:
		{
			RECT rcWork = MzGetWorkArea();
			CM8WeiboCommentWnd *pCommentWnd = new CM8WeiboCommentWnd();
			pCommentWnd->m_statuses = m_status;
			pCommentWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
			pCommentWnd->Show();
			theApp.m_stack.push(pCommentWnd);
		}
		break;
	case MZ_IDC_STATUS_TOOLBAR+4:
		{
			if ((m_favorite_flag++)%2 == 0)
			{
				if (theApp.PutFavoritesCreate(m_hWnd, m_status->id_))
				{
					MzMessageBox(m_hWnd, L"收藏成功！", NULL, MB_OK, 1000, TRUE);
					m_toolbar.SetButton(3, false, IDB_PNG_STAR_GREEN, IDB_PNG_STAR_BLUE);
				}
			}
			else
			{
				if (theApp.PutFavoritesDestroy(m_hWnd, m_status->id_))
				{
					MzMessageBox(m_hWnd, L"取消收藏成功！", NULL, MB_OK, 1000, TRUE);
					m_toolbar.SetButton(3, false, IDB_PNG_STAR_BLUE, IDB_PNG_STAR_GREEN);
				}
			}
		}
		break;
	case MZ_IDC_STATUS_TOOLBAR+5:
		{
			int size1, size2 = 0;
			size1 = theApp.FindAtUser(CStringA2W(m_status->text_).ptr());
			if (m_status->retweeted_status_)
			{
				size2 = theApp.FindAtUser(CStringA2W(m_status->retweeted_status_->text_).ptr()) + 1;
			}
			CMzString *p_users = new CMzString[size1+size2];
			theApp.FindAtUser(p_users, CStringA2W(m_status->text_).ptr());
			if (m_status->retweeted_status_)
			{
				theApp.FindAtUser(p_users+size1, CStringA2W(m_status->retweeted_status_->text_).ptr());
				p_users[size1+size2-1] = L"@";
				p_users[size1+size2-1] += CStringA2W(m_status->retweeted_status_->user_->screen_name_).ptr();
			}

			MzPopupMenu menu;
			for (int i = 0; i < size1+size2; i++)
				menu.AppendMenuItem(MZV2_MID_MIN+i+1, p_users[i].C_Str());
			menu.AppendMenuItem(MZV2_MID_MIN+size1+size2+1, L"短信分享");
			if (m_status->original_pic_[0] || (m_status->retweeted_status_ && m_status->retweeted_status_->original_pic_[0]))
			{
				menu.AppendMenuItem(MZV2_MID_MIN+size1+size2+2, L"浏览大图");
				menu.AppendMenuItem(MZV2_MID_MIN+size1+size2+3, L"图片另存");
			}
			if (theApp.isSelf(m_status->user_->id_))
				menu.AppendMenuItem(MZV2_MID_MIN+size1+size2+4, L"删除微博");

			int result = menu.MzTrackPopupMenu(m_hWnd, TRUE, NULL, TRUE);
			if (result >= MZV2_MID_MIN+1 && result <= MZV2_MID_MIN+size1+size2)
			{
				M8::m8_wb_get_users_show user_info;
				wchar_t *name = p_users[result-MZV2_MID_MIN-1].C_Str()+1;
				if (theApp.GetUserInfoUnknown(m_hWnd, t_wb_uid::IDType_screenname, CStringW2A(name).ptr(), &user_info))
				{
					RECT rcWork = MzGetWorkArea();
					CM8WeiboUserWnd *pUserWnd = new CM8WeiboUserWnd();
					pUserWnd->SetUserInfo(&user_info.user_);
					pUserWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
					pUserWnd->Show();
					theApp.m_stack.push(pUserWnd);
				}
			}
			else if (result == MZV2_MID_MIN+size1+size2+1)
			{
				if (theApp.IsValid())
				{
					wchar_t module[32];
					wchar_t cmdline[512];
					wsprintf(module, L"\\Windows\\SmsUi.exe");
					wsprintf(cmdline, L"-n ▓%s", CStringA2W(m_status->text_).ptr());
					PROCESS_INFORMATION proc_info;
					CreateProcess(module, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &proc_info);
				}
				else
				{
					MzMessageBox(m_hWnd, L"试用版不能短信分享！", NULL, MB_OK, 1000, TRUE);
				}
			}
			else if (result == MZV2_MID_MIN+size1+size2+2 || result == MZV2_MID_MIN+size1+size2+3)
			{
				CMzString orign_picture_path;
				CMzString picture_token_name;
				char* user_id = NULL;
				char* original_pic = NULL;
				if (m_status->original_pic_[0])
				{
					user_id = m_status->user_->id_;
					original_pic = m_status->original_pic_;
				}
				else
				{
					user_id = m_status->retweeted_status_->user_->id_;
					original_pic = m_status->retweeted_status_->original_pic_;
				}
				picture_token_name = theApp.UpdateOriginalPictureTokenName(user_id, original_pic, orign_picture_path);
				
				CMzString orign_picure_name = theApp.FindTokenFileName(orign_picture_path, picture_token_name);
				if (orign_picure_name.IsEmpty())
				{
					if(theApp.DownloadTokenFile(m_hWnd, user_id, false, original_pic, orign_picture_path, picture_token_name))
					{
						orign_picure_name = orign_picture_path+picture_token_name;
					}
				}
				if (orign_picure_name.Length() > 0)
				{
					if (result == MZV2_MID_MIN+size1+size2+3)
					{
						// 创建目录
						CMzString photo_path = L"\\Disk\\Photo\\Photo Album\\";
						theApp.CreateDirectory(photo_path);
						int pos = orign_picure_name.FindReverse(L'\\');
						photo_path += orign_picure_name.SubStr(pos+1, orign_picure_name.Length());
						CopyFile(orign_picure_name.C_Str(), photo_path.C_Str(), FALSE);
						MzMessageBox(m_hWnd, L"图片保存完成！", NULL, MB_OK, 1000, TRUE);
						break;
					}
					IMzSelect *pSelect = NULL;  
					IPhotoViewer *pImage = NULL;
					if (SUCCEEDED(CoCreateInstance(CLSID_PhotoView, NULL,CLSCTX_INPROC_SERVER, IID_MZ_PhotoViewer, (void **)&pImage)))
					{  
						if (SUCCEEDED(pImage->QueryInterface(IID_MZ_Select, (void**)&pSelect)))
						{
							pImage->SetParentWnd(m_hWnd);
							pImage->SetSelectPhotoMode(SELECT_FROM_FILEBROWSER);
							pImage->SetCurrentImageFileName(orign_picure_name);
							pSelect->Invoke();
							pSelect->Release();
						}  
						pImage->Release();
					}
				}
				else
				{
					MzMessageBox(m_hWnd, L"下载图片失败！", NULL, MB_OK, 1000, TRUE);
				}
			}
			else if (result == MZV2_MID_MIN+size1+size2+4)
			{
				if (MzMessageBox(m_hWnd, L"删除微博？", NULL, MB_OKCANCEL) == IDOK && 
					theApp.PutStatusesDestroy(m_hWnd, m_status->id_))
				{
					MzMessageBox(m_hWnd, L"删除微博成功！", NULL, MB_OK, 1000, TRUE);
					PostMessage(MZ_WM_COMMAND, MZ_IDC_COMMENT_VIEW_WORKING, -1);
				}
			}

			delete [] p_users;
		}
	}
}

int CM8WeiboStatusesWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	/*switch(message)
	 {
	 }*/
	 return SHK_RET_APPNOEXIT_SHELLTOP;
}
