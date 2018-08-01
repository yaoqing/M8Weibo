#pragma once

// Include MZFC header file
#include <mzfc_inc.h>
#include "M8Struct.h"
#include "M8WeiboNetworkEnv.h"
#include <stack>

class CM8WeiboParameter;
class CM8WeiboHomeWnd;
class CM8WeiboLoginWnd;

// Application class derived from CMzApp
class CM8WeiboApp: public CMzApp
{
public:
  CM8WeiboApp(void);
  ~CM8WeiboApp(void);

  virtual BOOL Init();
  virtual int Done();
  BOOL Exists();
  bool IsValid();
  bool Serial(char *uid, int size, M8::m8_status **pStatuses);
  bool UnSerial(char *uid, int &size, M8::m8_status **&pStatuses);

  // 将文本按制定个数分行
  int SplitString( wchar_t *p_text, int chars_per_line );
  bool SplitString( int lines, CMzString *p_lines, wchar_t *p_text, int chars_per_line );
  void FormatCreateAt(wchar_t *pointer, wchar_t etc[6][16]);
  void FormatSource(wchar_t *pointer, wchar_t *buffer);
  int FindAtUser(wchar_t *p_text);
  void FindAtUser(CMzString *p_users, wchar_t *p_text);
  void AssignString(wchar_t *begin, wchar_t *end, CMzString *p_users);

  // 返回帐号列表
  int GetAccountList(wchar_t *account_list, CMzString *&szAccounts);

  // 根据UID和头像URL返回头像文件名(不含扩展名)
  CMzString UpdateProfileTokenName(char *uid, char *profile_image_url, CMzString& profile_image_path);
  CMzString UpdateSmallPictureTokenName(char *uid, char *picture_url, CMzString& picture_cache_path);
  CMzString UpdateMiddlePictureTokenName(char *uid, char *picture_url, CMzString& picture_cache_path);
  CMzString UpdateOriginalPictureTokenName(char *uid, char *picture_url, CMzString& picture_cache_path);
  CMzString UpdateTokenName(char *uid, char *url, char *kind, CMzString& kind_path);
  CMzString FindTokenFileName(CMzString image_path, CMzString token_name);
  bool DownloadTokenFile(HWND hWnd, char *uid, bool uniq_uid, char *url, CMzString &token_path, CMzString &token_name);
  void CreateDirectory(CMzString &path);
  bool IsGif(char *uri);

  void GoHome();
  void ShowAll();
  void GoLogin();
  void PopWnd(CMzWndEx *pWnd);

  // 微博接口函数
  void InitWeiboInterface();
  bool LoginWeibo(HWND hWnd, CMzString account, CMzString password);

  bool isSelf(char *uid);
  M8::m8_user * GetUserInfoSelf();

  bool RefreshUserInfoSelf(HWND hWnd);
  bool GetUserInfoUnknown(HWND hWnd, int uid_type, char *uid, M8::m8_wb_get_users_show *p_user_info);
  bool GetStatusesFriendsTimeline(HWND hWnd, int opt_type, char *opt_id,
	  M8::m8_wb_get_statuses_friends_timeline *&p_statuses_friends_timeline);
  bool GetStatusesUserTimeline(HWND hWnd, char *uid, int opt_type, char *opt_id,
	  M8::m8_wb_get_statuses_user_timeline *&p_statuses_user_timeline);
  bool GetFavorites(HWND hWnd, int page, M8::m8_wb_get_favorites *&p_favorites);
  bool GetStatusesMentions(HWND hWnd, int opt_type, char *opt_id,
	  M8::m8_wb_get_statuses_mentions *&p_statuses_mentions);
  bool GetStatusesCounts(HWND hWnd, int size, M8::m8_counts *p_counts);
  bool GetStatusesCommentsList(HWND hWnd, char *wbId, int opt_type, char *opt_id,
	  M8::m8_wb_get_statuses_comments_list *&p_statuses_comments_list);
  bool GetFriendsList(HWND hWnd, char *uid, int cursor, M8::m8_wb_get_statuses_friends *& p_friends_list);
  bool GetFollowersList(HWND hWnd, char *uid, int cursor, M8::m8_wb_get_statuses_followers *& p_friends_list);
  bool GetUserSearch(HWND hWnd, char *name, int page, M8::m8_wb_users_search *&p_users_search);
  bool GetWeiboSearch(HWND hWnd, char *tag, int page, M8::m8_wb_search *&p_wb_search);
  bool PutFollowersDestroy(HWND hWnd, char *uid);

  bool GetStatusesCommentsToMe(HWND hWnd, int opt_type, char *opt_id,
	  M8::m8_wb_get_statuses_comments_tome *&p_statuses_comments_tome);

  bool PutStatusesUpdate(HWND hWnd, CMzString text);
  bool PutStatusesUpload(HWND hWnd, CMzString path, CMzString text);
  bool PutStatusesRepost(HWND hWnd, char *wbId, int is_comment, CMzString text);
  bool PutStatusesComment(HWND hWnd, char *wbId, char *wbCId, CMzString text, M8::m8_wb_put_statuses_comment *&p_status_comment);

  bool PutFriendshipsCreate(HWND hWnd, char *wbId);
  bool PutFriendshipsDestroy(HWND hWnd, char *wbId);
  bool GetFriendshipsExists(HWND hWnd, char *wbIdSrc, char *wbIdDest, M8::m8_wb_get_friendships_show *p_friendships_exists);

  bool PutStatusesDestroy(HWND hWnd, char *wbId);
  bool PutStatusesCommentDestroy(HWND hWnd, char *wbId, char *wbCId);

  bool PutFavoritesCreate(HWND hWnd, char *wbId);
  bool PutFavoritesDestroy(HWND hWnd, char *wbId);

  bool OutputMessage(HWND hWnd, unsigned long http_code, char *error);

  CM8WeiboParameter *m_parameter;
  std::stack<CMzWndEx *> m_stack;
  CM8WeiboNetworkEnv m_net_env;

private:
  WEIBORequest * wbRequest;
  struct t_wb_interface  wb_interface;
  M8::m8_wb_oauth_access_token *m_oauth_access_token;
  M8::m8_wb_get_users_show *m_user_info_self;
  //M8::m8_wb_get_users_show *m_user_info_unknown;
  //M8::m8_wb_get_statuses_friends_timeline *m_statuses_friends_timeline;
  //M8::m8_wb_get_statuses_counts *m_statuses_counts;

  bool m_valid;
  bool StoreVerify();

  int ReadStatus(M8::m8_status *pStatus, char* buffer);
  void WriteStatus(M8::m8_status *pStatus, FILE *fd);
  void WriteFileS(char* buffer, int size, FILE *fd);
};

extern CM8WeiboApp theApp;
