#pragma once

#include "datapropertiesgeneral.h"
#include "datapropertiesseries.h"
#include "datapropertiesgrids.h"
#include "datapropertieslabels.h"

// CDataProperties

class CDataProperties : public CPropertySheet
{
	DECLARE_DYNAMIC(CDataProperties)

public:
	CDataProperties(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDataProperties(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDataProperties();

protected:
	DECLARE_MESSAGE_MAP()
	CDataPropertiesGeneral m_GeneralPage;
	CDataPropertiesSeries m_SeriesPage;
	CDataPropertiesGrids m_GridsPage;
	CDataPropertiesLabels m_LabelsPage;
	void AddPages(void);
public:
	void SetGraphCtrl(CGraphCtrl* pCtrl);
	void SetGraphView(CGraphView* pView);
};


