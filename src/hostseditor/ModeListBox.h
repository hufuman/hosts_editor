#pragma once



class CModeListBox : public CWindowImpl<CModeListBox, CListBox>
{
public:

	BEGIN_MSG_MAP(CModeListBox)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		// COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
	END_MSG_MAP()

    LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
    {
        POINT Pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

        RECT rcWnd;
        this->GetWindowRect(&rcWnd);

        POINT PtClient = Pt;
        this->ScreenToClient(&PtClient);

        BOOL bOutside = FALSE;
        UINT uItem = this->ItemFromPoint(PtClient, bOutside);
        if(uItem < 0)
            uItem = 0;
        if(uItem < this->GetCount())
            this->SetCurSel(uItem);

        CMenu menu;
        menu.LoadMenu(IDR_MENU_MODE_NAME);
        menu.GetSubMenu(0).TrackPopupMenu(
            TPM_RIGHTBUTTON | TPM_VERPOSANIMATION | TPM_VERTICAL,
            Pt.x,
            Pt.y,
            this->GetParent().m_hWnd);
        return 0;
    }
};

