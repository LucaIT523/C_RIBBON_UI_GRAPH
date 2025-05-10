// ViewPrintPreview.cpp : implementation file
//

#include "stdafx.h"
#include "ViewPrintPreview.h"
#include "..\include\graph.h"
#include ".\viewprintpreview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewPrintPreview

IMPLEMENT_DYNCREATE(CViewPrintPreview, CView)

CViewPrintPreview::CViewPrintPreview()
{
	m_pOldFrame=NULL;
}

CViewPrintPreview::~CViewPrintPreview()
{
}


BEGIN_MESSAGE_MAP(CViewPrintPreview, CView)
	//{{AFX_MSG_MAP(CViewPrintPreview)
	ON_COMMAND(AFX_ID_PREVIEW_PRINT, OnPreviewPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewPrintPreview drawing

void CViewPrintPreview::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CViewPrintPreview diagnostics

#ifdef _DEBUG
void CViewPrintPreview::AssertValid() const
{
	CView::AssertValid();
}

void CViewPrintPreview::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewPrintPreview message handlers

void CViewPrintPreview::OnFilePrintPreview()
{
/*
	CPrintPreviewState* pState = new CPrintPreviewState;

	if (!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this,
							RUNTIME_CLASS(CViewPrintPreview), pState))
	{

		TRACE(traceAppMsg, 0, "Error: DoPrintPreview failed.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}
*/
	CView::OnFilePrintPreview();
}

BOOL CViewPrintPreview::OnPreparePrinting(CPrintInfo* pInfo) 
{
	return DoPreparePrinting(pInfo);
}

void CViewPrintPreview::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	m_pGraph->OnBeginPrinting(pDC, pInfo);
}

void CViewPrintPreview::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	m_pGraph->OnPrint(pDC, pInfo);
}

void CViewPrintPreview::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	m_pGraph->OnEndPrinting(pDC, pInfo);
}

void CViewPrintPreview::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
	// Show the original frame
	m_pOldFrame->ShowWindow(SW_SHOW);
	// Restore main frame pointer
	AfxGetApp()->m_pMainWnd = m_pOldFrame;
	m_pGraph->OnEndPrintPreview();
	// Kill parent frame and itself
	GetParentFrame()->DestroyWindow();
}

void CViewPrintPreview::OnPreviewPrint()
{
	m_pGraph->Print();
}

BOOL CViewPrintPreview::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (wParam == AFX_ID_PREVIEW_PRINT)
	{
		OnPreviewPrint();
		return TRUE;
	}
	else
		return CView::OnCommand(wParam, lParam);
}

void CViewPrintPreview::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
/*
	PRINTDLG   pd;
	pd.lStructSize = (DWORD) sizeof(PRINTDLG);
	if (AfxGetApp()->GetPrinterDeviceDefaults(&pd))
	{
		LPDEVMODE lpDevMode = (LPDEVMODE)::GlobalLock(pd.hDevMode);
		if (lpDevMode)
			lpDevMode->dmOrientation = DMORIENT_LANDSCAPE;
		pDC->ResetDC(lpDevMode);
		::GlobalUnlock(pd.hDevMode);
	}
*/
	CView::OnPrepareDC(pDC, pInfo);
}
