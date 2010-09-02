#pragma once


//IP地址的字符最大长度
#define IP_SIZE			24
#define KEY_SIZE			64
#define MAX_WORLDCOUNT			256
#define TIMELENTH			23

//↓这样在可执行文件中将是汉字显示，上线前要改成随机KEY↓KEY不得小于10个字节
#define GAMESERVER_TO_CLIENT_KEY "服务器端对客户端的封包密钥"
#define CLIENT_TO_GAMESERVER_KEY "服务器端对客户端的封包密钥"
#define LOGIN_TO_CLIENT_KEY      "服务器端对客户端的封包密钥"
#define CLIENT_TO_LOGIN_KEY      "服务器端对客户端的封包密钥"
#define OSTREAM_KEY              "底层全局封包流通用密钥"		
//↑这样在可执行文件中将是汉字显示，上线前要改成随机KEY↑KEY不得小于10个字节

#define PACK_COMPART "$-$"//封包分隔符
#define PACK_COMPART_SIZE strlen(PACK_COMPART)

#ifndef ENCRYPT
#define ENCRYPT(x,xlen,KEY,BeginPlace)	if( (x)!=NULL ) \
		{ \
		\
		CHAR* t_pBuffer = (x); \
		CHAR* pKey = {KEY}; \
		UINT KeyLen = (UINT)strlen(pKey); \
		for (UINT i = 0; i < (xlen); i++) \
			{ \
			*t_pBuffer ^= pKey[(i+BeginPlace)%KeyLen]; \
			t_pBuffer++; \
			} \
		}
#endif

#ifndef ENCRYPT_HEAD
#define ENCRYPT_HEAD(x,KEY)	if( (x)!=NULL ) \
		{ \
		CHAR* t_pBuffer = (x); \
		CHAR* pKey = {KEY}; \
		UINT KeyLen = (UINT)strlen(pKey); \
		for (UINT i = 0; i < PACKET_HEADER_SIZE; i++) \
			{ \
			*t_pBuffer ^= pKey[i%KeyLen]; \
			t_pBuffer++; \
			} \
		}
#endif