// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.cpp : implementation of the CGraphWnd class
//

#include "pch.h"
#include "framework.h"
#include "MyGraph.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString CGraphWnd::m_ClassName = _T("GraphWndWindow");
CString CGraphWnd::m_GraphViewClassName = _T("");
HICON CGraphWnd::m_hIcon = NULL;

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;
// CGraphWnd
IMPLEMENT_DYNAMIC(CGraphWnd, CFrameWndEx)

BEGIN_MESSAGE_MAP(CGraphWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CGraphWnd::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CGraphWnd::OnUpdateApplicationLook)
	ON_COMMAND(ID_FILE_PRINT, &CGraphWnd::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CGraphWnd::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGraphWnd::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CGraphWnd::OnUpdateFilePrintPreview)

	ON_MESSAGE(WM_COORDINATES_UPDATE, OnCoordinatesUpdate)
	ON_COMMAND(ID_SAVE_DATA, OnSaveData)
	ON_COMMAND(ID_OPEN_DATA, OnOpenData)
	ON_COMMAND(ID_EXPORT_TOCLIPBOARD, OnCopyToClipboard)
	ON_COMMAND(ID_EXPORT_TOFILE, OnExportToFile)
	ON_COMMAND(ID_EXPORT_TO_CSV, OnExportToCSV)
	ON_COMMAND(ID_EXPORT_TO_XML, OnExportToXML)
	ON_COMMAND(ID_SAVE_PRESET, OnSavePreset)
	ON_COMMAND(ID_OPEN_PRESET, OnOpenPreset)
	ON_COMMAND(ID_PRESETS_RESET, OnResetPreset)

	ON_COMMAND(ID_LEGEND_MOVESIZE, OnLegendMoveSize)
	ON_COMMAND(ID_LEGEND_HIDE, OnLegendHide)
	ON_COMMAND(ID_LEGEND_PROPERTIES, OnLegendProperties)
	ON_COMMAND(ID_GRAPH_PROPERTIES, OnGraphProperties)
	ON_COMMAND(ID_DATA_PROPERTIES, OnDataProperties)
	ON_COMMAND(ID_DATA_100, OnData100)

	ON_COMMAND_RANGE(ID_GRAPHCOMPONENTS_TITLE, ID_GRAPHCOMPONENTS_SUBTICKS, OnGraphComponents)
	ON_UPDATE_COMMAND_UI_RANGE(ID_GRAPHCOMPONENTS_TITLE, ID_GRAPHCOMPONENTS_SUBTICKS, OnUpdateUIGraphComponents)

	ON_COMMAND(ID_ZOOM_ZOOM, OnZoom)
	ON_COMMAND(ID_ZOOM_UNZOOM, OnUnZoom)
	ON_COMMAND(ID_ZOOM_ZOOM100, On100Zoom)
	ON_COMMAND(ID_TOOLS_PANTOOL, OnPan)
	ON_COMMAND(ID_TOOLS_CROSSTOOL, OnCross)
	ON_COMMAND(ID_TOOLS_SELECTIONTOOL, OnSelectMode)

	//ON_COMMAND_RANGE(ID_ZOOM_ZOOM, ID_TOOLS_PANTOOL, OnSelectionAndZomm)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_ZOOM, OnUpdateUIZomm)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_UNZOOM, OnUpdateUIUnZomm)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_ZOOM100, OnUpdateUIZomm100)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SELECTIONTOOL, OnUpdateUISELECTIONTOOL)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CROSSTOOL, OnUpdateUICROSSTOOL)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PANTOOL, OnUpdateUIPANTOOL)

	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_PANE2, OnUpdateStatusPage2)

END_MESSAGE_MAP()

// CGraphWnd construction/destruction

CGraphWnd::CGraphWnd() noexcept
{
	m_pView = NULL;

	HICON hIC;
	if (!m_hIcon)
	{
		m_hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_GRAPHFRAME));
		hIC = AfxGetApp()->LoadIcon(IDR_GRAPHFRAME);
		m_hIcon = hIC;
	}
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CGraphWnd::~CGraphWnd()
{
	if (m_pView != NULL) {
		delete m_pView;
		m_pView = NULL;
	}

	WNDCLASS classInfo;
	if (m_hIcon && !GetClassInfo(GetModuleHandle(NULL), m_ClassName, &classInfo)) {
		DestroyIcon(m_hIcon);
	}

	if (m_pGraph)
	{
		m_pGraph->ppGraphAlive = NULL;
		m_pGraph->bWindowOpen = 0;
		m_pGraph->SendClosingMessage();
	}
}
void CGraphWnd::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);
	InvalidateView();
}
void CGraphWnd::InvalidateView(void)
{
	//CRect viewRect, toolRect, statusRect;
	//GetClientRect(viewRect);
	//m_wndToolBar.GetWindowRect(toolRect);
	//m_wndStatusBar.GetWindowRect(statusRect);
	//viewRect.top += toolRect.Height();
	//viewRect.bottom -= statusRect.Height();
	//m_pView->SetWindowPos(NULL, viewRect.left, viewRect.top, viewRect.Width(), viewRect.Height(), SWP_NOZORDER);
}
void CGraphWnd::InitGraphInfo(CGraph* p_CGraph)
{
	m_pGraph = p_CGraph;
}
int CGraphWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//BOOL bNameValid;

	// create a view to occupy the client area of the frame
	//if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	//{
	//	TRACE0("Failed to create view window\n");
	//	return -1;
	//}
	m_pView = new CGraphView(m_pGraph);
	if (m_GraphViewClassName == "") {
		m_GraphViewClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, NULL, NULL, NULL);
	}
	m_pView->Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr);
	m_pView->UpdateWindow();



	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CString strTitlePane1 = L"x:0 , Y:0";
	CString strTitlePane2 = L"Selection";
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	return 0;
}

BOOL CGraphWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	//cs.lpszClass = AfxRegisterWndClass(0);

	WNDCLASS classInfo;
	GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &classInfo);
	classInfo.hbrBackground = NULL;
	classInfo.lpszClassName = m_ClassName;
	classInfo.hIcon = m_hIcon;

	return TRUE;
}

// CGraphWnd diagnostics

#ifdef _DEBUG
void CGraphWnd::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CGraphWnd::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CGraphWnd message handlers

//void CGraphWnd::OnSetFocus(CWnd* /*pOldWnd*/)
//{
//	// forward focus to the view window
//	m_wndView.SetFocus();
//}
//
//BOOL CGraphWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
//{
//	// let the view have first crack at the command
//	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//		return TRUE;
//
//	// otherwise, do default handling
//	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//}

void CGraphWnd::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CGraphWnd::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


void CGraphWnd::OnFilePrint()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CGraphWnd::OnFilePrintPreview()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // force Print Preview mode closed
	}
}

void CGraphWnd::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}

void CGraphWnd::OnOpenData(void)
{
	if (MessageBox(_T("Do you really want to replace your graph on screen with data from file?"), _T("Graph Viewer"), MB_ICONQUESTION | MB_YESNO) == IDYES) {
		m_pView->LoadData();
	}
}

void CGraphWnd::OnSaveData(void)
{
	m_pView->SaveData();
	return;
}

void CGraphWnd::OnCopyToClipboard(void)
{
	m_pView->CopyToClipboard();
}

void CGraphWnd::OnExportToFile(void)
{
	m_pView->ExportToFile();
}

void CGraphWnd::OnExportToCSV(void)
{
	m_pView->ExportToCSV();
}

void CGraphWnd::OnExportToXML(void)
{
	m_pView->ExportToXML();
}


void CGraphWnd::OnOpenPreset(void)
{
	m_pView->LoadPreset();
}

void CGraphWnd::OnSavePreset(void)
{
	if (m_pGraph->GetGraphsCount() != 1) return;
	m_pView->SavePreset();
}

void CGraphWnd::OnResetPreset(void)
{
	if (MessageBox(_T("Do you really want to reset your graph properties?"), _T("Graph Viewer"), MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		m_pGraph->ResetPreset();
		m_pView->Resize();
		m_pView->Invalidate();
	}
}


LRESULT CGraphWnd::OnCoordinatesUpdate(WPARAM wParam, LPARAM lParam)
{
	CMFCRibbonBaseElement* w_pElement = m_wndStatusBar.FindElement(ID_STATUSBAR_PANE1);
	if (w_pElement != NULL)
	{
//		UINT style = m_wndStatusBar.GetPaneStyle(posIndex);
		GRAPHPOINT* pt = (GRAPHPOINT*)lParam;
		CGraphCtrl* pGraphCtrl = (CGraphCtrl*)wParam;
		CString posText;
		if (pt)
		{
			if (pGraphCtrl)
			{
				posText.Format(_T(" x: %s; y: %s"),
					pGraphCtrl->GetXTicksLabelText(pt->x),
					pGraphCtrl->GetYTicksLabelText(pt->y));
			}
			else
			{
				posText.Format(_T(" x: %0.3f; y: %0.3f"),
					pGraphCtrl->m_LogX ? log(pt->x) : pt->x,
					pGraphCtrl->m_LogY ? log(pt->y) : pt->y);
			}
		}
		w_pElement->SetText(posText);

		m_wndStatusBar.RecalcLayout();
		m_wndStatusBar.RedrawWindow();

	}
	return 0;
}

void CGraphWnd::OnLegendMoveSize()
{
	m_pView->OnLegendMoveSize();
}

void CGraphWnd::OnLegendHide()
{
	m_pView->OnLegendHide();
}

void CGraphWnd::OnLegendProperties()
{
	m_pView->OnLegendProperties();
}

void CGraphWnd::OnGraphProperties()
{
	m_pView->OnGraphProperties();
}
void CGraphWnd::OnDataProperties(void)
{
	m_pView->OnDataProperties();
}

void CGraphWnd::OnData100(void)
{
	m_pView->OnData100();
}

void CGraphWnd::OnGraphComponents(UINT nID)
{
	m_pView->DoGraphComponentsClick(nID);
}
void CGraphWnd::OnUpdateUIGraphComponents(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_pView->UpdateCmdUIGraphComponentsClick(pCmdUI->m_nID));
}


void CGraphWnd::OnZoom(void)
{
	if (m_pView->GetZoomMode() == ZM_ZOOM)
		m_pView->SetZoomMode(ZM_NONE);
	else
		m_pView->SetZoomMode(ZM_ZOOM);

	UpdateStatusBar();
}

void CGraphWnd::OnUnZoom(void)
{
	if (m_pView->GetZoomMode() == ZM_UNZOOM)
		m_pView->SetZoomMode(ZM_NONE);
	else
		m_pView->SetZoomMode(ZM_UNZOOM);

	UpdateStatusBar();
}

void CGraphWnd::On100Zoom(void)
{
	if (m_pView->GetZoomMode() == ZM_100)
		m_pView->SetZoomMode(ZM_NONE);
	else
		m_pView->SetZoomMode(ZM_100);

	UpdateStatusBar();
}

void CGraphWnd::OnPan(void)
{
	if (m_pView->GetZoomMode() == ZM_PAN)
		m_pView->SetZoomMode(ZM_NONE);
	else
		m_pView->SetZoomMode(ZM_PAN);

	UpdateStatusBar();
}

void CGraphWnd::OnCross(void)
{
	if (m_pView->GetZoomMode() == ZM_CROSS)
		m_pView->SetZoomMode(ZM_NONE);
	else
		m_pView->SetZoomMode(ZM_CROSS);

	UpdateStatusBar();
}

void CGraphWnd::OnSelectMode(void)
{
	m_pView->SetZoomMode(ZM_NONE);
	UpdateStatusBar();
}

void CGraphWnd::OnUpdateUIZomm(CCmdUI* pCmdUI)
{
	if (m_pView->GetZoomMode() == ZM_ZOOM) {
		pCmdUI->SetCheck(TRUE);
	}
	else {
		pCmdUI->SetCheck(FALSE);
	}
}
void CGraphWnd::OnUpdateUIUnZomm(CCmdUI* pCmdUI)
{
	if (m_pView->GetZoomMode() == ZM_UNZOOM) {
		pCmdUI->SetCheck(TRUE);
	}
	else {
		pCmdUI->SetCheck(FALSE);
	}
}
void CGraphWnd::OnUpdateUIZomm100(CCmdUI* pCmdUI)
{
	if (m_pView->GetZoomMode() == ZM_100) {
		pCmdUI->SetCheck(TRUE);
	}
	else {
		pCmdUI->SetCheck(FALSE);
	}
}
void CGraphWnd::OnUpdateUISELECTIONTOOL(CCmdUI* pCmdUI)
{
	if (m_pView->GetZoomMode() == ZM_NONE) {
		pCmdUI->SetCheck(TRUE);
	}
	else {
		pCmdUI->SetCheck(FALSE);
	}
}
void CGraphWnd::OnUpdateUICROSSTOOL(CCmdUI* pCmdUI)
{
	if (m_pView->GetZoomMode() == ZM_CROSS) {
		pCmdUI->SetCheck(TRUE);
	}
	else {
		pCmdUI->SetCheck(FALSE);
	}
}
void CGraphWnd::OnUpdateUIPANTOOL(CCmdUI* pCmdUI)
{
	if (m_pView->GetZoomMode() == ZM_PAN) {
		pCmdUI->SetCheck(TRUE);
	}
	else {
		pCmdUI->SetCheck(FALSE);
	}
}
void CGraphWnd::OnUpdateStatusPage2(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CGraphWnd::UpdateStatusBar(void)
{
	CMFCRibbonBaseElement* w_pElement = m_wndStatusBar.FindElement(ID_STATUSBAR_PANE2);
	if (w_pElement != NULL)
	{
		CString selText;
		switch (m_pView->GetZoomMode())
		{
			case ZM_NONE: selText = "Selection"; break;
			case ZM_ZOOM: selText = "Zoom"; break;
			case ZM_UNZOOM: selText = "Unzoom"; break;
			case ZM_100: selText = "1:1"; break;
			case ZM_PAN: selText = "Pan"; break;
			case ZM_CROSS: selText = "Cross"; break;
		}
		w_pElement->SetText(selText);

		m_wndStatusBar.RecalcLayout();
		m_wndStatusBar.RedrawWindow();

	}

}
