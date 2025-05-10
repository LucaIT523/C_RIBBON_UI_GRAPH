#pragma once

#include "types.h"
#include "resource.h"

class CGraphCtrl;
class CGraphView;
// CLegendPropertiesGeneral dialog

class CLegendPropertiesGeneral : public CPropertyPage
{
	DECLARE_DYNAMIC(CLegendPropertiesGeneral)

public:
	CLegendPropertiesGeneral();
	virtual ~CLegendPropertiesGeneral();

// Dialog Data
	enum { IDD = IDD_LEGEND_PROPERTIES_GENERAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedColor1Select();
	afx_msg void OnBnClickedColor2Select();
	afx_msg void OnCustomDrawColors(NMHDR* pNMHDR, LRESULT* pResult);
public:
	BOOL m_LegendVisible;
	BOOL m_AutoPos;
	int m_BorderWidth;
	int m_FillType;
	BACKGROUND m_BackGround;
	CString m_GradientType;
	FONT m_Font;
	COLORREF m_BorderColor;
	
	virtual BOOL OnInitDialog();
	CGraphCtrl* m_pGraphCtrl;
	CGraphView* m_pView;
	afx_msg void OnBnClickedFillNone();
	afx_msg void OnBnClickedFillSolid();
	afx_msg void OnBnClickedFillGradient();
	afx_msg void OnBnClickedGrtypeSelect();
protected:
	void UpdateControlsState(void);
public:
	virtual BOOL OnApply();
	afx_msg void OnBnClickedFont();
	afx_msg void OnBnClickedLegendVisible();
	afx_msg void OnEnChangeBorderWidth();
	afx_msg void OnBnClickedLegendAutopos();
protected:
	BOOL Validate(void);
public:
	afx_msg void OnBnClickedBorderColorSelect();
};
