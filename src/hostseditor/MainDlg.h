// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////


#pragma once


class CMainDlg : public BaseDlg<CMainDlg>, public CMessageFilter
{
public:
	enum { IDD = IDD_DIALOG_MAIN };


	CMainDlg()
	{
        m_bModified = FALSE;
        m_nLastSelIndex = -1;
        m_bHostsChanged = FALSE;
    }

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return ::IsDialogMessage(m_hWnd, pMsg);
	}

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)

        MESSAGE_HANDLER(WM_CLOSE, OnClose)

        // Modes Name ListBox
		COMMAND_ID_HANDLER(ID_MODENAMECONTEXTMENU_ADD, OnModeNameAdd)
		COMMAND_ID_HANDLER(ID_MODENAMECONTEXTMENU_REMOVE, OnModeNameRemove)
		COMMAND_ID_HANDLER(ID_MODENAMECONTEXTMENU_RENAME, OnModeNameRename)
		COMMAND_ID_HANDLER(ID_MODENAMECONTEXTMENU_DUPLICATE, OnModeNameDuplicate)

        // Main Menu
        COMMAND_ID_HANDLER(ID_FILE_EXIT, OnFileExit)

        COMMAND_ID_HANDLER(ID_EDIT_APPLY, OnEditApply)
        COMMAND_ID_HANDLER(ID_EDIT_RESTORE, OnEditRestore)
        
        COMMAND_ID_HANDLER(ID_HELP_ABOUT, OnHelpAbout)

        COMMAND_HANDLER(IDC_LIST_MODES, LBN_SELCHANGE, OnModeSelChanged)
        COMMAND_HANDLER(IDC_EDIT_HOSTS, EN_CHANGE, OnHostsChanged)

        REFLECT_NOTIFICATIONS()
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
        m_EditModeContent.Attach(GetDlgItem(IDC_EDIT_HOSTS));

        InitLayout();

        ::DragAcceptFiles(m_hWnd, FALSE);

        LoadConfig();

		return TRUE;
	}

    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        m_ModeListBox.UnsubclassWindow(FALSE);
        m_EditModeContent.Detach();

        ::PostQuitMessage(0);
        return 0;
    }

    void CloseDialog()
    {
        if(!IsModified())
        {
            DestroyWindow();
            return;
        }

        int nResult = MsgBox(_T("Content modified, are you sure to exit without saving ?"), MB_OKCANCEL | MB_ICONQUESTION);
        if(IDOK == nResult)
            DestroyWindow();
    }

    LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        if(SaveConfig(TRUE, FALSE))
            DestroyWindow();
		return 0;
	}

    LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        if(!SaveConfig(TRUE, FALSE))
            return 0;

        CloseDialog();
        return 0;
    }
    LRESULT OnEditApply(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        int nCurSel = m_ModeListBox.GetCurSel();
        if(nCurSel < 0)
        {
            MsgBox(_T("No Mode Selected"), MB_OK | MB_ICONWARNING);
            return 0;
        }

        DWORD dwItemData = m_ModeListBox.GetItemData(nCurSel);
        stModeData* pData = CConfig::instance().GetModeById(dwItemData);

        CHosts hosts;
        if(hosts.Apply(pData))
        {
            m_ModeListBox.SetAppliedItemId(dwItemData);
            MsgBox(_T("Apply Successfully"), MB_OK | MB_ICONINFORMATION);
        }
        else
        {
            MsgBox(_T("Failed to Apply"), MB_OK | MB_ICONWARNING);
        }
        return 0;
    }
    LRESULT OnEditRestore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        CHosts hosts;
        if(hosts.Restore())
        {
            m_ModeListBox.SetAppliedItemId(-1);
            MsgBox(_T("Restore Successfully"), MB_OK | MB_ICONINFORMATION);
        }
        else
        {
            MsgBox(_T("Failed to Restore"), MB_OK | MB_ICONWARNING);
        }
        return 0;
    }
    LRESULT OnHelpAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        CAboutDlg dlg;
        dlg.DoModal();
        return 0;
    }

    LRESULT OnModeSelChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        UpdateHostsData(FALSE);
        UpdateHostsUI();
        return 0;
    }

    LRESULT OnHostsChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        m_bHostsChanged = TRUE;
        SetModified(TRUE);
        return 0;
    }
    
    // Modes Name ListBox
	LRESULT OnModeNameAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        CModeNameDlg dlg;
        if(IDOK != dlg.DoModal())
            return 0;

        stModeData data;
        data.dwModeId = dlg.GetModeId();
        data.strName = dlg.GetModeName();
        SelectMode(UIAddMode(-1, data));

        return 0;
    }
    
	LRESULT OnModeNameRemove(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        int nCurSel = m_ModeListBox.GetCurSel();
        if(nCurSel < 0)
            return 0;

        DWORD dwItemData = m_ModeListBox.GetItemData(nCurSel);
        CConfig::instance().RemoveById(dwItemData);
        m_ModeListBox.DeleteString(nCurSel);

        m_nLastSelIndex = -1;
        m_bHostsChanged = FALSE;

        SelectMode(0);
        SetModified(TRUE);

        if(m_ModeListBox.GetAppliedItemId() == dwItemData)
            m_ModeListBox.SetAppliedItemId(-1);

        return 0;
    }
    
	LRESULT OnModeNameRename(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        int nCurSel = m_ModeListBox.GetCurSel();
        if(nCurSel < 0)
            return 0;

        DWORD dwItemData = m_ModeListBox.GetItemData(nCurSel);
        stModeData* pData = CConfig::instance().GetModeById(dwItemData);
        if(pData == NULL)
        {
            ATLASSERT(FALSE && __FUNCTIONW__);
            return 0;
        }

        CModeNameDlg dlg(pData->dwModeId, pData->strName);
        if(IDOK != dlg.DoModal())
            return 0;

        UpdateHostsData(FALSE);
        m_ModeListBox.DeleteString(nCurSel);
        UIAddMode(nCurSel, *pData);
        SelectMode(nCurSel);
        SetModified(TRUE);

        return 0;
    }
    
	LRESULT OnModeNameDuplicate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        int nCurSel = m_ModeListBox.GetCurSel();
        if(nCurSel < 0)
            return 0;

        DWORD dwItemData = m_ModeListBox.GetItemData(nCurSel);
        stModeData* pData = CConfig::instance().GetModeById(dwItemData);
        if(pData == NULL)
        {
            ATLASSERT(FALSE && __FUNCTIONW__);
            return 0;
        }

        CModeNameDlg dlg;
        if (IDOK != dlg.DoModal())
            return 0;

        stModeData newData;
        newData.dwModeId = ::GetTickCount();
        newData.strContent = pData->strContent;
        newData.strName = dlg.GetModeName();
        CConfig::instance().AddMode(newData);

        SelectMode(UIAddMode(-1, newData));
        SetModified(TRUE);

        return 0;
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
        {
            bHandled = FALSE;
        }

        return 0;
    }

    // Layout
    void InitLayout()
    {
        m_WndLayout.Init(m_hWnd);
        m_WndLayout.AddControlById(IDC_LIST_MODES, Layout_VFill);
        m_WndLayout.AddControlById(IDC_EDIT_HOSTS, Layout_VFill | Layout_HFill);
    }

private:
    void SetModified(BOOL bModified)
    {
        m_bModified = bModified;
    }

    BOOL IsModified() const
    {
        return m_bModified;
    }

    BOOL LoadConfig()
    {
        if(!CConfig::instance().IsConfigExists())
            return TRUE;

        m_ModeListBox.ResetContent();
        m_EditModeContent.SetWindowText(_T(""));

        BOOL bResult = CConfig::instance().Load();
        if(!bResult)
        {
            MsgBox(_T("Failed to load config"), MB_OK | MB_ICONWARNING);
            return FALSE;
        }
        
        m_nLastSelIndex = -1;
        m_bHostsChanged = FALSE;

        UILoadConfig();
        SetModified(FALSE);

        return TRUE;
    }

    void UILoadConfig()
    {
        const HostsModes& modes = CConfig::instance().GetHostsModes();
        POSITION pos = modes.GetHeadPosition();
        while(pos != NULL)
        {
            const stModeData& mode = modes.GetNext(pos);
            UIAddMode(-1, mode);
        }
        if(modes.GetCount() > 0)
        {
            SelectMode(0);
        }
    }

    BOOL SaveConfig(BOOL bPrompt, BOOL bForce)
    {
        if(!IsModified() && !bForce)
            return TRUE;

        UpdateHostsData(TRUE);

        if(bPrompt)
        {
            int nResult = MsgBox(_T("Modes changed, do you want to save modes information?"), MB_YESNOCANCEL | MB_ICONQUESTION);
            if(nResult == IDCANCEL)
                return FALSE;

            if (nResult == IDNO)
            {
                SetModified(FALSE);
                return TRUE;
            }
        }

        CConfig::instance().Save();
        SetModified(FALSE);
        return TRUE;
    }

    void SelectMode(int nIndex)
    {
        if(nIndex < 0 || nIndex >= m_ModeListBox.GetCount())
        {
            UpdateHostsData(FALSE);
            return;
        }

        m_ModeListBox.SetCurSel(nIndex);

        UpdateHostsData(FALSE);
        UpdateHostsUI();
    }

    stModeData* GetCurData()
    {
        int nIndex = m_ModeListBox.GetCurSel();
        if(nIndex < 0)
            return NULL;
        DWORD dwItemData = m_ModeListBox.GetItemData(nIndex);
        stModeData* pData = CConfig::instance().GetModeById(dwItemData);
        return pData;
    }

    int UIAddMode(int nIndex, const stModeData& data)
    {
        if(nIndex == -1)
            nIndex = m_ModeListBox.AddString(data.strName);
        else
            nIndex = m_ModeListBox.InsertString(nIndex, data.strName);
        m_ModeListBox.SetItemData(nIndex, data.dwModeId);
        SetModified(TRUE);
        return nIndex;
    }

    void UpdateHostsData(BOOL bForce)
    {
        int nCurSel = m_ModeListBox.GetCurSel();
        
        if(!m_bHostsChanged)
        {
            m_nLastSelIndex = nCurSel;
            return;
        }

        if(m_nLastSelIndex != -1)
        {
            CString strContent;
            m_EditModeContent.GetWindowText(strContent);
            DWORD dwItemData = m_ModeListBox.GetItemData(m_nLastSelIndex);
            stModeData* pData = CConfig::instance().GetModeById(dwItemData);
            if(pData)
            {
                pData->strContent = strContent;
            }
        }

        m_bHostsChanged = FALSE;
        m_nLastSelIndex = nCurSel;
    }

    void UpdateHostsUI()
    {
        stModeData* pData = GetCurData();
        if(pData)
            m_EditModeContent.SetWindowText(pData->strContent);
        else
            m_EditModeContent.SetWindowText(_T(""));
    }

    CMenuHandle GetRecentMenu()
    {
        CMenuHandle mainMenu = GetMenu();
        CMenuHandle fileMenu = mainMenu.GetSubMenu(0);
        int nCount = fileMenu.GetMenuItemCount();

        MENUITEMINFO info = {sizeof(info), MIIM_SUBMENU};
        for(int i=0; i<nCount; ++ i)
        {
            UINT uId = fileMenu.GetMenuItemID(i);
            if(uId == -1)
            {
                fileMenu.GetMenuItemInfo(i, TRUE, &info);
                if(info.hSubMenu != NULL)
                {
                    return info.hSubMenu;
                }
            }
        }
        return NULL;
    }

private:
    CWndLayout      m_WndLayout;
    CString         m_strAppName;
    CModeListBox    m_ModeListBox;
    CEdit           m_EditModeContent;
    BOOL            m_bModified;

    int             m_nLastSelIndex;
    BOOL            m_bHostsChanged;
};
