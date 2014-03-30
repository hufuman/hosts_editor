// Main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"


#ifdef ID_FILE_SAVE
#undef ID_FILE_SAVE
#endif

#ifdef IDC_STATIC
#undef IDC_STATIC
#endif


#include "resource.h"
#include "ModeData.h"
#include "Util.h"
#include "Hosts.h"
#include "Config.h"
#include "WndLayout.h"
#include "LinkStatic.h"
#include "BaseDlg.h"
#include "AboutDlg.h"
#include "ModeListBox.h"
#include "ModeNameDlg.h"
#include "MainDlg.h"


CAppModule _Module;


int Run(LPTSTR /*lpCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    CMainDlg dlgMain;

    if(dlgMain.Create(NULL) == NULL)
    {
        ATLTRACE(_T("Main dialog creation failed!\n"));
        return 0;
    }

    Util::FilterWindowMessage(0x0049 /*WM_COPYGLOBALDATA*/, 1);
    Util::FilterWindowMessage(WM_DROPFILES, 1);

    dlgMain.ShowWindow(nCmdShow);

    int nRet = theLoop.Run();

    if(dlgMain.IsWindow())
        dlgMain.DestroyWindow();

    _Module.RemoveMessageLoop();
    return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int nCmdShow)
{
#if (_WIN32_IE >= 0x0300)
    INITCOMMONCONTROLSEX iccx;
    iccx.dwSize = sizeof(iccx);
    iccx.dwICC = ICC_BAR_CLASSES;	// change to support other controls
    ::InitCommonControlsEx(&iccx);
#else
    ::InitCommonControls();
#endif

    // Find Existed Instance
    HWND hWnd = ::FindWindow(_T("#32770"), _T("HostsEditor"));
    if(hWnd != NULL)
    {
        Util::MakeWindowVisible(hWnd);
        return 0;
    }

    _Module.Init(NULL, hInstance);

    int nRet = Run(lpCmdLine, nCmdShow);

    _Module.Term();
    return nRet;
}
