#pragma once

#include <libweibo/weibo.h>
#include "M8Weibo.h"

WB_MAX_LEN_DEFINE(LENGTH8, 8);

namespace M8
{
	// 获取OAUTH_REQUEST_TOKEN接口
	struct m8_wb_oauth_request_token
	{
		t_wb_oauth wbauth_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_wb_oauth_request_token()
		{
			int full_size = sizeof(m8_wb_oauth_request_token);
			int head_size = sizeof(t_wb_oauth_request_token);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 获取OAUTH_AUTHORIZE接口
	struct m8_wb_authorize
	{
		t_wb_oauth wbauth_;
		WBChar  usrid_[ WB_REAL_LEN(UID) ];
		WBChar  usrpwd_[ WB_REAL_LEN(PWD) ];
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_wb_authorize()
		{
			int full_size = sizeof(m8_wb_authorize);
			int head_size = sizeof(t_wb_authorize);
			memset((char *)this+head_size, 0, full_size-head_size);
		};
	};

	// 获取OAUTH_ACCESS_TOKEN接口
	struct m8_wb_oauth_access_token
	{
		t_wb_oauth wbauth_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		WBChar uid_[  WB_REAL_LEN(UID) ];
		m8_wb_oauth_access_token()
		{
			int full_size = sizeof(m8_wb_oauth_access_token);
			int head_size = sizeof(t_wb_oauth_access_token);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 用户结构
	struct m8_user
	{
		WBChar id_[ WB_REAL_LEN(WEIBO_ID) ];
		WBChar screen_name_[ WB_REAL_LEN(SCREEN_NAME) ];
		WBChar name_[ WB_REAL_LEN(PROFILE_NAME) ];
		WBChar province_[ WB_REAL_LEN(PROFILE_PROVINCE) ];
		WBChar city_[ WB_REAL_LEN(PROFILE_CITY) ];
		WBChar location_[ WB_REAL_LEN(PROFILE_LOCATION) ];
		WBChar description_[ WB_REAL_LEN(PROFILE_DESCRIPTION) ];
		WBChar url_[ WB_REAL_LEN(URL) ];
		WBChar profile_image_url_[ WB_REAL_LEN(URL) ];
		WBChar domain_[ WB_REAL_LEN(LENGTH8) ];
		WBChar gender_[ WB_REAL_LEN(PROFILE_GENDER) ];
		unsigned long followers_count_;
		unsigned long friends_count_;
		unsigned long statuses_count_;
		unsigned long favourites_count_;
		WBChar created_at_[ WB_REAL_LEN(TIME) ];
		WBChar following_[ WB_REAL_LEN(PROFILE_GENDER) ];
		WBChar allow_all_act_msg_[ WB_REAL_LEN(PROFILE_GENDER) ];
		WBChar geo_enabled_[ WB_REAL_LEN(PROFILE_GENDER) ];
		WBChar verified_[ WB_REAL_LEN(PROFILE_GENDER) ];
		WBChar remark_[ WB_REAL_LEN(PROFILE_NAME) ];
		m8_user() {memset(this, 0, sizeof(m8_user));}
	};

	// 微博结构
	struct m8_status
	{
		WBChar created_at_[ WB_REAL_LEN(TIME) ];
		WBChar id_[ WB_REAL_LEN(WEIBO_ID) ];
		WBChar text_[ WB_REAL_LEN(WB_INFO) ];
		WBChar source_[ WB_REAL_LEN(WB_INFO) ];
		WBChar favorited_[ WB_REAL_LEN(PROFILE_GENDER) ];
		WBChar truncated_[ WB_REAL_LEN(PROFILE_GENDER) ];
		WBChar in_reply_to_status_id_[ WB_REAL_LEN(WEIBO_ID) ];
		WBChar in_reply_to_user_id_[ WB_REAL_LEN(WEIBO_ID) ];
		WBChar in_reply_to_screen_name_[ WB_REAL_LEN(SCREEN_NAME) ];
		WBChar thumbnail_pic_[ WB_REAL_LEN(URL) ];
		WBChar bmiddle_pic_[ WB_REAL_LEN(URL) ];
		WBChar original_pic_[ WB_REAL_LEN(URL) ];
		WBChar geo_[ WB_REAL_LEN(GEOCODE) ];
		WBChar mid_[ WB_REAL_LEN(WEIBO_ID) ];
		unsigned long m_comments_;
		unsigned long m_links_;
		m8_user *user_;
		m8_status *retweeted_status_;
		m8_status() {memset(this, 0, sizeof(m8_status));}
		~m8_status()
		{
			if (user_)
			{
				delete user_;
				user_ = NULL;
			}
			m8_status *p = retweeted_status_;
			while (p)
			{
				p = retweeted_status_->retweeted_status_;
				delete retweeted_status_;
				retweeted_status_ = NULL;
			}
		}
	};

	// 转发及评论数
	struct m8_counts
	{
		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];
		unsigned long comments_;
		unsigned long rt_;
	};

	// 评论
	struct m8_comment
	{
		WBChar created_at_[ WB_REAL_LEN(TIME) ];
		WBChar id_[ WB_REAL_LEN(WEIBO_ID) ];
		WBChar text_[ WB_REAL_LEN(WB_INFO) ];
		WBChar source_[ WB_REAL_LEN(WB_INFO) ];
		m8_user user_;
		WBChar mid_[ WB_REAL_LEN(WEIBO_ID) ];
		m8_status status_;
	};

	// 获取用户信息接口
	struct m8_wb_get_users_show
	{
		t_wb_oauth wbauth_;
		t_wb_uid  wbuid_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_user user_;
		m8_status status_;
		m8_wb_get_users_show()
		{
			int full_size = sizeof(m8_wb_get_users_show);
			int head_size = sizeof(t_wb_get_users_show);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 获取用户所有关注用户最新n条微博信息
	struct m8_wb_get_statuses_friends_timeline
	{
		t_wb_oauth wbauth_;
		t_wb_option_params wbopt_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		int size_of_status_;
		m8_status *statuses_;
		m8_wb_get_statuses_friends_timeline()
		{
			int full_size = sizeof(m8_wb_get_statuses_friends_timeline);
			int head_size = sizeof(t_wb_get_statuses_friends_timeline);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
		~m8_wb_get_statuses_friends_timeline()
		{
			if (statuses_)
			{
				delete [] statuses_;
				statuses_ = NULL;
				size_of_status_ = 0;
			}
		}
	};

	// 返回用户的发布的最近n条信息
	struct m8_wb_get_statuses_user_timeline
	{
		t_wb_oauth wbauth_;
		t_wb_uid wbuid_;
		t_wb_option_params wbopt_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		int size_of_status_;
		m8_status *statuses_;
		m8_wb_get_statuses_user_timeline()
		{
			int full_size = sizeof(m8_wb_get_statuses_user_timeline);
			int head_size = sizeof(t_wb_get_statuses_user_timeline);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
		~m8_wb_get_statuses_user_timeline()
		{
			if (statuses_)
			{
				delete [] statuses_;
				statuses_ = NULL;
				size_of_status_ = 0;
			}
		}
	};

	// 批量统计微博的评论数及转发数(一次最多100个)
	struct m8_wb_get_statuses_counts 
	{
		t_wb_oauth wbauth_;
		WBChar wbIds_[BATCH_WBIDS_COUNTS][WB_REAL_LEN(WEIBO_ID)];///<微博ID , 最多 100
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		WBChar rtnIds_[BATCH_WBIDS_COUNTS][WB_REAL_LEN(WEIBO_ID)];///<返回的微博ID，和原始顺序不一样，需要调整
		unsigned long comments_[BATCH_WBIDS_COUNTS];
		unsigned long rt_[BATCH_WBIDS_COUNTS];
		m8_wb_get_statuses_counts()
		{
			int full_size = sizeof(m8_wb_get_statuses_counts);
			int head_size = sizeof(t_wb_get_statuses_counts);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 返回最新n条@我的微博 
	struct m8_wb_get_statuses_mentions
	{
		t_wb_oauth wbauth_;
		t_wb_option_params wbopt_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		int size_of_status_;
		m8_status *statuses_;
		m8_wb_get_statuses_mentions()
		{
			int full_size = sizeof(m8_wb_get_statuses_mentions);
			int head_size = sizeof(t_wb_get_statuses_mentions);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
		~m8_wb_get_statuses_mentions()
		{
			if (statuses_)
			{
				delete [] statuses_;
				statuses_ = NULL;
				size_of_status_ = 0;
			}
		}
	};

	// 发布一条微博信息
	struct m8_wb_put_statuses_update
	{
		t_wb_oauth wbauth_;
		WBChar szwbInfo_[WB_REAL_LEN(WB_INFO)];
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_status status_;
		m8_wb_put_statuses_update()
		{
			int full_size = sizeof(m8_wb_put_statuses_update);
			int head_size = sizeof(t_wb_put_statuses_update);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 上传图片及发布微博信息
	struct m8_wb_put_statuses_upload
	{
		t_wb_oauth wbauth_;
		WBChar szImagePath_[WB_REAL_LEN(MAX_PATH) ];
		WBChar szwbInfo_[WB_REAL_LEN(WB_INFO)];
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_status status_;
		m8_wb_put_statuses_upload()
		{
			int full_size = sizeof(m8_wb_put_statuses_upload);
			int head_size = sizeof(t_wb_put_statuses_upload);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 转发一条微博信息
	struct m8_wb_put_statuses_repost
	{
		t_wb_oauth wbauth_;
		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];
		WBChar szwbInfo_[WB_REAL_LEN(WB_INFO)];
		int iCommentFlags;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_status status_;
		m8_wb_put_statuses_repost()
		{
			int full_size = sizeof(m8_wb_put_statuses_repost);
			int head_size = sizeof(t_wb_put_statuses_repost);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 对一条微博信息进行评论
	struct m8_wb_put_statuses_comment
	{
		t_wb_oauth wbauth_;

		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];
		WBChar wbCId_[WB_REAL_LEN(WEIBO_ID)];
		WBChar szwbInfo_[WB_REAL_LEN(WB_INFO)];
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_status status_;
		m8_wb_put_statuses_comment()
		{
			int full_size = sizeof(m8_wb_put_statuses_comment);
			int head_size = sizeof(t_wb_put_statuses_comment);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	struct m8_friendships
	{
		WBChar id_[ WB_REAL_LEN(WEIBO_ID) ];
		WBChar screen_name_[ WB_REAL_LEN(SCREEN_NAME) ];
		WBChar following_[ WB_REAL_LEN(PROFILE_GENDER) ];
		WBChar followed_by_[ WB_REAL_LEN(PROFILE_GENDER) ];
		WBChar notifications_enabled_[ WB_REAL_LEN(PROFILE_GENDER) ];
	};

	// 关注某用户
	struct m8_wb_put_friendships_create
	{
		t_wb_oauth wbauth_;
		t_wb_uid wbuid_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_user user_;
		m8_status status_;
		m8_wb_put_friendships_create()
		{
			int full_size = sizeof(m8_wb_put_friendships_create);
			int head_size = sizeof(t_wb_put_friendships_create);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 取消关注
	struct m8_wb_put_friendships_destroy
	{
		t_wb_oauth wbauth_;
		t_wb_uid wbuid_;
		int is_follower;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_user user_;
		m8_status status_;
		m8_wb_put_friendships_destroy()
		{
			int full_size = sizeof(m8_wb_put_friendships_destroy);
			int head_size = sizeof(t_wb_put_friendships_destroy);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	/* 判断两个用户是否有关注关系，返回两个用户关系的详细情况  */
	struct m8_wb_get_friendships_show
	{
		t_wb_oauth wbauth_;
		t_wb_uid wbuid_source_;
		t_wb_uid wbuid_target_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_friendships source_;
		m8_friendships target_;
		m8_wb_get_friendships_show()
		{
			int full_size = sizeof(m8_wb_get_friendships_show);
			int head_size = sizeof(t_wb_get_friendships_show);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 获取指定微博的评论列表 
	struct m8_wb_get_statuses_comments_list
	{
		t_wb_oauth wbauth_;
		WBChar wbId_[ WB_REAL_LEN(WEIBO_ID) ];
		t_wb_option_params wbopt_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		int size_of_comment_;
		m8_comment *comments_;
		m8_wb_get_statuses_comments_list()
		{
			int full_size = sizeof(m8_wb_get_statuses_comments_list);
			int head_size = sizeof(t_wb_get_statuses_comments_list);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
		~m8_wb_get_statuses_comments_list()
		{
			if (comments_)
			{
				delete [] comments_;
				comments_ = NULL;
				size_of_comment_ = 0;
			}
		}
	};

	struct m8_user_and_status
	{
		m8_user user_;
		m8_status status_;
	};

	// 关注列表
	struct m8_wb_get_statuses_friends
	{
		t_wb_oauth wbauth_;
		t_wb_uid  wbuid_;
		t_wb_option_params wbopt_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		int size_of_friend_;
		m8_user_and_status *friends_;
		int prev_cursor_;
		int next_cursor_;
		m8_wb_get_statuses_friends()
		{
			int full_size = sizeof(m8_wb_get_statuses_friends);
			int head_size = sizeof(t_wb_get_statuses_friends);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
		~m8_wb_get_statuses_friends()
		{
			if (friends_)
			{
				delete [] friends_;
				friends_ = NULL;
				size_of_friend_ = 0;
			}
		}
	};

	// 粉丝列表
	struct m8_wb_get_statuses_followers
	{
		t_wb_oauth wbauth_;
		t_wb_uid  wbuid_;
		t_wb_option_params wbopt_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		int size_of_follower_;
		m8_user_and_status *followers_;
		int prev_cursor_;
		int next_cursor_;
		m8_wb_get_statuses_followers()
		{
			int full_size = sizeof(m8_wb_get_statuses_followers);
			int head_size = sizeof(t_wb_get_statuses_followers);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
		~m8_wb_get_statuses_followers()
		{
			if (followers_)
			{
				delete [] followers_;
				followers_ = NULL;
				size_of_follower_ = 0;
			}
		}
	};

	// 删除微博
	struct m8_wb_put_statuses_destroy
	{
		t_wb_oauth wbauth_;
		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_status status_;
		m8_wb_put_statuses_destroy()
		{
			int full_size = sizeof(m8_wb_put_statuses_destroy);
			int head_size = sizeof(t_wb_put_statuses_destroy);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 删除微博的评论
	struct m8_wb_put_statuses_comment_destroy
	{
		t_wb_oauth wbauth_;
		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];
		WBChar wbCId_[WB_REAL_LEN(WEIBO_ID)];
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_comment comment_;
		m8_wb_put_statuses_comment_destroy()
		{
			int full_size = sizeof(m8_wb_put_statuses_comment_destroy);
			int head_size = sizeof(t_wb_put_statuses_comment_destroy);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 获取当前用户的收藏列表 
	struct m8_wb_get_favorites
	{
		t_wb_oauth wbauth_;
		t_wb_option_params wbopt_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		int size_of_status_;
		m8_status *statuses_;
		int page_;
		m8_wb_get_favorites()
		{
			int full_size = sizeof(m8_wb_get_favorites);
			int head_size = sizeof(t_wb_get_favorites);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
		~m8_wb_get_favorites()
		{
			if (statuses_)
			{
				delete [] statuses_;
				statuses_ = NULL;
				size_of_status_ = 0;
			}
		}
	};

	// 添加收藏
	struct m8_wb_put_favorites_create
	{
		t_wb_oauth wbauth_;
		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_status status_;
		m8_wb_put_favorites_create()
		{
			int full_size = sizeof(m8_wb_put_favorites_create);
			int head_size = sizeof(t_wb_put_favorites_create);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 删除收藏
	struct m8_wb_put_favorites_destroy
	{
		t_wb_oauth wbauth_;
		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_status status_;
		m8_wb_put_favorites_destroy()
		{
			int full_size = sizeof(m8_wb_put_favorites_destroy);
			int head_size = sizeof(t_wb_put_favorites_destroy);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 获取当前用户收到的评论  
	struct m8_wb_get_statuses_comments_tome
	{
		t_wb_oauth wbauth_;
		t_wb_option_params wbopt_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		int size_of_comment_;
		m8_comment *comments_;
		m8_wb_get_statuses_comments_tome()
		{
			int full_size = sizeof(m8_wb_get_statuses_comments_tome);
			int head_size = sizeof(t_wb_get_statuses_comments_tome);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
		~m8_wb_get_statuses_comments_tome()
		{
			if (comments_)
			{
				delete [] comments_;
				comments_ = NULL;
				size_of_comment_ = 0;
			}
		}
	};

	// 删除粉丝
	struct m8_wb_put_followers_destroy
	{
		t_wb_oauth wbauth_;
		t_wb_uid wbuid_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		m8_wb_put_followers_destroy()
		{
			int full_size = sizeof(m8_wb_put_followers_destroy);
			int head_size = sizeof(t_wb_put_followers_destroy);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
	};

	// 用户搜索
	struct m8_wb_users_search
	{
		t_wb_users_search search_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		int size_of_user_;
		m8_user_and_status *users_;
		int prev_cursor_;
		int next_cursor_;
		m8_wb_users_search()
		{
			int full_size = sizeof(m8_wb_users_search);
			int head_size = sizeof(t_wb_users_search);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
		~m8_wb_users_search()
		{
			if (users_)
			{
				delete [] users_;
				users_ = NULL;
				size_of_user_ = 0;
			}
		}
	};

	// 搜索微博文章
	struct m8_wb_search
	{
		t_wb_search search_;
		unsigned long http_code_;
		WBChar error_[WB_REAL_LEN(LENGTH8)];
		int size_of_status_;
		m8_status *statuses_;
		int page_;
		m8_wb_search()
		{
			int full_size = sizeof(m8_wb_search);
			int head_size = sizeof(t_wb_search);
			memset((char *)this+head_size, 0, full_size-head_size);
		}
		~m8_wb_search()
		{
			if (statuses_)
			{
				delete [] statuses_;
				statuses_ = NULL;
				size_of_status_ = 0;
			}
		}
	};
}