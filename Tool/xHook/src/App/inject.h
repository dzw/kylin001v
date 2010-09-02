/***************************************************************|
						code ripped from jiurl at mail.china.com
						modifed by eyas <eyas at xfocus.org>
/***************************************************************/
#pragma once


#include <winsock2.h>
#include <stdio.h>
#include <tlhelp32.h>
#include  <psapi.h>

#pragma comment (lib,"Advapi32.lib")
#pragma comment (lib,"psapi.lib")

HMODULE g_hRemoteModule = NULL;
HANDLE g_hRemoteProcess = NULL;

BOOL SetPrivilege()
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	HANDLE hToken;

	if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&hToken))
	{
		printf("\nOpen Current Process Token failed:%d",GetLastError());
	}

	if(!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&luid))
	{
		printf("\nLookupPrivilegeValue error:%d", GetLastError() ); 
		return FALSE; 
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	//if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//else
	//	tp.Privileges[0].Attributes = 0;
	// Enable the privilege or disable all privileges.
	AdjustTokenPrivileges(
		   hToken, 
		   FALSE, 
		   &tp, 
		   sizeof(TOKEN_PRIVILEGES), 
		   (PTOKEN_PRIVILEGES) NULL, 
		   (PDWORD) NULL); 
 	// Call GetLastError to determine whether the function succeeded.
	if (GetLastError() != ERROR_SUCCESS) 
	{ 
		printf("AdjustTokenPrivileges failed: %u\n", GetLastError() ); 
		return FALSE; 
	} 
	return TRUE;
}
// void main(int argc, char **argv)
// {
// 	DWORD pid;
// 	HANDLE hSnapshot = NULL;
// 	PROCESSENTRY32 pe;
// 
// 	if(argc!=3)
// 	{
// 		printf( "code ripped from jiurl <jiurl at mail.china.com>\n"
// 				"modifed by eyas <eyas at xfocus.org>\n"
// 				"Usage: %s <dll_full_path)> <pid>\n\n"
// 				"pid != 0	-> inject dll to specify process\n"
// 				"pid == 0	-> inject dll to all process\n", argv[0]);
// 		return;
// 	}
// 	SetPrivilege();
// 
// 	pid = atoi(argv[2]);
// 	if(pid)
// 		inject(pid, argv[1]);
// 	//inject to all process
// 	else
// 	{
// 		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);
// 		pe.dwSize = sizeof(PROCESSENTRY32);
// 
// 		Process32First(hSnapshot,&pe);
// 		do
// 		{
// 			inject(pe.th32ProcessID, argv[1]);
// 		}
// 		while(Process32Next(hSnapshot,&pe)==TRUE);
// 
// 		CloseHandle (hSnapshot);
// 	}
// }

// 判断该进程是否已经被注入
HMODULE hasModules( DWORD processID, char* dll)
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;

	// Get a list of all the modules in this process.
	hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID );
	if (NULL == hProcess)
		return NULL;

	if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for ( i = 0; i  < (cbNeeded / sizeof(HMODULE)); i++ )
		{
			TCHAR szModName[MAX_PATH];
			memset(szModName,0,MAX_PATH);
			// Get the full path to the module's file.
			if ( GetModuleFileNameEx(hProcess, hMods[i], szModName,
				sizeof(szModName)/sizeof(TCHAR)))
			{
				if (strncmp(szModName,dll,sizeof(szModName)/sizeof(TCHAR)) == 0)
				{					
					break;
				}
			}
		}
	}
	CloseHandle( hProcess );
	
	if (i < (cbNeeded / sizeof(HMODULE)))
		return hMods[i];

	return NULL;
}

// 向指定进程注入代码
int inject(DWORD pid, char *dll = "xhook.dll")
{
	PWSTR pszLibFileRemote = NULL;

	g_hRemoteProcess = OpenProcess(
		 PROCESS_QUERY_INFORMATION |   // Required by Alpha
         PROCESS_CREATE_THREAD     |   // For CreateRemoteThread
         PROCESS_VM_OPERATION      |   // For VirtualAllocEx/VirtualFreeEx
         PROCESS_VM_WRITE,             // For WriteProcessMemory
         FALSE, pid);

	char CurPath[256] = {0};
	GetCurrentDirectory(256,CurPath);
	strcat(CurPath,"\\");
	strcat(CurPath, dll);

	if ( (g_hRemoteModule = hasModules(pid,CurPath)) )
		return -1;

	int len = (strlen(CurPath)+1)*2;
	WCHAR wCurPath[256];
	MultiByteToWideChar(CP_ACP,0,CurPath,-1,wCurPath,256);

	pszLibFileRemote = (PWSTR) 
		VirtualAllocEx(g_hRemoteProcess, NULL, len, MEM_COMMIT, PAGE_READWRITE);

	BOOL ret = WriteProcessMemory(g_hRemoteProcess, pszLibFileRemote, 
		(PVOID) wCurPath, len, NULL);
	
	if (!ret)
		return -1;

	PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
         GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");

	HANDLE hRemoteThread = CreateRemoteThread(g_hRemoteProcess, NULL, 0, 
		pfnThreadRtn, pszLibFileRemote, 0, NULL);

	if(hRemoteThread == NULL)
		printf("[-] inject \"%s\" to %d failed.\n", dll, pid);
	else
		printf("[+] inject \"%s\" to %d success.\n", dll, pid);

	return 0;
}

void unload()
{
	if (!g_hRemoteModule)
		return;

	//HMODULE hFindModule=NULL;

	LPTHREAD_START_ROUTINE pFreeLibrary=(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")),"FreeLibrary");

	__try
	{
		// 鸡冻人心的时刻
		HANDLE hRemoteThread = CreateRemoteThread(g_hRemoteProcess,NULL,0,pFreeLibrary,g_hRemoteModule,0,NULL);
		if (NULL==hRemoteThread)
		{
			MessageBox(NULL,"free library error!","Prompt",MB_OK);
			__leave;
		}
		WaitForSingleObject(hRemoteThread,INFINITE);
	}
	__finally
	{
		if (NULL!=g_hRemoteProcess)
		{
			CloseHandle(g_hRemoteProcess);
			g_hRemoteProcess=NULL;
		}
	}

}