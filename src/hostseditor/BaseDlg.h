#pragma once


template < typename T >
class BaseDlg : public CDialogImpl<T>
{
public:
    BaseDlg()
    {
        m_strAppName.LoadString(IDS_APP_NAME);
    }

    int MsgBox(LPCTSTR szText, DWORD dwFlags)
    {
        return ::MessageBox(m_hWnd, szText, m_strAppName, dwFlags);
    }

private:
    CString m_strAppName;
};