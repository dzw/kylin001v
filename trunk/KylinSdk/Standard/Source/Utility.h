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
	WIN32_FIND_DATA  wfd;
	KBOOL rValue = false;
	HANDLE hFind = FindFirstFile(strPath.c_str(), &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
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
	GetCurrentDirectory(MAX_PATH,szFileName);
	
	strcat(szFileName,"/dump");
	
	if(!CheckFolderExist(szFileName))
		CreateDirectory( szFileName, NULL );

	StringCchPrintf( szFileName, MAX_PATH, "%s/%04d%02d%02d-%02d%02d%02d.dmp", 
		szFileName,stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);
	hDumpFile = CreateFile(szFileName, GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	ExpParam.ThreadId = GetCurrentThreadId();
	ExpParam.ExceptionPointers = pExceptionPointers;
	ExpParam.ClientPointers = TRUE;

	bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
		hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);

	return EXCEPTION_EXECUTE_HANDLER;
}

namespace Kylin
{
	/** Converts KFLOAT in uint32 format to a a half in uint16 format
	*/
	static inline uint16 floatToHalfI(uint32 i)
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
	static inline uint32 halfToFloatI(uint16 y)
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
	static inline KFLOAT halfToFloat(uint16 y)
	{
		uint32 r = halfToFloatI(y);
		return *reinterpret_cast<KFLOAT*>(&r);
	}

	/** Convert a float32 to a float16 (NV_half_float)
	Courtesy of OpenEXR
	*/
	static inline uint16 floatToHalf(KFLOAT i)
	{
		return floatToHalfI(*reinterpret_cast<uint32*>(&i));
	}
}