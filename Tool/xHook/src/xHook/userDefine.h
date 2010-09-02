#pragma once


//IP��ַ���ַ���󳤶�
#define IP_SIZE			24
#define KEY_SIZE			64
#define MAX_WORLDCOUNT			256
#define TIMELENTH			23

//�������ڿ�ִ���ļ��н��Ǻ�����ʾ������ǰҪ�ĳ����KEY��KEY����С��10���ֽ�
#define GAMESERVER_TO_CLIENT_KEY "�������˶Կͻ��˵ķ����Կ"
#define CLIENT_TO_GAMESERVER_KEY "�������˶Կͻ��˵ķ����Կ"
#define LOGIN_TO_CLIENT_KEY      "�������˶Կͻ��˵ķ����Կ"
#define CLIENT_TO_LOGIN_KEY      "�������˶Կͻ��˵ķ����Կ"
#define OSTREAM_KEY              "�ײ�ȫ�ַ����ͨ����Կ"		
//�������ڿ�ִ���ļ��н��Ǻ�����ʾ������ǰҪ�ĳ����KEY��KEY����С��10���ֽ�

#define PACK_COMPART "$-$"//����ָ���
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