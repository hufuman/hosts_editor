#pragma once



struct stModeData
{
    DWORD   dwModeId;
    CString strName;
    CString strContent;

    BOOL IsValid() const
    {
        return (dwModeId > 0 && strName.GetLength() > 0);
    }

    void Empty()
    {
        dwModeId = 0;
        strName = _T("");
        strContent = _T("");
    }
};

typedef ATL::CAtlList<stModeData> HostsModes;
