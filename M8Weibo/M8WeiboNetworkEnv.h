#pragma once

#include <mzfc_inc.h>

class CM8WeiboNetworkEnv
{
public:
	CM8WeiboNetworkEnv(void);
	~CM8WeiboNetworkEnv(void);
	
	enum tagNE
	{
		NE_CMNONE,
		NE_CMNET,
		NE_CMWAP,
		NE_UNINET,
		NE_UNIWAP,
	};

	bool start(HWND hWnd, tagNE ne);
	bool stop(HWND hWnd);

	bool redial(HWND hWnd);

	CM8WeiboNetworkEnv::tagNE NeType() const { return m_ne_type; }
	void NeType(CM8WeiboNetworkEnv::tagNE val) { m_ne_type = val; }

private:
	tagNE m_ne_type;
};
