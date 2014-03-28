#pragma once



namespace Util
{
	BOOL ReadFile(LPCTSTR szFilePath, LPBYTE& pData, DWORD& dwLength);
	BOOL WriteFile(LPCTSTR szFilePath, LPCVOID pData, DWORD dwLength);
    
    CString BrowseForSaveFile(HWND hWnd, LPCTSTR szFilter, LPCTSTR szDefExt);
    CString BrowseForOpenFile(HWND hWnd, LPCTSTR szFilter);
};
