#pragma once


#include "ModeData.h"

class CHosts
{
    typedef ATL::CAtlList<CString> HostsLines;
public:
    CHosts(void);
    ~CHosts(void);

    BOOL Apply(stModeData* pModeData);
    BOOL Restore();

private:
    BOOL GetFilteredHosts(CString& strData);
    CString GetHostsFilePath();

private:
    CString m_HostsFilePath;
};

