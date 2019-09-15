// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "game.h"

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>
#if defined(_APP_MODULE_)
		,public CMessageFilter, public CIdleHandler
#endif
{
public:
	CMainFrame();
	//DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)
	DECLARE_FRAME_WND_CLASS_EX(_T("Make24"),IDR_MAINFRAME,CS_DBLCLKS,COLOR_WINDOW)

	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		//UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FILE_BASIC, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FILE_INTERMEDIATE, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FILE_ADVANCED, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_UNDO, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_UNDO_ALL, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_ADD_ALL, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_MULTIPLY_ALL, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_ADD, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_SUBTRACT, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_MULTIPLY, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_DIVIDE, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_IGNORE, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_KEY, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_HINT, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_APP_HELP, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_APP_HELP, OnAppHelp)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_FILE_GO, OnFileGo)
		COMMAND_ID_HANDLER(ID_FILE_BASIC, OnFileBasic)
		COMMAND_ID_HANDLER(ID_FILE_INTERMEDIATE, OnFileIntermediate)
		COMMAND_ID_HANDLER(ID_FILE_ADVANCED, OnFileAdvanced)
		COMMAND_ID_HANDLER(ID_FILE_OPTION, OnFileOption)
		COMMAND_ID_HANDLER(ID_EDIT_UNDO, OnEditUndo)
		COMMAND_ID_HANDLER(ID_EDIT_UNDO_ALL, OnEditUndoAll)
		COMMAND_ID_HANDLER(ID_EDIT_ADD_ALL, OnEditAddAll)
		COMMAND_ID_HANDLER(ID_EDIT_MULTIPLY_ALL, OnEditMultiplyAll)
		COMMAND_ID_HANDLER(ID_EDIT_ADD, OnEditAdd)
		COMMAND_ID_HANDLER(ID_EDIT_SUBTRACT, OnEditSubtract)
		COMMAND_ID_HANDLER(ID_EDIT_MULTIPLY, OnEditMultiply)
		COMMAND_ID_HANDLER(ID_EDIT_DIVIDE, OnEditDivide)
		COMMAND_ID_HANDLER(ID_EDIT_IGNORE, OnEditIgnore)
		COMMAND_ID_HANDLER(ID_EDIT_KEY, OnEditKey)
		COMMAND_ID_HANDLER(ID_EDIT_HINT, OnEditHint)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAppHelp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileGo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileBasic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileIntermediate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileAdvanced(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileOption(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditUndo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditUndoAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditAddAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditMultiplyAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditSubtract(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditMultiply(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditDivide(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditIgnore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditKey(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditHint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	enum
	{
		FRAME_WIDTH =640,
		FRAME_HEIGHT =480,

		ID_TIMER =1,
		INTERVAL_TIMER =1000,

		COLOR_BK =RGB(0,0x7A,0),	// color or background
	};

	CGame		m_game;

	DWORD		m_dwTestTime;
	DWORD		m_dwTimeout;

	bool		m_fFractional;

	__int8		m_iSelected;
	bool		m_fShowKey;
	bool		m_fDragging;
	POINT		m_point;
	DWORD		m_dwFlags;	// can merge / can changeoperator

	void Go();
	bool Begin();
	bool End();
	int ShowMessage(UINT uID,UINT nType);
	void SetDifficulty(int nDifficulty);
	bool CanMerge(__int8 iTo, __int8 iOperator) const;
	bool CanChangeOperator(__int8 iOperator) const;

	void OnUpdateUI();
	void OnOperateAll(__int8 iOperator);
	bool OnOperate(__int8 iOperator);
	bool OnDrop(__int8 iFrom,__int8 iTo, bool fCtrl);
	void OnOperationComplete(__int8 iTo);
	void OnSelectionChange(__int8 iSelected);

private:
	int			m_nWidth;
	int			m_nHeight;
	int			m_nFontHeight;
	RECT		m_pRects[NUM_OPERAND];
	RECT		m_rcPicture;
	RECT		m_rcChat;
	RECT		m_rcWizard;
	RECT		m_rcScore;
	CDC			m_dcMem;
	HACCEL		m_hAccel;

	BOOL CreateMemoryBitmap();
	void AssignViewsRegion();
	bool PtInSelectRegion(POINT pt) const;
	void UpdatePictureView();
	void UpdateChatView();
	void UpdateWizardView();
	void UpdateScoreView();
	int DrawPicture(int x, int y,const EXPRESSION* pExpression,bool bSelected);
	void DrawChat(int x,int y, int size, const EXPRESSION* pExpression);

	static void FormatNumber(PTSTR pszText,short nNumerator,short nDenominator);
	static void FormatExpression(PTSTR pszText,LPCTSTR pszExpression);
};
