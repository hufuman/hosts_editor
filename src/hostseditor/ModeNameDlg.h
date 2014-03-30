#pragma once


class CModeNameDlg : public BaseDlg<CModeNameDlg>
{
public:
	enum { IDD = IDD_MODENAME_DLG };

	BEGIN_MSG_MAP(CModeNameDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()
    
    CModeNameDlg()
    {
        m_dwModeId = 0;
    }

    CModeNameDlg(DWORD dwModeId, LPCTSTR szOrgName)
    {
        m_dwModeId = dwModeId;
        m_strOrgName = szOrgName;
    }

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
        SetDlgItemText(IDC_EDIT_NAME, m_strOrgName);
		CenterWindow(GetParent());
		return TRUE;
	}

	LRESULT OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
        GetDlgItemText(IDC_EDIT_NAME, m_strModeName);
        if(m_strModeName.GetLength() == 0)
        {
            MessageBox(_T("Mode Name Can't be Empty"));
            return 0;
        }

        if(!CConfig::IsValidModeName(m_strModeName))
        {
            MessageBox(_T("Mode name can contain only letters (a-z), numbers, and periods"));
            return 0;
        }

        if(m_dwModeId == 0)
        {
            // Add New Mode
            if(CConfig::instance().IsNameExists(m_strModeName))
            {
                MessageBox(_T("Mode Name Already Exists"));
                return 0;
            }

            stModeData data;
            data.dwModeId = ::GetTickCount();
            data.strName = m_strModeName;
            CConfig::instance().AddMode(data);
            m_dwModeId = data.dwModeId;
        }
        else
        {
            // Rename
            if(m_strOrgName != m_strModeName)
                CConfig::instance().RenameMode(m_dwModeId, m_strModeName);
        }



		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

    CString GetModeName() const
    {
        return m_strModeName;
    }

    DWORD GetModeId() const
    {
        return m_dwModeId;
    }

private:
    DWORD m_dwModeId;
    CString m_strOrgName;
    CString m_strModeName;
};

