#pragma once



class CModeListBox : public CWindowImpl<CModeListBox, CListBox>, public COwnerDraw<CModeListBox>
{
    enum ItemStatus
    {
        ItemStatusNormalEven,
        ItemStatusNormalOdd,
        ItemStatusSelected,
        ItemStatusApplied,
        ItemStatusMax,
    };

public:
    CModeListBox()
    {
        m_dwAppliedItemId = -1;

        m_brushBkg[ItemStatusNormalEven] = ::CreateSolidBrush(RGB(255, 255, 255));
        m_clrText[ItemStatusNormalEven] = RGB(0, 0, 0);

        m_brushBkg[ItemStatusNormalOdd] = ::CreateSolidBrush(RGB(243, 250, 255));
        m_clrText[ItemStatusNormalOdd] = RGB(0, 0, 0);

        m_brushBkg[ItemStatusSelected] = ::CreateSolidBrush(RGB(75, 169, 230));
        m_clrText[ItemStatusSelected] = RGB(255, 255, 255);

        m_brushBkg[ItemStatusApplied] = ::CreateSolidBrush(RGB(255, 127, 39));
        m_clrText[ItemStatusApplied] = RGB(255, 255, 255);
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

        DWORD dwBkMode = ::SetBkMode(hDC, TRANSPARENT);

        ItemStatus status = GetItemStatus(lpDrawItemStruct);
        ::FillRect(hDC, &rcItem, m_brushBkg[status]);
        DWORD dwTextColor = ::SetTextColor(hDC, m_clrText[status]);

        CString strData;
        this->GetText(lpDrawItemStruct->itemID, strData);
        ::DrawText(hDC,
            strData,
            strData.GetLength(),
            &lpDrawItemStruct->rcItem,
            DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE);

        ::SetTextColor(hDC, dwTextColor);
        ::SetBkMode(hDC, dwBkMode);
    }

    ItemStatus GetItemStatus(LPDRAWITEMSTRUCT lpDrawItemStruct)
    {
        if(lpDrawItemStruct->itemData == m_dwAppliedItemId)
        {
            return ItemStatusApplied;
        }
        else if(lpDrawItemStruct->itemState & ODS_SELECTED)
        {
            return ItemStatusSelected;
        }
        else if(lpDrawItemStruct->itemID % 2 == 0)
        {
            return ItemStatusNormalOdd;
        }
        else
        {
            return ItemStatusNormalEven;
        }
    }

private:
    DWORD   m_dwAppliedItemId;

    HBRUSH  m_brushBkg[ItemStatusMax];
    DWORD   m_clrText[ItemStatusMax];
};

