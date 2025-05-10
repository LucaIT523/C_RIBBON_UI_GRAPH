// LegendProperties.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\LegendProperties.h"


// CLegendProperties

IMPLEMENT_DYNAMIC(CLegendProperties, CPropertySheet)
CLegendProperties::CLegendProperties(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPages();
}

CLegendProperties::CLegendProperties(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPages();
}

CLegendProperties::~CLegendProperties()
{
}


BEGIN_MESSAGE_MAP(CLegendProperties, CPropertySheet)
END_MESSAGE_MAP()


// CLegendProperties message handlers

void CLegendProperties::AddPages(void)
{
	AddPage(&m_GeneralPage);
}

void CLegendProperties::SetGraphCtrl(CGraphCtrl* pCtrl)
{
	m_GeneralPage.m_pGraphCtrl = pCtrl;
}

void CLegendProperties::SetGraphView(CGraphView* pView)
{
	m_GeneralPage.m_pView = pView;
}
