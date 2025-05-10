// GraphProperties.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\GraphProperties.h"


// CGraphProperties

IMPLEMENT_DYNAMIC(CGraphProperties, CPropertySheet)
CGraphProperties::CGraphProperties(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPages();
}

CGraphProperties::CGraphProperties(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPages();
}

CGraphProperties::~CGraphProperties()
{
}


BEGIN_MESSAGE_MAP(CGraphProperties, CPropertySheet)
END_MESSAGE_MAP()


// CGraphProperties message handlers

void CGraphProperties::AddPages(void)
{
	AddPage(&m_GeneralPage);
	AddPage(&m_MarginsPage);
}

void CGraphProperties::SetGraphCtrl(CGraphCtrl* pCtrl)
{
	m_GeneralPage.m_pGraphCtrl = pCtrl;
	m_MarginsPage.m_pGraphCtrl = pCtrl;
}

void CGraphProperties::SetGraphView(CGraphView* pView)
{
	m_GeneralPage.m_pView = pView;
	m_MarginsPage.m_pView = pView;
}
