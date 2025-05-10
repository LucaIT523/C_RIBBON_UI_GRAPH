// GraphPropertiesMargins.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\graph.h"
#include "..\include\constants.h"
#include "..\include\GraphPropertiesMargins.h"
#include "..\include\graphctrl.h"
#include "..\include\graphview.h"
#include "..\include\tools.h"


// CGraphPropertiesMargins dialog

IMPLEMENT_DYNAMIC(CGraphPropertiesMargins, CPropertyPage)
CGraphPropertiesMargins::CGraphPropertiesMargins()
	: CPropertyPage(CGraphPropertiesMargins::IDD)
	, m_Left(LABELSTOBORDER)
	, m_Right(LABELSTOBORDER)
	, m_Top(LABELSTOBORDER)
	, m_Bottom(LABELSTOBORDER)
{
	m_hPageNone = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PAGE_NONE));
	m_hPageLeft = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PAGE_LEFT));
	m_hPageTop = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PAGE_TOP));
	m_hPageRight = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PAGE_RIGHT));
	m_hPageBottom = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PAGE_BOTTOM));
}

CGraphPropertiesMargins::~CGraphPropertiesMargins()
{
	if (m_hPageNone) DeleteObject(m_hPageNone);
	if (m_hPageLeft) DeleteObject(m_hPageLeft);
	if (m_hPageTop) DeleteObject(m_hPageTop);
	if (m_hPageRight) DeleteObject(m_hPageRight);
	if (m_hPageBottom) DeleteObject(m_hPageBottom);
}

void CGraphPropertiesMargins::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CGraphPropertiesMargins)
	DDX_Control(pDX, IDC_PAGE, m_cPageLayout);
	DDX_Text(pDX, IDC_LEFT, m_Left);
	DDX_Text(pDX, IDC_RIGHT, m_Right);
	DDX_Text(pDX, IDC_TOP, m_Top);
	DDX_Text(pDX, IDC_BOTTOM, m_Bottom);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGraphPropertiesMargins, CPropertyPage)
	ON_EN_SETFOCUS(IDC_LEFT, OnEnSetfocus)
	ON_EN_SETFOCUS(IDC_TOP, OnEnSetfocus)
	ON_EN_SETFOCUS(IDC_RIGHT, OnEnSetfocus)
	ON_EN_SETFOCUS(IDC_BOTTOM, OnEnSetfocus)
	ON_EN_KILLFOCUS(IDC_LEFT, OnEnKillfocus)
	ON_EN_KILLFOCUS(IDC_TOP, OnEnKillfocus)
	ON_EN_KILLFOCUS(IDC_RIGHT, OnEnKillfocus)
	ON_EN_KILLFOCUS(IDC_BOTTOM, OnEnKillfocus)
	ON_EN_CHANGE(IDC_LEFT, OnEnChange)
	ON_EN_CHANGE(IDC_RIGHT, OnEnChange)
	ON_EN_CHANGE(IDC_TOP, OnEnChange)
	ON_EN_CHANGE(IDC_BOTTOM, OnEnChange)
END_MESSAGE_MAP()


// CGraphPropertiesMargins message handlers

void CGraphPropertiesMargins::OnEnSetfocus()
{
	switch (GetFocus()->GetDlgCtrlID())
	{
	case IDC_LEFT: m_cPageLayout.SetBitmap(m_hPageLeft); break;
	case IDC_TOP: m_cPageLayout.SetBitmap(m_hPageTop); break;
	case IDC_RIGHT: m_cPageLayout.SetBitmap(m_hPageRight); break;
	case IDC_BOTTOM: m_cPageLayout.SetBitmap(m_hPageBottom); break;
	default: m_cPageLayout.SetBitmap(m_hPageNone); break;
	}
	m_cPageLayout.Invalidate();
}

void CGraphPropertiesMargins::OnEnKillfocus()
{
	m_cPageLayout.SetBitmap(m_hPageNone);
}

BOOL CGraphPropertiesMargins::OnApply()
{
	UpdateData(TRUE);

	if (!Validate()) return FALSE;

	if (m_pGraphCtrl)
	{
		m_pGraphCtrl->m_LeftBorder = m_Left;
		m_pGraphCtrl->m_RightBorder = m_Right;
		m_pGraphCtrl->m_TopBorder = m_Top;
		m_pGraphCtrl->m_BottomBorder = m_Bottom;

		if (m_pView)
		{
			m_pView->Resize();
			m_pView->Invalidate();
		}
	}

	return CPropertyPage::OnApply();
}

BOOL CGraphPropertiesMargins::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	GetDlgItem(IDC_LEFT_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)1000, (short)0));
	GetDlgItem(IDC_TOP_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)1000, (short)0));
	GetDlgItem(IDC_RIGHT_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)1000, (short)0));
	GetDlgItem(IDC_BOTTOM_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)1000, (short)0));

	if (m_pGraphCtrl)
	{
		m_Left = m_pGraphCtrl->m_LeftBorder;
		m_Right = m_pGraphCtrl->m_RightBorder;
		m_Top = m_pGraphCtrl->m_TopBorder;
		m_Bottom = m_pGraphCtrl->m_BottomBorder;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGraphPropertiesMargins::OnEnChange()
{
	SetModified(TRUE);
}

BOOL CGraphPropertiesMargins::Validate(void)
{
	if (!CheckIntMinMax(0, 1000, m_Left, this, GetDlgItem(IDC_LEFT))) return FALSE;
	if (!CheckIntMinMax(0, 1000, m_Right, this, GetDlgItem(IDC_RIGHT))) return FALSE;
	if (!CheckIntMinMax(0, 1000, m_Top, this, GetDlgItem(IDC_TOP))) return FALSE;
	if (!CheckIntMinMax(0, 1000, m_Bottom, this, GetDlgItem(IDC_BOTTOM))) return FALSE;

	return TRUE;
}
