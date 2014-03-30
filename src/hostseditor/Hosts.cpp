#include "StdAfx.h"
#include "Hosts.h"

#include "Util.h"
#include "Config.h"

namespace
{
    TCHAR g_szHostsTag[] = _T("##########-- by hosts editor -- #####");
}

CHosts::CHosts(void)
{
}


CHosts::~CHosts(void)
{
}

BOOL CHosts::Apply(stModeData* pModeData)
{
    CString strData;
    if(!GetFilteredHosts(strData))
        return FALSE;

    strData += _T("\r\n\r\n");
    strData += g_szHostsTag;
    strData += _T("\r\n");

    strData += pModeData->strContent;

    strData += g_szHostsTag;

    CString strHostsPath = GetHostsFilePath();
    return Util::WriteStringToFile(strHostsPath, strData, CP_ACP);
}

BOOL CHosts::Restore()
{
    CString strData;
    if(!GetFilteredHosts(strData))
        return FALSE;

    CString strHostsPath = GetHostsFilePath();
    return Util::WriteStringToFile(strHostsPath, strData, CP_ACP);
}

BOOL CHosts::GetFilteredHosts(CString& strResult)
{
    LPBYTE pData = NULL;
    DWORD dwLength = 0;

    CString strHostsPath = GetHostsFilePath();
    CString strData;
    if(!Util::ReadFileAsString(strHostsPath, strData))
        return FALSE;

    int nStart = 0;
    CString strLine;
    strResult = _T("");

    BOOL bFoundTag = FALSE;
    while( (strLine = strData.Tokenize(_T("\n"), nStart)) && nStart != -1)
    {
        DWORD dwLineLength = strLine.GetLength();
        strLine.Trim(_T("\r\n \t"));
        dwLineLength = strLine.GetLength();

        if(strLine.Find(g_szHostsTag) >= 0)
        {
            bFoundTag = !bFoundTag;
            continue;
        }
        if(bFoundTag)
            continue;

        strResult += strLine;
        strResult += _T("\r\n");
    }

    // Remove all empty lines at the end
    int nLineCount = 0;
    for(int i=strResult.GetLength()-1; i>=0; -- i)
    {
        if(strResult[i] == _T('\r')
            || strResult[i] == _T('\n'))
        {
            ++ nLineCount;
        }
        else
        {
            break;
        }
    }
    strResult = strResult.Mid(0, strResult.GetLength() - nLineCount);

    return TRUE;
}

CString CHosts::GetHostsFilePath()
{
    if(!m_HostsFilePath.IsEmpty())
        return m_HostsFilePath;

    TCHAR szPath[MAX_PATH];
    ::ExpandEnvironmentStrings(_T("%SystemRoot%"), szPath, MAX_PATH);
    m_HostsFilePath = szPath;
    m_HostsFilePath += _T("\\System32\\Drivers\\Etc\\Hosts");
    return m_HostsFilePath;
}
