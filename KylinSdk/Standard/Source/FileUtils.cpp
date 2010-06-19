#include "stdpch.h"
#include "FileUtils.h"

//////////////////////////////////////////////////////////////////////////
KSTR   FileUtils::m_sExePath = "";

KVOID FileUtils::GetFileList( KSTR path, KStrVector &list, KBOOL bSearchSubDir )
{
	HANDLE hFile; // Handle to file
	WIN32_FIND_DATAA FileInformation; // File information
	
	KSTR sFilePath;
	KSTR sPattern = path + "\\*.*";
	hFile = ::FindFirstFileA(sPattern.c_str(), &FileInformation);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(FileInformation.cFileName[0] != '.')
			{
				if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					
				}
				else
				{
					//sFilePath.erase();
					//sFilePath = path + "\\" + FileInformation.cFileName;
					// Save filename
					list.push_back(FileInformation.cFileName);
				}
			}
		} while(::FindNextFileA(hFile, &FileInformation) == TRUE);

		// Close handle
		::FindClose(hFile);

// 		DWORD dwError = ::GetLastError();
// 		if(dwError != ERROR_NO_MORE_FILES)
// 			return dwError;
	}
}