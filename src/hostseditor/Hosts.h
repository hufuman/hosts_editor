#pragma once




class CHosts
{
    typedef ATL::CAtlList<CString> HostsLines;
public:
    CHosts(void);
    ~CHosts(void);

    BOOL Apply(DWORD dwModeId);
    BOOL Restore();

private:
    BOOL GetFilteredHosts(CString& strData);
    CString GetHostsFilePath();

private:
    CString m_HostsFilePath;
};

