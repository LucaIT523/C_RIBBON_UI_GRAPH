#pragma once

#include "types.h"
#include "resource.h"

class CGraphCtrl;
class CGraphView;

// CDataPropertiesGeneral dialog

class CDataPropertiesGeneral : public CPropertyPage
{
	DECLARE_DYNAMIC(CDataPropertiesGeneral)

public:
	CDataPropertiesGeneral();
	virtual ~CDataPropertiesGeneral();

// Dialog Data
	enum { IDD = IDD_DATA_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFill();
	afx_msg void OnBnClickedGrtypeSelect();
	afx_msg void OnBnClickedColor1Select();
	afx_msg void OnBnClickedColor2Select();
	afx_msg void OnBnClickedFont();
	afx_msg void OnCustomDrawColors(NMHDR* pNMHDR, LRESULT* pResult);

	int m_BorderWidth;
	int m_PointSize;
	int m_FillType;
	BACKGROUND m_BackGround;
	CString m_GradientType;
	int m_Style;
	COLORREF m_BorderColor;

	CGraphCtrl* m_pGraphCtrl;
	CGraphView* m_pView;
protected:
	void UpdateControlsState(void);
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	afx_msg void OnBnClickedYfont();
	afx_msg void OnEnChange();
protected:
	BOOL Validate(void);
public:
	afx_msg void OnBnClickedBorderColorSelect();
	bool m_bLogX;
	bool m_bLogY;
	bool m_bShowLogZero;
	afx_msg void OnBnClickedCheckLogX();
	afx_msg void OnBnClickedCheckLogY();
	afx_msg void OnBnClickedCheckShowLogZero();
};
