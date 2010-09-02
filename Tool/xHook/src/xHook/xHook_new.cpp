#include "stdafx.h"
#include "xHook.h"
#include <winsock2.h>

#include "userDefine.h"


extern void tcp_log(SOCKET s, char *p, char *p2, int bytes, unsigned char *pszData);

void filterPacket(SOCKET s, char* buf, int len)
{
	
	buf[15] = 'w';
	buf[16] = 'w';

}