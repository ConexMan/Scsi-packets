// NewScsi.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "strings.h"
#include "scsi.h"
#include "Globals.h"

#include "cdjobs.cpp"
#include "cdrom.cpp"

#include "DialogDvd.cpp"
#include "DialogTools.cpp"
#include "Subs.cpp"
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "comctl32.lib")


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	BOOL comFlag;
	INITCOMMONCONTROLSEX InitCtrlEx;
	TBBUTTON tbrButtons[NUMBUTTONS];
	LRESULT Lres;
	WCHAR wBuffer[128];
	// TODO: Place code here.
	InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC = ICC_BAR_CLASSES | ICC_WIN95_CLASSES;
	comFlag = InitCommonControlsEx(&InitCtrlEx);
	if (!comFlag)
		return false;
	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_NEWSCSI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	tbrButtons[0].iBitmap			= 0;
	tbrButtons[0].idCommand			= IDM_EXIT_MAIN;
	tbrButtons[0].fsState			= TBSTATE_ENABLED;
	tbrButtons[0].fsStyle			= TBSTYLE_BUTTON;
	tbrButtons[0].dwData			= 0L;
	tbrButtons[0].iString			= (INT_PTR)L"Finish";

	tbrButtons[1].iBitmap			= 1;
	tbrButtons[1].idCommand			= IDM_DVD_MAIN;
	tbrButtons[1].fsState			= TBSTATE_ENABLED;
	tbrButtons[1].fsStyle			= TBSTYLE_BUTTON;
	tbrButtons[1].dwData			= 0L;
	tbrButtons[1].iString			= (INT_PTR)L"Dvd WorkShop";

	tbrButtons[2].iBitmap			= 2;
	tbrButtons[2].idCommand			= IDM_CD_MAIN;
	tbrButtons[2].fsState			= TBSTATE_ENABLED;
	tbrButtons[2].fsStyle			= TBSTYLE_BUTTON;
	tbrButtons[2].dwData			= 0L;
	tbrButtons[2].iString			= (INT_PTR)L"CD WorkShop";

	tbrButtons[3].iBitmap			= 3;
	tbrButtons[3].idCommand			= IDM_AUDIO_MAIN;
	tbrButtons[3].fsState			= TBSTATE_ENABLED;
	tbrButtons[3].fsStyle			= TBSTYLE_BUTTON;
	tbrButtons[3].dwData			= 0L;
	tbrButtons[3].iString			= (INT_PTR)L"Audio WorkShop";

	tbrButtons[4].iBitmap			= 4;
	tbrButtons[4].idCommand			= IDM_DEVICECAPS_MAIN;
	tbrButtons[4].fsState			= TBSTATE_ENABLED;
	tbrButtons[4].fsStyle			= TBSTYLE_BUTTON;
	tbrButtons[4].dwData			= 0L;
	tbrButtons[4].iString			= (INT_PTR)L"Device Caps";

	tbrButtons[5].iBitmap			= 5;
	tbrButtons[5].idCommand			= IDM_TOOLKITS_MAIN;
	tbrButtons[5].fsState			= TBSTATE_ENABLED;
	tbrButtons[5].fsStyle			= TBSTYLE_BUTTON;
	tbrButtons[5].dwData			= 0L;
	tbrButtons[5].iString			= (INT_PTR)L"ToolKits";
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	//*********************************************************************************
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NEWSCSI));
	MSG msg;
	//*********************************** Toolbar ************************************
	hToolbarMain = CreateToolbarEx(hWnd, WS_VISIBLE | TBSTYLE_TOOLTIPS | TTS_ALWAYSTIP | TTS_BALLOON,
		IDR_TOOLBAR_MAIN, NUMBUTTONS, hInst, IDR_TOOLBAR_MAIN, tbrButtons, NUMBUTTONS, 32, 32, 32, 32, sizeof(TBBUTTON));
	if (hToolbarMain == 0)
	{
			DWORD ErrorCode = GetLastError();
	return false;
	}
	Lres = SendMessage(hToolbarMain, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON) * NUMBUTTONS, 0);
	Lres = SendMessage(hToolbarMain, TB_ADDBUTTONS, (WPARAM)(UINT)0, (LPARAM)(LPTBBUTTON)&tbrButtons);
	ShowWindow(hToolbarMain, TRUE);
	Lres = SendMessage(hToolbarMain, TB_AUTOSIZE, 0, 0);
	UpdateWindow(hToolbarMain);
	//*********************************************************************************
	//*********************************** status bar **********************************
	hStatusMain = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0,
		hWnd, (HMENU)IDC_NEWSCSI, GetModuleHandle(NULL), NULL);

	int statwidths[] = { 160,285,395,-1 };
	SendMessage(hStatusMain, SB_SETPARTS, 4, (LPARAM)statwidths);
	SendMessage(hStatusMain, SB_SETTEXT, 3, (LPARAM)(WCHAR *)scsi.CdInfo.Details->DriveName.wProductPlusLetter);
	switch (scsi.MediaInfo.Current.CurrentMediaType)
	{
	case DVD:
		StringCchPrintf(wBuffer, _countof(wBuffer), L"Media Is DVD");
		break;
	case CD:
		StringCchPrintf(wBuffer, _countof(wBuffer), L"Media Is CD");
		break;
	case CD_R:
		StringCchPrintf(wBuffer, _countof(wBuffer), L"Media Is CD_R");
		break;
	case CD_RW:
		StringCchPrintf(wBuffer, _countof(wBuffer), L"Media Is CD_RW");
		break;
	default:
		StringCchPrintf(wBuffer, _countof(wBuffer), L"Media Is UnKnown");
		break;
	}
	SendMessage(hStatusMain, SB_SETTEXT, 2, (LPARAM)(WCHAR *)wBuffer);

	StringCchPrintf(wBuffer, _countof(wBuffer), L"Amount Of Cpu's %d ", scsi.SystemInfo.dwNumberOfProcessors);
	SendMessage(hStatusMain, SB_SETTEXT, 1, (LPARAM)(WCHAR *)wBuffer);
	SendMessage(hStatusMain, SB_SETTEXT, 0, (LPARAM)(WCHAR *)L"Hi there From  B.A SoftWare");
	//************************************************************************************

	//******************************** Main message loop: *******************************
  
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	//**************************************************************************************
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style					= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc			= WndProc;
    wcex.cbClsExtra				= 0;
    wcex.cbWndExtra				= 0;
    wcex.hInstance				= hInstance;
    wcex.hIcon					= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NEWSCSI));
    wcex.hCursor				= LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground			= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName			= NULL;// MAKEINTRESOURCEW(IDC_NEWSCSI);
    wcex.lpszClassName			= szWindowClass;
    wcex.hIconSm				= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW , CW_USEDEFAULT, 0, 600, 116, nullptr, nullptr, hInstance, nullptr);
     
    
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
				scsi.GetStartTime();
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				scsi.GetEndTime();
                break;

			case IDM_EXIT_MAIN:
				MessageBeep(MB_OK);
				PostQuitMessage(WM_QUIT);
				return TRUE;
				break;

			case IDM_DVD_MAIN:
				MessageBeep(MB_OK);
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DVD_WORK), hWnd, DialogDvd);
				break;

			case IDM_CD_MAIN:
				MessageBeep(MB_OK);
				break;

			case IDM_AUDIO_MAIN:
				MessageBeep(MB_OK);
				break;

			case IDM_DEVICECAPS_MAIN:
				MessageBeep(MB_OK);
				break;

			case IDM_TOOLKITS_MAIN:
				MessageBeep(MB_OK);
				DialogBox(hInst, MAKEINTRESOURCE(IDD_TOOLS_WORK), hWnd, DialogTools);
				break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_NOTIFY:
	{//open
		TOOLTIPTEXT *lpttt = (TOOLTIPTEXT *)(LPNMHDR)lParam;
		//sent to parent window wParam contains identifier of common control and lParam contains code sent by control and idFrom = id that sent it							 	
		switch (lpttt->hdr.idFrom)

		{//open2
		case IDM_EXIT_MAIN:
			wcscpy_s(szBuf, _countof(szBuf), L"All Done ?");
			break;

		case IDM_DVD_MAIN:
			wcscpy_s(szBuf, _countof(szBuf), L"Go To DVD");
			
			break;

		case IDM_CD_MAIN:
			wcscpy_s(szBuf, _countof(szBuf), L"Go To CD");
			break;

		case IDM_AUDIO_MAIN:
			wcscpy_s(szBuf, _countof(szBuf), L"Go To Audio");
			break;

		case IDM_DEVICECAPS_MAIN:
			wcscpy_s(szBuf, _countof(szBuf), L"Go To Device Caps");
			break;

		case IDM_TOOLKITS_MAIN:
			wcscpy_s(szBuf, _countof(szBuf), L"Go To ToolKits");
			break;

		default:
			return FALSE;
		}//close2

		switch (lpttt->hdr.code)
		{//open2
		case TTN_NEEDTEXT:
			lpttt->lpszText		= (LPWSTR)szBuf;
			lpttt->uFlags			= TTF_IDISHWND | TTF_CENTERTIP;
			lpttt->hinst				= hInst;
			break;

		case TTN_LINKCLICK://sent when the tooltip text is clicked
			MessageBeep(MB_OK);
			break;

		}//close2

		return 0;

	} //close1   

	case WM_CREATE:
		
		break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
	
            EndPaint(hWnd, &ps);

        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
//**************************************************************************************
//********************************** AboutBox *****************************************
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
//********************************************************************************************
