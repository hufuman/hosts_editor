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
        ::SetEndOfFile(hFile);
		::CloseHandle(hFile);
		return bResult;
	}

    namespace details
    {
        BYTE g_byUtf8BOM[] = {0xEF, 0xBB, 0xBF};
        BYTE g_byUnicodeBOM[] = {0xFF, 0xFE};

        BOOL ToUnicode(LPBYTE pSrcData, DWORD dwLength, DWORD dwCodePage, CString& strResult)
        {
            strResult = _T("");
            int nBufferSize = ::MultiByteToWideChar(dwCodePage,
                0,
                (LPCSTR)pSrcData,
                dwLength,
                NULL,
                0);
            if(nBufferSize <= 0)
                return (dwLength == 0);

            LPBYTE pBuffer = static_cast<LPBYTE>(malloc(nBufferSize * 2 + 2));
            if(pBuffer == NULL)
                return FALSE;

            nBufferSize = ::MultiByteToWideChar(dwCodePage,
                0,
                (LPCSTR)pSrcData,
                dwLength,
                (LPWSTR)pBuffer,
                nBufferSize);
            if(nBufferSize > 0)
            {
                pBuffer[nBufferSize * 2 + 1] = 0;
                pBuffer[nBufferSize * 2 + 0] = 0;
                strResult = reinterpret_cast<LPCTSTR>(pBuffer);
            }
            free(pBuffer);
            return (nBufferSize > 0);
        }

        BOOL ToAscii(LPBYTE& pDestData, DWORD& dwLength, const CString& strData, DWORD dwCodePage)
        {
            pDestData = NULL;
            dwLength = ::WideCharToMultiByte(dwCodePage,
                0,
                (LPCTSTR)strData,
                strData.GetLength(),
                NULL,
                0,
                NULL, NULL);
            if(dwLength <= 0)
                return (strData.GetLength() == 0);

            pDestData = static_cast<LPBYTE>(malloc(dwLength + 1));
            if(pDestData == NULL)
                return FALSE;

            dwLength = ::WideCharToMultiByte(dwCodePage,
                0,
                (LPCTSTR)strData,
                strData.GetLength(),
                reinterpret_cast<LPSTR>(pDestData),
                dwLength,
                NULL, NULL);
            if(static_cast<int>(dwLength) <= 0)
            {
                free(pDestData);
                pDestData = NULL;
                dwLength = 0;
            }
            else
            {
                pDestData[dwLength] = 0;
            }
            return (dwLength > 0);
        }
    }

    BOOL ReadFileAsString(LPCTSTR szFilePath, CString& strData)
    {
        LPBYTE pData = NULL;
        DWORD dwLength = 0;
        if(!Util::ReadFile(szFilePath, pData, dwLength))
            return FALSE;

        if(dwLength >= 2 && memcmp(details::g_byUnicodeBOM, pData, 2) == 0)
        {
            LPTSTR pDataMem = strData.GetBufferSetLength(dwLength - 2);
            memcpy(pDataMem, pData, dwLength - 2);
            strData.ReleaseBuffer();
        }
        else if(dwLength >= 3 && memcmp(details::g_byUtf8BOM, pData, 3) == 0)
        {
            if(!details::ToUnicode(pData + 3, dwLength - 3, CP_UTF8, strData))
                return FALSE;
        }
        else
        {
            if(!details::ToUnicode(pData, dwLength, CP_ACP, strData))
                return FALSE;
        }
        return TRUE;
    }

    BOOL WriteStringToFile(LPCTSTR szFilePath, const CString& strData, DWORD dwCodePage)
    {
        LPBYTE pDestData = NULL;
        DWORD dwLength = 0;
        if(!details::ToAscii(pDestData, dwLength, strData, dwCodePage))
            return FALSE;
        BOOL bResult = Util::WriteFile(szFilePath, pDestData, dwLength);
        free(pDestData);
        return bResult;
    }

    namespace details
    {
        CString BrowseForFile(BOOL bOpen, HWND hWnd, LPCTSTR szFilter, LPCTSTR szDefExt)
        {
            CString strResult;
            CFileDialog dlg(bOpen, 0, 0, OFN_NOCHANGEDIR | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_NODEREFERENCELINKS);
            dlg.m_ofn.lpstrFilter = szFilter;
            dlg.m_ofn.lpstrDefExt = szDefExt;
            if(dlg.DoModal(hWnd) != IDOK)
                return strResult;

            strResult = dlg.m_szFileName;
            return strResult;
        }
    }
    
    CString BrowseForSaveFile(HWND hWnd, LPCTSTR szFilter, LPCTSTR szDefExt)
    {
        return details::BrowseForFile(FALSE, hWnd, szFilter, szDefExt);
    }

    CString BrowseForOpenFile(HWND hWnd, LPCTSTR szFilter)
    {
        return details::BrowseForFile(TRUE, hWnd, szFilter, NULL);
    }
};
