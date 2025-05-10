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

// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "ChildView.h"
#include "GraphView.h"
#include "pch.h"

class CGraph;
class CGraphView;

class CGraphWnd : public CFrameWndEx
{
	
public:
	CGraphWnd() noexcept;
protected: 
	DECLARE_DYNAMIC(CGraphWnd)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CGraphWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCRibbonBar				m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages			m_PanelImages;
	CMFCRibbonStatusBar			m_wndStatusBar;
	CChildView					m_wndView;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	void	InitGraphInfo(CGraph* p_CGraph);
	void	InvalidateView(void);

	afx_msg LRESULT OnCoordinatesUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSaveData(void);
	afx_msg void OnOpenData(void);
	afx_msg void OnCopyToClipboard(void);
	afx_msg void OnExportToFile(void);
	afx_msg void OnExportToCSV(void);
	afx_msg void OnExportToXML(void);
	afx_msg void OnSavePreset(void);
	afx_msg void OnOpenPreset(void);
	afx_msg void OnResetPreset(void);
	afx_msg void OnLegendMoveSize(void);
	afx_msg void OnLegendHide(void);
	afx_msg void OnLegendProperties(void);
	afx_msg void OnGraphProperties(void);
	afx_msg void OnDataProperties(void);
	afx_msg void OnData100(void);


	//afx_msg void OnCheckGRAPHCOMPONENTS_TITLE();
	//afx_msg void OnUpdateGRAPHCOMPONENTS_TITLE(CCmdUI* pCmdUI);
	afx_msg void OnGraphComponents(UINT nID);
	afx_msg void OnUpdateUIGraphComponents(CCmdUI* pCmdUI);
//	afx_msg void OnSelectionAndZomm(UINT nID);
	afx_msg void OnUpdateUIZomm(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIUnZomm(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIZomm100(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUISELECTIONTOOL(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUICROSSTOOL(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIPANTOOL(CCmdUI* pCmdUI);

	afx_msg void OnUpdateStatusPage2(CCmdUI* pCmdUI);
	


	afx_msg void OnZoom(void);
	afx_msg void OnUnZoom(void);
	afx_msg void On100Zoom(void);
	afx_msg void OnPan(void);
	afx_msg void OnCross(void);
	afx_msg void OnSelectMode(void);

	void UpdateStatusBar(void);
public:
	static CString	m_GraphViewClassName;
	static CString	m_ClassName;
	static HICON	m_hIcon;

	CGraphView* m_pView;
	CGraph*		m_pGraph;

};


