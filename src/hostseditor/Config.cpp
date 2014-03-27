#include "StdAfx.h"
#include "Config.h"

#include "Util.h"

CConfig::CConfig(void)
{
}

CConfig::~CConfig(void)
{
	Clear();
}

CConfig& CConfig::instance()
{
    static CConfig inst;
    return inst;
}

BOOL CConfig::Load(LPCTSTR szConfigPath)
{
	m_strConfigPath = szConfigPath;
	Clear();

	LPBYTE pData = NULL;
	DWORD dwLength = 0;

	if(!Util::ReadFile(szConfigPath, pData, dwLength))
		return FALSE;

	if(pData == NULL || dwLength == 0)
		return TRUE;

	BOOL bResult = ParseData(pData, dwLength);
	free(pData);
	return bResult;
}

BOOL CConfig::Save(LPCTSTR szPath)
{
	LPBYTE pData = NULL;
	DWORD dwLength = 0;
	if(!PrepareData(pData, dwLength))
		return FALSE;

	BOOL bResult = Util::WriteFile(szPath, pData, dwLength);
	free(pData);
	return bResult;
}

BOOL CConfig::IsNameExists(LPCTSTR szName) const
{
	POSITION posMode = m_HostsModes.GetHeadPosition();
	while(posMode != NULL)
	{
		const stHostsMode& mode = m_HostsModes.GetNext(posMode);
        if(mode.strName.CompareNoCase(szName) == 0)
            return TRUE;
	}
    return FALSE;
}

void CConfig::AddModeName(LPCTSTR szName)
{
    stHostsMode mode;
    mode.strName = szName;
    m_HostsModes.AddTail(mode);
}

HostsModes& CConfig::GetHostsModes()
{
	return m_HostsModes;
}

void CConfig::Clear()
{
	m_HostsModes.RemoveAll();
}

BOOL CConfig::ParseData(LPCVOID pData, DWORD dwLength)
{
	CString strData(static_cast<LPCTSTR>(pData), dwLength / sizeof(TCHAR));

	int nStart = 0;
	CString strLine;
	
	stHostsMode mode;
	while( (strLine = strData.Tokenize(_T("\n"), nStart)) && nStart != -1)
	{
		DWORD dwLineLength = strLine.GetLength();
		strLine.Trim(_T("\r\n \t"));
		dwLineLength = strLine.GetLength();
		if(strLine.GetLength() == 0)
			continue;

		if(strLine[0] == _T('['))
		{
            if(mode.strName.GetLength() > 0)
				m_HostsModes.AddTail(mode);

			mode.strName = strLine.Mid(1, dwLineLength - 2);
		}
        else if(mode.strName.GetLength() > 0)
		{
			mode.strContent += strLine;
            mode.strContent += _T("\r\n");
		}
	}
	
    if(mode.strName.GetLength() > 0)
	{
		m_HostsModes.AddTail(mode);
	}
	return TRUE;
}

BOOL CConfig::PrepareData(LPBYTE& pData, DWORD& dwLength)
{
	CString strData;

	POSITION posMode = m_HostsModes.GetHeadPosition();
	while(posMode != NULL)
	{
		stHostsMode& mode = m_HostsModes.GetNext(posMode);
		strData += _T("[");
		strData += mode.strName;
		strData += _T("]\r\n");

		strData += mode.strContent;
		strData += _T("\r\n\r\n");
	}

	dwLength = strData.GetLength() * sizeof(TCHAR);
	pData = static_cast<LPBYTE>(malloc(dwLength));
	if(pData == NULL)
		return FALSE;

	memcpy(pData, (LPCTSTR)strData, dwLength);
	return TRUE;
}
