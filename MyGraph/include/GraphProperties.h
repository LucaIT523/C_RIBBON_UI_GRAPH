#pragma once

#include "graphpropertiesgeneral.h"
#include "graphpropertiesmargins.h"

// CGraphProperties

class CGraphProperties : public CPropertySheet
{
	DECLARE_DYNAMIC(CGraphProperties)

public:
	CGraphProperties(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CGraphProperties(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CGraphProperties();

protected:
	DECLARE_MESSAGE_MAP()
	CGraphPropertiesGeneral m_GeneralPage;
	CGraphPropertiesMargins m_MarginsPage;
protected:
	void AddPages(void);
public:
	void SetGraphCtrl(CGraphCtrl* pCtrl);
	void SetGraphView(CGraphView* pView);
};


