#pragma once

#include <afxtempl.h>
#include "types.h"
#include "resource.h"
#include "bitmaps\CSimpleDib.h"

class CGraphCtrl;
class CGraphView;
class CMarkerScene;
class CLineScene;
class CSimpleDIB;

struct SERIEINFO
{
	BOOL visible;
	BOOL legend;
	int marker;
	int line;
	int linewidth;
	int size;
	int markers;
	COLORREF color;
	COLORREF markercolor;
	TCHAR name[256];
};

// CDataPropertiesSeries dialog

class CDataPropertiesSeries : public CPropertyPage
{
	DECLARE_DYNAMIC(CDataPropertiesSeries)

public:
	CDataPropertiesSeries();
	virtual ~CDataPropertiesSeries();

// Dialog Data
	enum { IDD = IDD_SERIES_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_cSeriesList;
	CComboBox m_cMarkerStyle;
	CComboBox m_cLineStyle;
	CList<SERIEINFO, SERIEINFO&> m_Series;
	int m_nCurrentSerie;

	BOOL m_curVisible;
	BOOL m_curLegend;
	int m_curMarkerStyle;
	int m_curLineStyle;
	int m_curMarkerSize;
	int m_curMarkers;
	CString m_curName;
	COLORREF m_curColor;
	COLORREF m_curMarkerColor;

	CGraphCtrl* m_pGraphCtrl;
	CGraphView* m_pView;

	afx_msg void OnBnClickedVisible();
	afx_msg void OnBnClickedVisibleLegend();
	afx_msg void OnEnChangeMarkerSize();
	afx_msg void OnBnClickedColorSelect();
	afx_msg void OnLvnItemchangedSeries(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomDrawColors(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeName();
protected:
	BOOL SelectSerie(int nIndex);
	BOOL SaveData(int nIndex);
	void LoadData(int nIndex);
public:
	virtual BOOL OnApply();
protected:
	void UpdateControls(void);
	CImageList m_ListImages;
	CSimpleDIB m_MarkerDIB;
	CSimpleDIB m_LineDIB;
	CMarkerScene* m_pMarkerScene;
	CLineScene* m_pLineScene;
public:
	afx_msg void OnCbnSelendokMarkerStyle();
	afx_msg void OnCbnSelendokLineStyle();
	afx_msg LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListViewUpdate(WPARAM wParam, LPARAM lParam);
protected:
	void CreateImageList(void);
	BOOL m_ItemChangingHandling;
public:
	afx_msg void OnBnClickedMarkerColorSelect();
	afx_msg void OnEnChangeMarkersCount();
protected:
	BOOL Validate(void);
public:
	afx_msg void OnLvnItemchangingSeries(NMHDR *pNMHDR, LRESULT *pResult);
	int m_cLineWidth;
	afx_msg void OnEnChangeLineWidth();
  afx_msg void OnBnClickedButtonFft();
};
