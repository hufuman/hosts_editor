#pragma once


struct stHostsMode
{
    CString strName;
    CString strContent;
};

typedef ATL::CAtlList<stHostsMode> HostsModes;

class CConfig
{
    CConfig(void);
    CConfig(const CConfig&);
    CConfig& operator = (const CConfig&);
public:
    ~CConfig(void);

    static CConfig& instance();

    BOOL Load(LPCTSTR szConfigPath);
    BOOL Save(LPCTSTR szPath);

    BOOL IsNameExists(LPCTSTR szName) const;
    void AddModeName(LPCTSTR szName);

    HostsModes& GetHostsModes();

private:
    void Clear();

private:
    BOOL ParseData(LPCVOID pData, DWORD dwLength);
    BOOL PrepareData(LPBYTE& pData, DWORD& dwLength);

private:
    CString     m_strConfigPath;
    HostsModes  m_HostsModes;
};

