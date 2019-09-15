// Make24.cpp : main source file for Make24.exe
//

#include "stdafx.h"

#include "resource.h"

#include "MainFrm.h"

#if defined(_APP_MODULE_)
CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}
#endif

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
#if defined(_APP_MODULE_)
	//HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	//ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	//::DefWindowProc(NULL, 0, 0, 0L);

	//AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	HRESULT hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	//::CoUninitialize();

	return nRet;
#else
	hInstance;
	lpstrCmdLine;

	BOOL bDoIdle = TRUE;
	int nIdleCount = 0;
	BOOL bRet;
	CMainFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}
	wndMain.ShowWindow(nCmdShow);

	MSG msg;
	for(;;)
	{
		while(bDoIdle && !::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			nIdleCount++;
			if(!wndMain.OnIdle())
				bDoIdle = FALSE;
		}

		bRet = ::GetMessage(&msg, NULL, 0, 0);

		if(bRet == -1)
		{
			ATLTRACE2(atlTraceUI, 0, _T("::GetMessage returned -1 (error)\n"));
			continue;   // error, don't process
		}
		else if(!bRet)
		{
			ATLTRACE2(atlTraceUI, 0, _T("CMessageLoop::Run - exiting\n"));
			break;   // WM_QUIT, exit message loop
		}

		if(!wndMain.PreTranslateMessage(&msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		if(CMessageLoop::IsIdleMessage(&msg))
		{
			bDoIdle = TRUE;
			nIdleCount = 0;
		}
	}

	return (int)msg.wParam;
#endif
}
