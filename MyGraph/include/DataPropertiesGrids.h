#pragma once

#include "bitmaps\CSimpleDib.h"
#include "resource.h"

class CGraphCtrl;
class CGraphView;
class CLineScene;
class CSimpleDIB;

class CPropertyPage;

// CDataPropertiesGrids dialog

class CDataPropertiesGrids : public CPropertyPage
{
	DECLARE_DYNAMIC(CDataPropertiesGrids)

public:
	CDataPropertiesGrids();
	virtual ~CDataPropertiesGrids();

// Dialog Data
	enum { IDD = IDD_GRIDS_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	BOOL m_Grid;
	BOOL m_SubGrid;
	BOOL m_Ticks;
	BOOL m_SubTicks;
	BOOL m_Intelligent;
	BOOL m_Symmetric;
	int m_GridLine;
	int m_SubGridLine;
	CComboBox m_cGridLine;
	CComboBox m_cSubGridLine;
	COLORREF m_GridColor;
	COLORREF m_SubGridColor;
	int m_XGridSize;
	int m_YGridSize;
	int m_XSubGrids;
	int m_YSubGrids;
	int m_MinX;
	int m_MinY;
	int m_MaxX;
	int m_MaxY;

	CGraphCtrl* m_pGraphCtrl;
	CGraphView* m_pView;

protected:
	CSimpleDIB m_LineDIB;
	CLineScene* m_pLineScene;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGridColorSelect();
	afx_msg void OnBnClickedSubgridColorSelect();
	afx_msg void OnCbnSelendok();
	afx_msg void OnCheckClicked();
	afx_msg void OnEnChange();
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCustomDrawColors(NMHDR* pNMHDR, LRESULT* pResult);
protected:
	void UpdateControlsState(void);
	void FillLineStyleCombo(CComboBox& combo);
	BOOL Validate(void);
};
