#if !defined(AFX_VIEWPRINTPREVIEW_H__137FC880_1607_11D3_9317_8F51A5F9742F__INCLUDED_)
#define AFX_VIEWPRINTPREVIEW_H__137FC880_1607_11D3_9317_8F51A5F9742F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ViewPrintPreview.h : header file
//

#include "..\include\graph.h"

/////////////////////////////////////////////////////////////////////////////
// CViewPrintPreview view

class CViewPrintPreview : public CView
{
protected:
	CViewPrintPreview();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewPrintPreview)

// Attributes
public:
	CGraph* m_pGraph;
	CFrameWnd *m_pOldFrame;

// Operations
public:
	virtual void OnFilePrintPreview();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewPrintPreview)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	//}}AFX_VIRTUAL


// Implementation
protected:
	virtual ~CViewPrintPreview();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewPrintPreview)
	afx_msg void OnPreviewPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWPRINTPREVIEW_H__137FC880_1607_11D3_9317_8F51A5F9742F__INCLUDED_)
