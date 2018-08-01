#include "M8WeiboCommentViewWnd.h"
#include "M8WeiboApp.h"
#include "M8WeiboCommentWnd.h"
#include "M8WeiboUserWnd.h"
#include "resource.h"
#include "StringW2A.h"
#include "StringA2W.h"

void CM8WeiboCommentViewCanvas::init()
{
	m_comment = NULL;
	m_hWnd = NULL;
	m_color_bg = 0;
	m_right_margin = 0;
	memset(&m_rcComment, 0, sizeof(RECT));
}

int CM8WeiboCommentViewCanvas::OnLButtonDown(UINT fwKeys, int xPos, int yPos)
{
	int width = m_rcComment.right-m_rcComment.left;
	int height = m_rcComment.bottom-m_rcComment.top;
	if (xPos < 0) xPos = 1;
	if (yPos < 0) yPos = 1;
	if (xPos >= width) xPos = width-1;
	if (yPos >= height) yPos = height-1;
	RECT rcTemp = m_rcComment;
	OffsetRect(&rcTemp, -rcTemp.left, -rcTemp.top);
	rcTemp.right -= m_right_margin;
	POINT pos = {xPos, yPos};
	BOOL isInRect = PtInRect(&rcTemp, pos);
	if (isInRect)
	{
		::SendMessage(m_hWnd, MZ_WM_COMMAND, MZ_IDC_COMMENT_VIEW_CLICK_DOWN, (LPARAM)this);
	}
	return UiCanvas::OnLButtonDown(fwKeys, xPos, yPos);
}

int CM8WeiboCommentViewCanvas::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	int width = m_rcComment.right-m_rcComment.left;
	int height = m_rcComment.bottom-m_rcComment.top;
	if (xPos < 0) xPos = 1;
	if (yPos < 0) yPos = 1;
	if (xPos >= width) xPos = width-1;
	if (yPos >= height) yPos = height-1;
	RECT rcTemp = m_rcComment;
	OffsetRect(&rcTemp, -rcTemp.left, -rcTemp.top);
	rcTemp.right -= m_right_margin;
	POINT pos = {xPos, yPos};
	BOOL isInRect = PtInRect(&rcTemp, pos);
	::SendMessage(m_hWnd, MZ_WM_COMMAND, MZ_IDC_COMMENT_VIEW_CLICK_UP, (LPARAM)this);
	if (!isInRect)
	{
		::PostMessage(m_hWnd, MZ_WM_COMMAND, MZ_IDC_COMMENT_VIEW_REPLY, (LPARAM)m_comment);
		return UiCanvas::OnLButtonUp(fwKeys, xPos, yPos);
	}
	SetTimer(m_hWnd, (UINT)m_comment, 100, NULL);
	CM8WeiboCommentViewScroolWin *pWnd = (CM8WeiboCommentViewScroolWin *)GetParent();
	pWnd->m_dwTimerID = (UINT)m_comment;
	return UiCanvas::OnLButtonUp(fwKeys, xPos, yPos);
}

int CM8WeiboCommentViewScroolWin::OnLButtonDown(UINT fwKeys, int xPos, int yPos)
{
	KillTimer(m_hWnd, m_dwTimerID);
	printf("CM8WeiboCommentViewScroolWin::OnLButtonDown\n");
	return UiScrollWin::OnLButtonDown(fwKeys, xPos, yPos);
}

int CM8WeiboCommentViewScroolWin::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	printf("CM8WeiboCommentViewScroolWin::OnLButtonUp\n");
	return UiScrollWin::OnLButtonUp(fwKeys, xPos, yPos);
}

MZ_IMPLEMENT_DYNAMIC(CM8WeiboCommentViewWnd)

CM8WeiboCommentViewWnd::CM8WeiboCommentViewWnd(void)
{
	m_status = NULL;
	//m_comment_list = NULL;

	m_size_of_comment = 0;
	m_comments_canvas = NULL;

	MZ_COMMENT_COUNT_PER_LINE = 17;

	// 定义各种字体的颜色
	COLOR_OF_BG_COMMENT_P = RGB(96,152,214);
	COLOR_OF_FG_CONTENT_P = RGB(255,255,255);
	COLOR_OF_BG_COMMENT_0 = RGB(239,243,255);
	COLOR_OF_BG_COMMENT_1 = RGB(255,255,255);
	COLOR_OF_TEXT_CONTENT = RGB(89,89,89);
	COLOR_OF_TEXT_CONTENT = RGB(89,89,89);
	COLOR_OF_TEXT_AUTHOR = RGB(84,141,212);
	COLOR_OF_TEXT_CREATE_AT = RGB(128,128,128);

	// 定义各种字体的行高
	TEXT_LINE_HEIGHT = 26;
	AUTHOR_LINE_HEIGHT = 26;
	CREATE_AT_LINE_HEIGHT = 22;
	SPACE_LINE_HEIGHT = 3;

	// 定义单条微博的内空白
	PAD_TOP_COMMENT = 48;
	PAD_BOTTOM_COMMENT = 10;
	PAD_LEFT_COMMENT = 16;
	PAD_RIGHT_COMMENT = 56;

	// 定义作者的左空白、创建时间右空白和宽度
	MARGIN_LEFT_AUTHOR = 16;
	MARGIN_RIGHT_CREATED_AT = 56;
	WIDTH_OF_CREATED_AT = 108;

	imgNormalGoback = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOBACK_BLUE, true);
	imgPressedGoback = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_GOBACK_GREEN, true);
	imgNormalComment = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_COMMENT_BLUE, true);
	imgPressedComment = m_image_container.LoadImage(MzGetInstanceHandle(), IDB_PNG_COMMENT_GREEN, true);
}

CM8WeiboCommentViewWnd::~CM8WeiboCommentViewWnd(void)
{
	RemoveAllComment(t_wb_option_params::WBOPTType_unk);
}

BOOL CM8WeiboCommentViewWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
	{
		return FALSE;
	}

	int width = GetWidth();

	m_ctlCaption.SetID(MZ_IDC_COMMENT_VIEW_CAPTION);
	m_ctlCaption.SetPos(0, 0, width, MZM_HEIGHT_TEXT_TOOLBAR);
	m_ctlCaption.SetTextColor(RGB(89,89,89));
	m_ctlCaption.SetTextSize(30);
	m_ctlCaption.SetTextWeight(FW_MEDIUM);
	m_ctlCaption.SetText(L"评论列表");
	AddUiWin(&m_ctlCaption);

	m_button_goback.SetID(MZ_IDC_COMMENT_VIEW_GOBACK);
	m_button_goback.SetPos(24, 10, 48, 48);
	m_button_goback.SetButtonType(MZC_BUTTON_NONE);
	m_button_goback.SetImage_Normal(imgNormalGoback);
	m_button_goback.SetImage_Pressed(imgPressedGoback);
	m_ctlCaption.AddChild(&m_button_goback);

	m_button_comment.SetID(MZ_IDC_COMMENT_VIEW_COMMENT);
	m_button_comment.SetPos(width-24-48, 10, 48, 48);
	m_button_comment.SetButtonType(MZC_BUTTON_NONE);
	m_button_comment.SetImage_Normal(imgNormalComment);
	m_button_comment.SetImage_Pressed(imgPressedComment);
	m_ctlCaption.AddChild(&m_button_comment);

	m_scrool_win.SetID(MZ_IDC_COMMENT_VIEW_SCROOLWIN);
	m_scrool_win.SetPos(0, MZM_HEIGHT_TEXT_TOOLBAR, width, GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR);
	m_scrool_win.EnableScrollBarV(true);
	m_scrool_win.m_hWnd = m_hWnd;
	AddUiWin(&m_scrool_win);

	m_more_canvas.SetID(MZ_IDC_COMMENT_VIEW_MORE);
	m_more_canvas.SetPos(0,0,0,0);
	m_more_canvas.m_hWnd = m_hWnd;
	m_scrool_win.AddChild(&m_more_canvas);

	SetTimer(m_hWnd, MZ_ID_COMMENT_VIEW_TIMER, t_wb_option_params::WBOPTType_unk, NULL);
	return TRUE;
}


void CM8WeiboCommentViewWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == MZ_ID_COMMENT_VIEW_TIMER)
	{
		KillTimer(m_hWnd, MZ_ID_COMMENT_VIEW_TIMER);
		PostMessage(MZ_WM_COMMAND, MZ_IDC_COMMENT_VIEW_WORKING, t_wb_option_params::WBOPTType_unk);
	}
	else if (nIDEvent == m_scrool_win.m_dwTimerID)
	{
		KillTimer(m_hWnd, m_scrool_win.m_dwTimerID);

		MzPopupMenu menu;
		menu.AppendMenuItem(MZV2_MID_MIN+1, L"查看资料");
		menu.AppendMenuItem(MZV2_MID_MIN+2, L"回复评论");
		if (theApp.isSelf(m_status->user_->id_))
		{
			menu.AppendMenuItem(MZV2_MID_MIN+3, L"删除评论");
		}
		int result = menu.MzTrackPopupMenu(m_hWnd, TRUE, NULL, TRUE);
		switch (result)
		{
		case MZV2_MID_MIN+1:
			{
				RECT rcWork = MzGetWorkArea();
				CM8WeiboUserWnd *pUserWnd = new CM8WeiboUserWnd();
				pUserWnd->SetUserInfo(&((M8::m8_comment *)nIDEvent)->user_);
				pUserWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
				pUserWnd->Show();
				theApp.m_stack.push(pUserWnd);
			}
			break;
		case MZV2_MID_MIN+2:
			{
				::PostMessage(m_hWnd, MZ_WM_COMMAND, MZ_IDC_COMMENT_VIEW_REPLY, (LPARAM)nIDEvent);
			}
			break;
		case MZV2_MID_MIN+3:
			{
				if (theApp.PutStatusesCommentDestroy(m_hWnd, m_status->id_, ((M8::m8_comment *)nIDEvent)->id_))
				{
					MzMessageBox(m_hWnd, L"删除评论成功！", NULL, MB_OK, 1000, TRUE);
					PostMessage(MZ_WM_COMMAND, MZ_IDC_COMMENT_VIEW_WORKING, -1);
				}				
			}
			break;
		}
	}
}

void CM8WeiboCommentViewWnd::OnMzCommand(WPARAM wParam, LPARAM lParam)
{
	UINT_PTR id = LOWORD(wParam);
	switch(id)
	{
	case MZ_IDC_COMMENT_VIEW_GOBACK:
		{
			DestroyWindow();
			delete this;
			theApp.PopWnd(this);
		}
		break;
	case MZ_IDC_COMMENT_VIEW_COMMENT:
		{
			OnCommentReply(NULL);
		}
		break;
	case MZ_IDC_COMMENT_VIEW_REPLY:
		{
			M8::m8_comment *p_comment = (M8::m8_comment *)lParam;
			OnCommentReply(p_comment);
		}
		break;
	case MZ_IDC_COMMENT_VIEW_WORKING:
		{
			int opt_type = lParam;
			M8::m8_wb_get_statuses_comments_list* p_comments_list = NULL;
			if (!RefreshComment(opt_type, p_comments_list)) break;

			MzBeginWaitDlg(m_hWnd, NULL, true);
			m_scrool_win.ScrollTo();
			RemoveAllComment(opt_type);
			AddComment(opt_type, p_comments_list);
			M8::m8_comment **p_comment_list = NULL;
			m_size_of_comment = GetCommentArray(p_comment_list);
			DrawAllComment(p_comment_list);
			MoveMoreButton();
			delete [] p_comment_list;
			MzEndWaitDlg();
		}
		break;
	case MZ_IDC_COMMENT_VIEW_CLICK_DOWN:
		{
			CM8WeiboCommentViewCanvas *p_canvas = (CM8WeiboCommentViewCanvas*)lParam;
			OnCommentClick(p_canvas, true);
			Invalidate();
			UpdateWindow();
		}
		break;
	case MZ_IDC_COMMENT_VIEW_CLICK_UP:
		{
			CM8WeiboCommentViewCanvas *p_canvas = (CM8WeiboCommentViewCanvas*)lParam;
			OnCommentClick(p_canvas, false);
			Invalidate();
			UpdateWindow();
		}
		break;
	}
}


int CM8WeiboCommentViewWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	/*switch(message)
	 {
	 }*/
	 return SHK_RET_APPNOEXIT_SHELLTOP;
}


void CM8WeiboCommentViewWnd::MoveMoreButton()
{
	// 画更多按钮
	m_scrool_win.RemoveChild(&m_more_canvas);
	RECT rcLast;
	if (m_size_of_comment > 0)
	{
		rcLast = m_comments_canvas[m_size_of_comment-1].m_rcComment;
		rcLast.top = rcLast.bottom;
		rcLast.bottom = rcLast.top+64;
	}
	else
	{
		rcLast.left = 0;
		rcLast.right = GetWidth();
		rcLast.top = 0;
		rcLast.bottom = rcLast.top+64;
	}
	m_more_canvas.SetID(MZ_IDC_COMMENT_VIEW_MORE);
	m_more_canvas.SetPos(rcLast.left, rcLast.top, rcLast.right-rcLast.left, rcLast.bottom-rcLast.top);
	m_more_canvas.m_hWnd = m_hWnd;
	m_scrool_win.AddChild(&m_more_canvas);
}

void CM8WeiboCommentViewWnd::RemoveAllComment(int opt_type)
{
	// 删除现有的微博画布数组
	for (int i = 0; i < m_size_of_comment; i++)
	{
		m_scrool_win.RemoveChild(m_comments_canvas+i);
	}

	delete [] m_comments_canvas;
	m_comments_canvas = NULL;
	m_size_of_comment = 0;

	//delete m_comment_list;
	//m_comment_list = NULL;

	// 不是查询最新微博，需要删除现有保存的数据，防止页面过长
	// 是最新微博，将新数据保存在现有数据前面
	if (opt_type != t_wb_option_params::WBOPTType_since_id)
	{
		std::list<M8::m8_wb_get_statuses_comments_list *>::iterator iter1;
		for (iter1 = m_list_comments.begin(); iter1 != m_list_comments.end(); iter1++)
		{
			delete *iter1;
		}
		m_list_comments.clear();
	}
}

void CM8WeiboCommentViewWnd::DrawAllComment(M8::m8_comment **p_comment_list)
{
	m_comments_canvas = new CM8WeiboCommentViewCanvas[m_size_of_comment];
	for (int x = 0; x < m_size_of_comment; x++)
	{
		M8::m8_comment *p_comment = p_comment_list[x];

		// 按每行制定字符个数换行
		CStringA2W text(p_comment->text_);
		int lines = theApp.SplitString(text.ptr(), MZ_COMMENT_COUNT_PER_LINE);
		CMzString *p_lines =  new CMzString[lines];
		theApp.SplitString(lines, p_lines, text.ptr(), MZ_COMMENT_COUNT_PER_LINE);

		int width = GetWidth();

		// 定义微博宽度和高度
		FULL_WIDTH_OF_COMMENT = width;
		FULL_HEIGHT_OF_COMMENT = lines*(TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT)-SPACE_LINE_HEIGHT
			+PAD_TOP_COMMENT+PAD_BOTTOM_COMMENT;

		// 微博所占块位置
		RECT rcComment;
		if (x == 0)
		{
			rcComment.left = 0;
			rcComment.top = 0;
			rcComment.right = FULL_WIDTH_OF_COMMENT;
			rcComment.bottom = FULL_HEIGHT_OF_COMMENT+rcComment.top;
		}
		else
		{
			CM8WeiboCommentViewCanvas *last_status = m_comments_canvas+x-1;
			rcComment = last_status->m_rcComment;
			rcComment.top = rcComment.bottom;
			rcComment.bottom = FULL_HEIGHT_OF_COMMENT+rcComment.top;
		}
		m_comments_canvas[x].m_hWnd = m_hWnd;
		m_comments_canvas[x].m_rcComment = rcComment;
		m_comments_canvas[x].m_comment = p_comment;
		m_comments_canvas[x].m_color_bg = x%2 ? COLOR_OF_BG_COMMENT_1 : COLOR_OF_BG_COMMENT_0;
		m_comments_canvas[x].m_right_margin = PAD_RIGHT_COMMENT;

		// 创建微博块
		m_comments_canvas[x].SetID(MZ_IDC_COMMENT_VIEW_COMMENTES+x);
		m_comments_canvas[x].SetPos(rcComment.left, rcComment.top, rcComment.right -rcComment.left, rcComment.bottom-rcComment.top);
		m_comments_canvas[x].m_dcCanvas.Create(rcComment.right-rcComment.left, rcComment.bottom-rcComment.top);
		HDC hDC = m_comments_canvas[x].m_dcCanvas.GetDC();
		OnDrawComment(hDC, rcComment, m_comments_canvas[x].m_color_bg, false, p_comment, lines, p_lines);
		delete [] p_lines;

		m_scrool_win.AddChild(m_comments_canvas+x);
	}
}

void CM8WeiboCommentViewWnd::AddComment(int opt_type, M8::m8_wb_get_statuses_comments_list* p_comments_list)
{
	// 根据OPT_TYPE添加微博数据
	// 不是查询最新微博，需要删除现有保存的数据，防止页面过长
	// 是最新微博，将新数据保存在现有数据前面
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		m_list_comments.push_front(p_comments_list);
	}
	else
	{
		m_list_comments.push_back(p_comments_list);
	}
}

void CM8WeiboCommentViewWnd::OnDrawComment(HDC hDC, RECT rcComment, DWORD bg_color, bool pressed, M8::m8_comment * p_comment, int lines, CMzString * p_lines)
{
	LOGFONT font_struct={ 0, 0, 0, 0, 0, 0, 0, 0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"宋体"
	};

	// 填充微博背景
	RECT rcOrigin = rcComment;
	rcOrigin.right -= rcOrigin.left;
	rcOrigin.bottom -= rcOrigin.top;
	rcOrigin.left = 0;
	rcOrigin.top = 0;
	SetBkMode(hDC,TRANSPARENT);
	HBRUSH hBrush = CreateSolidBrush(pressed? COLOR_OF_BG_COMMENT_P : bg_color);
	FillRect(hDC, &rcOrigin, hBrush);
	DeleteObject(hBrush);

	// 初始化偏移量
	int offsetLeft = 0;
	int offsetTop = 0;
	int offsetRight = 0;
	int offsetBottom = 0;

	// 作者所占块位置
	RECT rcAuthor;
	rcAuthor.left = rcOrigin.left+offsetLeft+MARGIN_LEFT_AUTHOR;
	rcAuthor.top = rcOrigin.top+offsetTop;
	rcAuthor.right = rcOrigin.right-MARGIN_RIGHT_CREATED_AT-WIDTH_OF_CREATED_AT;
	rcAuthor.bottom = rcAuthor.top+PAD_TOP_COMMENT;

	// 输出作者
	font_struct.lfHeight = AUTHOR_LINE_HEIGHT;
	HFONT hFont = CreateFontIndirect(&font_struct);
	HGDIOBJ hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, pressed? COLOR_OF_FG_CONTENT_P: COLOR_OF_TEXT_AUTHOR);
	CStringA2W info(p_comment->user_.screen_name_);
	DrawText(hDC, info.ptr(), -1, &rcAuthor, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	// 时间所占块位置
	RECT rcCreateAt=rcAuthor;
	rcCreateAt.right = rcOrigin.right-offsetRight-MARGIN_RIGHT_CREATED_AT;
	rcCreateAt.left = rcCreateAt.right-WIDTH_OF_CREATED_AT;

	// 输出时间
	font_struct.lfHeight = CREATE_AT_LINE_HEIGHT;
	hFont = CreateFontIndirect(&font_struct);
	hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, pressed? COLOR_OF_FG_CONTENT_P: COLOR_OF_TEXT_CREATE_AT);
	CStringA2W create_at(p_comment->created_at_);
	wchar_t etc[6][16];
	wmemset((wchar_t *)etc, 0, 96);
	theApp.FormatCreateAt(create_at.ptr(), etc);
	wchar_t str_time[16];
	swprintf(str_time, L"%s-%s %s", etc[1], etc[2], etc[3]);
	DrawText(hDC, str_time, -1, &rcCreateAt, DT_SINGLELINE|DT_RIGHT|DT_VCENTER);
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	// 偏移量进入TEXT范围
	offsetLeft += PAD_LEFT_COMMENT;
	offsetTop += PAD_TOP_COMMENT;
	offsetRight += PAD_RIGHT_COMMENT;
	offsetBottom += PAD_BOTTOM_COMMENT;

	// 内容所占块位置
	RECT rcText;
	rcText.left = rcOrigin.left+offsetLeft;
	rcText.top = rcOrigin.top+offsetTop;
	rcText.right = rcOrigin.right-offsetRight;
	rcText.bottom = rcText.top+TEXT_LINE_HEIGHT;

	// 输出内容
	font_struct.lfHeight = TEXT_LINE_HEIGHT;
	hFont = CreateFontIndirect(&font_struct);
	hSavedFont = SelectObject(hDC, hFont);
	SetTextColor(hDC, pressed? COLOR_OF_FG_CONTENT_P: COLOR_OF_TEXT_CONTENT);
	for (int i = 0; i < lines; i++)
	{
		DrawText(hDC, p_lines[i].C_Str(), -1, &rcText, DT_SINGLELINE|DT_LEFT);
		rcText.top += TEXT_LINE_HEIGHT+SPACE_LINE_HEIGHT;
		rcText.bottom = rcText.top+TEXT_LINE_HEIGHT;
	}
	SelectObject(hDC, hSavedFont);
	DeleteObject(hFont);

	// 画评论图标
	RECT rcButton;
	int width = rcComment.right-rcComment.left;
	int height = rcComment.bottom-rcComment.top;
	rcButton.left = width-PAD_RIGHT_COMMENT+(PAD_RIGHT_COMMENT-48)/2;
	rcButton.right = rcButton.left+48;
	rcButton.top = (height-48)/2;
	rcButton.bottom = rcButton.top+48;
	if (pressed)
	{
		imgPressedComment->Draw(hDC, &rcButton);
	} 
	else
	{
		imgNormalComment->Draw(hDC, &rcButton);
	}
}

void CM8WeiboCommentViewWnd::OnCommentClick(CM8WeiboCommentViewCanvas * p_canvas, bool pressed)
{
	CStringA2W text(p_canvas->m_comment->text_);
	int lines = theApp.SplitString(text.ptr(), MZ_COMMENT_COUNT_PER_LINE);
	CMzString *p_lines =  new CMzString[lines];
	theApp.SplitString(lines, p_lines, text.ptr(), MZ_COMMENT_COUNT_PER_LINE);
	HDC hDC = p_canvas->m_dcCanvas.GetDC();
	OnDrawComment(hDC, p_canvas->m_rcComment, p_canvas->m_color_bg, pressed, p_canvas->m_comment, lines, p_lines);
	delete [] p_lines;
}

void CM8WeiboCommentViewWnd::OnCommentReply(M8::m8_comment *p_comment)
{
	RECT rcWork = MzGetWorkArea();
	CM8WeiboCommentWnd *pCommentWnd = new CM8WeiboCommentWnd();
	pCommentWnd->m_statuses = m_status;
	pCommentWnd->m_comment = p_comment;
	pCommentWnd->m_pCommentViewWnd = this;
	pCommentWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
	pCommentWnd->Show();
	theApp.m_stack.push(pCommentWnd);
}

int CM8WeiboCommentViewWnd::GetCommentArray(M8::m8_comment **&p_comment_list)
{
	int size = 0;
	int index = 0;
	p_comment_list = NULL;
	std::list<M8::m8_wb_get_statuses_comments_list *>::iterator iter;
	for (iter = m_list_comments.begin(); iter != m_list_comments.end(); iter++)
	{
		size += (*iter)->size_of_comment_;
	}

	p_comment_list = new M8::m8_comment*[size];

	for (iter = m_list_comments.begin(); iter != m_list_comments.end(); iter++)
	{
		for (int i = 0; i <(*iter)->size_of_comment_; i++)
		{
			p_comment_list[index++] = (*iter)->comments_+i;
		}
	}
	return size;
}

bool CM8WeiboCommentViewWnd::RefreshComment(int opt_type, M8::m8_wb_get_statuses_comments_list*& p_comments_list)
{
	char *commentId = NULL;
	if (opt_type == t_wb_option_params::WBOPTType_since_id)
	{
		int size = m_list_comments.size();
		if (size > 0)
		{
			M8::m8_wb_get_statuses_comments_list * front = m_list_comments.front();
			if (front->size_of_comment_ > 0)commentId = front->comments_[0].id_;
		}
	}
	else if (opt_type == t_wb_option_params::WBOPTType_max_id)
	{
		int size = m_list_comments.size();
		if (size > 0)
		{
			M8::m8_wb_get_statuses_comments_list * back = m_list_comments.back();
			if (back->size_of_comment_ > 0) commentId = back->comments_[back->size_of_comment_-1].id_;
		}
	}
	if (!theApp.GetStatusesCommentsList(m_hWnd, m_status->id_, opt_type, commentId, p_comments_list))
	{
		delete p_comments_list;
		return false;
	}
	if (p_comments_list->size_of_comment_ == 0 && opt_type != -1)
	{
		delete p_comments_list;
		return false;
	}
	return true;
}
