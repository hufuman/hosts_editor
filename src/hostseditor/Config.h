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

class CConfig
{
    CConfig(void);
    CConfig(const CConfig&);
    CConfig& operator = (const CConfig&);
public:
    ~CConfig(void);

    static CConfig& instance();

    BOOL Load(LPCTSTR szConfigPath);
    BOOL Save(LPCTSTR szConfigPath);

    BOOL IsNameExists(LPCTSTR szName) const;
    void AddMode(const stModeData& data);

    static BOOL IsValidModeName(LPCTSTR szName);

    HostsModes& GetHostsModes();
    CString GetFilePath() const;

    void SetSeparator(LPCTSTR szSeparator);
    CString GetSeparator() const;

    BOOL RenameMode(DWORD dwModeId, LPCTSTR szNewName);
    BOOL RemoveById(DWORD dwModeId);

    stModeData* GetModeById(DWORD dwModeId);

private:
    void Clear();

private:
    BOOL ParseData(LPCVOID pData, DWORD dwLength);
    BOOL PrepareData(LPBYTE& pData, DWORD& dwLength);

private:
    CString     m_strConfigPath;
    CString     m_strSeparator;
    HostsModes  m_HostsModes;
};

