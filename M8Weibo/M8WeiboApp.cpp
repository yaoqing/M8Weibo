#include "M8WeiboApp.h"
#include "M8WeiboParameter.h"
#include "M8WeiboLoginWnd.h"
#include "M8WeiboCoverWnd.h"
#include "M8WeiboIntroWnd.h"
#include "M8WeiboHomeWnd.h"
#include "StringW2A.h"
#include "StringA2W.h"
#include "M8Struct.h"
#include "Urlcode.h"
#include "strconv.h"
#include "md5.h"
#include "loHttp+.h"
#include "MyStoreLib.h"

struct UserData
{
	int size;
	char *buffer;
	UserData() { memset(this, 0, sizeof(UserData)); }
	~UserData() { delete [] buffer; memset(this, 0, sizeof(UserData)); }
};

void proc_http_resp_header_callback(unsigned long httpcode,const char* header , unsigned long len , void* pUsrData)
{
	UserData* pua = (UserData*)pUsrData;
	pua->size = len;
	pua->buffer = new char[len+1];
	memcpy(pua->buffer, header, len);
	pua->buffer[len] = '\0';
}

void proc_http_resp_body_callback(unsigned long httpcode , const char* body , unsigned long len , void* pUsrData)
{
	UserData* pua = (UserData*)pUsrData;
	pua->size = len;
	pua->buffer = new char[len+1];
	memcpy(pua->buffer, body, len);
	pua->buffer[len] = '\0';
}

CM8WeiboApp::CM8WeiboApp(void)
{
	m_parameter = new CM8WeiboParameter();
	wbRequest = NULL;
	m_oauth_access_token = NULL;
	m_user_info_self = NULL;
	m_valid = false;
}

CM8WeiboApp::~CM8WeiboApp(void)
{
	delete m_parameter;
	if (wbRequest) wb_interface.pfun_request_destroy(wbRequest);
	delete m_oauth_access_token;
	delete m_user_info_self;
}

BOOL CM8WeiboApp::Init()
{
	if (Exists()) return FALSE;
	m_valid = StoreVerify();
	CoInitializeEx(0, COINIT_MULTITHREADED);

	InitWeiboInterface();
	m_parameter->readConfig(GetModuleFileName(NULL));

	//int count = m_parameter->getAccountList();
	//if (count == 0) m_parameter->setAccountList(1);

	CMzWndEx *pWnd = NULL;
	RECT rcWork = MzGetWorkArea();
	if (m_parameter->getNoSplash())
		pWnd = new CM8WeiboCoverWnd();
	else
		pWnd = new CM8WeiboIntroWnd();
	pWnd->Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
	pWnd->Show();

	return TRUE;
}

void CM8WeiboApp::WriteFileS(char* buffer, int size, FILE *fd)
{
	int c = 0;
	while (c < size)
	{
		c += fwrite(buffer+c, sizeof(char), size-c, fd);
	}
}

void CM8WeiboApp::WriteStatus(M8::m8_status *pStatus, FILE *fd)
{
	DWORD dwZero = 0;
	if (pStatus)
	{
		// 写入内存地址
		DWORD dwAddr = (DWORD)(pStatus);
		WriteFileS((char *)(&dwAddr), sizeof(DWORD), fd);

		// 保存用户和引用微博数据结构前的所有字段
		WriteFileS((char *)(pStatus), sizeof(M8::m8_status)-sizeof(M8::m8_user *)-sizeof(M8::m8_status *), fd);

		// 保存用户字段
		if (pStatus->user_)
		{
			// 写入内存地址
			DWORD dwAddr = (DWORD)(pStatus->user_);
			WriteFileS((char *)(&dwAddr), sizeof(DWORD), fd);

			// 写入用户结构字段
			WriteFileS((char *)(pStatus->user_), sizeof(M8::m8_user), fd);
		}
		else
		{
			WriteFileS((char *)(&dwZero), sizeof(DWORD), fd);
		}

		WriteStatus(pStatus->retweeted_status_, fd);
	}
	else
	{
		WriteFileS((char *)(&dwZero), sizeof(DWORD), fd);
	}
}

int CM8WeiboApp::ReadStatus(M8::m8_status *pStatus, char* buffer)
{
	int offset = 0;

	// 读取用户和引用微博数据结构前的所有字段
	memcpy(pStatus, buffer+offset, sizeof(M8::m8_status)-sizeof(M8::m8_user *)-sizeof(M8::m8_status *));
	offset += (sizeof(M8::m8_status)-sizeof(M8::m8_user *)-sizeof(M8::m8_status *));

	// 读取用户字段
	M8::m8_user ** pUser = (M8::m8_user **)(buffer+offset);
	if (*pUser == NULL)
	{
		pStatus->user_ = NULL;
		offset += sizeof(DWORD);
	}
	else
	{
		pStatus->user_ = new M8::m8_user;
		offset += sizeof(DWORD);
		memcpy(pStatus->user_, buffer+offset, sizeof(M8::m8_user));
		offset += sizeof(M8::m8_user);
	}

	// 读取引用微博
	M8::m8_status ** pRT = (M8::m8_status **)(buffer+offset);
	if (*pRT == NULL)
	{
		pStatus->retweeted_status_ = NULL;
		offset += sizeof(DWORD);
	}
	else
	{
		pStatus->retweeted_status_ = new M8::m8_status;
		offset += sizeof(DWORD);
		offset += ReadStatus(pStatus->retweeted_status_, buffer+offset);
	}

	return offset;
}

bool CM8WeiboApp::Serial(char *uid, int size, M8::m8_status **pStatuses)
{
	CMzString szModuleName = GetModuleFileName(NULL);
	int pos = szModuleName.FindReverse(L'\\');
	CMzString szModulePath = szModuleName.SubStr(0, pos+1);
	szModuleName = szModulePath + CStringA2W(uid).ptr() + L".statuses";
	
	// 创建序列化文件
	FILE *fd = _wfopen(szModuleName, L"wb+");
	// 写入微博数量
	fwrite(&size, sizeof(int), 1, fd);
	// 写入微博内容
	for (int i = 0; i < size; i++)
	{
		WriteStatus(pStatuses[i], fd);
	}
	// 关闭文件
	fclose(fd);

	return true;
}

bool CM8WeiboApp::UnSerial(char *uid, int &size, M8::m8_status **&pStatuses)
{
	CMzString szModuleName = GetModuleFileName(NULL);
	int pos = szModuleName.FindReverse(L'\\');
	CMzString szModulePath = szModuleName.SubStr(0, pos+1);
	szModuleName = szModulePath + CStringA2W(uid).ptr() + L".statuses";

	// 读取文件大小
	FILE *fd = _wfopen(szModuleName, L"rb");
	if (fd == NULL)
	{
		return false;
	}
	fseek(fd, 0, SEEK_END);
	size = ftell(fd);
	fclose(fd);

	// 读取文件内容
	char *buffer = new char[size];
	fd = _wfopen(szModuleName, L"rb");
	int count = 0;
	while (count < size && !feof(fd))
	{
		count += fread(buffer+count,  sizeof(char), size, fd);
	}
	fclose(fd);

	// 读微博数量
	int offset = 0;
	memcpy(&size, buffer, sizeof(int));
	offset += sizeof(int);

	// 生成微博内容
	M8::m8_status ** pTempStatuses = new M8::m8_status *[size];
	M8::m8_status * pStatusArray = new M8::m8_status[size];
	for (int i = 0; i < size; i++)
	{
		pTempStatuses[i] = pStatusArray+i;
		offset += sizeof(DWORD);
		offset += ReadStatus(pTempStatuses[i], buffer+offset);
	}

	// 回收内存
	delete [] buffer;

	// 判断是否读取成功
	if (offset != count)
	{
		delete [] pStatusArray;
		delete [] pTempStatuses;
		return false;
	}
	else
	{
		pStatuses = pTempStatuses;
		return true;
	}
}

CMzString CM8WeiboApp::UpdateTokenName(char *uid, char *url, char *kind, CMzString& kind_path)
{
	// 根据profile_image_url生成MD5值
	MD5_CTX md5;
	char digest[20]={0};
	char buffer[40]={0};
	md5.MD5Update((unsigned char *)url, strlen(url));
	md5.MD5Final((unsigned char *)digest);
	for (int i = 0; i < 16; i++) sprintf(buffer+i*2, "%02X", (unsigned char)digest[i]);

	CMzString modul_name = theApp.GetModuleFileName(NULL);
	kind_path = modul_name.SubStr(0, modul_name.FindReverse(L'\\')+1);
	kind_path += CStringA2W(kind).ptr();
	kind_path += L"\\";

	char file_name[128];
	sprintf(file_name, "%s_%s.", uid, buffer);
	return CMzString(CStringA2W(file_name).ptr());
}

CMzString CM8WeiboApp::UpdateProfileTokenName(char *uid, char *profile_image_url, CMzString& profile_image_path)
{
	return UpdateTokenName(uid, profile_image_url, "profile_images", profile_image_path);
}

CMzString CM8WeiboApp::UpdateSmallPictureTokenName(char *uid, char *picture_url, CMzString& picture_cache_path)
{
	return UpdateTokenName(uid, picture_url, "picture_cache\\small", picture_cache_path);
}

CMzString CM8WeiboApp::UpdateMiddlePictureTokenName(char *uid, char *picture_url, CMzString& picture_cache_path)
{
	return UpdateTokenName(uid, picture_url, "picture_cache\\middle", picture_cache_path);
}

CMzString CM8WeiboApp::UpdateOriginalPictureTokenName(char *uid, char *picture_url, CMzString& picture_cache_path)
{
	return UpdateTokenName(uid, picture_url, "picture_cache\\original", picture_cache_path);
}

CMzString CM8WeiboApp::FindTokenFileName(CMzString image_path, CMzString token_name)
{
	//查找匹配的文件名
	WIN32_FIND_DATA fd;
	CMzString image_name = image_path + token_name + L"*";
	HANDLE handle = FindFirstFile(image_name.C_Str(), &fd);
	if (handle == INVALID_HANDLE_VALUE)
	{
		image_name = L"";
	}
	else
	{
		image_name = image_path + fd.cFileName;
	}
	CloseHandle(handle);
	return image_name;
}

void CM8WeiboApp::CreateDirectory( CMzString &path )
{
	DWORD dwAttr = GetFileAttributes(path.C_Str());
	if (dwAttr == 0xFFFFFFFF || dwAttr & FILE_ATTRIBUTE_DIRECTORY != FILE_ATTRIBUTE_DIRECTORY)
	{
		// 自动创建目录
		int pos = 0;
		while ((pos = path.Find(L'\\', pos)+1) > 0)
		{
			CMzString temp = path.SubStr(0, pos);
			DWORD dwAttr = GetFileAttributes(temp.C_Str());
			if (dwAttr == 0xFFFFFFFF || dwAttr & FILE_ATTRIBUTE_DIRECTORY != FILE_ATTRIBUTE_DIRECTORY)
			{
				::CreateDirectory(temp.C_Str(), NULL);
			}
		}
	}
}

bool CM8WeiboApp::DownloadTokenFile(HWND hWnd, char *uid, bool uniq_uid, char *url, CMzString &token_path, CMzString &token_name)
{
	// UID唯一，只能存在以UID开头的一个文件
	CMzString image_name;
	if (uniq_uid)
		image_name = token_path + CStringA2W(uid).ptr() + L"_*";
	else
		image_name = token_path + token_name + L"*";

	CreateDirectory(token_path);


	// 找不到或者找到MD5值不匹配的文件需要更新头像
	bool bUpdate = false;
	WIN32_FIND_DATA fd;
	HANDLE handle = FindFirstFile(image_name.C_Str(), &fd);
	if (handle == INVALID_HANDLE_VALUE)
	{
		bUpdate = true;
	}
	else
	{
		if (wmemcmp(token_name.C_Str(), fd.cFileName, token_name.Length()))
		{
			image_name = token_path + fd.cFileName;
			if (uniq_uid) DeleteFile(image_name.C_Str());
			bUpdate = true;
		}
	}
	CloseHandle(handle);

	long http_code = 0;
	if (bUpdate)
	{// 下载文件
		MzBeginWaitDlg(hWnd, NULL, true);
		lohttp::LOHttp* pHttp = lohttp::Http_createobject();
		lohttp::Http_seturl(pHttp, HTTP_T(url));
		lohttp::Http_setopt(pHttp, lohttp::LOHTTP_OPT_setmethod, lohttp::E_HTTP_GET_METHOD);
		lohttp::Http_start(pHttp);
		lohttp::Http_wait(pHttp);
		lohttp::LOHttpResponse* resp = lohttp::Http_getRespone(pHttp);
		http_code = lohttp::Http_resp_code(resp);
		if (http_code == 200)
		{
			UserData *phead = new UserData();
			UserData *pbody = new UserData();
			lohttp::Http_resp_readheader(resp, proc_http_resp_header_callback, phead);
			lohttp::Http_resp_readbody(resp, proc_http_resp_body_callback, pbody);

			char *pos = strstr(phead->buffer, "Content-Type:");
			if (pos)
			{
				// Content-Type: image/jpeg
				pos = strchr(pos, '/');
				if (pos)
				{
					char *end = strchr(++pos, '\r');
					char temp[16];
					memcpy(temp, pos, end-pos);
					temp[end-pos] = '\0';
					token_name += CStringA2W(temp).ptr();
				}
			}

			image_name = token_path + token_name;
			HANDLE handle = CreateFile(image_name.C_Str(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			int offset = 0;
			DWORD dwWrited;
			while (offset < pbody->size)
			{
				dwWrited = 0;
				if (WriteFile(handle, pbody->buffer+offset, pbody->size-offset, &dwWrited, NULL))
					offset += dwWrited;
				else
					break;
			}

			CloseHandle(handle);
			delete phead;
			delete pbody;
		}
		lohttp::Http_resp_destroy(resp);
		lohttp::Http_detachRespone(pHttp);
		lohttp::Http_destroyobject(pHttp);
		MzEndWaitDlg();
	}

	return bUpdate && http_code;
}

bool CM8WeiboApp::IsGif(char *uri)
{
	char* pos = strrchr(uri, '.');
	return ( pos && (*(pos+1) == 'g' || *(pos+1) == 'G') && (*(pos+2) == 'i' || *(pos+2) == 'I') && (*(pos+3) == 'f' || *(pos+3) == 'F') );
}

void CM8WeiboApp::GoHome()
{
	CMzWndEx* pWnd = NULL;
	while(m_stack.size() > 1)
	{
		pWnd = m_stack.top();
		pWnd->DestroyWindow();
		delete pWnd;
		m_stack.pop();		
	}
}

void CM8WeiboApp::GoLogin()
{
	CMzWndEx* pWnd = NULL;
	while(m_stack.size())
	{
		pWnd = m_stack.top();
		pWnd->DestroyWindow();
		delete pWnd;
		m_stack.pop();
	}
}

void CM8WeiboApp::ShowAll()
{
	CMzWndEx* pWnd = NULL;
	std::stack<CMzWndEx *> temp1 = m_stack;
	std::stack<CMzWndEx *> temp2;
	while (temp1.size() > 0)
	{
		temp2.push(temp1.top());
		temp1.pop();
	}
	while (temp2.size() > 0)
	{
		pWnd = temp2.top();
		temp2.pop();
		SetForegroundWindow(pWnd->m_hWnd);
	}
}

void CM8WeiboApp::PopWnd(CMzWndEx *pWnd)
{
	std::stack<CMzWndEx *> temp;
	CMzWndEx *pElement = NULL;
	while(m_stack.size())
	{
		pElement = m_stack.top();
		m_stack.pop();
		if (pElement == pWnd)
		{
			break;
		}
		else
		{
			temp.push(pElement);
		}
	}

	int size = temp.size();
	while(temp.size())
	{
		m_stack.push(temp.top());
		temp.pop();
	}
	if (size > 0) ShowAll();
}

int CM8WeiboApp::SplitString( wchar_t *p_text, int chars_per_line )
{
	int index = 0;
	float count = 0;
	int size = wcslen(p_text);
	for (int i = 0; i < size; i++)
	{
		count += (*p_text++ < 256)? 0.5: 1;
		if (count == chars_per_line-0.5 && *p_text < 256)
		{
			p_text++;i++;
			if (i < size) count += 0.5;
		}
		if (count == chars_per_line-0.5 || count == chars_per_line)
		{
			count = 0;
			index += 1;
		}
	}
	if (count > 0)
	{
		count = 0;
		index += 1;
	}
	return index;
}

bool CM8WeiboApp::SplitString( int lines, CMzString *p_lines, wchar_t *p_text, int chars_per_line )
{
	int buffer_size = (chars_per_line+1)*2;
	wchar_t *buffer =  new wchar_t [buffer_size];
	int index = 0;
	float count = 0;
	wchar_t *prev, *next;
	prev = next = p_text;
	int size = wcslen(p_text);
	for (int i = 0; i < size; i++)
	{
		count += (*next++ < 256)? 0.5: 1;
		if (count == chars_per_line-0.5 && *next < 256)
		{
			next++;i++;
			if (i < size) count += 0.5;
		}
		if (count == chars_per_line-0.5 || count == chars_per_line)
		{
			wmemset(buffer, 0, buffer_size);
			wmemcpy(buffer, prev, next-prev);
			if (index < lines)
			{
				p_lines[index] = buffer;
			}
			else
			{
				delete [] buffer;
				return false;
			}
			prev = next;
			count = 0;
			index += 1;
		}
	}

	if (prev != next)
	{
		wmemset(buffer, 0, buffer_size);
		wmemcpy(buffer, prev, next-prev);
		if (index < lines)
		{
			p_lines[index] = buffer;
		}
		else
		{
			delete [] buffer;
			return false;
		}
		prev = next;
		count = 0;
		index += 1;
	}

	delete [] buffer;
	return index == lines;
}

void CM8WeiboApp::FormatCreateAt(wchar_t *pointer, wchar_t etc[6][16])
{
	wchar_t * prev, *next;
	prev = next = pointer;
	int index = 0, size = wcslen(pointer);
	for (int i = 0; i < size; i++)
	{
		if (*next == L' ')
		{
			if (index==0)
			{
				wmemcpy(etc[index++], prev, next-prev);
			}
			else
			{
				wmemcpy(etc[index++], prev+1, next-prev-1);
			}
			prev = next;	
		}
		next++;
	}
	if (prev != next)
	{
		wmemcpy(etc[index++], prev+1, next-prev-1);
		prev = next;	
	}

	etc[1][3] = 0;//忽略3位以后的缩写
	if (wcscmp(etc[1], L"Jan") == 0)
		wcscpy(etc[1], L"01");
	else if (wcscmp(etc[1], L"Feb") == 0)
		wcscpy(etc[1], L"02");
	else if (wcscmp(etc[1], L"Mar") == 0)
		wcscpy(etc[1], L"03");
	else if (wcscmp(etc[1], L"Apr") == 0)
		wcscpy(etc[1], L"04");
	else if (wcscmp(etc[1], L"May") == 0)
		wcscpy(etc[1], L"05");
	else if (wcscmp(etc[1], L"Jun") == 0)
		wcscpy(etc[1], L"06");
	else if (wcscmp(etc[1], L"Jul") == 0)
		wcscpy(etc[1], L"07");
	else if (wcscmp(etc[1], L"Aug") == 0)
		wcscpy(etc[1], L"08");
	else if (wcscmp(etc[1], L"Sep") == 0)
		wcscpy(etc[1], L"09");
	else if (wcscmp(etc[1], L"Oct") == 0)
		wcscpy(etc[1], L"10");
	else if (wcscmp(etc[1], L"Nov") == 0)
		wcscpy(etc[1], L"11");
	else if (wcscmp(etc[1], L"Dec") == 0)
		wcscpy(etc[1], L"12");
	else
		wcscpy(etc[1], L"00");
	etc[3][5] = 0;//忽略秒
}

void CM8WeiboApp::FormatSource(wchar_t *pointer, wchar_t *buffer)
{
	wcscpy(buffer, L"来自");
	int size = wcslen(pointer);
	wchar_t *prev, *next;
	prev = next = pointer+size-4;
	for (int i = size-4; i>=0; i--)
	{
		if (*prev == L'>')
		{
			break;
		}
		prev--;
	}
	wmemcpy(buffer+2, prev+1,next-prev-1);
}

int CM8WeiboApp::FindAtUser(wchar_t *p_text)
{
	wchar_t *pos = wcschr(p_text, L'@');
	if (pos == NULL) return 0;

	wchar_t buff[2];
	while(*(++pos))
	{
		if (*pos == L' ' || *pos == L',' || *pos == L'.' || *pos == L':' || *pos == L'#' || *pos == L'!' || *pos == L'?' || *pos == L'|' || *pos == L'(' || *pos == L')' || *pos == L'[' || *pos == L']' || *pos == L'{' || *pos == L'}')
		{
			return 1+FindAtUser(++pos);
		}
		if (*pos == L'@') return 1+FindAtUser(pos);
		wcscpy(buff, L"　"); if (wmemcmp(buff, pos, 1) == 0) return 1+FindAtUser(++pos);
		wcscpy(buff, L"，"); if (wmemcmp(buff, pos, 1) == 0) return 1+FindAtUser(++pos);
		wcscpy(buff, L"。"); if (wmemcmp(buff, pos, 1) == 0) return 1+FindAtUser(++pos);
		wcscpy(buff, L"："); if (wmemcmp(buff, pos, 1) == 0) return 1+FindAtUser(++pos);
		wcscpy(buff, L"！"); if (wmemcmp(buff, pos, 1) == 0) return 1+FindAtUser(++pos);
		wcscpy(buff, L"？"); if (wmemcmp(buff, pos, 1) == 0) return 1+FindAtUser(++pos);
		wcscpy(buff, L"、"); if (wmemcmp(buff, pos, 1) == 0) return 1+FindAtUser(++pos);
		wcscpy(buff, L"（"); if (wmemcmp(buff, pos, 1) == 0) return 1+FindAtUser(++pos);
		wcscpy(buff, L"）"); if (wmemcmp(buff, pos, 1) == 0) return 1+FindAtUser(++pos);
		wcscpy(buff, L"【"); if (wmemcmp(buff, pos, 1) == 0) return 1+FindAtUser(++pos);
		wcscpy(buff, L"】"); if (wmemcmp(buff, pos, 1) == 0) return 1+FindAtUser(++pos);
		wcscpy(buff, L"｛"); if (wmemcmp(buff, pos, 1) == 0) return 1+FindAtUser(++pos);
		wcscpy(buff, L"｝"); if (wmemcmp(buff, pos, 1) == 0) return 1+FindAtUser(++pos);
	}

	return 1;
}

void CM8WeiboApp::FindAtUser(CMzString *p_users, wchar_t *p_text)
{
	wchar_t *end = wcschr(p_text, L'@');
	if (end == NULL) return;

	wchar_t *begin = end;
	wchar_t buff[2];
	while(*(++end))
	{
		if (*end == L' ' || *end == L',' || *end == L'.' || *end == L':' || *end == L'#' || *end == L'!' || *end == L'?' || *end == L'|' || *end == L'(' || *end == L')' || *end == L'[' || *end == L']' || *end == L'{' || *end == L'}')
		{
			AssignString(begin, end, p_users);
			FindAtUser(++p_users, ++end);
			return;
		}
		if (*end == L'@') {AssignString(begin, end, p_users); FindAtUser(++p_users, end); return;}
		wcscpy(buff, L"　"); if (wmemcmp(buff, end, 1) == 0) {AssignString(begin, end, p_users); FindAtUser(++p_users, ++end); return;}
		wcscpy(buff, L"，"); if (wmemcmp(buff, end, 1) == 0) {AssignString(begin, end, p_users); FindAtUser(++p_users, ++end); return;}
		wcscpy(buff, L"。"); if (wmemcmp(buff, end, 1) == 0) {AssignString(begin, end, p_users); FindAtUser(++p_users, ++end); return;}
		wcscpy(buff, L"："); if (wmemcmp(buff, end, 1) == 0) {AssignString(begin, end, p_users); FindAtUser(++p_users, ++end); return;}
		wcscpy(buff, L"！"); if (wmemcmp(buff, end, 1) == 0) {AssignString(begin, end, p_users); FindAtUser(++p_users, ++end); return;}
		wcscpy(buff, L"？"); if (wmemcmp(buff, end, 1) == 0) {AssignString(begin, end, p_users); FindAtUser(++p_users, ++end); return;}
		wcscpy(buff, L"、"); if (wmemcmp(buff, end, 1) == 0) {AssignString(begin, end, p_users); FindAtUser(++p_users, ++end); return;}
		wcscpy(buff, L"（"); if (wmemcmp(buff, end, 1) == 0) {AssignString(begin, end, p_users); FindAtUser(++p_users, ++end); return;}
		wcscpy(buff, L"）"); if (wmemcmp(buff, end, 1) == 0) {AssignString(begin, end, p_users); FindAtUser(++p_users, ++end); return;}
		wcscpy(buff, L"【"); if (wmemcmp(buff, end, 1) == 0) {AssignString(begin, end, p_users); FindAtUser(++p_users, ++end); return;}
		wcscpy(buff, L"】"); if (wmemcmp(buff, end, 1) == 0) {AssignString(begin, end, p_users); FindAtUser(++p_users, ++end); return;}
		wcscpy(buff, L"｛"); if (wmemcmp(buff, end, 1) == 0) {AssignString(begin, end, p_users); FindAtUser(++p_users, ++end); return;}
		wcscpy(buff, L"｝"); if (wmemcmp(buff, end, 1) == 0) {AssignString(begin, end, p_users); FindAtUser(++p_users, ++end); return;}
	}

	AssignString(begin, end, p_users);
}

void CM8WeiboApp::AssignString(wchar_t *begin, wchar_t *end, CMzString *p_users)
{
	int size = end-begin;
	wchar_t* buff = new wchar_t[size+1];
	wmemcpy(buff, begin, size);
	buff[size] = 0;
	*p_users = buff;
	delete [] buff;
}

int CM8WeiboApp::GetAccountList(wchar_t *account_list, CMzString *&szAccounts)
{
	std::list<int> list;
	wchar_t * prev, *next;
	prev = next = account_list;
	while ((next = wcschr(prev, L',')) != NULL)
	{
		wchar_t * temp = new wchar_t[next-prev+1];
		wmemcpy(temp, prev, next-prev);
		temp[next-prev] = 0;
		prev = next+1;
		if (*temp) list.push_back((int)temp);
	}
	if (prev)
	{
		if (*prev) list.push_back((int)prev);
	}

	int i = 0;
	szAccounts = new CMzString [list.size()];
	std::list<int>::iterator iter;
	for (iter = list.begin(); iter != list.end(); iter++)
	{
		wchar_t * temp = (wchar_t *)(*iter);
		szAccounts[i++] = temp;
		delete [] temp;
	}
	return list.size();
}

void CM8WeiboApp::InitWeiboInterface()
{
	// 初始化微博接口
	load_wb_interface(&wb_interface);
	if (wbRequest) wb_interface.pfun_request_destroy(wbRequest);
	wbRequest = wb_interface.pfun_request_create();
	wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_COOKIE) , 0);
	wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_APPKEY) , "2526469041");
	wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_SECRETKEY) , "7db5e56c8b40a94bb9708dea5b4d9c90");
	wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_RESULT_DATAFORMAT), E_FORMAT_JSON);
}

bool CM8WeiboApp::LoginWeibo(HWND hWnd, CMzString account, CMzString password)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	// OAUTH_REQUEST_TOKEN
	M8::m8_wb_oauth_request_token oauth_reqtoken;
	Wb_init_wb_struct(WEIBO_OPTION(OAUTH_REQUEST_TOKEN), &oauth_reqtoken);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(OAUTH_REQUEST_TOKEN), &oauth_reqtoken);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &oauth_reqtoken);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);

	bool is_ok = OutputMessage(hWnd, oauth_reqtoken.http_code_, oauth_reqtoken.error_);
	if (is_ok == false)
	{
		MzEndWaitDlg();
		return is_ok;
	}

	// OAUTH_AUTHORIZE
	M8::m8_wb_authorize oauth_authorize;
	Wb_init_wb_struct(WEIBO_OPTION(OAUTH_AUTHORIZE), &oauth_authorize);
	strcpy(oauth_authorize.wbauth_.oauth_token_, oauth_reqtoken.wbauth_.oauth_token_);
	strcpy(oauth_authorize.wbauth_.oauth_token_secret_, oauth_reqtoken.wbauth_.oauth_token_secret_);
	strcpy(oauth_authorize.usrid_, CStringW2A(account.C_Str()).ptr());
	//strcpy(oauth_authorize.usrpwd_, CStringW2A(password.C_Str()).ptr());
	strcpy(oauth_authorize.usrpwd_, CURLEncodeA(CStringW2A(password.C_Str()).ptr(), 0).c_str());
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(OAUTH_AUTHORIZE), &oauth_authorize);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &oauth_authorize);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);

	is_ok = OutputMessage(hWnd, oauth_authorize.http_code_, oauth_authorize.error_);
	if (is_ok == false)
	{
		MzEndWaitDlg();
		return is_ok;
	}

	// OAUTH_ACCESS_TOKEN
	delete m_oauth_access_token;
	m_oauth_access_token = new M8::m8_wb_oauth_access_token();
	Wb_init_wb_struct(WEIBO_OPTION(OAUTH_ACCESS_TOKEN), m_oauth_access_token);
	strcpy(m_oauth_access_token->wbauth_.oauth_token_, oauth_authorize.wbauth_.oauth_token_);
	strcpy(m_oauth_access_token->wbauth_.oauth_token_secret_, oauth_authorize.wbauth_.oauth_token_secret_);
	strcpy(m_oauth_access_token->wbauth_.oauth_verifier_, oauth_authorize.wbauth_.oauth_verifier_);
	m_oauth_access_token->http_code_ = 0;
	memset(m_oauth_access_token->error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(OAUTH_ACCESS_TOKEN), m_oauth_access_token);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), m_oauth_access_token);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	return OutputMessage(hWnd, m_oauth_access_token->http_code_, m_oauth_access_token->error_);
}

bool CM8WeiboApp::RefreshUserInfoSelf(HWND hWnd)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_get_users_show *p_tmp_user_info = new M8::m8_wb_get_users_show();
	Wb_init_wb_struct(WEIBO_OPTION(GETUSER_INFO), p_tmp_user_info);
	strcpy(p_tmp_user_info->wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_tmp_user_info->wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	p_tmp_user_info->http_code_ = 0;
	memset(p_tmp_user_info->error_, 0, WB_REAL_LEN(LENGTH8));
	p_tmp_user_info->wbuid_.uidtype_ = t_wb_uid::IDType_userid;
	strcpy(p_tmp_user_info->wbuid_.uid_, m_oauth_access_token->uid_);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETUSER_INFO), p_tmp_user_info);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_tmp_user_info);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	bool is_ok = OutputMessage(hWnd, p_tmp_user_info->http_code_, p_tmp_user_info->error_);
	if (is_ok)
	{
		delete m_user_info_self;
		m_user_info_self = p_tmp_user_info;
	}
	else
	{
		delete p_tmp_user_info;
		p_tmp_user_info = NULL;
	}
	return is_ok;
}

bool CM8WeiboApp::GetUserInfoUnknown(HWND hWnd, int uid_type, char *uid, M8::m8_wb_get_users_show *p_user_info)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	Wb_init_wb_struct(WEIBO_OPTION(GETUSER_INFO), p_user_info);
	strcpy(p_user_info->wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_user_info->wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	p_user_info->http_code_ = 0;
	memset(p_user_info->error_, 0, WB_REAL_LEN(LENGTH8));
	p_user_info->wbuid_.uidtype_ = uid_type;
	strcpy(p_user_info->wbuid_.uid_, uid);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETUSER_INFO), p_user_info);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_user_info);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	return OutputMessage(hWnd, p_user_info->http_code_, p_user_info->error_);
}

bool CM8WeiboApp::GetStatusesFriendsTimeline(HWND hWnd, int opt_type, char *opt_id, 
												 M8::m8_wb_get_statuses_friends_timeline *&p_statuses_friends_timeline)
{
	p_statuses_friends_timeline = NULL;

	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_get_statuses_friends_timeline *p_tmp_statuses_friends_timeline = new M8::m8_wb_get_statuses_friends_timeline();
	Wb_init_wb_struct(WEIBO_OPTION(GETSTATUSES_FRIENDS_TIMELINE), p_tmp_statuses_friends_timeline);
	strcpy(p_tmp_statuses_friends_timeline->wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_tmp_statuses_friends_timeline->wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	p_tmp_statuses_friends_timeline->http_code_ = 0;
	memset(p_tmp_statuses_friends_timeline->error_, 0, WB_REAL_LEN(LENGTH8));
	if (opt_type == t_wb_option_params::WBOPTType_since_id && opt_id)
	{
		strcpy(p_tmp_statuses_friends_timeline->wbopt_.szOpt_[t_wb_option_params::WBOPTType_since_id], opt_id);
	}
	if (opt_type == t_wb_option_params::WBOPTType_max_id && opt_id)
	{
		strcpy(p_tmp_statuses_friends_timeline->wbopt_.szOpt_[t_wb_option_params::WBOPTType_max_id], opt_id);
	}
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETSTATUSES_FRIENDS_TIMELINE), p_tmp_statuses_friends_timeline);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_tmp_statuses_friends_timeline);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);

	bool is_ok = OutputMessage(hWnd, p_tmp_statuses_friends_timeline->http_code_, p_tmp_statuses_friends_timeline->error_);
	if (is_ok)
	{
		M8::m8_wb_get_statuses_counts statuses_counts;
		Wb_init_wb_struct(WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), &statuses_counts);
		strcpy(statuses_counts.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
		strcpy(statuses_counts.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
		for (int i = 0; i < p_tmp_statuses_friends_timeline->size_of_status_; i++)
		{
			strcpy(statuses_counts.wbIds_[i], p_tmp_statuses_friends_timeline->statuses_[i].id_);
		}
		wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), &statuses_counts);
		wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &statuses_counts);
		wb_interface.pfun_request_start(wbRequest, false);
		wb_interface.pfun_request_stop(wbRequest);

		bool is_ok2;
		if (p_tmp_statuses_friends_timeline->size_of_status_ == 0)
		{
			is_ok2 = true;
		}
		else
		{
			is_ok2 = OutputMessage(hWnd, statuses_counts.http_code_, statuses_counts.error_);
		}

		if (is_ok2)
		{
			p_statuses_friends_timeline = p_tmp_statuses_friends_timeline;
			for (int i = 0; i < p_statuses_friends_timeline->size_of_status_; i++)
			{
				p_statuses_friends_timeline->statuses_[i].m_comments_ = statuses_counts.comments_[i];
				p_statuses_friends_timeline->statuses_[i].m_links_ = statuses_counts.rt_[i];
			}
		}
		else
		{
			delete p_tmp_statuses_friends_timeline;
			p_tmp_statuses_friends_timeline = NULL;
		}

		MzEndWaitDlg();
		return is_ok && is_ok2;
	}
	else
	{
		delete p_tmp_statuses_friends_timeline;
		p_tmp_statuses_friends_timeline = NULL;
		MzEndWaitDlg();
		return is_ok;
	}
}

bool CM8WeiboApp::GetStatusesUserTimeline(HWND hWnd, char *uid, int opt_type, char *opt_id, 
											   M8::m8_wb_get_statuses_user_timeline *&p_statuses_user_timeline)
{
	p_statuses_user_timeline = NULL;

	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_get_statuses_user_timeline *p_tmp_statuses_user_timeline = new M8::m8_wb_get_statuses_user_timeline();
	Wb_init_wb_struct(WEIBO_OPTION(GETSTATUSES_USE_TIMELINE), p_tmp_statuses_user_timeline);
	strcpy(p_tmp_statuses_user_timeline->wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_tmp_statuses_user_timeline->wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	p_tmp_statuses_user_timeline->http_code_ = 0;
	memset(p_tmp_statuses_user_timeline->error_, 0, WB_REAL_LEN(LENGTH8));
	p_tmp_statuses_user_timeline->wbuid_.uidtype_ = t_wb_uid::IDType_id;
	strcpy(p_tmp_statuses_user_timeline->wbuid_.uid_, uid);
	if (opt_type == t_wb_option_params::WBOPTType_since_id && opt_id)
	{
		strcpy(p_tmp_statuses_user_timeline->wbopt_.szOpt_[t_wb_option_params::WBOPTType_since_id], opt_id);
	}
	if (opt_type == t_wb_option_params::WBOPTType_max_id && opt_id)
	{
		strcpy(p_tmp_statuses_user_timeline->wbopt_.szOpt_[t_wb_option_params::WBOPTType_max_id], opt_id);
	}
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETSTATUSES_USE_TIMELINE), p_tmp_statuses_user_timeline);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_tmp_statuses_user_timeline);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);

	bool is_ok = OutputMessage(hWnd, p_tmp_statuses_user_timeline->http_code_, p_tmp_statuses_user_timeline->error_);
	if (is_ok)
	{
		M8::m8_wb_get_statuses_counts statuses_counts;
		Wb_init_wb_struct(WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), &statuses_counts);
		strcpy(statuses_counts.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
		strcpy(statuses_counts.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
		for (int i = 0; i < p_tmp_statuses_user_timeline->size_of_status_; i++)
		{
			strcpy(statuses_counts.wbIds_[i], p_tmp_statuses_user_timeline->statuses_[i].id_);
		}
		wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), &statuses_counts);
		wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &statuses_counts);
		wb_interface.pfun_request_start(wbRequest, false);
		wb_interface.pfun_request_stop(wbRequest);

		bool is_ok2;
		if (p_tmp_statuses_user_timeline->size_of_status_ == 0)
		{
			is_ok2 = true;
		} 
		else
		{
			is_ok2 = OutputMessage(hWnd, statuses_counts.http_code_, statuses_counts.error_);
		}

		if (is_ok2)
		{
			p_statuses_user_timeline = p_tmp_statuses_user_timeline;
			for (int i = 0; i < p_statuses_user_timeline->size_of_status_; i++)
			{
				p_statuses_user_timeline->statuses_[i].m_comments_ = statuses_counts.comments_[i];
				p_statuses_user_timeline->statuses_[i].m_links_ = statuses_counts.rt_[i];
			}
		}
		else
		{
			delete p_tmp_statuses_user_timeline;
			p_tmp_statuses_user_timeline = NULL;
		}

		MzEndWaitDlg();
		return is_ok && is_ok2;
	}
	else
	{
		delete p_tmp_statuses_user_timeline;
		p_tmp_statuses_user_timeline = NULL;
		MzEndWaitDlg();
		return is_ok;
	}
}

bool CM8WeiboApp::GetFavorites(HWND hWnd, int page, M8::m8_wb_get_favorites *&p_favorites)
{
	p_favorites = NULL;

	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_get_favorites *p_tmp_favorites = new M8::m8_wb_get_favorites();
	Wb_init_wb_struct(WEIBO_OPTION(GETFAVORITES), p_tmp_favorites);
	strcpy(p_tmp_favorites->wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_tmp_favorites->wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	sprintf(p_tmp_favorites->wbopt_.szOpt_[t_wb_option_params::WBOPTType_page], "%d", page);
	p_tmp_favorites->page_ = page;
	p_tmp_favorites->http_code_ = 0;
	memset(p_tmp_favorites->error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETFAVORITES), p_tmp_favorites);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_tmp_favorites);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);

	bool is_ok = OutputMessage(hWnd, p_tmp_favorites->http_code_, p_tmp_favorites->error_);
	if (is_ok)
	{
		M8::m8_wb_get_statuses_counts statuses_counts;
		Wb_init_wb_struct(WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), &statuses_counts);
		strcpy(statuses_counts.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
		strcpy(statuses_counts.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
		for (int i = 0; i < p_tmp_favorites->size_of_status_; i++)
		{
			strcpy(statuses_counts.wbIds_[i], p_tmp_favorites->statuses_[i].id_);
		}
		wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), &statuses_counts);
		wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &statuses_counts);
		wb_interface.pfun_request_start(wbRequest, false);
		wb_interface.pfun_request_stop(wbRequest);

		bool is_ok2;
		if (p_tmp_favorites->size_of_status_ == 0)
		{
			is_ok2 = true;
		}
		else
		{
			is_ok2 = OutputMessage(hWnd, statuses_counts.http_code_, statuses_counts.error_);
		}

		if (is_ok2)
		{
			p_favorites = p_tmp_favorites;
			for (int i = 0; i < p_favorites->size_of_status_; i++)
			{
				p_favorites->statuses_[i].m_comments_ = statuses_counts.comments_[i];
				p_favorites->statuses_[i].m_links_ = statuses_counts.rt_[i];
			}
		}
		else
		{
			delete p_tmp_favorites;
			p_tmp_favorites = NULL;
		}

		MzEndWaitDlg();
		return is_ok && is_ok2;
	}
	else
	{
		delete p_tmp_favorites;
		p_tmp_favorites = NULL;
		MzEndWaitDlg();
		return is_ok;
	}
}

bool CM8WeiboApp::GetStatusesMentions(HWND hWnd, int opt_type, char *opt_id,
						 M8::m8_wb_get_statuses_mentions *&p_statuses_mentions)
{
	p_statuses_mentions = NULL;

	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_get_statuses_mentions *p_tmp_statuses_mentions = new M8::m8_wb_get_statuses_mentions();
	Wb_init_wb_struct(WEIBO_OPTION(GETSTATUSES_MENTIONS), p_tmp_statuses_mentions);
	strcpy(p_tmp_statuses_mentions->wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_tmp_statuses_mentions->wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	p_tmp_statuses_mentions->http_code_ = 0;
	memset(p_tmp_statuses_mentions->error_, 0, WB_REAL_LEN(LENGTH8));
	if (opt_type == t_wb_option_params::WBOPTType_since_id && opt_id)
	{
		strcpy(p_tmp_statuses_mentions->wbopt_.szOpt_[t_wb_option_params::WBOPTType_since_id], opt_id);
	}
	if (opt_type == t_wb_option_params::WBOPTType_max_id && opt_id)
	{
		strcpy(p_tmp_statuses_mentions->wbopt_.szOpt_[t_wb_option_params::WBOPTType_max_id], opt_id);
	}
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETSTATUSES_MENTIONS), p_tmp_statuses_mentions);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_tmp_statuses_mentions);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);

	bool is_ok = OutputMessage(hWnd, p_tmp_statuses_mentions->http_code_, p_tmp_statuses_mentions->error_);
	if (is_ok)
	{
		M8::m8_wb_get_statuses_counts statuses_counts;
		Wb_init_wb_struct(WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), &statuses_counts);
		strcpy(statuses_counts.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
		strcpy(statuses_counts.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
		for (int i = 0; i < p_tmp_statuses_mentions->size_of_status_; i++)
		{
			strcpy(statuses_counts.wbIds_[i], p_tmp_statuses_mentions->statuses_[i].id_);
		}
		wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), &statuses_counts);
		wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &statuses_counts);
		wb_interface.pfun_request_start(wbRequest, false);
		wb_interface.pfun_request_stop(wbRequest);

		bool is_ok2;
		if (p_tmp_statuses_mentions->size_of_status_ == 0)
		{
			is_ok2 = true;
		}
		else
		{
			is_ok2 = OutputMessage(hWnd, statuses_counts.http_code_, statuses_counts.error_);
		}

		if (is_ok2)
		{
			p_statuses_mentions = p_tmp_statuses_mentions;
			for (int i = 0; i < p_statuses_mentions->size_of_status_; i++)
			{
				p_statuses_mentions->statuses_[i].m_comments_ = statuses_counts.comments_[i];
				p_statuses_mentions->statuses_[i].m_links_ = statuses_counts.rt_[i];
			}
		}
		else
		{
			delete p_tmp_statuses_mentions;
			p_tmp_statuses_mentions = NULL;
		}

		MzEndWaitDlg();
		return is_ok && is_ok2;
	}
	else
	{
		delete p_tmp_statuses_mentions;
		p_tmp_statuses_mentions = NULL;
		MzEndWaitDlg();
		return is_ok;
	}
}

bool CM8WeiboApp::GetStatusesCounts(HWND hWnd, int size, M8::m8_counts *p_counts)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_get_statuses_counts *p_statuses_counts = new M8::m8_wb_get_statuses_counts();
	Wb_init_wb_struct(WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), p_statuses_counts);
	strcpy(p_statuses_counts->wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_statuses_counts->wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	p_statuses_counts->http_code_ = 0;
	memset(p_statuses_counts->error_, 0, WB_REAL_LEN(LENGTH8));
	for (int i = 0; i < size; i++)
	{
		strcpy(p_statuses_counts->wbIds_[i], p_counts[i].wbId_);
	}
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), p_statuses_counts);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_statuses_counts);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	bool is_ok = OutputMessage(hWnd, p_statuses_counts->http_code_, p_statuses_counts->error_);
	if (is_ok)
	{
		for (int i = 0; i < size; i++)
		{
			p_counts[i].rt_ = p_statuses_counts->rt_[i];
			p_counts[i].comments_ = p_statuses_counts->comments_[i];
		}
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			p_counts[i].rt_ = 0;
			p_counts[i].comments_ = 0;
		}
	}

	delete p_statuses_counts;
	return is_ok;
}

bool CM8WeiboApp::GetStatusesCommentsList(HWND hWnd, char *wbId, int opt_type, char *opt_id,
							 M8::m8_wb_get_statuses_comments_list *&p_statuses_comments_list)
{
	p_statuses_comments_list = NULL;

	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_get_statuses_comments_list *p_tmp_statuses_comments_list = new M8::m8_wb_get_statuses_comments_list();
	Wb_init_wb_struct(WEIBO_OPTION(GETSTATUSES_COMMENTS_LIST), p_tmp_statuses_comments_list);
	strcpy(p_tmp_statuses_comments_list->wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_tmp_statuses_comments_list->wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	p_tmp_statuses_comments_list->http_code_ = 0;
	memset(p_tmp_statuses_comments_list->error_, 0, WB_REAL_LEN(LENGTH8));
	if (wbId) strcpy(p_tmp_statuses_comments_list->wbId_, wbId);
	if (opt_type == t_wb_option_params::WBOPTType_since_id && opt_id)
	{
		strcpy(p_tmp_statuses_comments_list->wbopt_.szOpt_[t_wb_option_params::WBOPTType_since_id], opt_id);
	}
	if (opt_type == t_wb_option_params::WBOPTType_max_id && opt_id)
	{
		strcpy(p_tmp_statuses_comments_list->wbopt_.szOpt_[t_wb_option_params::WBOPTType_max_id], opt_id);
	}
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETSTATUSES_COMMENTS_LIST), p_tmp_statuses_comments_list);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_tmp_statuses_comments_list);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	bool is_ok = OutputMessage(hWnd, p_tmp_statuses_comments_list->http_code_, p_tmp_statuses_comments_list->error_);
	if (is_ok)
	{
		p_statuses_comments_list = p_tmp_statuses_comments_list;
	}
	else
	{
		delete p_tmp_statuses_comments_list;
		p_tmp_statuses_comments_list = NULL;
	}

	return is_ok;
}

bool CM8WeiboApp::GetStatusesCommentsToMe(HWND hWnd, int opt_type, char *opt_id,
							 M8::m8_wb_get_statuses_comments_tome *&p_statuses_comments_tome)
{
	p_statuses_comments_tome = NULL;

	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_get_statuses_comments_tome *p_tmp_statuses_comments_tome = new M8::m8_wb_get_statuses_comments_tome();
	Wb_init_wb_struct(WEIBO_OPTION(GETSTATUSES_COMMENTS_TOME), p_tmp_statuses_comments_tome);
	strcpy(p_tmp_statuses_comments_tome->wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_tmp_statuses_comments_tome->wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	p_tmp_statuses_comments_tome->http_code_ = 0;
	memset(p_tmp_statuses_comments_tome->error_, 0, WB_REAL_LEN(LENGTH8));
	if (opt_type == t_wb_option_params::WBOPTType_since_id && opt_id)
	{
		strcpy(p_tmp_statuses_comments_tome->wbopt_.szOpt_[t_wb_option_params::WBOPTType_since_id], opt_id);
	}
	if (opt_type == t_wb_option_params::WBOPTType_max_id && opt_id)
	{
		strcpy(p_tmp_statuses_comments_tome->wbopt_.szOpt_[t_wb_option_params::WBOPTType_max_id], opt_id);
	}
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETSTATUSES_COMMENTS_TOME), p_tmp_statuses_comments_tome);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_tmp_statuses_comments_tome);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	bool is_ok = OutputMessage(hWnd, p_tmp_statuses_comments_tome->http_code_, p_tmp_statuses_comments_tome->error_);
	if (is_ok)
	{
		p_statuses_comments_tome = p_tmp_statuses_comments_tome;
	}
	else
	{
		delete p_tmp_statuses_comments_tome;
		p_tmp_statuses_comments_tome = NULL;
	}

	return is_ok;
}

bool CM8WeiboApp::isSelf(char *uid)
{
	return (strcmp(m_user_info_self->wbuid_.uid_, uid) == 0);
}

M8::m8_user * CM8WeiboApp::GetUserInfoSelf()
{
	return &(m_user_info_self->user_);
}

bool CM8WeiboApp::PutStatusesUpdate(HWND hWnd, CMzString text)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_put_statuses_update status_update;
	Wb_init_wb_struct(WEIBO_OPTION(PUTSTATUSES_UPDATE), &status_update);
	strcpy(status_update.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(status_update.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	status_update.http_code_ = 0;
	memset(status_update.error_, 0, WB_REAL_LEN(LENGTH8));
	if (text.Length() > WB_USE_LEN(WB_INFO)/2)
	{
		CMzString temp = text.SubStr(0, WB_USE_LEN(WB_INFO)/2);
		strcpy(status_update.szwbInfo_, CStringW2A(temp.C_Str()).ptr());
	}
	else
	{
		strcpy(status_update.szwbInfo_, CStringW2A(text.C_Str()).ptr());
	}
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(PUTSTATUSES_UPDATE), &status_update);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &status_update);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	return OutputMessage(hWnd, status_update.http_code_, status_update.error_);
}

bool CM8WeiboApp::PutStatusesUpload(HWND hWnd, CMzString path, CMzString text)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_put_statuses_upload status_upload;
	Wb_init_wb_struct(WEIBO_OPTION(PUTSTATUSES_UPLOAD), &status_upload);
	strcpy(status_upload.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(status_upload.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	status_upload.http_code_ = 0;
	memset(status_upload.error_, 0, WB_REAL_LEN(LENGTH8));
	strcpy(status_upload.szImagePath_, CStringW2A(path).ptr());
	if (text.Length() > WB_USE_LEN(WB_INFO)/2)
	{
		CMzString temp = text.SubStr(0, WB_USE_LEN(WB_INFO)/2);
		strcpy(status_upload.szwbInfo_, CStringW2A(temp.C_Str()).ptr());
	}
	else
	{
		strcpy(status_upload.szwbInfo_, CStringW2A(text.C_Str()).ptr());
	}
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(PUTSTATUSES_UPLOAD), &status_upload);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &status_upload);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	return OutputMessage(hWnd, status_upload.http_code_, status_upload.error_);
}

bool CM8WeiboApp::PutStatusesRepost(HWND hWnd, char *wbId, int is_comment, CMzString text)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_put_statuses_repost status_repost;
	Wb_init_wb_struct(WEIBO_OPTION(PUTSTATUSES_REPOST), &status_repost);
	strcpy(status_repost.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(status_repost.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	strcpy(status_repost.wbId_, wbId);
	status_repost.iCommentFlags = is_comment;
	status_repost.http_code_ = 0;
	memset(status_repost.error_, 0, WB_REAL_LEN(LENGTH8));
	if (text.Length() > WB_USE_LEN(WB_INFO)/2)
	{
		CMzString temp = text.SubStr(0, WB_USE_LEN(WB_INFO)/2);
		strcpy(status_repost.szwbInfo_, CStringW2A(temp.C_Str()).ptr());
	}
	else
	{
		strcpy(status_repost.szwbInfo_, CStringW2A(text.C_Str()).ptr());
	}
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(PUTSTATUSES_REPOST), &status_repost);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &status_repost);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	return OutputMessage(hWnd, status_repost.http_code_, status_repost.error_);
}

bool CM8WeiboApp::PutStatusesComment(HWND hWnd, char *wbId, char *wbCId, CMzString text, M8::m8_wb_put_statuses_comment *&p_status_comment)
{
	p_status_comment = NULL;
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_put_statuses_comment* p_tmp_status_comment = new M8::m8_wb_put_statuses_comment();
	Wb_init_wb_struct(WEIBO_OPTION(PUTSTATUSES_COMMENT), p_tmp_status_comment);
	strcpy(p_tmp_status_comment->wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_tmp_status_comment->wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	if (wbId) strcpy(p_tmp_status_comment->wbId_, wbId);
	if (wbCId) strcpy(p_tmp_status_comment->wbCId_, wbCId);
	p_tmp_status_comment->http_code_ = 0;
	memset(p_tmp_status_comment->error_, 0, WB_REAL_LEN(LENGTH8));
	if (text.Length() > WB_USE_LEN(WB_INFO)/2)
	{
		CMzString temp = text.SubStr(0, WB_USE_LEN(WB_INFO)/2);
		strcpy(p_tmp_status_comment->szwbInfo_, CStringW2A(temp.C_Str()).ptr());
	}
	else
	{
		strcpy(p_tmp_status_comment->szwbInfo_, CStringW2A(text.C_Str()).ptr());
	}
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(PUTSTATUSES_COMMENT), p_tmp_status_comment);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_tmp_status_comment);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	bool is_ok = OutputMessage(hWnd, p_tmp_status_comment->http_code_, p_tmp_status_comment->error_);
	if (is_ok)
	{
		p_status_comment = p_tmp_status_comment;
	} 
	else
	{
		delete p_tmp_status_comment;
		p_tmp_status_comment = NULL;
	}
	return is_ok;
}

bool CM8WeiboApp::PutFriendshipsCreate(HWND hWnd, char *wbId)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_put_friendships_create friendships_create;
	Wb_init_wb_struct(WEIBO_OPTION(PUTFRIENDSHIPS_CREATE), &friendships_create);
	strcpy(friendships_create.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(friendships_create.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	friendships_create.wbuid_.uidtype_ = t_wb_uid::IDType_userid;
	if (wbId) strcpy(friendships_create.wbuid_.uid_, wbId);
	friendships_create.http_code_ = 0;
	memset(friendships_create.error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(PUTFRIENDSHIPS_CREATE), &friendships_create);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &friendships_create);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	return OutputMessage(hWnd, friendships_create.http_code_, friendships_create.error_);
}

bool CM8WeiboApp::PutFriendshipsDestroy(HWND hWnd, char *wbId)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_put_friendships_destroy friendships_destroy;
	Wb_init_wb_struct(WEIBO_OPTION(PUTFRIENDSHIPS_DESTROY), &friendships_destroy);
	strcpy(friendships_destroy.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(friendships_destroy.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	friendships_destroy.wbuid_.uidtype_ = t_wb_uid::IDType_userid;
	if (wbId) strcpy(friendships_destroy.wbuid_.uid_, wbId);
	friendships_destroy.http_code_ = 0;
	memset(friendships_destroy.error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(PUTFRIENDSHIPS_DESTROY), &friendships_destroy);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &friendships_destroy);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	return OutputMessage(hWnd, friendships_destroy.http_code_, friendships_destroy.error_);
}

bool CM8WeiboApp::GetFriendshipsExists(HWND hWnd, char *wbIdSrc, char *wbIdDest, M8::m8_wb_get_friendships_show *p_friendships_exists)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	Wb_init_wb_struct(WEIBO_OPTION(GETFRIENDSHIPS_EXISTS), p_friendships_exists);
	strcpy(p_friendships_exists->wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_friendships_exists->wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	p_friendships_exists->wbuid_source_.uidtype_ = t_wb_uid::IDType_userid;
	if (wbIdSrc) strcpy(p_friendships_exists->wbuid_source_.uid_, wbIdSrc);
	p_friendships_exists->wbuid_target_.uidtype_ = t_wb_uid::IDType_userid;
	if (wbIdDest) strcpy(p_friendships_exists->wbuid_target_.uid_, wbIdDest);
	p_friendships_exists->http_code_ = 0;
	memset(p_friendships_exists->error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETFRIENDSHIPS_EXISTS), p_friendships_exists);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_friendships_exists);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	return OutputMessage(hWnd, p_friendships_exists->http_code_, p_friendships_exists->error_);
}

bool CM8WeiboApp::PutStatusesDestroy(HWND hWnd, char *wbId)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_put_statuses_destroy statuses_destroy;
	Wb_init_wb_struct(WEIBO_OPTION(PUTSTATUSES_DESTROY), &statuses_destroy);
	strcpy(statuses_destroy.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(statuses_destroy.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	if(wbId) strcpy(statuses_destroy.wbId_, wbId);
	statuses_destroy.http_code_ = 0;
	memset(statuses_destroy.error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(PUTSTATUSES_DESTROY), &statuses_destroy);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &statuses_destroy);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	return OutputMessage(hWnd, statuses_destroy.http_code_, statuses_destroy.error_);
}

bool CM8WeiboApp::PutFavoritesCreate(HWND hWnd, char *wbId)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_put_favorites_create favorites_create;
	Wb_init_wb_struct(WEIBO_OPTION(PUTFAVORITES_CREATE), &favorites_create);
	strcpy(favorites_create.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(favorites_create.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	if(wbId) strcpy(favorites_create.wbId_, wbId);
	favorites_create.http_code_ = 0;
	memset(favorites_create.error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(PUTFAVORITES_CREATE), &favorites_create);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &favorites_create);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	return OutputMessage(hWnd, favorites_create.http_code_, favorites_create.error_);
}

bool CM8WeiboApp::PutFavoritesDestroy(HWND hWnd, char *wbId)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_put_favorites_destroy favorites_destroy;
	Wb_init_wb_struct(WEIBO_OPTION(PUTFAVORITES_DESTROY), &favorites_destroy);
	strcpy(favorites_destroy.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(favorites_destroy.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	if(wbId) strcpy(favorites_destroy.wbId_, wbId);
	favorites_destroy.http_code_ = 0;
	memset(favorites_destroy.error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(PUTFAVORITES_DESTROY), &favorites_destroy);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &favorites_destroy);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	return OutputMessage(hWnd, favorites_destroy.http_code_, favorites_destroy.error_);
}

bool CM8WeiboApp::PutStatusesCommentDestroy(HWND hWnd, char *wbId, char *wbCId)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_put_statuses_comment_destroy statuses_comment_destroy;
	Wb_init_wb_struct(WEIBO_OPTION(PUTSTATUSES_COMMENT_DESTROY), &statuses_comment_destroy);
	strcpy(statuses_comment_destroy.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(statuses_comment_destroy.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	if(wbId) strcpy(statuses_comment_destroy.wbId_, wbId);
	if(wbCId) strcpy(statuses_comment_destroy.wbCId_, wbCId);
	statuses_comment_destroy.http_code_ = 0;
 	memset(statuses_comment_destroy.error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(PUTSTATUSES_COMMENT_DESTROY), &statuses_comment_destroy);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &statuses_comment_destroy);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	return OutputMessage(hWnd, statuses_comment_destroy.http_code_, statuses_comment_destroy.error_);
}

bool CM8WeiboApp::GetUserSearch(HWND hWnd, char *name, int page, M8::m8_wb_users_search *&p_users_search)
{
	p_users_search = NULL;
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_users_search* p_tmp_users_search = new M8::m8_wb_users_search();
	Wb_init_wb_struct(WEIBO_OPTION(GET_USERS_SEARCH), p_tmp_users_search);
	strcpy(p_tmp_users_search->search_.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_tmp_users_search->search_.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	strcpy(p_tmp_users_search->search_.q_, name);
	strcpy(p_tmp_users_search->search_.snick_, "1");
	p_tmp_users_search->search_.count_ = 20;
	p_tmp_users_search->search_.sort_ = 2;
	p_tmp_users_search->search_.page_ = page;
	p_tmp_users_search->prev_cursor_ = page;
	p_tmp_users_search->next_cursor_ = page+1;
	p_tmp_users_search->http_code_ = 0;
	memset(p_tmp_users_search->error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GET_USERS_SEARCH), p_tmp_users_search);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_tmp_users_search);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	bool is_ok = OutputMessage(hWnd, p_tmp_users_search->http_code_, p_tmp_users_search->error_);
	if (is_ok)
	{
		p_users_search = p_tmp_users_search;
	} 
	else
	{
		delete p_tmp_users_search;
		p_tmp_users_search = NULL;
	}
	return is_ok;
}

bool CM8WeiboApp::GetWeiboSearch(HWND hWnd, char *tag, int page, M8::m8_wb_search *&p_wb_search)
{
	p_wb_search = NULL;
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_search* p_tmp_wb_search = new M8::m8_wb_search();
	Wb_init_wb_struct(WEIBO_OPTION(GET_WB_SEARCH), p_tmp_wb_search);
	strcpy(p_tmp_wb_search->search_.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_tmp_wb_search->search_.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	strcpy(p_tmp_wb_search->search_.q_, tag);
	p_tmp_wb_search->search_.count_ = 20;
	p_tmp_wb_search->search_.page_ = page;
	p_tmp_wb_search->page_ = page;
	p_tmp_wb_search->http_code_ = 0;
	memset(p_tmp_wb_search->error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GET_WB_SEARCH), p_tmp_wb_search);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_tmp_wb_search);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);

	bool is_ok = OutputMessage(hWnd, p_tmp_wb_search->http_code_, p_tmp_wb_search->error_);
	if (is_ok)
	{
		M8::m8_wb_get_statuses_counts statuses_counts;
		Wb_init_wb_struct(WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), &statuses_counts);
		strcpy(statuses_counts.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
		strcpy(statuses_counts.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
		for (int i = 0; i < p_tmp_wb_search->size_of_status_; i++)
		{
			strcpy(statuses_counts.wbIds_[i], p_tmp_wb_search->statuses_[i].id_);
		}
		wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), &statuses_counts);
		wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &statuses_counts);
		wb_interface.pfun_request_start(wbRequest, false);
		wb_interface.pfun_request_stop(wbRequest);

		bool is_ok2;
		if (p_tmp_wb_search->size_of_status_ == 0)
		{
			is_ok2 = true;
		}
		else
		{
			is_ok2 = OutputMessage(hWnd, statuses_counts.http_code_, statuses_counts.error_);
		}

		if (is_ok2)
		{
			p_wb_search = p_tmp_wb_search;
			for (int i = 0; i < p_tmp_wb_search->size_of_status_; i++)
			{
				p_tmp_wb_search->statuses_[i].m_comments_ = statuses_counts.comments_[i];
				p_tmp_wb_search->statuses_[i].m_links_ = statuses_counts.rt_[i];
			}
		}
		else
		{
			delete p_tmp_wb_search;
			p_tmp_wb_search = NULL;
		}

		MzEndWaitDlg();
		return is_ok && is_ok2;
	}
	else
	{
		delete p_tmp_wb_search;
		p_tmp_wb_search = NULL;
		MzEndWaitDlg();
		return is_ok;
	}
}

bool CM8WeiboApp::GetFriendsList(HWND hWnd, char *uid, int cursor, 
								 M8::m8_wb_get_statuses_friends *& p_friends_list)
{
	p_friends_list = NULL;
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_get_statuses_friends* p_tmp_friends_list = new M8::m8_wb_get_statuses_friends();
	Wb_init_wb_struct(WEIBO_OPTION(GETFRINDS_LIST), p_tmp_friends_list);
	strcpy(p_tmp_friends_list->wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_tmp_friends_list->wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	if (uid) p_tmp_friends_list->wbuid_.uidtype_ = t_wb_uid::IDType_userid;
	if (uid) strcpy(p_tmp_friends_list->wbuid_.uid_, uid);
	sprintf(p_tmp_friends_list->wbopt_.szOpt_[t_wb_option_params::WBOPTType_cursor], "%d", cursor);
	p_tmp_friends_list->http_code_ = 0;
	memset(p_tmp_friends_list->error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETFRINDS_LIST), p_tmp_friends_list);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_tmp_friends_list);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	bool is_ok = OutputMessage(hWnd, p_tmp_friends_list->http_code_, p_tmp_friends_list->error_);
	if (is_ok)
	{
		p_friends_list = p_tmp_friends_list;
	} 
	else
	{
		delete p_tmp_friends_list;
		p_tmp_friends_list = NULL;
	}
	return is_ok;
}

bool CM8WeiboApp::GetFollowersList(HWND hWnd, char *uid, int cursor, M8::m8_wb_get_statuses_followers *& p_followers_list)
{
	p_followers_list = NULL;
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_get_statuses_followers* p_tmp_followers_list = new M8::m8_wb_get_statuses_followers();
	Wb_init_wb_struct(WEIBO_OPTION(GETFOLLOWERS_LIST), p_tmp_followers_list);
	strcpy(p_tmp_followers_list->wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(p_tmp_followers_list->wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	if (uid) p_tmp_followers_list->wbuid_.uidtype_ = t_wb_uid::IDType_userid;
	if (uid) strcpy(p_tmp_followers_list->wbuid_.uid_, uid);
	sprintf(p_tmp_followers_list->wbopt_.szOpt_[t_wb_option_params::WBOPTType_cursor], "%d", cursor);
	p_tmp_followers_list->http_code_ = 0;
	memset(p_tmp_followers_list->error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(GETFOLLOWERS_LIST), p_tmp_followers_list);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), p_tmp_followers_list);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	bool is_ok = OutputMessage(hWnd, p_tmp_followers_list->http_code_, p_tmp_followers_list->error_);
	if (is_ok)
	{
		p_followers_list = p_tmp_followers_list;
	} 
	else
	{
		delete p_tmp_followers_list;
		p_tmp_followers_list = NULL;
	}
	return is_ok;
}

bool CM8WeiboApp::PutFollowersDestroy(HWND hWnd, char *uid)
{
	MzBeginWaitDlg(hWnd, NULL, true);
	M8::m8_wb_put_followers_destroy followers_destroy;
	Wb_init_wb_struct(WEIBO_OPTION(PUTFOLLOWERS_DESTROY), &followers_destroy);
	strcpy(followers_destroy.wbauth_.oauth_token_, m_oauth_access_token->wbauth_.oauth_token_);
	strcpy(followers_destroy.wbauth_.oauth_token_secret_, m_oauth_access_token->wbauth_.oauth_token_secret_);
	followers_destroy.wbuid_.uidtype_ = t_wb_uid::IDType_userid;
	if (uid) strcpy(followers_destroy.wbuid_.uid_, uid);
	followers_destroy.http_code_ = 0;
	memset(followers_destroy.error_, 0, WB_REAL_LEN(LENGTH8));
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_SENDDATA), WEIBO_OPTION(PUTFOLLOWERS_DESTROY), &followers_destroy);
	wb_interface.pfun_request_setopt(wbRequest, WEIBO_OPTION(REQ_USERDATA), &followers_destroy);
	wb_interface.pfun_request_start(wbRequest, false);
	wb_interface.pfun_request_stop(wbRequest);
	MzEndWaitDlg();

	return OutputMessage(hWnd, followers_destroy.http_code_, followers_destroy.error_);
}

bool CM8WeiboApp::OutputMessage(HWND hWnd, unsigned long http_code, char *error)
{
	if (http_code == 0)
	{
		MzMessageBox(hWnd, L"你的网络好像有点问题，请重新\n试一下吧！", NULL, MB_OK);
		m_net_env.redial(hWnd);
	}
	else if (http_code == 200 && *error == 0)
	{
		return true;
	}
	else
	{
		char buffer[256];
		sprintf(buffer, "http返回错误代码：%d", http_code);
		if (*error)
		{
			char *ptr = error;
			int size = strlen(error);
			for(int i = 0; i < size; i++)
			{
				if (*ptr == ':')
				{
					*ptr = 0;
					break;
				}
				ptr++;
			}
			strcat(buffer, "\n微博返回错误代码：");
			strcat(buffer, error);
		}
		MzMessageBox(hWnd, CStringA2W(buffer).ptr(), NULL, MB_OK);
	}
	return false;
}

int CM8WeiboApp::Done()
{
	return CMzApp::Done();
}

bool CM8WeiboApp::IsValid()
{
	return m_valid;
}

BOOL CM8WeiboApp::Exists()
{
	HANDLE hMutex = CreateMutex(NULL, TRUE, _T("M8WeiboMutex"));
	BOOL bFound = (GetLastError() == ERROR_ALREADY_EXISTS)? TRUE: FALSE;
	if (hMutex) ReleaseMutex(hMutex);
	if (!bFound) return FALSE;

	HWND hWnd = FindWindow(CM8WeiboLoginWnd::m_pwszCM8WeiboLoginWndClassName, NULL);
	if (hWnd)
	{
		if(!IsWindowVisible(hWnd)) ShowWindow(hWnd, SW_SHOW);
		SetForegroundWindow(hWnd);
	}
	else
	{
		hWnd = FindWindow(CM8WeiboHomeWnd::m_pwszCM8WeiboHomeWndClassName, NULL);
		if (hWnd)
		{
			if(!IsWindowVisible(hWnd)) ShowWindow(hWnd, SW_SHOW);
			COPYDATASTRUCT cpd = {0, 0, NULL};
			::SendMessage(hWnd, WM_COPYDATA, -1, (LPARAM)&cpd);
		}
	}
	return TRUE;
}

bool CM8WeiboApp::StoreVerify()
{
	MYSTORE_VERIFY_CONTEXT mystore;
	memset(&mystore, 0, sizeof(MYSTORE_VERIFY_CONTEXT));
	CMzString module_name = GetModuleFileName(NULL);
	/*DWORD dwRet = MyStoreVerify(module_name.C_Str(), &mystore);
	wchar_t buffer[64];
	wsprintf(buffer, L"Lisence返回结果：%d", dwRet);
	MzMessageBox(NULL, buffer, NULL, MB_OK);
	wsprintf(buffer, L"Lisence返回合法性：%d", mystore.LicenseValid);
	MzMessageBox(NULL, buffer, NULL, MB_OK);
	wsprintf(buffer, L"Lisence返回保留字段0：%02X", mystore.Reserved[0]);
	MzMessageBox(NULL, buffer, NULL, MB_OK);*/
	return (MyStoreVerify(module_name.C_Str(), &mystore) == 0);
}

// Global App Variable
CM8WeiboApp theApp;
