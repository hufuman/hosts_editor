#pragma once



class CModeListBox : public CWindowImpl<CModeListBox, CListBox>, public COwnerDraw<CModeListBox>
{
public:
    CModeListBox()
    {
        m_dwAppliedItemId = -1;
    }

	BEGIN_MSG_MAP(CModeListBox)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
        
        CHAIN_MSG_MAP_ALT(COwnerDraw<CModeListBox>, 1)
        DEFAULT_REFLECTION_HANDLER()
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
        if(uItem < static_cast<UINT>(this->GetCount()))
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

    DWORD GetAppliedItemId() const
    {
        return m_dwAppliedItemId;
    }

    void SetAppliedItemId(DWORD dwAppliedItemId)
    {
        if(m_dwAppliedItemId == dwAppliedItemId)
            return;
        m_dwAppliedItemId = dwAppliedItemId;
        this->Invalidate();
    }

    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
    {
        if(lpDrawItemStruct->itemID == -1 || lpDrawItemStruct->itemID == -2)
            return;
        
        HDC hDC = lpDrawItemStruct->hDC;
        RECT& rcItem = lpDrawItemStruct->rcItem;
        UINT itemState = lpDrawItemStruct->itemState;

        BOOL bFocused = (itemState & ODS_FOCUS);
        BOOL bSelected = (itemState & ODS_SELECTED);

        DWORD dwTextColor = 0;
        DWORD dwBkMode = 0;
        if(bSelected)
        {
            ::FillRect(hDC, &rcItem, static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)));
            dwTextColor = ::SetTextColor(hDC, RGB(255, 255, 255));
            dwBkMode = ::SetBkMode(hDC, TRANSPARENT);
        }
        else
        {
            ::FillRect(hDC, &rcItem, static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH)));
        }

        CString strData;
        this->GetText(lpDrawItemStruct->itemID, strData);
        ::DrawText(hDC,
            strData,
            strData.GetLength(),
            &lpDrawItemStruct->rcItem,
            DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE);

        if(bFocused)
        {
            RECT rcFocus = lpDrawItemStruct->rcItem;
            ::InflateRect(&rcFocus, -1, -1);
            ::DrawFocusRect(hDC, &rcFocus);
        }

        if(bSelected)
        {
            ::SetTextColor(hDC, dwTextColor);
            ::SetBkMode(hDC, dwBkMode);
        }
    }

private:
    DWORD m_dwAppliedItemId;
};

