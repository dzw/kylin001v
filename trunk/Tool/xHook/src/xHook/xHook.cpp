// xHook.cpp : Defines the entry point for the DLL application.
//

/*
HOOK方法：COPY原DLL，修改函数入口点。

2004-06-20
1)基本完成，未做更多测试。

2004-08-03
1)增加HOOK函数gethostbyname
*/
#include "stdafx.h"
#include "xHook.h"
#include <malloc.h>
#include <stdio.h>
#include <Psapi.h>
#include <winsock2.h>
#include <Tlhelp32.h>
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"psapi")

#pragma pack(1)

#define __BeginFilterPacket(s,buf,len)  	char* newbuf = new char[len]; \
	memset(newbuf,0,len); \
	memcpy(newbuf,buf,len); \
	filterPacket(s,newbuf,len);

#define __EndFilterPacket() delete[] newbuf;

typedef struct
{
	BYTE   mov_eax;//0xB8
	LPVOID address;
	WORD   jump_eax;//0xE0FF
} ASMJUMP, *PASMJUMP;

typedef struct
{
	HMODULE		hModule;
	LPVOID		lpNewBaseOfDll;
	MODULEINFO	modinfo;
}DLLINFO, *PDLLINFO;

DWORD		g_dwPID;
char		g_szProcessName[128] = {0};
char		g_szSimpleLogFile[128] = {0};
char		g_szDetailLogFile[128] = {0};
DLLINFO		ws2_32;
DWORD		new_send, new_WSASend, new_recv, new_WSARecv;
DWORD		new_connect, new_WSAConnect, new_gethostbyname;
DWORD		new_recvfrom, new_sendto, new_WSARecvFrom, new_WSASendTo;

extern void filterPacket(SOCKET s, char* buf, int len);

BOOL pid_2_name(DWORD dwPid, char *pszName, DWORD dwSize);
BOOL LogSimple(char *pMsg, SYSTEMTIME *pST);
BOOL LogData(char *pSimple, USHORT uLocalPort, BYTE *pszData, int bytes);//记录详细数据

BOOL LogError(char *pMsg, DWORD dwErrorCode);
void tcp_log(SOCKET s, char *p, char *p2, int bytes, unsigned char *pszData)
{
	return;
	//------------------------------------------------------------------------
	struct sockaddr_in	sa;
	char	buff[512];
	int		size;
	DWORD	dwError;
	USHORT	uLocalPort;

	dwError = GetLastError();

	if(bytes<0)
		bytes = 0;

	memset(buff, 0, sizeof(buff));
	size = sizeof(sa);
	getsockname(s, (struct sockaddr *)&sa, &size);
	uLocalPort = ntohs(sa.sin_port);
	sprintf(buff, "TCP %s:%d %s ", inet_ntoa(sa.sin_addr), uLocalPort, p2);
	size = sizeof(sa);
	getpeername(s, (struct sockaddr *)&sa, &size);
	if(bytes<0)
		bytes = 0;
	sprintf(&buff[strlen(buff)], "%s:%d  %d bytes (%s)", inet_ntoa(sa.sin_addr), ntohs(sa.sin_port), bytes, p);

	LogData(buff, uLocalPort, pszData, bytes);

	SetLastError(dwError);
}

void udp_log(SOCKET s, char *p, char *p2, int bytes, struct sockaddr_in *to,
			 unsigned char *pszData, char *pSign)
{
	return;
	//------------------------------------------------------------------------
	struct sockaddr_in	sa;
	char	buff[512];
	int		size;
	DWORD	dwError;
	USHORT	uLocalPort;

	dwError = GetLastError();

	if(bytes<0)
		bytes = 0;

	memset(buff, 0, sizeof(buff));
	size = sizeof(sa);
	getsockname(s, (struct sockaddr *)&sa, &size);
	uLocalPort = ntohs(sa.sin_port);
	sprintf(buff, "%s %s:%d %s ",pSign, inet_ntoa(sa.sin_addr), uLocalPort, p2);

	if(bytes<0)
		bytes = 0;
	sprintf(&buff[strlen(buff)], "%s:%d  %d bytes (%s)", inet_ntoa(to->sin_addr), ntohs(to->sin_port), bytes, p);

	LogData(buff, uLocalPort, pszData, bytes);

	SetLastError(dwError);
}
BOOL LogData(char *pSimple, USHORT uLocalPort, BYTE *pszData, int bytes)
{

	SYSTEMTIME	st;
	FILE		*stream = NULL;
	int			i, j;
	BYTE		*pszAscii = NULL;
	char		*pszHex = NULL;
	BOOL		bRet = FALSE;

	GetLocalTime(&st);
	LogSimple(pSimple, &st);

	__try
	{
		if( (!bytes) || (!pszData) )
			__leave;

		if((stream = fopen(g_szDetailLogFile, "a")) ==NULL)
		{
			LogError("LogData:fopen()", GetLastError());
			__leave;
		}

		//转换为可见字符
		pszAscii = (BYTE *)malloc(bytes+1);
		if(!pszAscii)
		{
			LogError("LogData:malloc() for ascii", GetLastError());
			__leave;
		}
		memcpy(pszAscii, pszData, bytes);
		
		for(i=0;i<bytes;i++)
		{
			if( ((BYTE)pszAscii[i] < (BYTE)'\x20') || 
				((BYTE)pszAscii[i] > (BYTE)'\x7E') )
				(BYTE)pszAscii[i] = (BYTE)'.';
		}
		pszAscii[i] = '\x0';

		//转换为16进制
		pszHex = (char *)malloc(bytes*3 + 4);
		if(!pszHex)
		{
			LogError("LogData:malloc() for hex", GetLastError());
			__leave;
		}
		for(i=0,j=0;i<bytes;i++)
		{
			j += sprintf(&pszHex[j], " %.2X", (BYTE)pszData[i]);
		}

		fprintf(stream, "\n#%.5d %.2d-%.2d-%.2d %.2d:%.2d:%.2d %s\n", 
				uLocalPort,
				st.wYear, st.wMonth, st.wDay,
				st.wHour, st.wMinute, st.wSecond,
				pSimple);
	
		//记录到文件
		for(i=0,j=0;i<bytes;i+=0x10,j+=0x30)
		{
			fprintf(stream, "#%.5d %-48.48s    %-16.16s\n", 
				uLocalPort, &pszHex[j], &pszAscii[i]);
		}
	}//end of try
	__finally
	{
		if(stream)
			fclose(stream);
		if(pszAscii)
			free(pszAscii);
		if(pszHex)
			free(pszHex);
	}
	return TRUE;
}
int __stdcall hack_WSARecvFrom(
  SOCKET s,
  LPWSABUF lpBuffers,
  DWORD dwBufferCount,
  LPDWORD lpNumberOfBytesRecvd,
  LPDWORD lpFlags,
  struct sockaddr FAR *lpFrom,
  LPINT lpFromlen,
  LPWSAOVERLAPPED lpOverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
)
{
	int		retvalue;

	//调用原函数
	__asm
	{
		push	lpCompletionRoutine
		push	lpOverlapped
		push	lpFromlen
		push	lpFrom
		push	lpFlags
		push	lpNumberOfBytesRecvd
		push	dwBufferCount
		push	lpBuffers
		push	s
		mov		eax, new_WSARecvFrom
		call	eax
		mov		retvalue,eax
	}

	//记录信息
	udp_log(s, "WSARecvFrom", "<-", *lpNumberOfBytesRecvd, 
		(struct sockaddr_in *)lpFrom, (BYTE *)lpBuffers->buf, "UDP");

	return retvalue;
}

int __stdcall hack_recvfrom(
  SOCKET s,                   
  char FAR* buf,              
  int len,                    
  int flags,                  
  struct sockaddr FAR *from,  
  int FAR *fromlen                          
)
{
	int		retvalue;

	//调用原函数
	__asm
	{
		push	fromlen
		push	from
		push	flags
		push	len
		push	buf
		push	s
		mov		eax, new_recvfrom
		call	eax
		mov		retvalue, eax
	}
	//记录信息
	udp_log(s, "recvfrom", "<-", retvalue, (struct sockaddr_in *)from, (BYTE *)buf, "UDP");

	return retvalue;
}

int __stdcall hack_sendto(
  SOCKET s,                        
  const char FAR *buf,            
  int len,                         
  int flags,                       
  const struct sockaddr FAR *to,  
  int tolen                        
)
{
	int		retvalue;

	//调用原函数
	__asm
	{
		push	tolen
		push	to
		push	flags
		push	len
		push	buf
		push	s
		mov		eax, new_sendto
		call	eax
		mov		retvalue, eax
	}
	//记录信息
	udp_log(s, "sendto", "->", retvalue, (struct sockaddr_in *)to, (BYTE *)buf, "UDP");

	return retvalue;
}

int __stdcall hack_WSASendTo(
  SOCKET s,
  LPWSABUF lpBuffers,
  DWORD dwBufferCount,
  LPDWORD lpNumberOfBytesSent,
  DWORD dwFlags,
  const struct sockaddr FAR *lpTo,
  int iToLen,
  LPWSAOVERLAPPED lpOverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
)
{
	int		retvalue;

	//调用原函数
	__asm
	{
		push	lpCompletionRoutine
		push	lpOverlapped
		push	iToLen
		push	lpTo
		push	dwFlags
		push	lpNumberOfBytesSent
		push	dwBufferCount
		push	lpBuffers
		push	s
		mov		eax, new_WSASendTo
		call	eax
		mov		retvalue,eax
	}
	udp_log(s, "WSASendTo", "->", *lpNumberOfBytesSent, (struct sockaddr_in *)lpTo,
		(BYTE *)lpBuffers->buf, "UDP");

	return retvalue;
}

int __stdcall hack_WSASend(
  SOCKET s,
  LPWSABUF lpBuffers,
  DWORD dwBufferCount,
  LPDWORD lpNumberOfBytesSent,
  DWORD dwFlags,
  LPWSAOVERLAPPED lpOverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
)
{
	int		retvalue;

	//调用原函数
	__asm
	{
		push	lpCompletionRoutine
		push	lpOverlapped
		push	dwFlags
		push	lpNumberOfBytesSent
		push	dwBufferCount
		push	lpBuffers
		push	s
		mov		eax, new_WSASend
		call	eax
		mov		retvalue,eax
	}

	//记录信息
	tcp_log(s, "WSASend", "->", *lpNumberOfBytesSent, (BYTE *)lpBuffers->buf);

	return retvalue;
}

int __stdcall hack_WSARecv(
  SOCKET s,
  LPWSABUF lpBuffers,
  DWORD dwBufferCount,
  LPDWORD lpNumberOfBytesRecvd,
  LPDWORD lpFlags,
  LPWSAOVERLAPPED lpOverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
)
{
	int		retvalue;

	//调用原函数
	__asm
	{
		push	lpCompletionRoutine
		push	lpOverlapped
		push	lpFlags
		push	lpNumberOfBytesRecvd
		push	dwBufferCount
		push	lpBuffers
		push	s
		mov		eax, new_WSARecv
		call	eax
		mov		retvalue, eax
	}

	//记录信息
	tcp_log(s, "WSARecv", "<-", *lpNumberOfBytesRecvd, (BYTE *)lpBuffers->buf);

	return retvalue;
}


int __stdcall hack_recv(
  SOCKET s,       
  char FAR *buf,  
  int len,        
  int flags       
)
{
	int		retvalue;

	//调用原函数
	__asm
	{
		push	flags
		push	len
		push	buf
		push	s
		mov		eax, new_recv
		call	eax
		mov		retvalue, eax
	}
	//记录信息
	tcp_log(s, "recv", "<-", retvalue, (BYTE *)buf);

	return retvalue;
}

/*由调用者将参数从堆栈清除*/
int __stdcall hack_connect( 
	SOCKET s,                          
  const struct sockaddr FAR *name,  
  int namelen             
)
{
	int		retvalue;

	//调用原函数
	__asm
	{
		push	namelen
		push	name
		push	s
		mov		eax, new_connect
		call	eax
		mov		retvalue, eax
	}

	//记录信息
	udp_log(s, "connect", "->", retvalue, (struct sockaddr_in *)name, 0, "TCP");

	return retvalue;
}

/*由调用者将参数从堆栈清除*/
int __stdcall hack_WSAConnect(
	SOCKET s,                          
  const struct sockaddr FAR *name,  
  int namelen,                       
  LPWSABUF lpCallerData,             
  LPWSABUF lpCalleeData,             
  LPQOS lpSQOS,                      
  LPQOS lpGQOS 
)
{
	int		retvalue;

	//调用原函数
	__asm
	{
		push	lpGQOS
		push	lpSQOS
		push	lpCalleeData
		push	lpCallerData
		push	namelen
		push	name
		push	s
		mov		eax, new_WSAConnect
		call	eax
		mov		retvalue, eax
	}
	//记录信息
	udp_log(s, "WSAConnect", "->", retvalue, (struct sockaddr_in *)name, 0, "TCP");

	return retvalue;
}

struct hostent FAR * __stdcall hack_gethostbyname(
  const char FAR *name  
)
{
	SYSTEMTIME	st;
	char		buff[1024];
	struct hostent FAR	*p;

	GetLocalTime(&st);
	_snprintf(buff, sizeof(buff)-1, "gethostbyname <%s>", name);
	LogSimple(buff, &st);
	__asm
	{
		push	name
		mov		eax,new_gethostbyname
		call	eax
		mov		p, eax
	}
	return p;
}

/*由调用者将参数从堆栈清除*/
int __stdcall hack_send(SOCKET s,              
  const char FAR *buf,  
  int len,               
  int flags              
)
{
	int		retvalue;
	__BeginFilterPacket(s,buf,len)

	//调用原函数
	__asm
	{
		push	flags
		push	len
		push	newbuf
		push	s
		mov		eax, new_send
		call	eax
		mov		retvalue, eax
	}
	//记录信息
	tcp_log(s, "send", "->", retvalue, (BYTE *)newbuf);
	
	__EndFilterPacket()

	return retvalue;
}

DWORD WINAPI mainfn(LPVOID lp);

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			CreateThread(0, 0, mainfn, 0, 0, 0);
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

int hook_api(PDLLINFO pDllInfo, char *name, DWORD hackfunc, DWORD *pNewFunc)
{
	DWORD						dw, dwOrigFunc;
	ASMJUMP						jmpcode;
	MEMORY_BASIC_INFORMATION	mbi;

	dwOrigFunc = (DWORD)GetProcAddress(pDllInfo->hModule, name);
	if(dwOrigFunc == NULL)
	{
		LogError("GetProcAddress()", GetLastError());
		return 0;
	}

	if(!VirtualQuery((void *)dwOrigFunc,&mbi,sizeof(MEMORY_BASIC_INFORMATION)))
	{
		LogError("VirtualQuery()", GetLastError());
		return 0;
	}

	//if(!VirtualProtect(mbi.AllocationBase,mbi.RegionSize,
    //               PAGE_EXECUTE_READWRITE,&dw))
	//应该用mbi.BaseAddress
	if(!VirtualProtect(mbi.BaseAddress,mbi.RegionSize,
                   PAGE_EXECUTE_READWRITE,&dw))
	{
		LogError("VirtualProtect()", GetLastError());
		return 0;
	}

	jmpcode.mov_eax = (BYTE)0xB8;
	jmpcode.address = (void *)hackfunc;
	jmpcode.jump_eax = (WORD)0xE0FF;

	//计算原函数COPY的位置
	*pNewFunc = dwOrigFunc - (DWORD)pDllInfo->modinfo.lpBaseOfDll + (DWORD)pDllInfo->lpNewBaseOfDll;

	//修改原函数入口处内容
	memcpy((void *)dwOrigFunc, (unsigned char *)&jmpcode, sizeof(ASMJUMP));

	return 1;
}
BOOL InitDll(char *pszDll, PDLLINFO pDllInfo)
{
	pDllInfo->hModule = GetModuleHandle(pszDll);
	if(!pDllInfo->hModule)
	{
		LogError("GetModuleHandle()", GetLastError());
		return 0;
	}
	if(!GetModuleInformation(GetCurrentProcess(), pDllInfo->hModule, &pDllInfo->modinfo, sizeof(MODULEINFO)))
	{
		LogError("GetModuleInformation()", GetLastError());
		return 0;
	}
	
	pDllInfo->lpNewBaseOfDll = malloc(pDllInfo->modinfo.SizeOfImage);
	if(!pDllInfo->lpNewBaseOfDll)
	{
		LogError("malloc()", GetLastError());
		return 0;
	}
	memcpy(pDllInfo->lpNewBaseOfDll, pDllInfo->modinfo.lpBaseOfDll, pDllInfo->modinfo.SizeOfImage);
	return 1;
}
DWORD WINAPI mainfn(LPVOID lp)
{

	g_dwPID = GetCurrentProcessId();
	pid_2_name(GetCurrentProcessId(), g_szProcessName, sizeof(g_szProcessName));
	_snprintf(g_szSimpleLogFile, sizeof(g_szSimpleLogFile)-1,
		"c:\\%d.%s.simple.log", g_dwPID, g_szProcessName);
	_snprintf(g_szDetailLogFile, sizeof(g_szDetailLogFile)-1,
		"c:\\%d.%s.detail.log", g_dwPID, g_szProcessName);

	InitDll("ws2_32.dll", &ws2_32);

	hook_api(&ws2_32, "connect", (DWORD)hack_connect, &new_connect);
	hook_api(&ws2_32, "WSAConnect", (DWORD)hack_WSAConnect, &new_WSAConnect);

	hook_api(&ws2_32, "send", (DWORD)hack_send, &new_send);
	hook_api(&ws2_32, "WSASend", (DWORD)hack_WSASend, &new_WSASend);
	hook_api(&ws2_32, "recv", (DWORD)hack_recv, &new_recv);
	hook_api(&ws2_32, "WSARecv", (DWORD)hack_WSARecv, &new_WSARecv);
	
	hook_api(&ws2_32, "sendto", (DWORD)hack_sendto, &new_sendto);
	hook_api(&ws2_32, "WSASendTo", (DWORD)hack_WSASendTo, &new_WSASendTo);
	hook_api(&ws2_32, "recvfrom", (DWORD)hack_recvfrom, &new_recvfrom);
	hook_api(&ws2_32, "WSARecvFrom", (DWORD)hack_WSARecvFrom, &new_WSARecvFrom);

	hook_api(&ws2_32, "gethostbyname", (DWORD)hack_gethostbyname, &new_gethostbyname);

	return 0;
}


// This is an example of an exported variable
XHOOK_API int nXHook=0;

// This is an example of an exported function.
XHOOK_API int fnXHook(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see xHook.h for the class definition
CXHook::CXHook()
{ 
	return; 
}

BOOL LogSimple(char *pMsg, SYSTEMTIME *pST)
{
	FILE		*stream;

	if((stream = fopen(g_szSimpleLogFile, "a")) ==NULL) 		
		return FALSE;
	fprintf(stream, "%.2d-%.2d-%.2d %.2d:%.2d:%.2d %s\n", 
		pST->wYear, pST->wMonth, pST->wDay,
		pST->wHour, pST->wMinute, pST->wSecond,
		pMsg);
	fclose(stream);
	return TRUE;
}

BOOL LogError(char *pMsg, DWORD dwErrorCode)
{
	FILE		*stream;
	int			index = 0;
	SYSTEMTIME	st;

	if((stream = fopen(g_szSimpleLogFile, "a")) ==NULL) 		
		return FALSE;

	GetLocalTime(&st);

	if(dwErrorCode)
		fprintf(stream, "%.2d-%.2d-%.2d %.2d:%.2d:%.2d [-] %s error:%d\n", 
			st.wYear, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond,
			pMsg, dwErrorCode);
	else
		fprintf(stream, "%.2d-%.2d-%.2d %.2d:%.2d:%.2d [+] %s\n", 
			st.wYear, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond,
			pMsg, dwErrorCode);

	fclose(stream);
	return TRUE;
}

BOOL pid_2_name(DWORD dwPid, char *pszName, DWORD dwSize) 
{ 
    HANDLE         hProcessSnap = NULL; 
    BOOL           bRet			= FALSE; 
	DWORD		   dwCopy;
    PROCESSENTRY32 pe32			= {0}; 
 
    //  Take a snapshot of all processes in the system. 
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
    if (hProcessSnap == INVALID_HANDLE_VALUE) 
        return (FALSE); 
    //  Fill in the size of the structure before using it. 
    pe32.dwSize = sizeof(PROCESSENTRY32); 
    //  Walk the snapshot of the processes, and for each process, 
    //  display information. 
    if (Process32First(hProcessSnap, &pe32)) 
    { 
        do 
        { 
			if(pe32.th32ProcessID == dwPid)
			{
				dwCopy = dwSize < strlen(pe32.szExeFile) ? dwSize:strlen(pe32.szExeFile);
				dwCopy -= 4;
				strncpy(pszName, pe32.szExeFile, dwCopy);
				bRet = TRUE;
				break;
			}
        } 
        while (Process32Next(hProcessSnap, &pe32)); 
         
    } 
 
    // Do not forget to clean up the snapshot object. 
    CloseHandle (hProcessSnap); 
    return (bRet); 
} 

