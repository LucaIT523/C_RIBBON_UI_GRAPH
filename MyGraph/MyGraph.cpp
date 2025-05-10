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

// MyGraph.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MyGraph.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyGraphApp

BEGIN_MESSAGE_MAP(CMyGraphApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMyGraphApp::OnAppAbout)
END_MESSAGE_MAP()


// CMyGraphApp construction

CMyGraphApp::CMyGraphApp() noexcept
{

	m_nAppLook = 0;
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MyGraph.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CMyGraphApp object

CMyGraphApp theApp;


CGraph* g_pGraph = NULL;


CGraph* TestInit()
{

	if (!g_pGraph)
		g_pGraph = new CGraph(NULL);

	//if (!pGraph->Create(CRect(100, 100, 700, 700), _T("Test")))
	//	return;

	g_pGraph->ppGraphAlive = &g_pGraph;

	{
		int n = 0;
		n = n;

		CGraphCtrl* pGraphCtrl = g_pGraph->AddGraphCtrl();
		pGraphCtrl->m_IntelligentAxisScale = TRUE;
		pGraphCtrl->m_ShowSubGrid = FALSE;
		pGraphCtrl->m_Title = "Current";
		pGraphCtrl->m_TitleFont = FONT(16, FALSE, FALSE, FALSE, FALSE, _T("Verdana"), FALSE, RGB(0, 0, 0));
		pGraphCtrl->m_SubTitle = "  ";
		pGraphCtrl->m_SubTitleFont = FONT(14, TRUE, FALSE, FALSE, FALSE, _T("Verdana"), FALSE, RGB(0, 0, 0));
		pGraphCtrl->m_ShowSubTitle = TRUE;

		pGraphCtrl->m_XLabel = "Electrical degrees";
		pGraphCtrl->m_XLabelFont = FONT(14, FALSE, FALSE, FALSE, FALSE, _T("Verdana"), FALSE, RGB(0, 0, 0));
		pGraphCtrl->m_ShowXLabel = TRUE;

		pGraphCtrl->m_YLabel = "A";
		pGraphCtrl->m_YLabelFont = FONT(14, FALSE, FALSE, FALSE, FALSE, _T("Verdana"), 90, RGB(0, 0, 0));
		pGraphCtrl->m_ShowYLabel = TRUE;

		pGraphCtrl->m_BackGround.Type = GRAPH_FT_SOLID;
		pGraphCtrl->m_BackGround.Color1 = RGB(200, 200, 200);
		pGraphCtrl->m_GraphFieldBackGround.Type = GRAPH_FT_SOLID;
		pGraphCtrl->m_LegendBackGround.Type = GRAPH_FT_NONE;
		pGraphCtrl->m_GraphFieldBackGround.Color1 = RGB(255, 255, 255);
		pGraphCtrl->m_ShowSubGrid = FALSE;
		pGraphCtrl->m_ShowXSubTicks = TRUE;
		pGraphCtrl->m_ShowYSubTicks = TRUE;


		pGraphCtrl->m_ShowLegend = FALSE;
		pGraphCtrl->m_RightBorder = 60;

		{

			FILE* fpt;
			fpt = _tfopen(_T("current_1.txt"), _T("r"));
			float	temp1, temp2, temp3, xdata, ydata;

			GRAPHSERIES* pSerie = new GRAPHSERIES;
			pSerie->count = 1800;
			_tcscpy(pSerie->lpName, _T("Ia"));
			pSerie->color = RGB(255, 0, 0);
			pSerie->markercolor = RGB(255, 0, 0);
			pSerie->marker = MS_NONE;
			pSerie->markers = 25;
			pSerie->markersize = 10;
			pSerie->pData = new GRAPHPOINT[pSerie->count];
			for (int i = 0; i < pSerie->count; i++)
			{
				_ftscanf(fpt, _T("%f %f %f %f\n"), &xdata, &ydata, &temp1, &temp2);
				pSerie->pData[i].x = double(xdata);
				pSerie->pData[i].y = double(ydata) * 1e-11;
			}
			pGraphCtrl->AddSeries(pSerie);

			fclose(fpt);
		}
		{

			FILE* fpt;
			fpt = _tfopen(_T("current_1.txt"), _T("r"));
			float	temp1, temp2, temp3, xdata, ydata;

			GRAPHSERIES* pSerie = new GRAPHSERIES;
			pSerie->count = 1800;
			_tcscpy(pSerie->lpName, _T("Ib"));
			pSerie->color = RGB(0, 255, 0);
			pSerie->markercolor = RGB(0, 255, 0);
			pSerie->marker = MS_NONE;
			pSerie->markers = 25;
			pSerie->markersize = 10;
			pSerie->linewidth = 2;
			pSerie->pData = new GRAPHPOINT[pSerie->count];
			for (int i = 0; i < pSerie->count; i++)
			{
				_ftscanf(fpt, _T("%f %f %f %f\n"), &xdata, &ydata, &temp1, &temp2);
				pSerie->pData[i].x = double(xdata);
				pSerie->pData[i].y = double(temp1);
			}
			pGraphCtrl->AddSeries(pSerie);

			fclose(fpt);
		}
		{

			FILE* fpt;
			fpt = _tfopen(_T("current_1.txt"), _T("r"));
			float	temp1, temp2, temp3, xdata, ydata;

			GRAPHSERIES* pSerie = new GRAPHSERIES;
			pSerie->count = 1800;
			_tcscpy(pSerie->lpName, _T("Ic"));
			pSerie->color = RGB(0, 0, 255);
			pSerie->markercolor = RGB(0, 0, 255);
			pSerie->marker = MS_NONE;
			pSerie->markers = 25;
			pSerie->markersize = 10;
			pSerie->linewidth = 2;
			pSerie->pData = new GRAPHPOINT[pSerie->count];
			for (int i = 0; i < pSerie->count; i++)
			{
				_ftscanf(fpt, _T("%f %f %f %f\n"), &xdata, &ydata, &temp1, &temp2);
				pSerie->pData[i].x = double(xdata);
				pSerie->pData[i].y = double(temp2);
			}
			pGraphCtrl->AddSeries(pSerie);

			fclose(fpt);
		}
		pGraphCtrl->m_GraphStyle = GS_LINES;
		pGraphCtrl->m_AntiAlaising = TRUE;

		pGraphCtrl->m_TicksXLabelsFont.size = 12;
		pGraphCtrl->m_TicksYLabelsFont.size = 12;

		_tcscpy(pGraphCtrl->m_LegendFont.name, _T("Verdana"));
		pGraphCtrl->m_LegendFont.size = 10;
		pGraphCtrl->m_LegendFont.bold = TRUE;


		pGraphCtrl->Prepare();

	}

	return g_pGraph;
	//	pGraph->Show();
}

// CMyGraphApp initialization

BOOL CMyGraphApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	//INITCOMMONCONTROLSEX InitCtrls;
	//InitCtrls.dwSize = sizeof(InitCtrls);
	//// Set this to include all the common control classes you want to use
	//// in your application.
	//InitCtrls.dwICC = ICC_WIN95_CLASSES;
	//InitCommonControlsEx(&InitCtrls);

	//CWinAppEx::InitInstance();


	//// Initialize OLE libraries
	//if (!AfxOleInit())
	//{
	//	AfxMessageBox(IDP_OLE_INIT_FAILED);
	//	return FALSE;
	//}

	//AfxEnableControlContainer();

	//EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	//InitContextMenuManager();
	//InitKeyboardManager();
	//InitTooltipManager();

	//CMFCToolTipInfo ttParams;
	//ttParams.m_bVislManagerTheme = TRUE;
	//theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
	//	RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CGraphWnd* pFrame = new CGraphWnd;
	if (!pFrame)
		return FALSE;

	CGraph* w_CGraph = TestInit();
	pFrame->InitGraphInfo(w_CGraph);
	w_CGraph->m_pGraphWnd = pFrame;

	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_GRAPHFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr, nullptr);


	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CMyGraphApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CMyGraphApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CMyGraphApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMyGraphApp customization load/save methods

void CMyGraphApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMyGraphApp::LoadCustomState()
{
}

void CMyGraphApp::SaveCustomState()
{
}

// CMyGraphApp message handlers



