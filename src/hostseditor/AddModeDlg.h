#pragma once


class CAddModeDlg : public CDialogImpl<CAddModeDlg>
{
public:
	enum { IDD = IDD_ADDMODE_DLG };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()


	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
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

        if(CConfig::instance().IsNameExists(m_strModeName))
        {
            MessageBox(_T("Mode Name Already Exists"));
            return 0;
        }

        CConfig::instance().AddModeName(m_strModeName);

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

private:
    CString m_strModeName;
};

