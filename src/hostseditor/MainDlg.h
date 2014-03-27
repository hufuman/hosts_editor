// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////


#pragma once


class CMainDlg : public CDialogImpl<CMainDlg>, public CMessageFilter
{
public:
	enum { IDD = IDD_DIALOG_MAIN };


	CMainDlg()
	{
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return ::IsDialogMessage(m_hWnd, pMsg);
	}

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)

		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

        // Modes Name ListBox
		COMMAND_ID_HANDLER(ID_MODENAMECONTEXTMENU_ADD, OnModeNameAdd)
		COMMAND_ID_HANDLER(ID_MODENAMECONTEXTMENU_REMOVE, OnModeNameRemove)
		COMMAND_ID_HANDLER(ID_MODENAMECONTEXTMENU_RENAME, OnModeNameRename)
		COMMAND_ID_HANDLER(ID_MODENAMECONTEXTMENU_DUPLICATE, OnModeNameDuplicate)

		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		// Add "About..." menu item to system menu.

		// IDM_ABOUTBOX must be in the system command range.
		_ASSERTE((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
		_ASSERTE(IDM_ABOUTBOX < 0xF000);

		CMenu SysMenu = GetSystemMenu(FALSE);
		if(::IsMenu(SysMenu))
		{
			TCHAR szAboutMenu[256];
			if(::LoadString(_Module.GetResourceInstance(), IDS_ABOUTBOX, szAboutMenu, 255) > 0)
			{
				SysMenu.AppendMenu(MF_SEPARATOR);
				SysMenu.AppendMenu(MF_STRING, IDM_ABOUTBOX, szAboutMenu);
			}
		}
		SysMenu.Detach();

		// register object for message filtering
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		pLoop->AddMessageFilter(this);

        m_strAppName.LoadString(IDS_APP_NAME);

        m_ModeListBox.SubclassWindow(GetDlgItem(IDC_LIST_MODES));

        InitLayout();

		return TRUE;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}
    
	LRESULT OnModeNameAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        CAddModeDlg dlg;
        if(IDOK != dlg.DoModal())
            return 0;

        CString strModeName = dlg.GetModeName();
        m_ModeListBox.AddString(strModeName);

        return 0;
    }
    
	LRESULT OnModeNameRemove(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        return 0;
    }
    
	LRESULT OnModeNameRename(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        return 0;
    }
    
	LRESULT OnModeNameDuplicate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        return 0;
    }

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}

	LRESULT OnSysCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		UINT uCmdType = (UINT)wParam;

		if((uCmdType & 0xFFF0) == IDM_ABOUTBOX)
		{
			CAboutDlg dlg;
			dlg.DoModal();
		}
		else
			bHandled = FALSE;

		return 0;
	}

    // Layout
    void InitLayout()
    {
        m_WndLayout.Init(m_hWnd);
    }

private:
    CWndLayout      m_WndLayout;
    CString         m_strAppName;
    CModeListBox    m_ModeListBox;
};
