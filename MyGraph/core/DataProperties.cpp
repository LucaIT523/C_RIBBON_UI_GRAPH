// DataProperties.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\DataProperties.h"


// CDataProperties

IMPLEMENT_DYNAMIC(CDataProperties, CPropertySheet)
CDataProperties::CDataProperties(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPages();
}

CDataProperties::CDataProperties(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPages();
}

CDataProperties::~CDataProperties()
{
}


BEGIN_MESSAGE_MAP(CDataProperties, CPropertySheet)
END_MESSAGE_MAP()


// CDataProperties message handlers

void CDataProperties::AddPages(void)
{
	AddPage(&m_GeneralPage);
	AddPage(&m_LabelsPage);
	AddPage(&m_GridsPage);
	AddPage(&m_SeriesPage);
}

void CDataProperties::SetGraphCtrl(CGraphCtrl* pCtrl)
{
	m_GeneralPage.m_pGraphCtrl = pCtrl;
	m_GridsPage.m_pGraphCtrl = pCtrl;
	m_SeriesPage.m_pGraphCtrl = pCtrl;
	m_LabelsPage.m_pGraphCtrl = pCtrl;
}

void CDataProperties::SetGraphView(CGraphView* pView)
{
	m_GeneralPage.m_pView = pView;
	m_GridsPage.m_pView = pView;
	m_SeriesPage.m_pView = pView;
	m_LabelsPage.m_pView = pView;
}
