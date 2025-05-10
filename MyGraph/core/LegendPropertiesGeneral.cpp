// LegendPropertiesGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\graph.h"
#include "..\include\LegendPropertiesGeneral.h"
#include "..\include\graphctrl.h"
#include "..\include\graphview.h"
#include "..\include\tools.h"


// CLegendPropertiesGeneral dialog

IMPLEMENT_DYNAMIC(CLegendPropertiesGeneral, CPropertyPage)
CLegendPropertiesGeneral::CLegendPropertiesGeneral()
	: CPropertyPage(CLegendPropertiesGeneral::IDD)
	, m_pGraphCtrl(NULL)
	, m_pView(NULL)
	, m_LegendVisible(FALSE)
	, m_AutoPos(FALSE)
	, m_BorderWidth(1)
	, m_FillType(0)
	, m_GradientType("None")
	, m_BorderColor(RGB(0, 0, 0))
{
}

CLegendPropertiesGeneral::~CLegendPropertiesGeneral()
{
}

void CLegendPropertiesGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CLegendPropertiesGeneral)
	DDX_Check(pDX, IDC_LEGEND_VISIBLE, m_LegendVisible);
	DDX_Check(pDX, IDC_LEGEND_AUTOPOS, m_AutoPos);
	DDX_Text(pDX, IDC_BORDER_WIDTH, m_BorderWidth);
	DDX_Radio(pDX, IDC_FILL_NONE, m_FillType);
	DDX_Text(pDX, IDC_GRADIENT_TYPE, m_GradientType);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLegendPropertiesGeneral, CPropertyPage)
	ON_BN_CLICKED(IDC_COLOR1_SELECT, OnBnClickedColor1Select)
	ON_BN_CLICKED(IDC_COLOR2_SELECT, OnBnClickedColor2Select)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COLOR1, OnCustomDrawColors)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COLOR2, OnCustomDrawColors)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BORDER_COLOR, OnCustomDrawColors)
	ON_BN_CLICKED(IDC_FILL_NONE, OnBnClickedFillNone)
	ON_BN_CLICKED(IDC_FILL_SOLID, OnBnClickedFillSolid)
	ON_BN_CLICKED(IDC_FILL_GRADIENT, OnBnClickedFillGradient)
	ON_BN_CLICKED(IDC_GRTYPE_SELECT, OnBnClickedGrtypeSelect)
	ON_BN_CLICKED(IDC_FONT, OnBnClickedFont)
	ON_BN_CLICKED(IDC_LEGEND_VISIBLE, OnBnClickedLegendVisible)
	ON_EN_CHANGE(IDC_BORDER_WIDTH, OnEnChangeBorderWidth)
	ON_BN_CLICKED(IDC_LEGEND_AUTOPOS, OnBnClickedLegendAutopos)
	ON_BN_CLICKED(IDC_BORDER_COLOR_SELECT, OnBnClickedBorderColorSelect)
END_MESSAGE_MAP()


// CLegendPropertiesGeneral message handlers

void CLegendPropertiesGeneral::OnBnClickedColor1Select()
{
	CColorDialog dlg(m_BackGround.Color1, CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT, this);
	if (dlg.DoModal() == IDOK)
	{
		m_BackGround.Color1 = dlg.GetColor();
		GetDlgItem(IDC_COLOR1)->Invalidate();
		SetModified(TRUE);
	}
}

void CLegendPropertiesGeneral::OnBnClickedColor2Select()
{
	CColorDialog dlg(m_BackGround.Color2, CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT, this);
	if (dlg.DoModal() == IDOK)
	{
		m_BackGround.Color2 = dlg.GetColor();
		GetDlgItem(IDC_COLOR2)->Invalidate();
		SetModified(TRUE);
	}
}

BOOL CLegendPropertiesGeneral::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	GetDlgItem(IDC_BORDER_WIDTH_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)10, (short)0));

	if (m_pGraphCtrl)
	{
		m_BackGround = m_pGraphCtrl->m_LegendBackGround;
		m_FillType = m_BackGround.Type;
		m_LegendVisible = m_pGraphCtrl->m_ShowLegend;
		m_AutoPos = m_pGraphCtrl->m_bAutoLegendPos;
		m_BorderWidth = (int)_round(m_pGraphCtrl->m_LegendBorderWidth);
		m_Font = m_pGraphCtrl->m_LegendFont;
		m_BorderColor = m_pGraphCtrl->m_LegendBorderColor;
	}

	UpdateData(FALSE);
	UpdateControlsState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLegendPropertiesGeneral::OnBnClickedFillNone()
{
	UpdateControlsState();
	SetModified(TRUE);
}

void CLegendPropertiesGeneral::OnBnClickedFillSolid()
{
	UpdateControlsState();
	SetModified(TRUE);
}

void CLegendPropertiesGeneral::OnBnClickedFillGradient()
{
	UpdateControlsState();
	SetModified(TRUE);
}

void CLegendPropertiesGeneral::OnBnClickedGrtypeSelect()
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

void CLegendPropertiesGeneral::UpdateControlsState(void)
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
}

void CLegendPropertiesGeneral::OnCustomDrawColors(NMHDR* pNMHDR, LRESULT* pResult)
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

BOOL CLegendPropertiesGeneral::OnApply()
{
	UpdateData(TRUE);

	if (!Validate()) return FALSE;

	if (m_pGraphCtrl)
	{
		m_BackGround.Type = (GRAPH_FILLTYPE)m_FillType;
		m_pGraphCtrl->m_LegendBackGround = m_BackGround;
		m_pGraphCtrl->m_ShowLegend = m_LegendVisible;
		m_pGraphCtrl->m_bAutoLegendPos = m_AutoPos;
		m_pGraphCtrl->m_LegendBorderWidth = m_BorderWidth;
		m_pGraphCtrl->m_LegendFont = m_Font;
		m_pGraphCtrl->m_LegendBorderColor = m_BorderColor;

		if (m_pView)
		{
			m_pView->Resize();
			m_pView->Invalidate();
		}
	}

	return CPropertyPage::OnApply();
}

void CLegendPropertiesGeneral::OnBnClickedFont()
{
	if (!m_pView) return;
	LOGFONT font;
	FONTtoLOGFONT(m_Font, font, *m_pView->GetDC());
	CFontDialog dlg(&font, CF_EFFECTS | CF_SCREENFONTS, NULL, this);
	dlg.m_cf.rgbColors = m_Font.color;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&font);
		LOGFONTtoFONT(font, m_Font, *m_pView->GetDC());
		m_Font.color = dlg.GetColor();
		SetModified(TRUE);
	}
}

void CLegendPropertiesGeneral::OnBnClickedLegendVisible()
{
	SetModified(TRUE);
}

void CLegendPropertiesGeneral::OnEnChangeBorderWidth()
{
	SetModified(TRUE);
}

void CLegendPropertiesGeneral::OnBnClickedLegendAutopos()
{
	SetModified(TRUE);
}


BOOL CLegendPropertiesGeneral::Validate(void)
{
	if (!CheckIntMinMax(0, 10, m_BorderWidth, this, GetDlgItem(IDC_BORDER_WIDTH_SPIN))) return FALSE;

	return TRUE;
}

void CLegendPropertiesGeneral::OnBnClickedBorderColorSelect()
{
	CColorDialog dlg(m_BorderColor, CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT, this);
	if (dlg.DoModal() == IDOK)
	{
		m_BorderColor = dlg.GetColor();
		GetDlgItem(IDC_BORDER_COLOR)->Invalidate();
		SetModified(TRUE);
	}
}
