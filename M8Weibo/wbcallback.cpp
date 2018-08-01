/**
*  @brief weibo call back
*  @file  wbcallback.cpp
*  @author loach 
*  @Email < loachmr@sina.com >
*
* Copyright (C) 1996-2010 SINA Corporation, All Rights Reserved
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/

#include "wbParser/splitstr.h"
#include "wbParser/wbParser.h"
#include "wbcallback.h"
#include <stdio.h>
#include <stdlib.h>
#include "M8Struct.h"

void Parse_status_JSON( wbParserNS::REQOBJ* pRoot, M8::m8_status * p_status );
void Parse_user_JSON( wbParserNS::REQOBJ* pRoot, M8::m8_user * p_user );
void Parse_friendships_JSON( wbParserNS::REQOBJ* pRoot, M8::m8_friendships * p_friendships );
void Parse_comment_JSON( wbParserNS::REQOBJ* pRoot, M8::m8_comment* p_comment );

////////////////////////////////////////////////////////////////////////////////////////////////////

// for declare function
#define WEIBO_struct_statuscallback_init(name) \
	static void Wb_struct_statuscallback_init_##name##_cb(int state , int errCode , void* pUserdata)

#define WEIBO_struct_statuscallback_init_fun(name) Wb_struct_statuscallback_init_##name##_cb

//
#define WEIBO_struct_bodycallback_init(name) \
	static void Wb_struct_bodycallback_init_##name##_cb(unsigned long httpCode , const char* body , unsigned long len , void* pUserdata)

#define WEIBO_struct_bodycallback_init_fun(name) Wb_struct_bodycallback_init_##name##_cb

//
#define WEIBO_struct_headerycallback_init(name) \
	static void Wb_struct_headercallback_init_##name##_cb(unsigned long httpCode , const char* header , unsigned long len , void* pUserdata)

#define WEIBO_struct_headercallback_init_fun(name) Wb_struct_headercallback_init_##name##_cb

#include "wbParser/wbParseWrapper.h"

#ifdef _MSC_VER
#	if defined(WEIBO_VC60)
#		pragma comment(lib,"WBParseWrapper_VC60.lib")
#	elif defined(WEIBO_VC70)
#		pragma comment(lib,"WBParseWrapper_VC70.lib")
#	elif defined(WEIBO_VC80)
#		pragma comment(lib,"WBParseWrapper_VC80.lib")
#	elif defined(WEIBO_VC90)
#		pragma comment(lib,"WBParseWrapper_VC90.lib")
#	endif //
#endif //_MSC_VER

//
WEIBO_struct_statuscallback_init(BASE)
{
}

WEIBO_struct_bodycallback_init(BASE)
{
	printf("    BASE : \n    %s \n\n" , body );
}

//------------------------------------获取下行数据集(timeline)接口---------------------------------------------//
//

// 2.获取最新更新的公共微博消息
WEIBO_struct_statuscallback_init(GETSTATUSES_PUBLIC_TIMELINE)
{

}

WEIBO_struct_bodycallback_init(GETSTATUSES_PUBLIC_TIMELINE)
{
	printf("    GETSTATUSES_PUBLIC_TIMELINE : \n    %s \n\n" , body );

}

// 3.获取当前用户所关注用户的最新微博信息
WEIBO_struct_statuscallback_init(GETSTATUSES_FRIENDS_TIMELINE)
{
}
WEIBO_struct_bodycallback_init(GETSTATUSES_FRIENDS_TIMELINE)
{
	/*char buffer[128];
	memcpy(buffer, body, 77);
	buffer[77]  = 0;
	printf("    GETSTATUSES_FRIENDS_TIMELINE : \n    %s \n\n" , buffer );*/

	M8::m8_wb_get_statuses_friends_timeline* p_statuses_friends_timeline = (M8::m8_wb_get_statuses_friends_timeline*)pUserdata;
	/*if (p_statuses_friends_timeline) p_statuses_friends_timeline->http_code_ = httpCode;
	if(p_statuses_friends_timeline  && httpCode == 200 )
	{
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if( !pRoot ){
			return ;
		}
		wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_friends_timeline->error_, WB_USE_LEN(LENGTH8));
		int size = wbParserNS::GetObject_JSON_SIZE(pRoot);
		p_statuses_friends_timeline->size_of_status_ = size;
		p_statuses_friends_timeline->statuses_ = new M8::m8_status[size];
		//memset(p_statuses_friends_timeline->statuses_, 0, sizeof(M8::m8_status)*size);
		for (int i = 0; i < size; i++)
		{
			M8::m8_status *p_status = 	p_statuses_friends_timeline->statuses_+i;
			wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Idx_JSON(i, pRoot);
			Parse_status_JSON(pChild, p_status);
		}
		wbParserNS::Parse_free_JSON(pRoot);
	}
	else
	{
		printf("    GETSTATUSES_FRIENDS_TIMELINE : \n    %s \n\n" , buffer );
	}*/

	if (p_statuses_friends_timeline)
	{
		p_statuses_friends_timeline->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_friends_timeline->error_, WB_USE_LEN(LENGTH8));
			if (*(p_statuses_friends_timeline->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			int size = wbParserNS::GetObject_JSON_SIZE(pRoot);
			p_statuses_friends_timeline->size_of_status_ = size;
			p_statuses_friends_timeline->statuses_ = new M8::m8_status[size];
			//memset(p_statuses_friends_timeline->statuses_, 0, sizeof(M8::m8_status)*size);
			for (int i = 0; i < size; i++)
			{
				M8::m8_status *p_status = p_statuses_friends_timeline->statuses_+i;
				wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Idx_JSON(i, pRoot);
				Parse_status_JSON(pChild, p_status);
			}
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    GETSTATUSES_FRIENDS_TIMELINE : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    GETSTATUSES_FRIENDS_TIMELINE : \n    %s \n\n" , body );
	}
}

// 4.获取用户发布的微博信息列表
WEIBO_struct_statuscallback_init(GETSTATUSES_USE_TIMELINE)
{

}
WEIBO_struct_bodycallback_init(GETSTATUSES_USE_TIMELINE)
{
	M8::m8_wb_get_statuses_user_timeline* p_statuses_user_timeline = (M8::m8_wb_get_statuses_user_timeline*)pUserdata;
	if (p_statuses_user_timeline)
	{
		p_statuses_user_timeline->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_user_timeline->error_, WB_USE_LEN(LENGTH8));
			if (*(p_statuses_user_timeline->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			int size = wbParserNS::GetObject_JSON_SIZE(pRoot);
			p_statuses_user_timeline->size_of_status_ = size;
			p_statuses_user_timeline->statuses_ = new M8::m8_status[size];
			//memset(p_statuses_user_timeline->statuses_, 0, sizeof(M8::m8_status)*size);
			for (int i = 0; i < size; i++)
			{
				M8::m8_status *p_status = p_statuses_user_timeline->statuses_+i;
				wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Idx_JSON(i, pRoot);
				Parse_status_JSON(pChild, p_status);
			}
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    GETSTATUSES_USE_TIMELINE : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    GETSTATUSES_USE_TIMELINE : \n    %s \n\n" , body );
	}
}

// 5.获取@当前用户的微博列表
WEIBO_struct_statuscallback_init(GETSTATUSES_MENTIONS)
{

}
WEIBO_struct_bodycallback_init(GETSTATUSES_MENTIONS)
{
	M8::m8_wb_get_statuses_mentions* p_statuses_mentions = (M8::m8_wb_get_statuses_mentions*)pUserdata;
	if (p_statuses_mentions)
	{
		p_statuses_mentions->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_mentions->error_, WB_USE_LEN(LENGTH8));
			if (*(p_statuses_mentions->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			int size = wbParserNS::GetObject_JSON_SIZE(pRoot);
			p_statuses_mentions->size_of_status_ = size;
			p_statuses_mentions->statuses_ = new M8::m8_status[size];
			//memset(p_statuses_mentions->statuses_, 0, sizeof(M8::m8_status)*size);
			for (int i = 0; i < size; i++)
			{
				M8::m8_status *p_status = p_statuses_mentions->statuses_+i;
				wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Idx_JSON(i, pRoot);
				Parse_status_JSON(pChild, p_status);
			}
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    GETSTATUSES_MENTIONS : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    GETSTATUSES_MENTIONS : \n    %s \n\n" , body );
	}
}


// 6.获取当前用户发送及收到的评论列表
WEIBO_struct_statuscallback_init(GETSTATUSES_COMMENTS_TIMELINE)
{
}

WEIBO_struct_bodycallback_init(GETSTATUSES_COMMENTS_TIMELINE)
{
	printf("    GETSTATUSES_COMMENTS_TIMELINE : \n    %s \n\n" , body );

}

// 7.获取当前用户发出的评论
WEIBO_struct_statuscallback_init(GETSTATUSES_COMMENTS_BYME)
{

}
WEIBO_struct_bodycallback_init(GETSTATUSES_COMMENTS_BYME)
{
	printf("    GETSTATUSES_COMMENTS_BYME : \n    %s \n\n" , body );
}

// 8 .获取指定微博的评论列表
WEIBO_struct_statuscallback_init(GETSTATUSES_COMMENTS_LIST)
{

}

WEIBO_struct_bodycallback_init(GETSTATUSES_COMMENTS_LIST)
{
	M8::m8_wb_get_statuses_comments_list* p_statuses_comments_list = (M8::m8_wb_get_statuses_comments_list*)pUserdata;
	if (p_statuses_comments_list)
	{
		p_statuses_comments_list->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_comments_list->error_, WB_USE_LEN(LENGTH8));
			if (*(p_statuses_comments_list->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			int size = wbParserNS::GetObject_JSON_SIZE(pRoot);
			p_statuses_comments_list->size_of_comment_ = size;
			p_statuses_comments_list->comments_ = new M8::m8_comment[size];
			//memset(p_statuses_comments_list->comments_, 0, sizeof(M8::m8_status)*size);
			for (int i = 0; i < size; i++)
			{
				M8::m8_comment *p_comment = p_statuses_comments_list->comments_+i;
				wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Idx_JSON(i, pRoot);
				Parse_comment_JSON(pChild, p_comment);
			}
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    GETSTATUSES_COMMENTS_LIST : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    GETSTATUSES_COMMENTS_LIST : \n    %s \n\n" , body );
	}
}

//  9 .批量获取一组微博的评论数及转发数
WEIBO_struct_statuscallback_init(GETSTATUSES_COMMENTS_COUNTS)
{

}

WEIBO_struct_bodycallback_init(GETSTATUSES_COMMENTS_COUNTS)
{
	M8::m8_wb_get_statuses_counts* p_statuses_counts = (M8::m8_wb_get_statuses_counts*)pUserdata;
	/*if (p_statuses_counts) p_statuses_counts->http_code_ = httpCode;
	if( p_statuses_counts && httpCode == 200 )
	{
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if( !pRoot ){
			return ;
		}
		wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_counts->error_, WB_USE_LEN(LENGTH8));
		int size = wbParserNS::GetObject_JSON_SIZE(pRoot);
		for (int i = 0; i < size; i++)
		{
			wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Idx_JSON(i, pRoot);
			wbParserNS::GetCHAR_Key_JSON("id", pChild, p_statuses_counts->rtnIds_[i], WB_USE_LEN(WEIBO_ID));
			p_statuses_counts->comments_[i] = wbParserNS::GetLONG_Key_JSON("comments", pChild);
			p_statuses_counts->rt_[i] = wbParserNS::GetLONG_Key_JSON("rt", pChild);
		}
		wbParserNS::Parse_free_JSON(pRoot);

		// 分配排序用内存
		WBChar  *p_rtnIds = new WBChar[size * (WB_REAL_LEN(WEIBO_ID))];
		unsigned long *p_comments = new unsigned long [size];
		unsigned long *p_rt = new unsigned long [size];

		// 根据返回id进行重新排序
		char *p_status_id = NULL;
		for (int i = 0; i < size; i++)
		{
			p_status_id = p_statuses_counts->wbIds_[i];
			for (int j = 0; j < size; j++)
			{
				if (strcmp(p_status_id, p_statuses_counts->rtnIds_[j]) == 0)
				{
					strcpy(p_rtnIds+i*(WB_REAL_LEN(WEIBO_ID)), p_status_id);
					p_comments[i] = p_statuses_counts->comments_[j];
					p_rt[i] = p_statuses_counts->rt_[j];
					break;
				}
			}
		}
		memcpy(p_statuses_counts->rtnIds_, p_rtnIds, size * (WB_REAL_LEN(WEIBO_ID)));
		memcpy(p_statuses_counts->comments_, p_comments, size * sizeof(unsigned long));
		memcpy(p_statuses_counts->rt_, p_rt, size * sizeof(unsigned long));

		// 回收排序用内存
		delete[] p_rtnIds;
		delete[] p_comments;
		delete[] p_rt;
	}
	else
	{
		printf("    GETSTATUSES_COMMENTS_COUNTS : \n    %s \n\n" , body );
	}*/

	if (p_statuses_counts)
	{
		p_statuses_counts->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_counts->error_, WB_USE_LEN(LENGTH8));
			if (*(p_statuses_counts->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			int size = wbParserNS::GetObject_JSON_SIZE(pRoot);
			for (int i = 0; i < size; i++)
			{
				wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Idx_JSON(i, pRoot);
				wbParserNS::GetCHAR_Key_JSON("id", pChild, p_statuses_counts->rtnIds_[i], WB_USE_LEN(WEIBO_ID));
				p_statuses_counts->comments_[i] = wbParserNS::GetLONG_Key_JSON("comments", pChild);
				p_statuses_counts->rt_[i] = wbParserNS::GetLONG_Key_JSON("rt", pChild);
			}
			wbParserNS::Parse_free_JSON(pRoot);

			// 分配排序用内存
			WBChar  *p_rtnIds = new WBChar[size * (WB_REAL_LEN(WEIBO_ID))];
			unsigned long *p_comments = new unsigned long [size];
			unsigned long *p_rt = new unsigned long [size];

			// 根据返回id进行重新排序
			char *p_status_id = NULL;
			for (int i = 0; i < size; i++)
			{
				p_status_id = p_statuses_counts->wbIds_[i];
				for (int j = 0; j < size; j++)
				{
					if (strcmp(p_status_id, p_statuses_counts->rtnIds_[j]) == 0)
					{
						strcpy(p_rtnIds+i*(WB_REAL_LEN(WEIBO_ID)), p_status_id);
						p_comments[i] = p_statuses_counts->comments_[j];
						p_rt[i] = p_statuses_counts->rt_[j];
						break;
					}
				}
			}
			memcpy(p_statuses_counts->rtnIds_, p_rtnIds, size * (WB_REAL_LEN(WEIBO_ID)));
			memcpy(p_statuses_counts->comments_, p_comments, size * sizeof(unsigned long));
			memcpy(p_statuses_counts->rt_, p_rt, size * sizeof(unsigned long));

			// 回收排序用内存
			delete[] p_rtnIds;
			delete[] p_comments;
			delete[] p_rt;
		}
		else
		{
			printf("    GETSTATUSES_COMMENTS_COUNTS : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    GETSTATUSES_COMMENTS_COUNTS : \n    %s \n\n" , body );
	}
}


// 10.获取当前用户未读消息数
WEIBO_struct_statuscallback_init(GETSTATUSES_UNREAD)
{

}
WEIBO_struct_bodycallback_init(GETSTATUSES_UNREAD)
{
	printf("    GETSTATUSES_UNREAD : \n    %s \n\n" , body );

}

//---------------------------------------微博访问接口--------------------------------------------------//

// 11 根据ID获取单条微博信息内容
WEIBO_struct_statuscallback_init(GETSTATUSES_SHOW)
{
}
WEIBO_struct_bodycallback_init(GETSTATUSES_SHOW)
{
	printf("    GETSTATUSES_SHOW : \n    %s \n\n" , body );

}

// 12 根据微博ID和用户ID跳转到单条微博页面
WEIBO_struct_statuscallback_init(GOTOSTATUSES_ID)
{
}
WEIBO_struct_bodycallback_init(GOTOSTATUSES_ID)
{
	// 跳转页面
	printf("    GOTOSTATUSES_ID : \n    %s \n\n" , body );
}

// 13 发布一条微博信息
WEIBO_struct_statuscallback_init(PUTSTATUSES_UPDATE)
{
}
WEIBO_struct_bodycallback_init(PUTSTATUSES_UPDATE)
{
	M8::m8_wb_put_statuses_update* p_statuses_update = (M8::m8_wb_put_statuses_update*)pUserdata;
	/*if (p_statuses_update) p_statuses_update->http_code_ = httpCode;
	wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
	if (p_statuses_update && pRoot) wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_update->error_, WB_USE_LEN(LENGTH8));
	if( p_statuses_update && httpCode == 200 )
	{
		Parse_status_JSON(pRoot, &p_statuses_update->status_);
	}
	else
	{
		printf("    PUTSTATUSES_UPDATE : \n    %s \n\n" , body );
	}
	wbParserNS::Parse_free_JSON(pRoot);*/

	if (p_statuses_update)
	{
		p_statuses_update->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_update->error_, WB_USE_LEN(LENGTH8));
			if (*(p_statuses_update->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			Parse_status_JSON(pRoot, &p_statuses_update->status_);
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    PUTSTATUSES_UPDATE : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    PUTSTATUSES_UPDATE : \n    %s \n\n" , body );
	}
}


// 14 上传图片并发布一条微博信息
WEIBO_struct_statuscallback_init(PUTSTATUSES_UPLOAD)
{

}
WEIBO_struct_bodycallback_init(PUTSTATUSES_UPLOAD)
{
	M8::m8_wb_put_statuses_upload* p_statuses_upload = (M8::m8_wb_put_statuses_upload*)pUserdata;
	if (p_statuses_upload)
	{
		p_statuses_upload->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_upload->error_, WB_USE_LEN(LENGTH8));
			if (*(p_statuses_upload->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			Parse_status_JSON(pRoot, &p_statuses_upload->status_);
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    PUTSTATUSES_UPLOAD : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    PUTSTATUSES_UPLOAD : \n    %s \n\n" , body );
	}
}

// 15 删除一条微博信息
WEIBO_struct_statuscallback_init(PUTSTATUSES_DESTROY)
{

}
WEIBO_struct_bodycallback_init(PUTSTATUSES_DESTROY)
{
	M8::m8_wb_put_statuses_destroy* p_statuses_destroy = (M8::m8_wb_put_statuses_destroy*)pUserdata;
	if (p_statuses_destroy)
	{
		p_statuses_destroy->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_destroy->error_, WB_USE_LEN(LENGTH8));
			// 处理逻辑
			Parse_status_JSON(pRoot, &p_statuses_destroy->status_);
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    PUTSTATUSES_DESTROY : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    PUTSTATUSES_DESTROY : \n    %s \n\n" , body );
	}
}

// 16 转发一条微博信息（可加评论）
WEIBO_struct_statuscallback_init(PUTSTATUSES_REPOST)
{

}

WEIBO_struct_bodycallback_init(PUTSTATUSES_REPOST)
{
	M8::m8_wb_put_statuses_repost* p_statuses_repost = (M8::m8_wb_put_statuses_repost*)pUserdata;
	if (p_statuses_repost)
	{
		p_statuses_repost->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_repost->error_, WB_USE_LEN(LENGTH8));
			if (*(p_statuses_repost->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			Parse_status_JSON(pRoot, &p_statuses_repost->status_);
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    PUTSTATUSES_REPOST : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    PUTSTATUSES_REPOST : \n    %s \n\n" , body );
	}
}



// 17 对一条微博信息进行评论
WEIBO_struct_statuscallback_init(PUTSTATUSES_COMMENT)
{

}
WEIBO_struct_bodycallback_init(PUTSTATUSES_COMMENT)
{
	M8::m8_wb_put_statuses_comment* p_statuses_comment = (M8::m8_wb_put_statuses_comment*)pUserdata;
	if (p_statuses_comment)
	{
		p_statuses_comment->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_comment->error_, WB_USE_LEN(LENGTH8));
			if (*(p_statuses_comment->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			Parse_status_JSON(pRoot, &p_statuses_comment->status_);
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    PUTSTATUSES_COMMENT : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    PUTSTATUSES_COMMENT : \n    %s \n\n" , body );
	}
}

// 18 删除当前用户的微博评论信息
WEIBO_struct_statuscallback_init(PUTSTATUSES_COMMENT_DESTROY)
{

}
WEIBO_struct_bodycallback_init(PUTSTATUSES_COMMENT_DESTROY)
{
	M8::m8_wb_put_statuses_comment_destroy* p_statuses_comment_destroy = (M8::m8_wb_put_statuses_comment_destroy*)pUserdata;
	if (p_statuses_comment_destroy)
	{
		p_statuses_comment_destroy->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_comment_destroy->error_, WB_USE_LEN(LENGTH8));
			// 处理逻辑
			Parse_comment_JSON(pRoot, &p_statuses_comment_destroy->comment_);
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    PUTSTATUSES_COMMENT_DESTROY : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    PUTSTATUSES_COMMENT_DESTROY : \n    %s \n\n" , body );
	}
}


// 19 回复微博评论信息
WEIBO_struct_statuscallback_init(PUTSTATUSES_REPLY)
{

}
WEIBO_struct_bodycallback_init(PUTSTATUSES_REPLY)
{
	printf("    PUTSTATUSES_REPLY : \n    %s \n\n" , body );

}

//---------------------------------------用户接口--------------------------------------------------//



// 20 根据用户ID获取用户资料（授权用户）
WEIBO_struct_statuscallback_init(GETUSER_INFO)
{

}
WEIBO_struct_bodycallback_init(GETUSER_INFO)
{
	M8::m8_wb_get_users_show* p_users_show = (M8::m8_wb_get_users_show*)pUserdata;
	/*if (p_users_show) p_users_show->http_code_ = httpCode;
	if( p_users_show && httpCode == 200 )
	{
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if( !pRoot ){
			return ;
		}
		wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_users_show->error_, WB_USE_LEN(LENGTH8));
		Parse_user_JSON(pRoot, &p_users_show->user_);
		wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Key_JSON("status", pRoot);
		Parse_status_JSON(pChild, &p_users_show->status_);
		wbParserNS::Parse_free_JSON(pRoot);
	}
	else
	{
		printf("    GETUSER_INFO : \n    %s \n\n" , body );
	}*/

	if (p_users_show)
	{
		p_users_show->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_users_show->error_, WB_USE_LEN(LENGTH8));
			if (*(p_users_show->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			Parse_user_JSON(pRoot, &p_users_show->user_);
			wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Key_JSON("status", pRoot);
			Parse_status_JSON(pChild, &p_users_show->status_);
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    GETUSER_INFO : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    GETUSER_INFO : \n    %s \n\n" , body );
	}
}


//  21 获取当前用户关注对象列表及最新一条微博信息
WEIBO_struct_statuscallback_init(GETFRINDS_LIST)
{

}
WEIBO_struct_bodycallback_init(GETFRINDS_LIST)
{
	M8::m8_wb_get_statuses_friends* p_friends_list = (M8::m8_wb_get_statuses_friends*)pUserdata;
	if (p_friends_list)
	{
		p_friends_list->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_friends_list->error_, WB_USE_LEN(LENGTH8));
			if (*(p_friends_list->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			p_friends_list->prev_cursor_ = wbParserNS::GetLONG_Key_JSON("prev_cursor", pRoot);
			p_friends_list->next_cursor_ = wbParserNS::GetLONG_Key_JSON("next_cursor", pRoot);
			wbParserNS::REQOBJ* pFriends = wbParserNS::GetObject_Key_JSON("users", pRoot);
			int size = wbParserNS::GetObject_JSON_SIZE(pFriends);
			p_friends_list->size_of_friend_ = size;
			p_friends_list->friends_ = new M8::m8_user_and_status[size];
			//memset(p_friends_list->friends_, 0, sizeof(M8::m8_status)*size);
			for (int i = 0; i < size; i++)
			{
				M8::m8_user_and_status *p_friend = p_friends_list->friends_+i;
				wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Idx_JSON(i, pFriends);
				Parse_user_JSON(pChild, &p_friend->user_);
				pChild = wbParserNS::GetObject_Key_JSON("status", pChild);
				Parse_status_JSON(pChild, &p_friend->status_);
			}
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    GETFRINDS_LIST : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    GETFRINDS_LIST : \n    %s \n\n" , body );
	}
}


// 22 获取当前用户粉丝列表及最新一条微博信息
WEIBO_struct_statuscallback_init(GETFOLLOWERS_LIST)
{

}
WEIBO_struct_bodycallback_init(GETFOLLOWERS_LIST)
{
	M8::m8_wb_get_statuses_followers* p_followers_list = (M8::m8_wb_get_statuses_followers*)pUserdata;
	if (p_followers_list)
	{
		p_followers_list->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_followers_list->error_, WB_USE_LEN(LENGTH8));
			if (*(p_followers_list->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			p_followers_list->prev_cursor_ = wbParserNS::GetLONG_Key_JSON("prev_cursor", pRoot);
			p_followers_list->next_cursor_ = wbParserNS::GetLONG_Key_JSON("next_cursor", pRoot);
			wbParserNS::REQOBJ* pFollowers = wbParserNS::GetObject_Key_JSON("users", pRoot);
			int size = wbParserNS::GetObject_JSON_SIZE(pFollowers);
			p_followers_list->size_of_follower_ = size;
			p_followers_list->followers_ = new M8::m8_user_and_status[size];
			//memset(p_followers_list->followers_, 0, sizeof(M8::m8_status)*size);
			for (int i = 0; i < size; i++)
			{
				M8::m8_user_and_status *p_friend = p_followers_list->followers_+i;
				wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Idx_JSON(i, pFollowers);
				Parse_user_JSON(pChild, &p_friend->user_);
				pChild = wbParserNS::GetObject_Key_JSON("status", pChild);
				Parse_status_JSON(pChild, &p_friend->status_);
			}
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    GETFOLLOWERS_LIST : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    GETFOLLOWERS_LIST : \n    %s \n\n" , body );
	}
}


//---------------------------------------私信接口--------------------------------------------------//

// 23 获取当前用户最新私信列表
WEIBO_struct_statuscallback_init(GETDIRECTMSG)
{

}
WEIBO_struct_bodycallback_init(GETDIRECTMSG)
{
	printf("    GETDIRECTMSG : \n    %s \n\n" , body );
}


// 24 获取当前用户发送的最新私信列表
WEIBO_struct_statuscallback_init(GETDIRESTMSG_SENT)
{

}
WEIBO_struct_bodycallback_init(GETDIRESTMSG_SENT)
{
	printf("    GETDIRESTMSG_SENT : \n    %s \n\n" , body );

}


// 25 发送一条私信
WEIBO_struct_statuscallback_init(PUTDIRECTMSG_NEW)
{

}
WEIBO_struct_bodycallback_init(PUTDIRECTMSG_NEW)
{
	printf("    PUTDIRECTMSG_NEW : \n    %s \n\n" , body );
}


//26 删除一条私信
WEIBO_struct_statuscallback_init(PUTDIRECTMSG_DESTROY)
{

}
WEIBO_struct_bodycallback_init(PUTDIRECTMSG_DESTROY)
{
	printf("    PUTDIRECTMSG_DESTROY : \n    %s \n\n" , body );
}

//26 删除一条私信
WEIBO_struct_statuscallback_init(GETDIRECTMSG_WITH)
{

}
WEIBO_struct_bodycallback_init(GETDIRECTMSG_WITH)
{
	printf("    PUTDIRECTMSG_DESTROY : \n    %s \n\n" , body );
}


//---------------------------------------关注接口--------------------------------------------------//

// 27 关注某用户
WEIBO_struct_statuscallback_init(PUTFRIENDSHIPS_CREATE)
{

}
WEIBO_struct_bodycallback_init(PUTFRIENDSHIPS_CREATE)
{
	M8::m8_wb_put_friendships_create* p_friendships_create = (M8::m8_wb_put_friendships_create*)pUserdata;
	if (p_friendships_create)
	{
		p_friendships_create->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_friendships_create->error_, WB_USE_LEN(LENGTH8));
			// 处理逻辑
			Parse_user_JSON(pRoot, &(p_friendships_create->user_));
			wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Key_JSON("status", pRoot);
			Parse_status_JSON(pChild, &(p_friendships_create->status_));
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    PUTFRIENDSHIPS_CREATE : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    PUTFRIENDSHIPS_CREATE : \n    %s \n\n" , body );
	}
}

// 批量关注接口
WEIBO_struct_statuscallback_init(PUTFRIENDSHIPS_CREATE_BATCH)
{

}
WEIBO_struct_bodycallback_init(PUTFRIENDSHIPS_CREATE_BATCH)
{
	printf("    PUTFRIENDSHIPS_CREATE_BATCH : \n    %s \n\n" , body );

}

// 28 取消关注
WEIBO_struct_statuscallback_init(PUTFRIENDSHIPS_DESTROY)
{

}
WEIBO_struct_bodycallback_init(PUTFRIENDSHIPS_DESTROY)
{
	M8::m8_wb_put_friendships_destroy* p_friendships_destroy = (M8::m8_wb_put_friendships_destroy*)pUserdata;
	if (p_friendships_destroy)
	{
		p_friendships_destroy->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_friendships_destroy->error_, WB_USE_LEN(LENGTH8));
			// 处理逻辑
			Parse_user_JSON(pRoot, &(p_friendships_destroy->user_));
			wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Key_JSON("status", pRoot);
			Parse_status_JSON(pChild, &(p_friendships_destroy->status_));
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    PUTFRIENDSHIPS_DESTROY : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    PUTFRIENDSHIPS_DESTROY : \n    %s \n\n" , body );
	}
}

// 29 是否关注某用户
WEIBO_struct_statuscallback_init(GETFRIENDSHIPS_EXISTS)
{

}
WEIBO_struct_bodycallback_init(GETFRIENDSHIPS_EXISTS)
{
	M8::m8_wb_get_friendships_show* p_friendships_exists = (M8::m8_wb_get_friendships_show*)pUserdata;
	if (p_friendships_exists)
	{
		p_friendships_exists->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_friendships_exists->error_, WB_USE_LEN(LENGTH8));
			// 处理逻辑
			wbParserNS::REQOBJ* pSource = wbParserNS::GetObject_Key_JSON("source", pRoot);
			Parse_friendships_JSON(pSource, &(p_friendships_exists->source_));
			wbParserNS::REQOBJ* pTarget = wbParserNS::GetObject_Key_JSON("target", pRoot);
			Parse_friendships_JSON(pTarget, &(p_friendships_exists->target_));
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    GETFRIENDSHIPS_EXISTS : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    GETFRIENDSHIPS_EXISTS : \n    %s \n\n" , body );
	}
}

//
WEIBO_struct_statuscallback_init(GETFRIENDSHIPS_BATCH_EXISTS)
{

}
WEIBO_struct_bodycallback_init(GETFRIENDSHIPS_BATCH_EXISTS)
{
	printf("    GETFRIENDSHIPS_BATCH_EXISTS : \n    %s \n\n" , body );
	// TODO:
}




//
//---------------------------------------Social Graph--------------------------------------------------//

//  30 获取用户关注对象uid列表
WEIBO_struct_statuscallback_init(GETFRIEND_IDS)
{

}
WEIBO_struct_bodycallback_init(GETFRIEND_IDS)
{
	printf("    GETFRIEND_IDS : \n    %s \n\n" , body );

}


// 31 获取用户粉丝对象uid列表
WEIBO_struct_statuscallback_init(GETFOLLOWER_IDS)
{

}
WEIBO_struct_bodycallback_init(GETFOLLOWER_IDS)
{
	printf("    GETFOLLOWER_IDS : \n    %s \n\n" , body );

}

// 删除粉丝
WEIBO_struct_statuscallback_init(PUTFOLLOWERS_DESTROY)
{

}
WEIBO_struct_bodycallback_init(PUTFOLLOWERS_DESTROY)
{
	printf("    PUTFOLLOWERS_DESTROY : \n    %s \n\n" , body );

}

//---------------------------------------账号接口--------------------------------------------------//



//32 验证当前用户身份是否合法
WEIBO_struct_statuscallback_init(GETACCOUNT_VERIFY)
{

}

WEIBO_struct_bodycallback_init(GETACCOUNT_VERIFY)
{
	printf("    GETACCOUNT_VERIFY : \n    %s \n\n" , body );
}

//33 获取当前用户API访问频率限制
WEIBO_struct_statuscallback_init(GETACCOUNT_RATELIMIT)
{

}
WEIBO_struct_bodycallback_init(GETACCOUNT_RATELIMIT)
{
	printf("    GETACCOUNT_RATELIMIT : \n    %s \n\n" , body );

}

//34 当前用户退出登录
WEIBO_struct_statuscallback_init(PUTACCOUNT_QUITSESSION)
{

}
WEIBO_struct_bodycallback_init(PUTACCOUNT_QUITSESSION)
{
	printf("    PUTACCOUNT_QUITSESSION : \n    %s \n\n" , body );

}

// 35 更改头像
WEIBO_struct_statuscallback_init(PUTACCOUNT_UPDATE_PROFILEIMAGE)
{

}
WEIBO_struct_bodycallback_init(PUTACCOUNT_UPDATE_PROFILEIMAGE)
{
	printf("    PUTACCOUNT_UPDATE_PROFILEIMAGE : \n    %s \n\n" , body );

}


// 36 更改资料
WEIBO_struct_statuscallback_init(PUTACCOUNT_UPDATE_PROFILE)
{

}
WEIBO_struct_bodycallback_init(PUTACCOUNT_UPDATE_PROFILE)
{
	printf("    PUTACCOUNT_UPDATE_PROFILE : \n    %s \n\n" , body );

}

//37 注册新浪微博帐号
WEIBO_struct_statuscallback_init(PUTACCOUNT_REGISTER)
{

}
WEIBO_struct_bodycallback_init(PUTACCOUNT_REGISTER)
{
	printf("    PUTACCOUNT_REGISTER : \n    %s \n\n" , body );
	//TODO :
}

//--------------------------------------- 收藏接口--------------------------------------------------//
// 38 获取当前用户的收藏列表 
WEIBO_struct_statuscallback_init(GETFAVORITES)
{

}

WEIBO_struct_bodycallback_init(GETFAVORITES)
{
	M8::m8_wb_get_favorites* p_favorites = (M8::m8_wb_get_favorites*)pUserdata;
	if (p_favorites)
	{
		p_favorites->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_favorites->error_, WB_USE_LEN(LENGTH8));
			if (*(p_favorites->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			int size = wbParserNS::GetObject_JSON_SIZE(pRoot);
			p_favorites->size_of_status_ = size;
			p_favorites->statuses_ = new M8::m8_status[size];
			//memset(p_favorites->statuses_, 0, sizeof(M8::m8_status)*size);
			for (int i = 0; i < size; i++)
			{
				M8::m8_status *p_status = p_favorites->statuses_+i;
				wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Idx_JSON(i, pRoot);
				Parse_status_JSON(pChild, p_status);
			}
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    GETFAVORITES : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    GETFAVORITES : \n    %s \n\n" , body );
	}
}

// 39 添加收藏 
WEIBO_struct_statuscallback_init(PUTFAVORITES_CREATE)
{

}

WEIBO_struct_bodycallback_init(PUTFAVORITES_CREATE)
{
	M8::m8_wb_put_favorites_create* p_favorites_create = (M8::m8_wb_put_favorites_create*)pUserdata;
	if (p_favorites_create)
	{
		p_favorites_create->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_favorites_create->error_, WB_USE_LEN(LENGTH8));
			// 处理逻辑
			Parse_status_JSON(pRoot, &p_favorites_create->status_);
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    PUTFAVORITES_CREATE : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    PUTFAVORITES_CREATE : \n    %s \n\n" , body );
	}
}

// 40 删除当前用户收藏的微博信息 
WEIBO_struct_statuscallback_init(PUTFAVORITES_DESTROY)
{

}

WEIBO_struct_bodycallback_init(PUTFAVORITES_DESTROY)
{
	M8::m8_wb_put_favorites_destroy* p_favorites_destroy = (M8::m8_wb_put_favorites_destroy*)pUserdata;
	if (p_favorites_destroy)
	{
		p_favorites_destroy->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_favorites_destroy->error_, WB_USE_LEN(LENGTH8));
			// 处理逻辑
			Parse_status_JSON(pRoot, &p_favorites_destroy->status_);
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    PUTFAVORITES_DESTROY : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    PUTFAVORITES_DESTROY : \n    %s \n\n" , body );
	}
}


//登录/OAuth

WEIBO_struct_statuscallback_init(OAUTH_REQUEST_TOKEN)
{

}

//#define __USE_TEST_EXPLOER__

#ifdef __USE_TEST_EXPLOER__
#include <Windows.h>
#endif //#ifdef __USE_TEST_EXPLOER__

WEIBO_struct_bodycallback_init(OAUTH_REQUEST_TOKEN)// 获取未授权的Request Token
{
	// 分析把数据写到 t_wb_oauth_access_token
	M8::m8_wb_oauth_request_token* p_oauth_request_token = (M8::m8_wb_oauth_request_token*)pUserdata;
	if(p_oauth_request_token)
	{
		p_oauth_request_token->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_oauth_request_token->error_, WB_USE_LEN(LENGTH8));
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			wb_parse_oauth(body , p_oauth_request_token->wbauth_.oauth_token_ , p_oauth_request_token->wbauth_.oauth_token_secret_,0 );
			printf("    oauth_request_token =  %s \n" , p_oauth_request_token->wbauth_.oauth_token_ );
			printf("    oauth_request_token_secret =  %s \n" , p_oauth_request_token->wbauth_.oauth_token_secret_ );
#ifdef __USE_TEST_EXPLOER__
			//
			char open[255] = {0};
			sprintf_s(open,255,"http://api.t.sina.com.cn/oauth/authorize?oauth_token=%s",p_oauth_request_token->wbauth_.oauth_token_);
			ShellExecuteA( NULL,"open",open,NULL,NULL,SW_SHOWNORMAL );
			//
#endif // __USE_TEST_EXPLOER__
		}
	}
	else
	{
		printf("    OAUTH_REQUEST_TOKEN : \n    %s \n\n" , body );
	}
}

//
// 请求用户授权Token 
WEIBO_struct_statuscallback_init(OAUTH_AUTHORIZE)
{	
}

WEIBO_struct_bodycallback_init(OAUTH_AUTHORIZE)
{
	M8::m8_wb_authorize* p_oauth_authorize = (M8::m8_wb_authorize*)pUserdata;
	/*if( p_oauth_authorize && httpCode == 200 )
	{
		wbParserNS::REQOBJ *pRoot = wbParserNS::Parse_data_JSON(body);
		if( !pRoot ){
			return ;
		}
		wbParserNS::GetCHAR_Key_JSON("oauth_verifier", pRoot , p_oauth_authorize->oauth_verifier_ , 64);
		printf("    oauth_acess_token =  %s \n"          , p_oauth_authorize->oauth_token_);
		printf("    oauth_acess_token_secret =  %s \n"          , p_oauth_authorize->oauth_token_secret_);
		printf("    oauth_acess_token_verifier =  %s \n" , p_oauth_authorize->oauth_verifier_ );
		wbParserNS::Parse_free_JSON(pRoot);
	}
	else
	{
		printf("    OAUTH_AUTHORIZE : \n    %s \n\n" , body );
	}*/

	if (p_oauth_authorize)
	{
		p_oauth_authorize->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_oauth_authorize->error_, WB_USE_LEN(LENGTH8));
			// 处理逻辑
			wbParserNS::GetCHAR_Key_JSON("oauth_verifier", pRoot , p_oauth_authorize->wbauth_.oauth_verifier_ , 64);
			printf("    oauth_acess_token =  %s \n"          , p_oauth_authorize->wbauth_.oauth_token_);
			printf("    oauth_acess_token_secret =  %s \n"          , p_oauth_authorize->wbauth_.oauth_token_secret_);
			printf("    oauth_acess_token_verifier =  %s \n" , p_oauth_authorize->wbauth_.oauth_verifier_ );
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    OAUTH_AUTHORIZE : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    OAUTH_AUTHORIZE : \n    %s \n\n" , body );
	}
}

//
// 获取授权过的Access Token
WEIBO_struct_statuscallback_init(OAUTH_ACCESS_TOKEN)
{

}

WEIBO_struct_bodycallback_init(OAUTH_ACCESS_TOKEN)
{
	// 分析把数据写到 t_wb_oauth,这个是最终结果（oauth_token AND oauth_token_secret)
	// 以后所有其他请求，就需要 使用这个 t_wb_oauth 中的数据了
	M8::m8_wb_oauth_access_token* p_oauth_accesss_token = (M8::m8_wb_oauth_access_token*)pUserdata;
	/*if (p_oauth_accesss_token) p_oauth_accesss_token->http_code_ = httpCode;
	if( p_oauth_accesss_token && httpCode == 200 )
	{
		wbParserNS::REQOBJ *pRoot = wbParserNS::Parse_data_JSON(body);
		if( pRoot ){
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_oauth_accesss_token->error_, WB_USE_LEN(LENGTH8));
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			wb_parse_oauth(body , p_oauth_accesss_token->wbauth_.oauth_token_ , p_oauth_accesss_token->wbauth_.oauth_token_secret_ , p_oauth_accesss_token->uid_);
			printf("    oauth_acess_token =  %s \n" , p_oauth_accesss_token->wbauth_.oauth_token_);
			printf("    oauth_acess_token_secret =  %s \n" , p_oauth_accesss_token->wbauth_.oauth_token_secret_);
			printf("    oauth_acess_token_uid =  %s \n" , p_oauth_accesss_token->uid_);
		}
	}
	else
	{
		printf("    OAUTH_ACCESS_TOKEN : \n    %s \n\n" , body );
	}*/

	if (p_oauth_accesss_token)
	{
		p_oauth_accesss_token->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_oauth_accesss_token->error_, WB_USE_LEN(LENGTH8));
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			wb_parse_oauth(body , p_oauth_accesss_token->wbauth_.oauth_token_ , p_oauth_accesss_token->wbauth_.oauth_token_secret_ , p_oauth_accesss_token->uid_);
			printf("    oauth_acess_token =  %s \n" , p_oauth_accesss_token->wbauth_.oauth_token_);
			printf("    oauth_acess_token_secret =  %s \n" , p_oauth_accesss_token->wbauth_.oauth_token_secret_);
			printf("    oauth_acess_token_uid =  %s \n" , p_oauth_accesss_token->uid_);
		}
	}
	else
	{
		printf("    OAUTH_ACCESS_TOKEN : \n    %s \n\n" , body );
	}
}
// 获取授权过的Access Token
WEIBO_struct_statuscallback_init(XAUTH_ACCESS_TOKEN)
{

}
WEIBO_struct_bodycallback_init(XAUTH_ACCESS_TOKEN)
{
	// 分析把数据写到 t_wb_oauth,这个是最终结果（oauth_token AND oauth_token_secret)
	// 以后所有其他请求，就需要 使用这个 t_wb_oauth 中的数据了
	struct t_wb_oauth* poauth = (struct t_wb_oauth*)pUserdata;
	if( poauth && httpCode == 200 )
	{
		wb_parse_oauth(body , poauth->oauth_token_ , poauth->oauth_token_secret_ ,0);
		printf("    oauth_acess_token =  %s \n" , poauth->oauth_token_ );
		printf("    oauth_acess_token_secret =  %s \n" , poauth->oauth_token_secret_ );
	}
	else
	{
		printf("    XAUTH_ACCESS_TOKEN : \n    %s \n\n" , body );
	}
}

// 表情
WEIBO_struct_statuscallback_init(GET_EMOTIONS)
{

}

WEIBO_struct_bodycallback_init(GET_EMOTIONS)
{
	printf("    GET_EMOTIONS : \n    %s \n\n" , body );
}

// 用户SEARCH
WEIBO_struct_statuscallback_init(GET_USERS_SEARCH)
{

}

WEIBO_struct_bodycallback_init(GET_USERS_SEARCH)
{
	M8::m8_wb_users_search* p_users_search = (M8::m8_wb_users_search*)pUserdata;
	if (p_users_search)
	{
		p_users_search->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_users_search->error_, WB_USE_LEN(LENGTH8));
			if (*(p_users_search->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			int size = wbParserNS::GetObject_JSON_SIZE(pRoot);
			p_users_search->size_of_user_ = size;
			p_users_search->users_ = new M8::m8_user_and_status[size];
			//memset(p_users_search->statuses_, 0, sizeof(M8::m8_status)*size);
			for (int i = 0; i < size; i++)
			{
				M8::m8_user_and_status *p_user = p_users_search->users_+i;
				wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Idx_JSON(i, pRoot);
				Parse_user_JSON(pChild, &p_user->user_);
			}
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    GET_USERS_SEARCH : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    GET_USERS_SEARCH : \n    %s \n\n" , body );
	}
}

// 微博SEARCH
WEIBO_struct_statuscallback_init(GET_WB_SEARCH)
{

}

WEIBO_struct_bodycallback_init(GET_WB_SEARCH)
{
	M8::m8_wb_search* p_search = (M8::m8_wb_search*)pUserdata;
	if (p_search)
	{
		p_search->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_search->error_, WB_USE_LEN(LENGTH8));
			if (*(p_search->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Key_JSON("results", pRoot);
			int size = wbParserNS::GetObject_JSON_SIZE(pChild);
			p_search->size_of_status_ = size;
			p_search->statuses_ = new M8::m8_status[size];
			//memset(p_search->statuses_, 0, sizeof(M8::m8_status)*size);
			for (int i = 0; i < size; i++)
			{
				M8::m8_status *p_status = p_search->statuses_+i;
				p_status->user_ = new M8::m8_user();
				wbParserNS::REQOBJ* pStatus = wbParserNS::GetObject_Idx_JSON(i, pChild);
				
				wbParserNS::GetCHAR_Key_JSON("id", pStatus, p_status->id_, WB_USE_LEN(WEIBO_ID));
				wbParserNS::GetCHAR_Key_JSON("id", pStatus, p_status->mid_, WB_USE_LEN(WEIBO_ID));
				wbParserNS::GetCHAR_Key_JSON("created_at", pStatus, p_status->created_at_, WB_USE_LEN(TIME));
				wbParserNS::GetCHAR_Key_JSON("text", pStatus, p_status->text_, WB_USE_LEN(WB_INFO));
				wbParserNS::GetCHAR_Key_JSON("source", pStatus, p_status->source_, WB_USE_LEN(WB_INFO));

				wbParserNS::GetCHAR_Key_JSON("from_user_id", pStatus, p_status->user_->id_, WB_USE_LEN(WEIBO_ID));
				wbParserNS::GetCHAR_Key_JSON("profile_image_url", pStatus, p_status->user_->profile_image_url_, WB_USE_LEN(URL));
				wbParserNS::GetCHAR_Key_JSON("from_user", pStatus, p_status->user_->screen_name_, WB_USE_LEN(SCREEN_NAME));
				wbParserNS::GetCHAR_Key_JSON("from_user", pStatus,p_status->user_->name_, WB_USE_LEN(PROFILE_NAME));
			}
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    GET_WB_SEARCH : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    GET_WB_SEARCH : \n    %s \n\n" , body );
	}
}

// 
WEIBO_struct_statuscallback_init(GET_STATUSES_SEARCH)
{

}

WEIBO_struct_bodycallback_init(GET_STATUSES_SEARCH)
{
	printf("    GET_STATUSES_SEARCH : \n    %s \n\n" , body );
}

// 省份
WEIBO_struct_statuscallback_init(GET_PROVINCES)
{

}

WEIBO_struct_bodycallback_init(GET_PROVINCES)
{
	printf("    GET_PROVINCES : \n    %s \n\n" , body );
}

//cookie方式
WEIBO_struct_statuscallback_init(COOKIE)
{

}


WEIBO_struct_bodycallback_init(COOKIE)
{
	struct t_wb_oauth* poauth = (struct t_wb_oauth*)pUserdata;

	//
	int iCount = 0;
	wbParserNS::t_wbParse_Cookie cookie;
	wbParserNS::t_wbParse_Cookie *pCookie = &cookie;
	wbParserNS::USE_WBPARSE_FUNC(Cookie,BASE,body,len,iCount,(void**)&pCookie,NULL,NULL);
	
	strcpy(poauth->tgt_,pCookie->tgt);

	printf("    COOKIE : \n    %s \n\n" , body );
}

WEIBO_struct_headerycallback_init(COOKIE)
{
	struct t_wb_oauth* poauth = (struct t_wb_oauth*)pUserdata;
	printf("    COOKIE : \n    %s \n\n" , header );
	if( poauth && httpCode == 200 )
	{
		//wb_parse_cookie(header , poauth->oauth_token_ , poauth->oauth_token_secret_ );
		printf("    SUE =  %s \n" , poauth->oauth_token_ );
		printf("    SUP =  %s \n" , poauth->oauth_token_secret_ );
	}
}

//cookie方式
WEIBO_struct_statuscallback_init(UPDATETGT)
{

}

WEIBO_struct_bodycallback_init(UPDATETGT)
{
	struct t_wb_oauth* poauth = (struct t_wb_oauth*)pUserdata;

	printf("    COOKIE : \n    %s \n\n" , body );
}

WEIBO_struct_headerycallback_init(UPDATETGT)
{
	struct t_wb_oauth* poauth = (struct t_wb_oauth*)pUserdata;
	printf("    COOKIE : \n    %s \n\n" , header );
	if( poauth && httpCode == 200 )
	{
		//wb_parse_cookie(header , poauth->oauth_token_ , poauth->oauth_token_secret_ );
		printf("    SUE =  %s \n" , poauth->oauth_token_ );
		printf("    SUP =  %s \n" , poauth->oauth_token_secret_ );
	}
}

//自定义URL
WEIBO_struct_statuscallback_init(CUSTOM)
{

}

WEIBO_struct_bodycallback_init(CUSTOM)
{

}

WEIBO_struct_statuscallback_init(GETSTATUSES_COMMENTS_TOME)
{
}
WEIBO_struct_bodycallback_init(GETSTATUSES_COMMENTS_TOME)
{
	M8::m8_wb_get_statuses_comments_tome* p_statuses_comments_tome = (M8::m8_wb_get_statuses_comments_tome*)pUserdata;
	if (p_statuses_comments_tome)
	{
		p_statuses_comments_tome->http_code_ = httpCode;
		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(body);
		if (pRoot)
		{
			wbParserNS::GetCHAR_Key_JSON("error", pRoot, p_statuses_comments_tome->error_, WB_USE_LEN(LENGTH8));
			if (*(p_statuses_comments_tome->error_))
			{
				wbParserNS::Parse_free_JSON(pRoot);
				return;
			}
			// 处理逻辑
			int size = wbParserNS::GetObject_JSON_SIZE(pRoot);
			p_statuses_comments_tome->size_of_comment_ = size;
			p_statuses_comments_tome->comments_ = new M8::m8_comment[size];
			//memset(p_statuses_comments_tome->comments_, 0, sizeof(M8::m8_status)*size);
			for (int i = 0; i < size; i++)
			{
				M8::m8_comment *p_comment = p_statuses_comments_tome->comments_+i;
				wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Idx_JSON(i, pRoot);
				Parse_comment_JSON(pChild, p_comment);
			}
			wbParserNS::Parse_free_JSON(pRoot);
		}
		else
		{
			printf("    GETSTATUSES_COMMENTS_BYME : \n    %s \n\n" , body );
		}
	}
	else
	{
		printf("    GETSTATUSES_COMMENTS_BYME : \n    %s \n\n" , body );
	}
}

WEIBO_struct_statuscallback_init(PUTSTATUSES_RESET_COUNT)
{
}
WEIBO_struct_bodycallback_init(PUTSTATUSES_RESET_COUNT)
{
}


// 获取系统推荐用户
WEIBO_struct_statuscallback_init(GET_USERS_HOT)
{

}

WEIBO_struct_bodycallback_init(GET_USERS_HOT)
{
	printf("    GET_USERS_HOT : \n    %s \n\n" , body );
}

//更新修改当前登录用户所关注的某个好友的备注信息New!
WEIBO_struct_statuscallback_init(POST_USERS_REMARK)
{

}

WEIBO_struct_bodycallback_init(POST_USERS_REMARK)
{
	printf("    POST_USERS_REMARK : \n    %s \n\n" , body );
}


//Users/suggestions 返回当前用户可能感兴趣的用户
WEIBO_struct_statuscallback_init(GET_USERS_SUGGESTIONS)
{

}

WEIBO_struct_bodycallback_init(GET_USERS_SUGGESTIONS)
{
	printf("    GET_USERS_SUGGESTIONS : \n    %s \n\n" , body );
}




#include "callback/wbcb_block.inl"
#include "callback/wbcb_hotpoint.inl"
#include "callback/wbcb_tags.inl"
#include "callback/wbcb_trends.inl"
#include "callback/wbcb_invitecontact.inl"

//media 
WEIBO_struct_statuscallback_init(GET_MEDIA_SHORTURL_BATCH)
{

}
WEIBO_struct_bodycallback_init(GET_MEDIA_SHORTURL_BATCH)
{
	printf("    GET_MEDIA_SHORTURL_BATCH : \n    %s \n\n" , body );

	//int iOutCount = 0;
	//wbParserNS::t_wbParse_Media_ShortUrlBatch *pRet = NULL;
	//wbParserNS::WBPARSE_HANDLE handle =  wbParserNS::USE_WBPARSE_FUNC( MEDIA,SHORTURL,body,len,iOutCount,(void**)&pRet,NULL,NULL );
	//if( handle ){
	//	wbParserNS::USE_WBFREE_FUNC( MEDIA,SHORTURL,handle);
	//}
}

//-------------------------------------------------------------------------------------------//

static
struct t_wb_callback_byloach callback_byloach[  WEIBO_OPTION(LAST) ] =
{
#define INIT_CALLBACK_BYLOACH(NAME) {  WEIBO_struct_statuscallback_init_fun(NAME) , WEIBO_struct_bodycallback_init_fun(NAME) , NULL}
#define INIT_CALLBACK_BYWELBON(NAME) { WEIBO_struct_statuscallback_init_fun(NAME) , WEIBO_struct_bodycallback_init_fun(NAME) , NULL}

	{0,0,0},
	INIT_CALLBACK_BYLOACH(BASE),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_PUBLIC_TIMELINE),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_FRIENDS_TIMELINE),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_USE_TIMELINE),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_MENTIONS),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_COMMENTS_TIMELINE),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_COMMENTS_BYME),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_COMMENTS_TOME),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_COMMENTS_LIST),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_COMMENTS_COUNTS),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_UNREAD),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_RESET_COUNT),

	//微博访问
	INIT_CALLBACK_BYLOACH(GETSTATUSES_SHOW),
	INIT_CALLBACK_BYLOACH(GOTOSTATUSES_ID),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_UPDATE),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_UPLOAD),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_DESTROY),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_REPOST),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_COMMENT),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_COMMENT_DESTROY),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_REPLY),

	//用户
	INIT_CALLBACK_BYLOACH(GETUSER_INFO),
	INIT_CALLBACK_BYLOACH(GETFRINDS_LIST),
	INIT_CALLBACK_BYLOACH(GETFOLLOWERS_LIST),

	//私信
	INIT_CALLBACK_BYLOACH(GETDIRECTMSG),
	INIT_CALLBACK_BYLOACH(GETDIRESTMSG_SENT),
	INIT_CALLBACK_BYLOACH(PUTDIRECTMSG_NEW),
	INIT_CALLBACK_BYLOACH(PUTDIRECTMSG_DESTROY),
	INIT_CALLBACK_BYLOACH(GETDIRECTMSG_WITH),

	//关注
	INIT_CALLBACK_BYLOACH(PUTFRIENDSHIPS_CREATE),
	INIT_CALLBACK_BYLOACH(PUTFRIENDSHIPS_CREATE_BATCH),
	INIT_CALLBACK_BYLOACH(PUTFRIENDSHIPS_DESTROY),
	INIT_CALLBACK_BYLOACH(GETFRIENDSHIPS_EXISTS),
	INIT_CALLBACK_BYLOACH(GETFRIENDSHIPS_BATCH_EXISTS),

	//Social Graph
	INIT_CALLBACK_BYLOACH(GETFRIEND_IDS),
	INIT_CALLBACK_BYLOACH(GETFOLLOWER_IDS),
	INIT_CALLBACK_BYLOACH(PUTFOLLOWERS_DESTROY),

	//账号
	INIT_CALLBACK_BYLOACH(GETACCOUNT_VERIFY),
	INIT_CALLBACK_BYLOACH(GETACCOUNT_RATELIMIT),
	INIT_CALLBACK_BYLOACH(PUTACCOUNT_QUITSESSION),
	INIT_CALLBACK_BYLOACH(PUTACCOUNT_UPDATE_PROFILEIMAGE),
	INIT_CALLBACK_BYLOACH(PUTACCOUNT_UPDATE_PROFILE),
	INIT_CALLBACK_BYLOACH(PUTACCOUNT_REGISTER),

	// 收藏
	INIT_CALLBACK_BYLOACH(GETFAVORITES),
	INIT_CALLBACK_BYLOACH(PUTFAVORITES_CREATE),
	INIT_CALLBACK_BYLOACH(PUTFAVORITES_DESTROY),

	//登录/OAuth
	INIT_CALLBACK_BYLOACH(OAUTH_REQUEST_TOKEN),
	INIT_CALLBACK_BYLOACH(OAUTH_AUTHORIZE),
	INIT_CALLBACK_BYLOACH(OAUTH_ACCESS_TOKEN),

	INIT_CALLBACK_BYLOACH(GET_EMOTIONS),
	INIT_CALLBACK_BYLOACH(GET_USERS_SEARCH),
	INIT_CALLBACK_BYLOACH(GET_WB_SEARCH),
	INIT_CALLBACK_BYLOACH(GET_STATUSES_SEARCH),
	INIT_CALLBACK_BYLOACH(GET_PROVINCES),

	// COOKIE
	{ WEIBO_struct_statuscallback_init_fun(COOKIE) , WEIBO_struct_bodycallback_init_fun(COOKIE) , WEIBO_struct_headercallback_init_fun(COOKIE)},
	INIT_CALLBACK_BYLOACH(UPDATETGT),// UPDATETGT,

	//自定义URL
	INIT_CALLBACK_BYLOACH(CUSTOM),

	//
	INIT_CALLBACK_BYWELBON(GET_USERS_HOT),// 获取系统推荐用户
	INIT_CALLBACK_BYWELBON(POST_USERS_REMARK),//更新修改当前登录用户所关注的某个好友的备注信息New!
	INIT_CALLBACK_BYWELBON(GET_USERS_SUGGESTIONS), //Users/suggestions 返回当前用户可能感兴趣的用户

	// 话题接口 ,by welbon,2011-01-10
	INIT_CALLBACK_BYWELBON(GET_TRENDS),//trends 获取某人的话题
	INIT_CALLBACK_BYWELBON(GET_TRENDS_STATUSES),//trends/statuses 获取某一话题下的微博
	INIT_CALLBACK_BYWELBON(POST_TRENDS_FOLLOW),//trends/follow 关注某一个话题
	INIT_CALLBACK_BYWELBON(DELETE_TRENDS_DESTROY),//trends/destroy 取消关注的某一个话题
	INIT_CALLBACK_BYWELBON(GET_TRENDS_HOURLY),//trends/destroy 按小时返回热门话题
	INIT_CALLBACK_BYWELBON(GET_TRENDS_DAYLIY),//trends/daily 按日期返回热门话题。返回某一日期的话题
	INIT_CALLBACK_BYWELBON(GET_TRENDS_WEEKLIY),//trends/weekly 按周返回热门话题。返回某一日期之前某一周的话题

	// 黑名单接口 ,by welbon,2011-01-10
	INIT_CALLBACK_BYWELBON(POST_BLOCKS_CREATE),//将某用户加入黑名单
	INIT_CALLBACK_BYWELBON(POST_BLOCKS_DESTROY),//将某用户移出黑名单
	INIT_CALLBACK_BYWELBON(GET_BLOCKS_EXISTS),//检测某用户是否是黑名单用户
	INIT_CALLBACK_BYWELBON(GET_BLOCKS_BLOCKING),//列出黑名单用户(输出用户详细信息)
	INIT_CALLBACK_BYWELBON(GET_BLOCKS_BLOCKING_IDS),//列出分页黑名单用户（只输出id）

	//用户标签接口 ,by welbon,2011-01-10
	INIT_CALLBACK_BYWELBON(GET_TAGS),//tags 返回指定用户的标签列表
	INIT_CALLBACK_BYWELBON(POST_TAGS_CREATE),//tags/create 添加用户标签
	INIT_CALLBACK_BYWELBON(GET_TAGS_SUGGESTIONS),//tags/suggestions 返回用户感兴趣的标签
	INIT_CALLBACK_BYWELBON(POST_TAGS_DESTROY),//tags/destroy 删除标签
	INIT_CALLBACK_BYWELBON(POST_TAGS_DESTROY_BATCH),//tags/destroy_batch 批量删除标签

	// 邀请接口
	INIT_CALLBACK_BYWELBON(POST_INVITE_MAILCONTACT),//邀请邮箱联系人
	INIT_CALLBACK_BYWELBON(POST_INVITE_MSNCONTACT), //邀请MSN联系人
	INIT_CALLBACK_BYWELBON(POST_INVITE_SENDMAILS),  //发送邀请邮件

	// media
	INIT_CALLBACK_BYWELBON(GET_MEDIA_SHORTURL_BATCH),// 批量获取短链接
    //登录/XAuth
	INIT_CALLBACK_BYLOACH(XAUTH_ACCESS_TOKEN),

};

struct t_wb_callback_byloach* Wb_get_struct_callback( int option )
{
	if( WEIBO_OPTION(UNK) >= option ||
		WEIBO_OPTION(LAST) <= option )
	{
		option = 0;
	}
	return &callback_byloach[option];
}

void Parse_status_JSON(wbParserNS::REQOBJ* pRoot, M8::m8_status * p_status)
{
	wbParserNS::GetCHAR_Key_JSON("created_at", pRoot, p_status->created_at_, WB_USE_LEN(TIME));
	wbParserNS::GetCHAR_Key_JSON("id", pRoot, p_status->id_, WB_USE_LEN(WEIBO_ID));
	wbParserNS::GetCHAR_Key_JSON("text", pRoot, p_status->text_, WB_USE_LEN(WB_INFO));
	wbParserNS::GetCHAR_Key_JSON("source", pRoot, p_status->source_, WB_USE_LEN(WB_INFO));
	wbParserNS::GetCHAR_Key_JSON("favorited", pRoot, p_status->favorited_, WB_USE_LEN(PROFILE_GENDER));
	wbParserNS::GetCHAR_Key_JSON("truncated", pRoot, p_status->truncated_, WB_USE_LEN(PROFILE_GENDER));
	wbParserNS::GetCHAR_Key_JSON("in_reply_to_status_id", pRoot, p_status->in_reply_to_status_id_, WB_USE_LEN(WEIBO_ID));
	wbParserNS::GetCHAR_Key_JSON("in_reply_to_user_id", pRoot, p_status->in_reply_to_user_id_, WB_USE_LEN(WEIBO_ID));
	wbParserNS::GetCHAR_Key_JSON("in_reply_to_screen_name", pRoot, p_status->in_reply_to_screen_name_, WB_USE_LEN(SCREEN_NAME));
	wbParserNS::GetCHAR_Key_JSON("thumbnail_pic", pRoot, p_status->thumbnail_pic_, WB_USE_LEN(URL));
	wbParserNS::GetCHAR_Key_JSON("bmiddle_pic", pRoot, p_status->bmiddle_pic_, WB_USE_LEN(URL));
	wbParserNS::GetCHAR_Key_JSON("original_pic", pRoot, p_status->original_pic_, WB_USE_LEN(URL));
	wbParserNS::GetCHAR_Key_JSON("geo", pRoot, p_status->geo_, WB_USE_LEN(GEOCODE));
	wbParserNS::GetCHAR_Key_JSON("mid", pRoot, p_status->mid_, WB_USE_LEN(WEIBO_ID));

	wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Key_JSON("user", pRoot);
	//if (p_status->user_) delete p_status->user_;
	if (pChild == NULL)
	{
		p_status->user_ = NULL;
	}
	else
	{
		p_status->user_ = new M8::m8_user();
		Parse_user_JSON(pChild, p_status->user_);
	}

	pChild = wbParserNS::GetObject_Key_JSON("retweeted_status", pRoot);
	//if (p_status->retweeted_status_) delete p_status->retweeted_status_;
	if (pChild == NULL)
	{
		p_status->retweeted_status_ = NULL;
	}
	else
	{
		p_status->retweeted_status_ = new M8::m8_status();
		Parse_status_JSON(pChild, p_status->retweeted_status_);
	}
}

void Parse_user_JSON( wbParserNS::REQOBJ* pRoot, M8::m8_user * p_user )
{
	wbParserNS::GetCHAR_Key_JSON("id", pRoot, p_user->id_, WB_USE_LEN(WEIBO_ID));
	wbParserNS::GetCHAR_Key_JSON("screen_name", pRoot, p_user->screen_name_, WB_USE_LEN(SCREEN_NAME));
	wbParserNS::GetCHAR_Key_JSON("name", pRoot, p_user->name_, WB_USE_LEN(PROFILE_NAME));
	wbParserNS::GetCHAR_Key_JSON("province", pRoot, p_user->province_, WB_USE_LEN(PROFILE_PROVINCE));
	wbParserNS::GetCHAR_Key_JSON("city", pRoot, p_user->city_, WB_USE_LEN(PROFILE_CITY));
	wbParserNS::GetCHAR_Key_JSON("location", pRoot, p_user->location_, WB_USE_LEN(PROFILE_LOCATION));
	wbParserNS::GetCHAR_Key_JSON("description", pRoot, p_user->description_, WB_USE_LEN(PROFILE_DESCRIPTION));
	wbParserNS::GetCHAR_Key_JSON("url", pRoot, p_user->url_, WB_USE_LEN(URL));
	wbParserNS::GetCHAR_Key_JSON("profile_image_url", pRoot, p_user->profile_image_url_, WB_USE_LEN(URL));
	wbParserNS::GetCHAR_Key_JSON("domain", pRoot, p_user->domain_, WB_USE_LEN(LENGTH8));
	wbParserNS::GetCHAR_Key_JSON("gender", pRoot, p_user->gender_, WB_USE_LEN(PROFILE_GENDER));
	p_user->followers_count_ = wbParserNS::GetLONG_Key_JSON("followers_count", pRoot);
	p_user->friends_count_ = wbParserNS::GetLONG_Key_JSON("friends_count", pRoot);
	p_user->statuses_count_ = wbParserNS::GetLONG_Key_JSON("statuses_count", pRoot);
	p_user->favourites_count_ = wbParserNS::GetLONG_Key_JSON("favourites_count", pRoot);
	wbParserNS::GetCHAR_Key_JSON("created_at", pRoot, p_user->created_at_, WB_USE_LEN(TIME));
	wbParserNS::GetCHAR_Key_JSON("following", pRoot, p_user->following_, WB_USE_LEN(PROFILE_GENDER));
	wbParserNS::GetCHAR_Key_JSON("allow_all_act_msg", pRoot, p_user->allow_all_act_msg_, WB_USE_LEN(PROFILE_GENDER));
	wbParserNS::GetCHAR_Key_JSON("geo_enabled", pRoot, p_user->geo_enabled_, WB_USE_LEN(PROFILE_GENDER));
	wbParserNS::GetCHAR_Key_JSON("verified", pRoot, p_user->verified_, WB_USE_LEN(PROFILE_GENDER));
	wbParserNS::GetCHAR_Key_JSON("remark", pRoot, p_user->remark_, WB_USE_LEN(PROFILE_NAME));
}

void Parse_friendships_JSON( wbParserNS::REQOBJ* pRoot, M8::m8_friendships * p_friendships )
{
	wbParserNS::GetCHAR_Key_JSON("id", pRoot, p_friendships->id_, WB_USE_LEN(WEIBO_ID));
	wbParserNS::GetCHAR_Key_JSON("screen_name", pRoot, p_friendships->screen_name_, WB_USE_LEN(SCREEN_NAME));
	wbParserNS::GetCHAR_Key_JSON("following", pRoot, p_friendships->following_, WB_USE_LEN(PROFILE_GENDER));
	wbParserNS::GetCHAR_Key_JSON("followed_by", pRoot, p_friendships->followed_by_, WB_USE_LEN(PROFILE_GENDER));
	wbParserNS::GetCHAR_Key_JSON("notifications_enabled", pRoot, p_friendships->notifications_enabled_, WB_USE_LEN(PROFILE_GENDER));
}

void Parse_comment_JSON( wbParserNS::REQOBJ* pRoot, M8::m8_comment* p_comment )
{
	wbParserNS::GetCHAR_Key_JSON("created_at", pRoot, p_comment->created_at_, WB_USE_LEN(TIME));
	wbParserNS::GetCHAR_Key_JSON("id", pRoot, p_comment->id_, WB_USE_LEN(WEIBO_ID));
	wbParserNS::GetCHAR_Key_JSON("text", pRoot, p_comment->text_, WB_USE_LEN(WB_INFO));
	wbParserNS::GetCHAR_Key_JSON("source", pRoot, p_comment->source_, WB_USE_LEN(WB_INFO));
	wbParserNS::GetCHAR_Key_JSON("mid", pRoot, p_comment->mid_, WB_USE_LEN(WB_INFO));
	wbParserNS::REQOBJ* pChild = wbParserNS::GetObject_Key_JSON("user", pRoot);
	Parse_user_JSON(pChild, &p_comment->user_);
	pChild = wbParserNS::GetObject_Key_JSON("status", pRoot);
	Parse_status_JSON(pChild, &p_comment->status_);
}
