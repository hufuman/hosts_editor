#pragma once


#include "ModeData.h"

typedef CAtlList<CString> FilePathList;

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

    BOOL RenameMode(DWORD dwModeId, LPCTSTR szNewName);
    BOOL RemoveById(DWORD dwModeId);

    stModeData* GetModeById(DWORD dwModeId);

    const FilePathList& GetFileHistoryList() const;

    // return whether files history changed
    BOOL AddFile(LPCTSTR szFile);

private:
    void Clear();

    BOOL ParseData(LPCVOID pData, DWORD dwLength);
    BOOL PrepareData(LPBYTE& pData, DWORD& dwLength);

private:
    CString     m_strConfigPath;
    HostsModes  m_HostsModes;

    FilePathList    m_FilePathList;
};

