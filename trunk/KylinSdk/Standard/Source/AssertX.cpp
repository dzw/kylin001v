//--------------------------------------------------------------------------------
// 
// Filename   : Assert.cpp 
//
//--------------------------------------------------------------------------------

// include files
#include "stdpch.h"
#include "Assertx.h"
#include <time.h>
#include "Utility.h"
#ifdef __LINUX__
#include <execinfo.h>
#endif


VOID __show__( const CHAR* szTemp )
{

#ifdef __LINUX__
	printf("Assert:%s",szTemp);
#endif

// 	//������־
// #ifndef GAME_CLIENT
// 	FILE* f = fopen( "./Log/assert.log", "a" ) ;
// 	fwrite( szTemp, 1, strlen(szTemp), f ) ;
// 	fwrite( "\r\n", 1, 2, f ) ;
// 	fclose(f) ;
// #endif

#if defined(WIN32)
	static MyLock lock ;
	//if( g_Command_Assert!=1 )
	{
		lock.Lock() ;
		INT iRet = ::MessageBoxA( NULL, szTemp, "�쳣", MB_OK ) ;
		lock.Unlock() ;
	}
#elif defined(__LINUX__)

#endif

// #ifdef _DEBUG
// 	throw(std::string(szTemp));
// #else
// 	throw(1);
// #endif
}

VOID __messagebox__(const CHAR*msg )
{
#if defined(WIN32)
	::MessageBoxA( NULL, msg, "��Ϣ", MB_OK ) ;
#elif defined(__LINUX__)
#endif
}
//--------------------------------------------------------------------------------
//
// __assert__
//
//
//--------------------------------------------------------------------------------
VOID __assert__ ( const CHAR * file , UINT line , const CHAR * func , const CHAR * expr )
{
	CHAR szTemp[1024] = {0};

#ifdef __LINUX__ //������ʽ
	sprintf( szTemp, "[%s][%d][%s][%s]\n", file, line, func, expr ) ;
#else
	sprintf( szTemp, "[%s][%d][%s][%s]", file, line, func, expr ) ;
#endif
	__show__(szTemp) ;
}

VOID __assertex__ ( const CHAR * file , UINT line , const CHAR * func , const CHAR * expr ,const CHAR* msg)
{
	CHAR szTemp[1024] = {0};

#ifdef __LINUX__
	sprintf( szTemp, "[%s][%d][%s][%s]\n[%s]\n", file, line, func, expr ,msg ) ;
#else
	sprintf( szTemp, "[%s][%d][%s][%s]\n[%s]", file, line, func, expr ,msg ) ;
#endif
	__show__(szTemp) ;
}

VOID __assertspecial__ ( const CHAR * file , UINT line , const CHAR * func , const CHAR * expr ,const CHAR* msg)
{
	CHAR szTemp[1024] = {0};

#ifdef __LINUX__
	sprintf( szTemp, "S[%s][%d][%s][%s]\n[%s]\n", file, line, func, expr ,msg ) ;
#else
	sprintf( szTemp, "S[%s][%d][%s][%s]\n[%s]", file, line, func, expr ,msg ) ;
#endif
	__show__(szTemp) ;
}
//--------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------
VOID __protocol_assert__ ( const CHAR * file , UINT line , const CHAR * func , const CHAR * expr )
{
	printf( "[%s][%d][%s][%s]", file, line, func, expr ) ;
}
