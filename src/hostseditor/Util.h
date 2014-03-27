#pragma once



namespace Util
{
	BOOL ReadFile(LPCTSTR szFilePath, LPBYTE& pData, DWORD& dwLength);
	BOOL WriteFile(LPCTSTR szFilePath, LPCVOID pData, DWORD dwLength);
};
