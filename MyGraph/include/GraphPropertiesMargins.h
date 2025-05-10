#pragma once

#include "resource.h"

class CGraphCtrl;
class CGraphView;

// CGraphPropertiesMargins dialog

class CGraphPropertiesMargins : public CPropertyPage
{
	DECLARE_DYNAMIC(CGraphPropertiesMargins)

public:
	CGraphPropertiesMargins();
	virtual ~CGraphPropertiesMargins();

	CGraphCtrl* m_pGraphCtrl;
	CGraphView* m_pView;

// Dialog Data
	enum { IDD = IDD_MARGINS_PROPETIES };

protected:
	HBITMAP m_hPageNone;
	HBITMAP m_hPageLeft;
	HBITMAP m_hPageTop;
	HBITMAP m_hPageRight;
	HBITMAP m_hPageBottom;
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnSetfocus();
	afx_msg void OnEnKillfocus();

	CStatic m_cPageLayout;
	int m_Left;
	int m_Right;
	int m_Top;
	int m_Bottom;
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChange();
protected:
	BOOL Validate(void);
};
