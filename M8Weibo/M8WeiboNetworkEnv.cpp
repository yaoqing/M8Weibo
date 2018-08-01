#include "M8WeiboNetworkEnv.h"
#include <CallNotifyApi.h>

CM8WeiboNetworkEnv::CM8WeiboNetworkEnv(void)
{
}

CM8WeiboNetworkEnv::~CM8WeiboNetworkEnv(void)
{
}

bool CM8WeiboNetworkEnv::start(HWND hWnd, tagNE ne)
{
	m_ne_type = ne;
	bool is_ok = false;
	MzBeginWaitDlg(hWnd, NULL, true);

	if (ne == NE_CMNET)
	{
		if (RESULT_OK == Dial_StartGprsConnect2(NULL, GPRS_FORCE_APP_TYPE, L"中国移动CMNET"))
			is_ok = true;
	}
	else if (ne == NE_CMWAP)
	{
		if (RESULT_OK == Dial_StartGprsConnect2(NULL, GPRS_FORCE_APP_TYPE, L"中国移动CMWAP"))
			is_ok = true;
	}
	else if (ne == NE_UNINET)
	{
		if (RESULT_OK == Dial_StartGprsConnect2(NULL, GPRS_FORCE_APP_TYPE, L"中国联通UNINET"))
			is_ok = true;
	}
	else if (ne == NE_UNIWAP)
	{
		if (RESULT_OK == Dial_StartGprsConnect2(NULL, GPRS_FORCE_APP_TYPE, L"中国联通UNIWAP"))
			is_ok = true;
	}
	else
	{
			is_ok = true;
	}

	MzEndWaitDlg();
	return is_ok;
}

bool CM8WeiboNetworkEnv::stop(HWND hWnd)
{
	bool is_ok = false;
	MzBeginWaitDlg(hWnd, NULL, true);

	if (RESULT_OK == Dial_StopGprsConnect2(hWnd))
	{
		is_ok = true;
	}

	MzEndWaitDlg();
	return is_ok;
}

bool CM8WeiboNetworkEnv::redial(HWND hWnd)
{
	return start(hWnd, m_ne_type);
}