#pragma once

#include "types.h"
#include "bitmaps\CSimpleDib.h"

// CGraphView

class CGraph;
class CSimpleDIB;

class CGraphView : public CWnd
{
	friend class CGraph;
public:
	CGraphView(CGraph* pGraph = NULL);
	virtual ~CGraphView();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	CGraph* m_pGraph;
	GRAPH_ZOOMMODE m_ZoomMode;
public:
	CGraph* GetGraph(void);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	GRAPH_ZOOMMODE GetZoomMode(void);
	void SetZoomMode(GRAPH_ZOOMMODE mode);
protected:
	void UpdateCursor(void);
	CSimpleDIB clientDIB;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
protected:
	HCURSOR m_hZoomCursor;
	HCURSOR m_hUnZoomCursor;
	HCURSOR m_hStdCursor;
	HCURSOR m_hNoCursor;
	HCURSOR m_hHandCursor;
	HCURSOR m_hCrossCursor;
	HCURSOR m_hSizeAllCursor;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
protected:
	void LoadCursors(void);
	void DestroyCursors(void);
	LMOUSEDOWNINFO m_LButtonInfo;
	void GetLMouseInfoForPoint(LMOUSEDOWNINFO* pInfo, CPoint point);
	CRect m_ZoomRect;
	CPoint m_PanPoint;
	CPoint m_CrossPoint;
	BOOL m_bPrintPreview;
	CSingleDocTemplate* m_pTemplate;
	void DrawZoomRect(CDC* pDC);
	BOOL m_Zooming;
	BOOL m_Panning;
	GRAPHPOINT m_GraphMousePos;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	void CaptureMouse(void);
	void StopCrossing(void);
	void DrawCross(CDC* pDC, CPoint pt);
public:
	void Resize(void);
	void CopyToClipboard(void);
	void ExportToFile(void);
	void ExportToCSV(void);
	void ExportToXML(void);
protected:
	int FindType(CString& ext);
	BOOL PromptForImageFileName(CString& fileName, DWORD dwFlags, BOOL bOpenFileDialog, int* pType);
	BOOL PromptForPresetFileName(CString& fileName, DWORD dwFlags, BOOL bOpenFileDialog);
	BOOL PromptForDataFileName(CString& fileName, DWORD dwFlags, BOOL bOpenFileDialog);
	BOOL PromptForGraphFileName(CString& fileName, DWORD dwFlags, BOOL bOpenFileDialog);
	BOOL PromptForCSVFileName(CString& fileName, DWORD dwFlags);
	BOOL PromptForXMLFileName(CString& fileName, DWORD dwFlags);
	CString GetExtFromType(int nDocType);
	CString GetFileTypes(BOOL bOpenFileDialog);
	int GetIndexFromType(int nDocType, BOOL bOpenFileDialog);
	int GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog);
public:
	void SavePreset(void);
	void LoadPreset(LPCTSTR pchFile = NULL);
	void SaveData(void);
	void LoadData(void);
	void SaveGraph(void);
	void LoadGraph(void);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
protected:
	void LoadMenus(void);
	void DestroyMenus(void);
	HMENU m_hContextMenu;
	CGraphCtrl* m_pSelectedGraph;
	void UpdateContextMenu(HMENU hMenu);
public:
	void UpdateCommonMenuItems(HMENU hMenu);
	void DoGraphComponentsClick(UINT nID);
	BOOL UpdateCmdUIGraphComponentsClick(UINT nID);
	void OnLegendMoveSize(void);
	void OnLegendHide(void);
	void OnLegendProperties(void);
	void OnGraphProperties(void);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
	CRectTracker m_LegendTracker;
	void RecalcLegendTrackerRect(CGraphCtrl* pGraph);
	void RecalcLegendRect(CGraphCtrl* pGraph);
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void StopMoving(void);
	void OnDataProperties(void);
	void OnData100(void);
	BOOL IsPanning(void);
	void OnGraphPageSetup(void);
	void OnGraphPrintPreview(void);
	void OnGraphPrint(void);
};


