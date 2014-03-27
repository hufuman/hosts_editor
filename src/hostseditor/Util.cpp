#include "StdAfx.h"
#include "Util.h"



namespace Util
{
	BOOL ReadFile(LPCTSTR szFilePath, LPBYTE& pData, DWORD& dwLength)
	{
		pData = NULL;
		dwLength = 0;

		HANDLE hFile = ::CreateFile(szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
			return FALSE;

		BOOL bResult = FALSE;
		for(;;)
		{
			// Size
			dwLength = ::GetFileSize(hFile, NULL);
			if(dwLength == 0)
			{
				bResult = TRUE;
				break;
			}

			// Buffer
			pData = static_cast<LPBYTE>(malloc(dwLength));
			if(pData == NULL)
				break;

			DWORD dwRead = 0;
			bResult = ::ReadFile(hFile, pData, dwLength, &dwRead, NULL) && (dwRead == dwLength);
			break;
		}

		::CloseHandle(hFile);
		if(!bResult && pData != NULL)
		{
			free(pData);
			dwLength = 0;
			pData = NULL;
		}
		return bResult;
	}


	BOOL WriteFile(LPCTSTR szFilePath, LPCVOID pData, DWORD dwLength)
	{
		HANDLE hFile = ::CreateFile(szFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
			return FALSE;

		DWORD dwWritten = 0;
		BOOL bResult = ::WriteFile(hFile, pData, dwLength, &dwWritten, NULL) && (dwWritten == dwLength);
		::CloseHandle(hFile);
		return bResult;
	}
};
