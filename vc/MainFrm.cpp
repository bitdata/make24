// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainFrm.h"

class CHelp
{
public:
	CHelp()
	{	
		GetModuleFileName(NULL,m_szFile,MAX_PATH);
		lstrcpyn(_tcsrchr(m_szFile,_T('.')),_T(".chm"),5);
	}
	bool IsExists() const
	{
		return (-1!=GetFileAttributes(m_szFile));
	}
	void Execute()
	{
		SHELLEXECUTEINFO sei={0,};
		sei.cbSize			=sizeof(SHELLEXECUTEINFO);
		sei.lpFile			=m_szFile;
		sei.nShow			=SW_SHOW;
		ShellExecuteEx(&sei);
	}
private:
	TCHAR	m_szFile[MAX_PATH];
};

class CProfile
{
public:
	CProfile()
	{	
		GetModuleFileName(NULL,m_szFile,MAX_PATH);
		lstrcpyn(_tcsrchr(m_szFile,_T('.')),_T(".ini"),5);
	}
	bool IsExists() const
	{
		return (-1!=GetFileAttributes(m_szFile));
	}
	int GetDifficulty() const
	{
		switch(GetPrivateProfileInt(_T("Option"),_T("Difficulty"),0,m_szFile))
		{
		case DIFFICULTY_HIGHEST:
		case DIFFICULTY_HIGH:
			return IsFractional()?DIFFICULTY_HIGHEST:DIFFICULTY_HIGH;
		case DIFFICULTY_NORMAL:
		case DIFFICULTY_LOW:
			return DIFFICULTY_LOW;
		default:
			break;
		}
		return 0;
	}
	bool IsIncludeJQK() const
	{
		return (0!=GetPrivateProfileInt(_T("Option"),_T("IncludeJQK"),0,m_szFile));
	}
	bool IsKnowledgeware() const
	{
		return (0!=GetPrivateProfileInt(_T("Option"),_T("Knowledgeware"),0,m_szFile));
	}
	bool IsSemiautomatic() const
	{
		return (0!=GetPrivateProfileInt(_T("Option"),_T("Semiautomatic"),0,m_szFile));
	}
	bool IsFractional() const
	{
		return (0!=GetPrivateProfileInt(_T("Option"),_T("Fractional"),0,m_szFile));
	}
	DWORD GetTime() const
	{
		return GetPrivateProfileInt(_T("Option"),_T("Time"),40*60*1000,m_szFile);
	}
	void SetDifficulty(int nDifficulty)
	{
		TCHAR szDifficulty[16];
		_itot_s(nDifficulty,szDifficulty,sizeof(szDifficulty)/sizeof(TCHAR),10);
		WritePrivateProfileString(_T("Option"),_T("Difficulty"),szDifficulty,m_szFile);
	}
	void SetOption(int nDifficulty, bool fIncludeJQK, bool fKnowledgeware, bool fSemiautomatic, bool fFractional, DWORD dwTime)
	{
		SetDifficulty(nDifficulty);
		WritePrivateProfileString(_T("Option"),_T("IncludeJQK"),fIncludeJQK?_T("1"):_T("0"),m_szFile);
		WritePrivateProfileString(_T("Option"),_T("Knowledgeware"),fKnowledgeware?_T("1"):_T("0"),m_szFile);
		WritePrivateProfileString(_T("Option"),_T("Semiautomatic"),fSemiautomatic?_T("1"):_T("0"),m_szFile);
		WritePrivateProfileString(_T("Option"),_T("Fractional"),fFractional?_T("1"):_T("0"),m_szFile);
		TCHAR szTime[16];
		_itot_s(dwTime,szTime,sizeof(szTime)/sizeof(TCHAR),10);
		WritePrivateProfileString(_T("Option"),_T("Time"),szTime,m_szFile);
	}

private:
	TCHAR	m_szFile[MAX_PATH];
};

class COptionDlg : public CDialogImpl<COptionDlg>
{
public:
	enum { IDD = IDD_OPTION };

	COptionDlg()
		:m_fIncludeJQK(false),
		m_fKnowledgeware(false),
		m_fSemiautomatic(false),
		m_fFractional(false),
		m_fEnableFractional(false),
		m_nTime(0)
	{
	}

	BEGIN_MSG_MAP(COptionDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CheckRadioButton(IDC_RADIO_JQK_OFF,IDC_RADIO_JQK_ON,m_fIncludeJQK?IDC_RADIO_JQK_ON:IDC_RADIO_JQK_OFF);
		CheckRadioButton(IDC_RADIO_KNOWLEDGEWARE_OFF,IDC_RADIO_KNOWLEDGEWARE_ON,m_fKnowledgeware?IDC_RADIO_KNOWLEDGEWARE_ON:IDC_RADIO_KNOWLEDGEWARE_OFF);
#if !defined(_SEMIAUTOMATIC_)
		::EnableWindow(GetDlgItem(IDC_RADIO_SEMIAUTOMATIC_OFF),FALSE);
		::EnableWindow(GetDlgItem(IDC_RADIO_SEMIAUTOMATIC_ON),FALSE);
#endif
		CheckRadioButton(IDC_RADIO_SEMIAUTOMATIC_OFF,IDC_RADIO_SEMIAUTOMATIC_ON,m_fSemiautomatic?IDC_RADIO_SEMIAUTOMATIC_ON:IDC_RADIO_SEMIAUTOMATIC_OFF);
		CheckRadioButton(IDC_RADIO_FRACTIONAL_OFF,IDC_RADIO_FRACTIONAL_ON,m_fFractional?IDC_RADIO_FRACTIONAL_ON:IDC_RADIO_FRACTIONAL_OFF);
#if defined(_FRACTIONAL_)
		::EnableWindow(GetDlgItem(IDC_RADIO_FRACTIONAL_OFF),m_fEnableFractional);
		::EnableWindow(GetDlgItem(IDC_RADIO_FRACTIONAL_ON),m_fEnableFractional);
#endif
		SetDlgItemInt(IDC_EDIT_TIME,m_nTime);

		CenterWindow(GetParent());
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if(IDOK==wID)
		{
			m_fIncludeJQK =(1==IsDlgButtonChecked(IDC_RADIO_JQK_ON));
			m_fKnowledgeware =(1==IsDlgButtonChecked(IDC_RADIO_KNOWLEDGEWARE_ON));
			m_fSemiautomatic =(1==IsDlgButtonChecked(IDC_RADIO_SEMIAUTOMATIC_ON));
			if(m_fEnableFractional)
			{
				m_fFractional =(1==IsDlgButtonChecked(IDC_RADIO_FRACTIONAL_ON));
			}
			int nTime =GetDlgItemInt(IDC_EDIT_TIME);
			if(!nTime)
			{
				TCHAR szText[MAX_PATH],szTitle[MAX_PATH];
				LoadString(GetModuleHandle(NULL),IDS_ERROR_TIME,szText,MAX_PATH);
				GetWindowText(szTitle,MAX_PATH);
				MessageBox(szText,szTitle,MB_ICONERROR);
				return 0;
			}
			m_nTime =nTime;
		}
		EndDialog(wID);
		return 0;
	}

public:

	bool	m_fIncludeJQK;
	bool	m_fKnowledgeware;
	bool	m_fSemiautomatic;
	bool	m_fFractional;
	bool	m_fEnableFractional;
	int		m_nTime;
};

// ------------------------------------------------------------------------------------------------

CMainFrame::CMainFrame()
	:m_dwTestTime(40*60*1000),
	m_fFractional(false),
	m_dwTimeout(0),
	m_iSelected(-1),
	m_fShowKey(false),
	m_fDragging(false),
	m_dwFlags(0),
	m_nWidth(0),
	m_nHeight(0),
	m_nFontHeight(0),
	m_hAccel(NULL)
{
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(TranslateAccelerator(m_hWnd,m_hAccel,pMsg))
		return TRUE;
	return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	OnUpdateUI();
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//CreateSimpleStatusBar();
	//UISetCheck(ID_VIEW_STATUS_BAR, 0);

#if defined(_APP_MODULE_)
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
#endif
	
	m_hAccel =LoadAccelerators(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_MAINFRAME));

	ModifyStyle(WS_MAXIMIZEBOX|WS_SIZEBOX,0);
	SetWindowPos(NULL,0,0,FRAME_WIDTH,FRAME_HEIGHT,SWP_NOMOVE|SWP_NOZORDER);
	CenterWindow();

	cdtInit(&m_nWidth,&m_nHeight);
	CreateMemoryBitmap();
	AssignViewsRegion();

	UISetCheck(ID_FILE_BASIC,1);

	CHelp help;
	UIEnable(ID_APP_HELP,help.IsExists());

	CProfile profile;
	if(profile.IsExists())
	{
		m_dwTestTime =profile.GetTime();
		m_fFractional =profile.IsFractional();
		int nDifficulty =profile.GetDifficulty();
		m_game.SetOption(nDifficulty,profile.IsIncludeJQK(),profile.IsKnowledgeware(),profile.IsSemiautomatic());
		switch(nDifficulty)
		{
		case DIFFICULTY_HIGHEST:
		case DIFFICULTY_HIGH:
			UISetCheck(ID_FILE_BASIC,0);
			UISetCheck(ID_FILE_ADVANCED,1);
			break;
		case DIFFICULTY_NORMAL:
		case DIFFICULTY_LOW:
			UISetCheck(ID_FILE_BASIC,0);
			UISetCheck(ID_FILE_INTERMEDIATE,1);
			break;
		default:
			break;
		}
	}

	Go();
	return 0;
}
LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	m_dcMem.DeleteDC();
	DestroyAcceleratorTable(m_hAccel);
	cdtTerm();

	bHandled =FALSE;
	return 0;
}

LRESULT CMainFrame::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	{
	RECT rc;
	GetClientRect(&rc);
	m_dcMem.Rectangle(&rc);
	}
	if(m_game.IsActive())
	{
		UpdateChatView();
		UpdateScoreView();
		UpdateWizardView();
		UpdatePictureView();
	}
	CPaintDC dc(m_hWnd);
	RECT& rc =dc.m_ps.rcPaint;
	dc.BitBlt(rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,m_dcMem,rc.left,rc.top,SRCCOPY);
	bHandled =FALSE;
	return 0;
}

LRESULT CMainFrame::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(ID_TIMER==(UINT)wParam)
	{
		if(!m_fShowKey)
		{
			m_dwTimeout +=INTERVAL_TIMER;
			if(m_dwTimeout>=m_dwTestTime)
			{
				KillTimer(ID_TIMER);
				if(IDYES==ShowMessage(IDS_QUERY_TIMEOUT,MB_ICONQUESTION|MB_YESNO))
				{
					Go();
				}
				else
				{
					PostMessage(WM_CLOSE);	
				}
			}
			else
			{
				InvalidateRect(&m_rcScore,FALSE);
			}
		}
	}

	return 0;
}

LRESULT CMainFrame::OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	LPRECT pRect =(LPRECT)lParam;
	pRect->right =pRect->left+FRAME_WIDTH;
	pRect->bottom =pRect->top+FRAME_HEIGHT;

	return TRUE;
}

LRESULT CMainFrame::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(m_game.IsActive() && !m_fShowKey)
	{
		m_point.x =GET_X_LPARAM(lParam);
		m_point.y =GET_Y_LPARAM(lParam);
		if(PtInRect(&m_rcPicture,m_point))
		{
			__int8 iSelected =-1;
			for(__int8 i=0;i<NUM_OPERAND;i++)
			{
				if(PtInRect(m_pRects+i,m_point))
				{
					if(m_game.GetExpression(i))
					{
						iSelected =i;
						m_fDragging =true;
					}
				}
			}
			if(iSelected!=m_iSelected)
			{
				OnSelectionChange(iSelected);
				Invalidate(FALSE);
			}
		}
	}

	return 0;
}

LRESULT CMainFrame::OnLButtonUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	POINT pt;
	pt.x =GET_X_LPARAM(lParam);
	pt.y =GET_Y_LPARAM(lParam);
	if(m_fDragging)
	{
		ATLASSERT(m_iSelected>=0);
		m_fDragging =false;
		for(__int8 i=0;i<NUM_OPERAND;i++)
		{
			if(PtInRect(m_pRects+i,pt))
			{
				if(i!=m_iSelected)
				{
					if(OnDrop(m_iSelected,i,!!(MK_CONTROL & wParam)))
					{
						OnOperationComplete(i);
						return 0;
					}
				}
				break;
			}
		}
		Invalidate(FALSE);
	}
	else if(!m_fShowKey && PtInRect(&m_rcWizard,pt) && PtInRect(&m_rcWizard,m_point))
	{
		RECT rc;
		for(__int8 i=0;i<NUM_OPERAND;i++)
		{
			if(i==m_iSelected)
				continue;
			rc.left =m_pRects[i].left;
			rc.right =m_pRects[i].right;
			for(__int8 j=ADD;j<=DIVIDE;j++)
			{
				rc.top =m_rcWizard.top+(m_rcWizard.bottom-m_rcWizard.top)/DIVIDE*(j-ADD);
				rc.bottom =rc.top+(m_rcWizard.bottom-m_rcWizard.top)/DIVIDE;
				if(PtInRect(&rc,pt))
				{
					if(CanMerge(i,j))
					{
						if(m_game.Merge(m_iSelected,i,j))
						{
							OnOperationComplete(i);
						}
					}
					return 0;
				}
			}
		}
	}

	return 0;
}

LRESULT CMainFrame::OnLButtonDblClk(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(m_game.IsActive() && m_iSelected>=0 && !m_fShowKey)
	{
		const EXPRESSION* pExpression =m_game.GetExpression(m_iSelected);
		if(pExpression->iOperator)
		{
			POINT pt; 
			pt.x =GET_X_LPARAM(lParam);
			pt.y =GET_Y_LPARAM(lParam);
			if(PtInSelectRegion(pt))
			{
				for(__int8 iOperator =(pExpression->iOperator%DIVIDE)+1;iOperator!=pExpression->iOperator;iOperator=(iOperator%DIVIDE)+1)
				{
					// 如果按下CTRL键，则不考虑加减法
					if((MK_CONTROL & wParam) && (ADD==iOperator || SUBTRACT==iOperator))
						continue;
					if(CanChangeOperator(iOperator))
					{
						if(m_game.ChangeOperator(m_iSelected,iOperator))
						{
							OnOperationComplete(m_iSelected);
							break;
						}
					}
				}
			}
		}
	}

	return 0;
}

LRESULT CMainFrame::OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(m_fDragging && (m_iSelected>=0) && (wParam&MK_LBUTTON) && !m_fShowKey)
	{
		Invalidate(FALSE);
	}

	return 0;
}

LRESULT CMainFrame::OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(m_game.IsActive() && m_iSelected>=0)
	{
		POINT pt; 
		pt.x =GET_X_LPARAM(lParam);
		pt.y =GET_Y_LPARAM(lParam);
		ScreenToClient(&pt);
		if(PtInSelectRegion(pt))
		{
			CMenuHandle menu(GetMenu());
			menu.GetSubMenu(1).GetSubMenu(5).TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_HORPOSANIMATION,GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),m_hWnd);
		}
	}
	return 0;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

/*
LRESULT CMainFrame::OnViewStatusBar(WORD , WORD , HWND , BOOL& )
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}
*/

LRESULT CMainFrame::OnAppHelp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CHelp help;
	if(help.IsExists())
	{
		help.Execute();
	}

	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//CSimpleDialog<IDD_ABOUTBOX> dlg;
	//dlg.DoModal();
	TCHAR szTitle[MAX_PATH],szStuff[MAX_PATH];
	LoadString(GetModuleHandle(NULL),IDS_APP_TITLE,szTitle,MAX_PATH);
	LoadString(GetModuleHandle(NULL),IDS_COPY_RIGHT,szStuff,MAX_PATH);
	HICON hIcon =LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_MAINFRAME));
	ShellAbout(m_hWnd,szTitle,szStuff,hIcon);
	DestroyIcon(hIcon);
	return 0;
}

LRESULT CMainFrame::OnFileGo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	Go();

	return 0;
}

LRESULT CMainFrame::OnFileBasic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UISetCheck(ID_FILE_BASIC,1);
	UISetCheck(ID_FILE_INTERMEDIATE,0);
	UISetCheck(ID_FILE_ADVANCED,0);
	SetDifficulty(0);

	return 0;
}

LRESULT CMainFrame::OnFileIntermediate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UISetCheck(ID_FILE_BASIC,0);
	UISetCheck(ID_FILE_INTERMEDIATE,1);
	UISetCheck(ID_FILE_ADVANCED,0);
	SetDifficulty(DIFFICULTY_LOW);

	return 0;
}

LRESULT CMainFrame::OnFileAdvanced(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UISetCheck(ID_FILE_BASIC,0);
	UISetCheck(ID_FILE_INTERMEDIATE,0);
	UISetCheck(ID_FILE_ADVANCED,1);
	SetDifficulty(m_fFractional?DIFFICULTY_HIGHEST:DIFFICULTY_HIGH);

	return 0;
}

LRESULT CMainFrame::OnFileOption(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	COptionDlg dlg;
	dlg.m_fIncludeJQK =m_game.IsIncludeJQK();
	dlg.m_fKnowledgeware =m_game.IsKnowledgeware();
	dlg.m_fSemiautomatic =m_game.IsSemiautomatic();
	dlg.m_fFractional =m_fFractional;
	dlg.m_fEnableFractional =(m_game.GetDifficulty()>=DIFFICULTY_HIGH);
	dlg.m_nTime =m_dwTestTime/60000;
	if(IDOK==dlg.DoModal(m_hWnd))
	{
		int nDifficulty =(m_game.GetDifficulty()>=DIFFICULTY_HIGH)?(m_fFractional?DIFFICULTY_HIGHEST:DIFFICULTY_HIGH):m_game.GetDifficulty();
		m_fFractional =dlg.m_fFractional;
		m_game.SetOption(nDifficulty,dlg.m_fIncludeJQK,dlg.m_fKnowledgeware,dlg.m_fSemiautomatic);
		m_dwTestTime =dlg.m_nTime*60000;
		CProfile profile;
		if(profile.IsExists())
		{
			profile.SetOption(nDifficulty,dlg.m_fIncludeJQK,dlg.m_fKnowledgeware,dlg.m_fSemiautomatic,m_fFractional,m_dwTestTime);
		}
		Go();
	}

	return 0;
}

LRESULT CMainFrame::OnEditUndo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if(m_game.CanUndo())
	{
		bool pFlags[NUM_OPERAND];
		for(__int8 i=0;i<NUM_OPERAND;i++)
		{
			pFlags[i] =!!m_game.GetExpression(i);
		}
		if(m_game.Undo())
		{
			__int8 iSelected =m_iSelected;
			for(__int8 j=0;j<NUM_OPERAND;j++)
			{
				if(m_game.GetExpression(j) && !pFlags[j])
				{
					iSelected =j;
					break;
				}
			}
			OnSelectionChange(iSelected);
			Invalidate(FALSE);
		}
	}

	return 0;
}

LRESULT CMainFrame::OnEditUndoAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_game.UndoAll();
	OnSelectionChange(0);
	Invalidate(FALSE);

	return 0;
}

LRESULT CMainFrame::OnEditAddAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OnOperateAll(ADD);

	return 0;
}

LRESULT CMainFrame::OnEditMultiplyAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OnOperateAll(MULTIPLY);

	return 0;
}

LRESULT CMainFrame::OnEditAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OnOperate(ADD);
	return 0;
}

LRESULT CMainFrame::OnEditSubtract(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OnOperate(SUBTRACT);
	return 0;
}

LRESULT CMainFrame::OnEditMultiply(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OnOperate(MULTIPLY);
	return 0;
}

LRESULT CMainFrame::OnEditDivide(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OnOperate(DIVIDE);
	return 0;
}

LRESULT CMainFrame::OnEditIgnore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	End();
	Begin();

	return 0;
}

LRESULT CMainFrame::OnEditKey(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_fShowKey =true;
	InvalidateRect(&m_rcScore,FALSE);

	return 0;
}

LRESULT CMainFrame::OnEditHint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	__int8 iTo =m_game.Step();
	if(iTo>=0)
	{
		OnOperationComplete(iTo);
	}

	return 0;
}

inline void CMainFrame::Go()
{
	m_dwTimeout =0;
	SetTimer(ID_TIMER,INTERVAL_TIMER,NULL);
	m_game.Init();
	Begin();
}
inline bool CMainFrame::Begin()
{
	OnSelectionChange(-1);
	m_fDragging =false;
	m_fShowKey =false;
	if(!m_game.BeginQuestion())
		return false;
	OnSelectionChange(0);
	Invalidate(FALSE);
	return true;
}
inline bool CMainFrame::End()
{
	return m_game.EndQuestion();
}
inline void CMainFrame::SetDifficulty(int nDifficulty)
{
	if(nDifficulty!=m_game.GetDifficulty())
	{
		CProfile profile;
		if(profile.IsExists())
		{
			profile.SetDifficulty(nDifficulty);
		}
	}
	m_game.SetDifficulty(nDifficulty);
	Go();
}
inline int CMainFrame::ShowMessage(UINT uID,UINT uType)
{
	TCHAR szTitle[MAX_PATH],szText[MAX_PATH];
	LoadString(GetModuleHandle(NULL),IDR_MAINFRAME,szTitle,MAX_PATH);
	LoadString(GetModuleHandle(NULL),uID,szText,MAX_PATH);
	return MessageBox(szText,szTitle,uType);
}
inline bool CMainFrame::CanMerge(__int8 iTo, __int8 iOperator) const
{
	return !!(m_dwFlags & (0x1<<(iOperator*NUM_OPERAND+iTo)) );
}
inline bool CMainFrame::CanChangeOperator(__int8 iOperator) const
{
	return !!(m_dwFlags & (0x1<<(iOperator*NUM_OPERAND+m_iSelected)) );
}

void CMainFrame::OnUpdateUI()
{
	BOOL bCanUndo =(!m_fShowKey && m_game.CanUndo());
	BOOL bCanIgnore =m_game.IsActive();
	BOOL bCanKey =(m_game.IsActive() && !m_fShowKey);
	BOOL bCanAdd =FALSE;
	BOOL bCanSubtract =FALSE;
	BOOL bCanMultiply =FALSE;
	BOOL bCanDivide =FALSE;
	BOOL bCanHint =(!m_fShowKey && m_game.CanStep());
	if(m_iSelected>=0 && !m_fShowKey)
	{
		const EXPRESSION* pExpression =m_game.GetExpression(m_iSelected);
		if(pExpression)
		{
			bCanAdd =CanChangeOperator(ADD);
			bCanSubtract =CanChangeOperator(SUBTRACT);
			bCanMultiply =CanChangeOperator(MULTIPLY);
			bCanDivide =CanChangeOperator(DIVIDE);
		}
	}
	BOOL bCanAddAll =FALSE;
	if(!m_fShowKey && m_game.IsActive())
	{
		int nOperand =0;
		for(__int8 i=0;i<NUM_OPERAND;i++)
		{
			if(m_game.GetExpression(i))
			{
				nOperand++;
			}
		}
		bCanAddAll =(nOperand>1);
	}
	UIEnable(ID_EDIT_UNDO,bCanUndo);
	UIEnable(ID_EDIT_UNDO_ALL,bCanUndo);
	UIEnable(ID_EDIT_ADD_ALL,bCanAddAll);
	UIEnable(ID_EDIT_MULTIPLY_ALL,bCanAddAll);
	UIEnable(ID_EDIT_ADD,bCanAdd);
	UIEnable(ID_EDIT_SUBTRACT,bCanSubtract);
	UIEnable(ID_EDIT_MULTIPLY,bCanMultiply);
	UIEnable(ID_EDIT_DIVIDE,bCanDivide);
	UIEnable(ID_EDIT_IGNORE,bCanIgnore);
	UIEnable(ID_EDIT_KEY,bCanKey);
	UIEnable(ID_EDIT_HINT,bCanHint);
}
void CMainFrame::OnOperateAll(__int8 iOperator)
{
	if(!m_game.IsActive())
		return;
	__int8 iPos =-1;
	for(__int8 i=0;i<NUM_OPERAND-1;i++)
	{
		if(!m_game.GetExpression(i))
			continue;
		for(__int8 j=i+1;j<NUM_OPERAND;j++)
		{
			if(!m_game.GetExpression(j))
				continue;
			m_game.Merge(i,j,iOperator);
			i =j;
			iPos =j;
		}
	}
	OnOperationComplete(iPos);
}
bool CMainFrame::OnOperate(__int8 iOperator)
{
	if(m_iSelected<0)
		return false;
	const EXPRESSION* pExpression =m_game.GetExpression(m_iSelected);
	if(!pExpression)
		return false;
	if(!m_game.ChangeOperator(m_iSelected,iOperator))
		return false;
	OnOperationComplete(m_iSelected);
	return true;
}
bool CMainFrame::OnDrop(__int8 iFrom, __int8 iTo, bool fCtrl)
{
	const EXPRESSION* pRight =m_game.GetExpression(iTo);
	if(!pRight)
		return false;
	const EXPRESSION* pLeft = m_game.GetExpression(iFrom);
	// 尝试各种运算
	int iCompare =pLeft->nNumerator*pRight->nDenominator-pRight->nNumerator*pLeft->nDenominator;
	__int8 pOperators[NUM_OPERATION] ={0};
	int nOperator =0;	
	// 如果按下CTRL键，则跳过加减法
	if(!fCtrl)
	{
		// 如果左面的表达式较大，支持减法
		if(CanMerge(iTo,SUBTRACT))
		{
			pOperators[nOperator++] =SUBTRACT;
		}
		pOperators[nOperator++] =ADD;
	}
	// 如果左面的表达式较大，尝试除法
	if(iCompare>0)
	{
		pOperators[nOperator++] =DIVIDE;
	}
	pOperators[nOperator++] =MULTIPLY;
	if(iCompare<=0 && CanMerge(iTo,DIVIDE))
	{
		pOperators[nOperator++] =DIVIDE;
	}
	for(int j=0;j<nOperator;j++)
	{
		if(m_game.Merge(iFrom,iTo,pOperators[j]))
			return true;
	}
	return false;
}
void CMainFrame::OnOperationComplete(__int8 iTo)
{
	if(m_game.IsResolved())
	{
		if(!End())
		{
			int nDifficulty =m_game.GetDifficulty();
			if(nDifficulty<DIFFICULTY_HIGH)
			{
				if(IDYES==ShowMessage(IDS_QUERY_PASS,MB_YESNO|MB_ICONQUESTION))
				{
					m_game.SetDifficulty((0==nDifficulty)?DIFFICULTY_LOW:(m_fFractional?DIFFICULTY_HIGHEST:DIFFICULTY_HIGH));
				}
			}
		}
		Begin();
	}
	else
	{
		OnSelectionChange(iTo);
		Invalidate(FALSE);
	}
}
void CMainFrame::OnSelectionChange(__int8 iSelected)
{
	m_iSelected =iSelected;
	m_dwFlags =0;
	if(iSelected<0)
		return;
	for(__int8 k=ADD;k<=DIVIDE;k++)
	{
		if(m_game.CanChangeOperator(iSelected,k))
		{
			m_dwFlags |= 0x1<<(k*NUM_OPERAND+iSelected);
		}
	}
	for(__int8 i=0;i<NUM_OPERAND;i++)
	{
		if(i==iSelected)
			continue;
		for(__int8 j=ADD;j<=DIVIDE;j++)
		{
			if(m_game.CanMerge(iSelected,i,j))
			{
				m_dwFlags |= 0x1<<(j*NUM_OPERAND+i);
			}
		}
	}
}

BOOL CMainFrame::CreateMemoryBitmap()
{
	RECT rc;
	GetClientRect(&rc);
	CClientDC dc(m_hWnd);
	m_dcMem.CreateCompatibleDC(dc);
	DeleteObject(m_dcMem.SelectBitmap(CreateCompatibleBitmap(dc,rc.right-rc.left,rc.bottom-rc.top)));
	DeleteObject(m_dcMem.SelectBrush(CreateSolidBrush(COLOR_BK)));
	DeleteObject(m_dcMem.SelectFont((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
	m_dcMem.SetBkMode(TRANSPARENT);
	return FALSE;
}
void CMainFrame::AssignViewsRegion()
{
	RECT rc;
	GetClientRect(&rc);

	RECT rcFont=rc;
	m_dcMem.DrawText(_T("0"),-1,&rcFont,DT_CALCRECT);
	m_nFontHeight =rcFont.bottom-rc.top;

	for(int i=0;i<NUM_OPERAND;i++)
	{
		RECT& rc =m_pRects[i];
		rc.left =(m_nWidth+m_nWidth/8)*i;
		rc.top =0;
		rc.right =rc.left+m_nWidth;
		rc.bottom =m_nHeight;
	}

	m_rcPicture.left =0;
	m_rcPicture.top =0;
	m_rcPicture.right =m_pRects[NUM_OPERAND-1].right;
	m_rcPicture.bottom =m_nHeight*5/3;
	m_rcWizard.left =0;
	m_rcWizard.top =m_rcPicture.bottom+m_nHeight/4;
	m_rcWizard.right =m_rcPicture.right;
	m_rcWizard.bottom =rc.bottom-m_nFontHeight;
	m_rcChat.left =m_rcPicture.right+m_nWidth;
	m_rcChat.top =0;
	m_rcChat.right =rc.right;
	m_rcChat.bottom =m_rcPicture.bottom;
	m_rcScore.left =m_rcChat.left;
	m_rcScore.top =m_rcWizard.top;
	m_rcScore.right =rc.right;
	m_rcScore.bottom =rc.bottom-m_nFontHeight;
}
inline bool CMainFrame::PtInSelectRegion(POINT pt) const
{
	ATLASSERT(m_iSelected>=0);
	return (PtInRect(&m_pRects[m_iSelected],pt) || PtInRect(&m_rcChat,pt));
}
void CMainFrame::UpdatePictureView()
{
	for(__int8 i=0;i<NUM_OPERAND;i++)
	{
		int x =m_pRects[i].left;
		const EXPRESSION* p =m_game.GetExpression(i);
		if(p)
		{
			bool fSelected =(i==m_iSelected);
			RECT rc;
			rc.left =m_pRects[i].left;
			rc.right =m_pRects[i].right;
			rc.bottom =m_rcPicture.bottom;
			rc.top =rc.bottom-m_nFontHeight;
			COLORREF crOld =0;
			if(fSelected)
			{
				crOld =m_dcMem.SetTextColor(RGB(0xFF,0xFF,0xFF));
			}
			TCHAR szNumber[32];
			FormatNumber(szNumber,p->nNumerator,p->nDenominator);
			m_dcMem.DrawText(szNumber,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			if(fSelected)
			{
				m_dcMem.SetTextColor(crOld);
			}
			// draw picture
			if(!fSelected|| !m_fDragging)
			{
				DrawPicture(x,0,p,fSelected);
			}
		}
		else
		{
			cdtDraw(m_dcMem,x,0,THE_O,mdGhost,COLOR_BK);
		}
	}
	if(m_fDragging)
	{
		ATLASSERT(m_iSelected>=0);
		const EXPRESSION* p =m_game.GetExpression(m_iSelected);
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		DrawPicture(m_pRects[m_iSelected].left+pt.x-m_point.x,pt.y-m_point.y,p,false);
	}
}
void CMainFrame::UpdateChatView()
{
	if(m_iSelected<0)
		return;
	const EXPRESSION* p =m_game.GetExpression(m_iSelected);
	DrawChat((m_rcChat.left+m_rcChat.right)/2,m_rcChat.top+m_nFontHeight*2,(m_rcChat.right-m_rcChat.left)/8,p);
	TCHAR szText[MAX_PATH],szExpression[MAX_PATH];
	CMake24::ToString(p,szExpression,sizeof(szExpression)/sizeof(TCHAR));
	FormatExpression(szText,szExpression);
	_tcscat_s(szText,MAX_PATH,_T(" = "));
	FormatNumber(szText+lstrlen(szText),p->nNumerator,p->nDenominator);
	m_dcMem.DrawText(szText,-1,&m_rcChat,DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
}
void CMainFrame::UpdateWizardView()
{
	TCHAR szText[MAX_PATH];
	LoadString(GetModuleHandle(NULL),IDS_WIZARD,szText,MAX_PATH);
	m_dcMem.TextOut(m_rcWizard.left+2,m_rcWizard.top,szText);
	for(__int8 i=0;i<NUM_OPERAND;i++)
	{
		if(i==m_iSelected)
			continue;
		for(__int8 j=ADD;j<=DIVIDE;j++)
		{
			if(CanMerge(i,j))
			{
				int x =m_pRects[i].left+m_nWidth/2;
				int y =m_rcWizard.top+m_nFontHeight*2+(m_rcWizard.bottom-m_rcWizard.top-m_nFontHeight*2)/DIVIDE*(j-ADD)+m_nFontHeight;
				EXPRESSION pExpressions[3] ={0};
				for(__int8 k=0;k<3;k++)
				{
					EXPRESSION* p =pExpressions+k;
					p->iIndex =k;
					p->iLeft =-1;
					p->iRight =-1;
				}
				const EXPRESSION* p =m_game.GetExpression(m_iSelected);
				pExpressions[0].nDenominator =p->nDenominator;
				pExpressions[0].nNumerator =p->nNumerator;
				p =m_game.GetExpression(i);
				pExpressions[1].nDenominator =p->nDenominator;
				pExpressions[1].nNumerator =p->nNumerator;
				pExpressions[2].iLeft =0;
				pExpressions[2].iRight =1;
				pExpressions[2].iOperator =j;
				DrawChat(x,y,(m_rcWizard.right-m_rcWizard.left)/14,pExpressions+2);
			}
		}
	}
}
void CMainFrame::UpdateScoreView()
{
	TCHAR szFormat[MAX_PATH],szText[MAX_PATH];
	LoadString(GetModuleHandle(NULL),IDS_FORMAT_SCORE,szFormat,MAX_PATH);
	StringCchPrintf(szText,MAX_PATH,szFormat,m_game.GetSuccessCount(),m_dwTimeout/1000);
	m_dcMem.TextOut(m_rcScore.left,m_rcScore.bottom-m_nFontHeight,szText);
	LoadString(GetModuleHandle(NULL),m_fShowKey?IDS_KEY:IDS_HISTORY,szText,MAX_PATH);
	m_dcMem.TextOut(m_rcScore.left,m_rcScore.top,szText);

	KEY pKeys[128];
	int n =0;
	if(m_fShowKey)
	{
		n =m_game.GetKeys(pKeys,sizeof(pKeys)/sizeof(pKeys[0]));
	}
	else
	{
		n =m_game.GetSuccessHistories(pKeys,sizeof(pKeys)/sizeof(pKeys[0]));
	}
	for(int i=0,y=m_rcScore.top+m_nFontHeight*2;i<n && y<m_rcScore.bottom-m_nFontHeight*3;i++,y+=m_nFontHeight*3/2)
	{
		m_dcMem.TextOut(m_rcScore.left,y,pKeys[i].szExpression);
	}
}
int CMainFrame::DrawPicture(int x, int y,const EXPRESSION* pExpression,bool bSelected)
{
	if(!pExpression->iOperator)
	{
		cdtDraw(m_dcMem,x,y,
			m_game.GetSuit(pExpression->iIndex)+(pExpression->nNumerator-1)*4,
			//(bSelected?mdHilite:mdFace),COLOR_BK);
			mdFace,COLOR_BK);
		return y+m_nHeight/8;
	}
	y =DrawPicture(x,y,Right(pExpression),bSelected);
	return DrawPicture(x,y,Left(pExpression),bSelected);
}
void CMainFrame::DrawChat(int x,int y,int size,const EXPRESSION* pExpression)
{
	TCHAR szText[32];
	if(pExpression->iOperator)
	{
		int distance =size;
		if(0!=Left(pExpression)->iOperator && 0!=Right(pExpression)->iOperator)
		{
			distance =size*3/2;
		}
		POINT ptLeft,ptRight;
		ptLeft.x =x-distance;
		ptLeft.y =y+distance;
		ptRight.x =x+distance;
		ptRight.y =ptLeft.y;
		m_dcMem.MoveTo(ptLeft);
		m_dcMem.LineTo(x,y);
		m_dcMem.LineTo(ptRight);
		DrawChat(ptLeft.x,ptLeft.y,size,Left(pExpression));
		DrawChat(ptRight.x,ptRight.y,size,Right(pExpression));
		
		const TCHAR szOperators[] =_T(" +-*//");
		TCHAR szOperator[4];
		szOperator[0] =szOperators[pExpression->iOperator];
		szOperator[1] =0;
		FormatExpression(szText,szOperator);
	}
	else
	{
		FormatNumber(szText,pExpression->nNumerator,pExpression->nDenominator);
	}
	int r =m_nFontHeight*4/5;
	RECT rc;
	rc.left =x-r;
	rc.right =x+r;
	rc.top =y-r;
	rc.bottom =y+r;
	if(pExpression->iOperator)
	{
		m_dcMem.Ellipse(&rc);
	}
	else
	{
		m_dcMem.Rectangle(&rc);
	}
	rc.left -=r;
	rc.right +=r;
	m_dcMem.DrawText(szText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CMainFrame::FormatNumber(PTSTR pszText,short nNumerator,short nDenominator)
{
	if(nDenominator!=1)
	{
		StringCchPrintf(pszText,32,_T("%d/%d"),nNumerator,nDenominator);
	}
	else
	{
		_itot_s(nNumerator,pszText,32,10);
	}
}
void CMainFrame::FormatExpression(PTSTR pszText,LPCTSTR pszExpression)
{
	const TCHAR szAdd[] =_T("＋");
	const TCHAR szSubtract[] =_T("－");
	const TCHAR szMultiply[] =_T("×");
	const TCHAR szDivide[] =_T("÷");

	LPCTSTR pszIn =pszExpression;
	PTSTR pszOut =pszText;
	while(*pszIn)
	{
		switch(*pszIn)
		{
		case _T('+'):
			lstrcpyn(pszOut,szAdd,sizeof(szAdd)/sizeof(TCHAR));
			pszOut +=sizeof(szAdd)/sizeof(TCHAR)-1;
			break;
		case _T('-'):
			lstrcpyn(pszOut,szSubtract,sizeof(szSubtract)/sizeof(TCHAR));
			pszOut +=sizeof(szSubtract)/sizeof(TCHAR)-1;
			break;
		case _T('*'):
			lstrcpyn(pszOut,szMultiply,sizeof(szMultiply)/sizeof(TCHAR));
			pszOut +=sizeof(szMultiply)/sizeof(TCHAR)-1;
			break;
		case _T('/'):
			lstrcpyn(pszOut,szDivide,sizeof(szDivide)/sizeof(TCHAR));
			pszOut +=sizeof(szDivide)/sizeof(TCHAR)-1;
			break;
		default:
			*pszOut++ =*pszIn;
		}
		pszIn++;
	}
	*pszOut =0;
}
