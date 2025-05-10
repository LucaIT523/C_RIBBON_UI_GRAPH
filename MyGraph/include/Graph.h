#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "types.h"
//#include "resource_graph.h"
#include "resource.h"

// CGraph command target
#include <msxml2.h>





class CGraphWnd;
class CGraphView;
class CGraphCtrl;

class DLL_EXP CGraph : public CObject
{
public:
	CGraph(HWND hParentWnd = NULL);
	virtual ~CGraph();
public:
	CGraphWnd* m_pGraphWnd;
	CObList m_Graphs;
  HWND m_hParentWnd;
public:

	BOOL	bWindowOpen, bSendClosingMessage;
	BOOL	GetGraphWindowVisible(); // added August 22 2023
	CGraph	**ppGraphAlive;
	void	SendClosingMessage();



//	BOOL Create(LPRECT lpInitRect = NULL, LPCTSTR lpWindowTitle = NULL);
	BOOL Show(LPCTSTR lpWindowTitle = NULL);
	int GetGraphsCount(void);
	CGraphCtrl* GetGraphCtrl(int index);
	void DeleteGraphCtrl(int index);
	CGraphCtrl* AddGraphCtrl(void);
	void GetGraphRect(CGraphCtrl* pGraph, LPRECT rect, LPRECT total);
	void GetGraphRect(int index, LPRECT rect, LPRECT total);
	double m_ZoomFactor;
	double m_MinZoomFactor;
	double m_MaxZoomFactor;
	BOOL m_bFFTButtonEnabled;
	CGraphCtrl* GetGraphCtrlForPoint(CPoint point, CPoint* localPoint = NULL);
protected:
	BOOL m_Prepared;
	LPTSTR m_pDefaultTitle;
public:
	void GraphWndDestroyed(void);
	void GetRect(CRect* lpRect);
	BOOL SavePreset(LPCTSTR lpFileName, CGraphCtrl* pGraphCtrl = NULL);
	BOOL LoadPreset(LPCTSTR lpFileName, CGraphCtrl* pGraphCtrl = NULL);
	BOOL SaveGraph(LPCTSTR lpFileName, CGraphCtrl* pGraphCtrl = NULL);
	BOOL LoadGraph(LPCTSTR lpFileName, CGraphCtrl* pGraphCtrl = NULL);
	BOOL SaveGraphData(LPCTSTR lpFileName, CGraphCtrl* pGraphCtrl = NULL);
	BOOL LoadGraphData(LPCTSTR lpFileName, CGraphCtrl* pGraphCtrl = NULL);
protected:
	IXMLDOMDocument2* CreateXMLDocument(void);
	void FreeXMLDocument(IXMLDOMDocument2* pDocument);
public:
	static BOOL Initialize(void);
	static void Uninitialize(void);
protected:
	VARIANT GetXMLAttribute(IXMLDOMElement* pRoot, LPCTSTR lpNode, LPCTSTR lpAttribute);
public:
	void ReadXMLIntAttribute(IXMLDOMElement* pRoot, LPCTSTR lpNode, LPCTSTR lpAttribute, int* lpData);
	void ReadXMLDoubleAttribute(IXMLDOMElement* pRoot, LPCTSTR lpNode, LPCTSTR lpAttribute, double* lpData);
	void ReadXMLStringAttribute(IXMLDOMElement* pRoot, LPCTSTR lpNode, LPCTSTR lpAttribute, CString* lpData);
protected:
	BOOL SavePresetToXML(IXMLDOMDocument2* pDocument, IXMLDOMElement* lpRoot, CGraphCtrl* pCtrl);
	BOOL LoadPresetFromXML(IXMLDOMDocument2* pDocument, IXMLDOMElement* lpRoot, CGraphCtrl* pCtrl);
	BOOL SaveGraphDataToXML(IXMLDOMDocument2* pDocument, IXMLDOMElement* lpRoot, CGraphCtrl* pCtrl);
	BOOL LoadGraphDataFromXML(IXMLDOMDocument2* pDocument, IXMLDOMElement* lpRoot, CGraphCtrl* pCtrl);
public:
	void ResetPreset(void);
	CGraphView* GetView(void);
	void SetWindowTitle(LPCTSTR lpWindowTitle = NULL);
	void CloseGraph(void);	

    void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
    void OnPrint(CDC *pDC, CPrintInfo *pInfo);
    void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);

	void OnEndPrintPreview();
	void Print();
  HWND GetParent();
};


