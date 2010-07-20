#pragma once

#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#include <strsafe.h>

#pragma comment( lib, "DbgHelp" )

// 检查是否游戏被调试
// KBOOL AntiHackIsDebuggerPresent(KVOID)
// {
// 	KBOOL bRet = true;
// 	__asm
// 	{
// 		MOV EAX, FS:[00000018H]
// 		MOV EAX, KDWORD PTR[EAX+30H]
// 		MOVZX EAX, KBYTE PTR[EAX+002H]
// 		MOV bRet, EAX
// 	}
// 	return (bRet);
// }

///目录是否存在的检查：
inline KBOOL CheckFolderExist(KSTR strPath)
{
	LPWIN32_FIND_DATAA  wfd;
	KBOOL rValue = false;
	HANDLE hFind = FindFirstFileA(strPath.c_str(), wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = true;   
	}
	FindClose(hFind);
	return rValue;
}

//////////////////////////////////////////////////////////////////////////
inline KINT GenerateDump(EXCEPTION_POINTERS* pExceptionPointers)
{
	KBOOL bMiniDumpSuccessful;
	KCHAR szFileName[MAX_PATH]; 
	HANDLE hDumpFile;
	SYSTEMTIME stLocalTime;
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;

	GetLocalTime( &stLocalTime );
	GetCurrentDirectoryA(MAX_PATH,szFileName);
	
	strcat(szFileName,"/dump");
	
	if(!CheckFolderExist(szFileName))
		CreateDirectoryA( szFileName, NULL );

	StringCchPrintfA( szFileName, MAX_PATH, "%s/%04d%02d%02d-%02d%02d%02d.dmp", 
		szFileName,stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);
	hDumpFile = CreateFileA(szFileName, GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	ExpParam.ThreadId = GetCurrentThreadId();
	ExpParam.ExceptionPointers = pExceptionPointers;
	ExpParam.ClientPointers = TRUE;

	bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
		hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);

	return EXCEPTION_EXECUTE_HANDLER;
}

//共享锁
#if defined(WIN32)
class MyLock
{
	CRITICAL_SECTION m_Lock ;
public :
	MyLock( ){ InitializeCriticalSection(&m_Lock); } ;
	~MyLock( ){ DeleteCriticalSection(&m_Lock); } ;
	VOID	Lock( ){ EnterCriticalSection(&m_Lock); } ;
	VOID	Unlock( ){ LeaveCriticalSection(&m_Lock); } ;
};
#endif

namespace Kylin
{
	/** Converts KFLOAT in uint32 format to a a half in uint16 format
	*/
	static inline unsigned short floatToHalfI(unsigned int i)
	{
		register KINT s =  (i >> 16) & 0x00008000;
		register KINT e = ((i >> 23) & 0x000000ff) - (127 - 15);
		register KINT m =   i        & 0x007fffff;

		if (e <= 0)
		{
			if (e < -10)
			{
				return 0;
			}
			m = (m | 0x00800000) >> (1 - e);

			return s | (m >> 13);
		}
		else if (e == 0xff - (127 - 15))
		{
			if (m == 0) // Inf
			{
				return s | 0x7c00;
			} 
			else    // NAN
			{
				m >>= 13;
				return s | 0x7c00 | m | (m == 0);
			}
		}
		else
		{
			if (e > 30) // Overflow
			{
				return s | 0x7c00;
			}

			return s | (e << 10) | (m >> 13);
		}
	}

	/** Converts a half in uint16 format to a KFLOAT
	in uint32 format
	*/
	static inline unsigned int halfToFloatI(unsigned short y)
	{
		register KINT s = (y >> 15) & 0x00000001;
		register KINT e = (y >> 10) & 0x0000001f;
		register KINT m =  y        & 0x000003ff;

		if (e == 0)
		{
			if (m == 0) // Plus or minus zero
			{
				return s << 31;
			}
			else // Denormalized number -- renormalize it
			{
				while (!(m & 0x00000400))
				{
					m <<= 1;
					e -=  1;
				}

				e += 1;
				m &= ~0x00000400;
			}
		}
		else if (e == 31)
		{
			if (m == 0) // Inf
			{
				return (s << 31) | 0x7f800000;
			}
			else // NaN
			{
				return (s << 31) | 0x7f800000 | (m << 13);
			}
		}

		e = e + (127 - 15);
		m = m << 13;

		return (s << 31) | (e << 23) | m;
	}

	/**
	* Convert a float16 (NV_half_float) to a float32
	* Courtesy of OpenEXR
	*/
	static inline KFLOAT halfToFloat(unsigned short y)
	{
		unsigned int r = halfToFloatI(y);
		return *reinterpret_cast<KFLOAT*>(&r);
	}

	/** Convert a float32 to a float16 (NV_half_float)
	Courtesy of OpenEXR
	*/
	static inline unsigned short floatToHalf(KFLOAT i)
	{
		return floatToHalfI(*reinterpret_cast<unsigned int*>(&i));
	}
}