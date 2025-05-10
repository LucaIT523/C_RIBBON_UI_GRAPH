#pragma once

#include "types.h"
//#include "resource_graph.h"
#include "Resource.h"

class CGraphCtrl;
class CGraphView;

// CGraphPropertiesGeneral dialog

class CGraphPropertiesGeneral : public CPropertyPage
{
	DECLARE_DYNAMIC(CGraphPropertiesGeneral)

public:
	CGraphPropertiesGeneral();
	virtual ~CGraphPropertiesGeneral();

// Dialog Data
	enum { IDD = IDD_GRAPH_PROPERTIES_GENERAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_ShowTitle;
	BOOL m_ShowSubTitle;
	BOOL m_ShowXLabel;
	BOOL m_ShowYLabel;
	CString m_Title;
	CString m_SubTitle;
	CString m_XLabel;
	CString m_YLabel;
	FONT m_TitleFont;
	FONT m_SubTitleFont;
	FONT m_XLabelFont;
	FONT m_YLabelFont;
	BACKGROUND m_BackGround;
	CString m_GradientType;
	int m_FillType;

	CGraphCtrl* m_pGraphCtrl;
	CGraphView* m_pView;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedShowtitle();
	afx_msg void OnBnClickedShowsubtitle();
	afx_msg void OnBnClickedShowxlabel();
	afx_msg void OnBnClickedShowylabel();
	afx_msg void OnEnChangeTitle();
	afx_msg void OnEnChangeSubtitle();
	afx_msg void OnEnChangeXlabel();
	afx_msg void OnEnChangeYlabel();
	afx_msg void OnBnClickedTitlefont();
	afx_msg void OnBnClickedSubtitlefont();
	afx_msg void OnBnClickedXlabelfont();
	afx_msg void OnBnClickedYlabelfont();
	afx_msg void OnBnClickedFillNone();
	afx_msg void OnBnClickedFillSolid();
	afx_msg void OnBnClickedFillGradient();
	afx_msg void OnBnClickedGrtypeSelect();
	afx_msg void OnBnClickedColor1Select();
	afx_msg void OnBnClickedColor2Select();
	afx_msg void OnCustomDrawColors(NMHDR* pNMHDR, LRESULT* pResult);
protected:
	void UpdateControlsState(void);
public:
	virtual BOOL OnApply();
};
