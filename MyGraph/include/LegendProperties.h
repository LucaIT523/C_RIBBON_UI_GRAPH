#pragma once

#include "legendpropertiesgeneral.h"

// CLegendProperties

class CLegendProperties : public CPropertySheet
{
	DECLARE_DYNAMIC(CLegendProperties)

public:
	CLegendProperties(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CLegendProperties(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CLegendProperties();

protected:
	DECLARE_MESSAGE_MAP()
	void AddPages(void);
	CLegendPropertiesGeneral m_GeneralPage;
public:
	void SetGraphCtrl(CGraphCtrl* pCtrl);
	void SetGraphView(CGraphView* pView);
};


