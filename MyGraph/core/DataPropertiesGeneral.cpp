// DataPropertiesGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\graph.h"
#include "..\include\DataPropertiesGeneral.h"
#include "..\include\linescene.h"
#include "..\include\graphctrl.h"
#include "..\include\graphview.h"
#include "..\include\tools.h"


// CDataPropertiesGeneral dialog

IMPLEMENT_DYNAMIC(CDataPropertiesGeneral, CPropertyPage)
CDataPropertiesGeneral::CDataPropertiesGeneral()
	: CPropertyPage(CDataPropertiesGeneral::IDD)
	, m_pGraphCtrl(NULL)
	, m_pView(NULL)
	, m_BorderWidth(1)
	, m_FillType(0)
	, m_GradientType("None")
	, m_PointSize(5)
	, m_BorderColor(RGB(0, 0, 0))
	, m_bLogX(false)
	, m_bLogY(false)
	, m_bShowLogZero(false)
{
}

CDataPropertiesGeneral::~CDataPropertiesGeneral()
{
}

void CDataPropertiesGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CDataPropertiesGeneral)
	DDX_Text(pDX, IDC_BORDER_WIDTH, m_BorderWidth);
	DDX_Radio(pDX, IDC_FILL_NONE, m_FillType);
	DDX_Text(pDX, IDC_GRADIENT_TYPE, m_GradientType);
	DDX_Radio(pDX, IDC_STYLE_LINES, m_Style);
	DDX_Text(pDX, IDC_POINT_SIZE, m_PointSize);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataPropertiesGeneral, CPropertyPage)
	ON_BN_CLICKED(IDC_FILL_NONE, OnBnClickedFill)
	ON_BN_CLICKED(IDC_FILL_SOLID, OnBnClickedFill)
	ON_BN_CLICKED(IDC_FILL_GRADIENT, OnBnClickedFill)
	ON_BN_CLICKED(IDC_GRTYPE_SELECT, OnBnClickedGrtypeSelect)
	ON_BN_CLICKED(IDC_COLOR1_SELECT, OnBnClickedColor1Select)
	ON_BN_CLICKED(IDC_COLOR2_SELECT, OnBnClickedColor2Select)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COLOR1, OnCustomDrawColors)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COLOR2, OnCustomDrawColors)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BORDER_COLOR, OnCustomDrawColors)
	ON_BN_CLICKED(IDC_STYLE_LINES, OnBnClickedFill)
	ON_BN_CLICKED(IDC_STYLE_SCATTER, OnBnClickedFill)
	ON_BN_CLICKED(IDC_STYLE_STEM, OnBnClickedFill)
	ON_EN_CHANGE(IDC_BORDER_WIDTH, OnEnChange)
	ON_EN_CHANGE(IDC_LINE_WIDTH, OnEnChange)
	ON_EN_CHANGE(IDC_POINT_SIZE, OnEnChange)
	ON_BN_CLICKED(IDC_BORDER_COLOR_SELECT, OnBnClickedBorderColorSelect)
	ON_BN_CLICKED(IDC_CHECK_LOG_X, OnBnClickedCheckLogX)
	ON_BN_CLICKED(IDC_CHECK_LOG_Y, OnBnClickedCheckLogY)
	ON_BN_CLICKED(IDC_CHECK_SHOW_LOG_ZERO, OnBnClickedCheckShowLogZero)
END_MESSAGE_MAP()


// CDataPropertiesGeneral message handlers

void CDataPropertiesGeneral::OnBnClickedFill()
{
	UpdateControlsState();
	SetModified(TRUE);
}

void CDataPropertiesGeneral::OnBnClickedGrtypeSelect()
{
	HMENU hMenu = LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DIALOGS_CONTEXT));
	if (hMenu)
	{
		HMENU hSubMenu = GetSubMenu(hMenu, 0);
		if (hSubMenu)
		{
			CRect rect;
			GetDlgItem(IDC_GRTYPE_SELECT)->GetWindowRect(rect);
			CheckMenuItem(hSubMenu, m_BackGround.grType + ID_GRADIENT_NONE, MF_CHECKED | MF_BYCOMMAND);
			UINT nCmd = TrackPopupMenu(hSubMenu, TPM_RETURNCMD, rect.left, rect.bottom, NULL, m_hWnd, NULL);
			if (m_pGraphCtrl && nCmd >= ID_GRADIENT_LEFTTORIGHT && nCmd <= ID_GRADIENT_RIGHTTOPTOLEFTBOTTOM)
			{
				m_BackGround.grType = (GRAPH_GRADIENTTYPE)(nCmd - ID_GRADIENT_NONE);
				GetMenuString(hSubMenu, nCmd, m_GradientType.GetBuffer(256), 256, MF_BYCOMMAND);
				m_GradientType.ReleaseBuffer();
				UpdateData(FALSE);
				SetModified(TRUE);
			}
		}
		DestroyMenu(hMenu);
	}
}

void CDataPropertiesGeneral::OnBnClickedColor1Select()
{
	CColorDialog dlg(m_BackGround.Color1, CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT, this);
	if (dlg.DoModal() == IDOK)
	{
		m_BackGround.Color1 = dlg.GetColor();
		GetDlgItem(IDC_COLOR1)->Invalidate();
		SetModified(TRUE);
	}
}

void CDataPropertiesGeneral::OnBnClickedColor2Select()
{
	CColorDialog dlg(m_BackGround.Color2, CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT, this);
	if (dlg.DoModal() == IDOK)
	{
		m_BackGround.Color2 = dlg.GetColor();
		GetDlgItem(IDC_COLOR2)->Invalidate();
		SetModified(TRUE);
	}
}

void CDataPropertiesGeneral::UpdateControlsState(void)
{
	UpdateData(TRUE);

	GetDlgItem(IDC_GRADIENT_TYPE)->EnableWindow((GRAPH_FILLTYPE)m_FillType == GRAPH_FT_GRADIENT);
	GetDlgItem(IDC_GRTYPE_SELECT)->EnableWindow((GRAPH_FILLTYPE)m_FillType == GRAPH_FT_GRADIENT);
	GetDlgItem(IDC_COLOR1_SELECT)->EnableWindow((GRAPH_FILLTYPE)m_FillType != GRAPH_FT_NONE);
	GetDlgItem(IDC_COLOR1_STATIC)->EnableWindow((GRAPH_FILLTYPE)m_FillType != GRAPH_FT_NONE);
	GetDlgItem(IDC_COLOR2_SELECT)->EnableWindow((GRAPH_FILLTYPE)m_FillType == GRAPH_FT_GRADIENT);
	GetDlgItem(IDC_COLOR2_STATIC)->EnableWindow((GRAPH_FILLTYPE)m_FillType == GRAPH_FT_GRADIENT);
	GetDlgItem(IDC_COLOR1)->ShowWindow((GRAPH_FILLTYPE)m_FillType != GRAPH_FT_NONE ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_COLOR2)->ShowWindow((GRAPH_FILLTYPE)m_FillType == GRAPH_FT_GRADIENT ? SW_SHOW : SW_HIDE);
	CheckDlgButton(IDC_CHECK_LOG_X, m_bLogX);
	CheckDlgButton(IDC_CHECK_LOG_Y, m_bLogY);
	CheckDlgButton(IDC_CHECK_SHOW_LOG_ZERO, m_bShowLogZero);
}

void CDataPropertiesGeneral::OnCustomDrawColors(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	LPNMCUSTOMDRAW pNMDraw = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if (pNMDraw->hdr.idFrom != IDC_COLOR1 && pNMDraw->hdr.idFrom != IDC_COLOR2 && pNMDraw->hdr.idFrom != IDC_BORDER_COLOR) 
		return;
	CWnd* pCtrl = GetDlgItem((int)pNMDraw->hdr.idFrom);
	if (pCtrl)
	{
		COLORREF color = RGB(0, 0, 0);
		if (pNMDraw->hdr.idFrom == IDC_COLOR1)
			color = m_BackGround.Color1;
		else if (pNMDraw->hdr.idFrom == IDC_COLOR2)
			color = m_BackGround.Color2;
		else if (pNMDraw->hdr.idFrom == IDC_BORDER_COLOR)
			color = m_BorderColor;
		FillRect(pNMDraw->hdc, &pNMDraw->rc, CBrush(color));
	}
}

BOOL CDataPropertiesGeneral::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	GetDlgItem(IDC_BORDER_WIDTH_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)10, (short)0));
	GetDlgItem(IDC_POINT_SIZE_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)10, (short)0));

	if (m_pGraphCtrl)
	{
		m_BackGround = m_pGraphCtrl->m_GraphFieldBackGround;
		m_FillType = m_BackGround.Type;
		m_BorderWidth = (int)_round(m_pGraphCtrl->m_GraphBorderWidth);
		m_PointSize = (int)_round(m_pGraphCtrl->m_PointSize);
		m_Style = m_pGraphCtrl->m_GraphStyle - 1;
		m_BorderColor = m_pGraphCtrl->m_GraphBorderColor;
		m_bLogX = m_pGraphCtrl->m_LogX==TRUE;
		m_bLogY = m_pGraphCtrl->m_LogY==TRUE;
		m_bShowLogZero = m_pGraphCtrl->m_ShowLogZero==TRUE;
	}
	UpdateData(FALSE);
	UpdateControlsState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDataPropertiesGeneral::OnApply()
{
	UpdateData(TRUE);

	if (!Validate()) return FALSE;

	if (m_pGraphCtrl)
	{
		m_BackGround.Type = (GRAPH_FILLTYPE)m_FillType;
		m_pGraphCtrl->m_GraphFieldBackGround = m_BackGround;
		m_pGraphCtrl->m_GraphBorderWidth = m_BorderWidth;
		m_pGraphCtrl->m_PointSize = m_PointSize;
		m_pGraphCtrl->m_GraphStyle = (GRAPH_GRAPHSTYLE)(m_Style + 1);
		m_pGraphCtrl->m_GraphBorderColor = m_BorderColor;
		m_pGraphCtrl->m_LogX = m_bLogX;
		m_pGraphCtrl->m_LogY = m_bLogY;
		m_pGraphCtrl->m_ShowLogZero = m_bShowLogZero;

		if (m_pView)
		{
			m_pView->Resize();
			m_pView->Invalidate();
		}
	}

	return CPropertyPage::OnApply();
}

void CDataPropertiesGeneral::OnEnChange()
{
	SetModified(TRUE);
}

BOOL CDataPropertiesGeneral::Validate(void)
{
	if (!CheckIntMinMax(0, 10, m_BorderWidth, this, GetDlgItem(IDC_BORDER_WIDTH))) return FALSE;
	if (!CheckIntMinMax(0, 10, m_PointSize, this, GetDlgItem(IDC_POINT_SIZE))) return FALSE;

	return TRUE;
}

void CDataPropertiesGeneral::OnBnClickedBorderColorSelect()
{
	CColorDialog dlg(m_BorderColor, CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT, this);
	if (dlg.DoModal() == IDOK)
	{
		m_BorderColor = dlg.GetColor();
		GetDlgItem(IDC_BORDER_COLOR)->Invalidate();
		SetModified(TRUE);
	}
}

void CDataPropertiesGeneral::OnBnClickedCheckLogX()
{
	bool bLogX = IsDlgButtonChecked(IDC_CHECK_LOG_X) != 0;
	if (m_bLogX != bLogX)
	{
		m_bLogX = bLogX;
		SetModified(TRUE);
	}
}

void CDataPropertiesGeneral::OnBnClickedCheckLogY()
{
	bool bLogY = IsDlgButtonChecked(IDC_CHECK_LOG_Y) != 0;
	if (m_bLogY != bLogY)
	{
		m_bLogY = bLogY;
		SetModified(TRUE);
	}
}

void CDataPropertiesGeneral::OnBnClickedCheckShowLogZero()
{
	bool bShowLogZero = IsDlgButtonChecked(IDC_CHECK_SHOW_LOG_ZERO) != 0;
	if (m_bShowLogZero != bShowLogZero)
	{
		m_bShowLogZero = bShowLogZero;
		SetModified(TRUE);
	}
}
