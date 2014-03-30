#pragma once



namespace Util
{
	BOOL ReadFile(LPCTSTR szFilePath, LPBYTE& pData, DWORD& dwLength);
	BOOL WriteFile(LPCTSTR szFilePath, LPCVOID pData, DWORD dwLength);

    BOOL ReadFileAsString(LPCTSTR szFilePath, CString& strData);
    BOOL WriteStringToFile(LPCTSTR szFilePath, const CString& strData, DWORD dwCodePage);
    
    CString BrowseForSaveFile(HWND hWnd, LPCTSTR szFilter, LPCTSTR szDefExt);
    CString BrowseForOpenFile(HWND hWnd, LPCTSTR szFilter);
};
