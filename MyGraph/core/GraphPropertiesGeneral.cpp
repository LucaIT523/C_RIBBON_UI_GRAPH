// GraphPropertiesGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\Graph.h"
#include "..\include\GraphPropertiesGeneral.h"
#include "..\include\GraphCtrl.h"
#include "..\include\GraphView.h"
#include "..\include\tools.h"


// CGraphPropertiesGeneral dialog

IMPLEMENT_DYNAMIC(CGraphPropertiesGeneral, CPropertyPage)
CGraphPropertiesGeneral::CGraphPropertiesGeneral()
	: CPropertyPage(IDD_GRAPH_PROPERTIES_GENERAL)
	, m_pGraphCtrl(NULL)
	, m_pView(NULL)
	, m_GradientType("None")
{
}

CGraphPropertiesGeneral::~CGraphPropertiesGeneral()
{
}

void CGraphPropertiesGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CGraphPropertiesGeneral)
	DDX_Check(pDX, IDC_SHOWTITLE, m_ShowTitle);
	DDX_Check(pDX, IDC_SHOWSUBTITLE, m_ShowSubTitle);
	DDX_Check(pDX, IDC_SHOWXLABEL, m_ShowXLabel);
	DDX_Check(pDX, IDC_SHOWYLABEL, m_ShowYLabel);
	DDX_Text(pDX, IDC_TITLE, m_Title);
	DDX_Text(pDX, IDC_SUBTITLE, m_SubTitle);
	DDX_Text(pDX, IDC_XLABEL, m_XLabel);
	DDX_Text(pDX, IDC_YLABEL, m_YLabel);
	DDX_Text(pDX, IDC_GRADIENT_TYPE, m_GradientType);
	DDX_Radio(pDX, IDC_FILL_NONE, m_FillType);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGraphPropertiesGeneral, CPropertyPage)
	ON_BN_CLICKED(IDC_SHOWTITLE, OnBnClickedShowtitle)
	ON_BN_CLICKED(IDC_SHOWSUBTITLE, OnBnClickedShowsubtitle)
	ON_BN_CLICKED(IDC_SHOWXLABEL, OnBnClickedShowxlabel)
	ON_BN_CLICKED(IDC_SHOWYLABEL, OnBnClickedShowylabel)
	ON_EN_CHANGE(IDC_TITLE, OnEnChangeTitle)
	ON_EN_CHANGE(IDC_SUBTITLE, OnEnChangeSubtitle)
	ON_EN_CHANGE(IDC_XLABEL, OnEnChangeXlabel)
	ON_EN_CHANGE(IDC_YLABEL, OnEnChangeYlabel)
	ON_BN_CLICKED(IDC_TITLEFONT, OnBnClickedTitlefont)
	ON_BN_CLICKED(IDC_SUBTITLEFONT, OnBnClickedSubtitlefont)
	ON_BN_CLICKED(IDC_XLABELFONT, OnBnClickedXlabelfont)
	ON_BN_CLICKED(IDC_YLABELFONT, OnBnClickedYlabelfont)
	ON_BN_CLICKED(IDC_FILL_NONE, OnBnClickedFillNone)
	ON_BN_CLICKED(IDC_FILL_SOLID, OnBnClickedFillSolid)
	ON_BN_CLICKED(IDC_FILL_GRADIENT, OnBnClickedFillGradient)
	ON_BN_CLICKED(IDC_GRTYPE_SELECT, OnBnClickedGrtypeSelect)
	ON_BN_CLICKED(IDC_COLOR1_SELECT, OnBnClickedColor1Select)
	ON_BN_CLICKED(IDC_COLOR2_SELECT, OnBnClickedColor2Select)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COLOR1, OnCustomDrawColors)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COLOR2, OnCustomDrawColors)
END_MESSAGE_MAP()


// CGraphPropertiesGeneral message handlers

BOOL CGraphPropertiesGeneral::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	if (m_pGraphCtrl)
	{
		m_ShowTitle = m_pGraphCtrl->m_ShowTitle;
		m_ShowSubTitle = m_pGraphCtrl->m_ShowSubTitle;
		m_ShowXLabel = m_pGraphCtrl->m_ShowXLabel;
		m_ShowYLabel = m_pGraphCtrl->m_ShowYLabel;
		m_Title = m_pGraphCtrl->m_Title;
		m_SubTitle = m_pGraphCtrl->m_SubTitle;
		m_XLabel = m_pGraphCtrl->m_XLabel;
		m_YLabel = m_pGraphCtrl->m_YLabel;
		m_TitleFont = m_pGraphCtrl->m_TitleFont;
		m_SubTitleFont = m_pGraphCtrl->m_SubTitleFont;
		m_XLabelFont = m_pGraphCtrl->m_XLabelFont;
		m_YLabelFont = m_pGraphCtrl->m_YLabelFont;
		m_BackGround = m_pGraphCtrl->m_BackGround;
		m_FillType = m_BackGround.Type;
	}

	UpdateData(FALSE);
	UpdateControlsState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CGraphPropertiesGeneral::OnBnClickedShowtitle()
{
	SetModified(TRUE);
	UpdateControlsState();
}

void CGraphPropertiesGeneral::OnBnClickedShowsubtitle()
{
	SetModified(TRUE);
	UpdateControlsState();
}

void CGraphPropertiesGeneral::OnBnClickedShowxlabel()
{
	SetModified(TRUE);
	UpdateControlsState();
}

void CGraphPropertiesGeneral::OnBnClickedShowylabel()
{
	SetModified(TRUE);
	UpdateControlsState();
}

void CGraphPropertiesGeneral::OnEnChangeTitle()
{
	SetModified(TRUE);
}

void CGraphPropertiesGeneral::OnEnChangeSubtitle()
{
	SetModified(TRUE);
}

void CGraphPropertiesGeneral::OnEnChangeXlabel()
{
	SetModified(TRUE);
}

void CGraphPropertiesGeneral::OnEnChangeYlabel()
{
	SetModified(TRUE);
}

void CGraphPropertiesGeneral::OnBnClickedTitlefont()
{
	if (!m_pView) return;
	int angle = m_TitleFont.angle;
	LOGFONT font;
	FONTtoLOGFONT(m_TitleFont, font, *m_pView->GetDC());
	CFontDialog dlg(&font, CF_EFFECTS | CF_SCREENFONTS, NULL, this);
	dlg.m_cf.rgbColors = m_TitleFont.color;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&font);
		LOGFONTtoFONT(font, m_TitleFont, *m_pView->GetDC());
		m_TitleFont.color = dlg.GetColor();
		m_TitleFont.angle = angle;
		SetModified(TRUE);
	}
}

void CGraphPropertiesGeneral::OnBnClickedSubtitlefont()
{
	if (!m_pView) return;
	int angle = m_SubTitleFont.angle;
	LOGFONT font;
	FONTtoLOGFONT(m_SubTitleFont, font, *m_pView->GetDC());
	CFontDialog dlg(&font, CF_EFFECTS | CF_SCREENFONTS, NULL, this);
	dlg.m_cf.rgbColors = m_SubTitleFont.color;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&font);
		LOGFONTtoFONT(font, m_SubTitleFont, *m_pView->GetDC());
		m_SubTitleFont.color = dlg.GetColor();
		m_SubTitleFont.angle = angle;
		SetModified(TRUE);
	}
}

void CGraphPropertiesGeneral::OnBnClickedXlabelfont()
{
	if (!m_pView) return;
	int angle = m_XLabelFont.angle;
	LOGFONT font;
	FONTtoLOGFONT(m_XLabelFont, font, *m_pView->GetDC());
	CFontDialog dlg(&font, CF_EFFECTS | CF_SCREENFONTS, NULL, this);
	dlg.m_cf.rgbColors = m_XLabelFont.color;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&font);
		LOGFONTtoFONT(font, m_XLabelFont, *m_pView->GetDC());
		m_XLabelFont.color = dlg.GetColor();
		m_XLabelFont.angle = angle;
		SetModified(TRUE);
	}
}

void CGraphPropertiesGeneral::OnBnClickedYlabelfont()
{
	if (!m_pView) return;
	int angle = m_YLabelFont.angle;
	LOGFONT font;
	FONTtoLOGFONT(m_YLabelFont, font, *m_pView->GetDC());
	CFontDialog dlg(&font, CF_EFFECTS | CF_SCREENFONTS, NULL, this);
	dlg.m_cf.rgbColors = m_YLabelFont.color;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&font);
		LOGFONTtoFONT(font, m_YLabelFont, *m_pView->GetDC());
		m_YLabelFont.color = dlg.GetColor();
		m_YLabelFont.angle = angle;
		SetModified(TRUE);
	}
}

void CGraphPropertiesGeneral::OnBnClickedFillNone()
{
	UpdateControlsState();
	SetModified(TRUE);
}

void CGraphPropertiesGeneral::OnBnClickedFillSolid()
{
	UpdateControlsState();
	SetModified(TRUE);
}

void CGraphPropertiesGeneral::OnBnClickedFillGradient()
{
	UpdateControlsState();
	SetModified(TRUE);
}

void CGraphPropertiesGeneral::OnBnClickedGrtypeSelect()
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

void CGraphPropertiesGeneral::OnBnClickedColor1Select()
{
	CColorDialog dlg(m_BackGround.Color1, CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT, this);
	if (dlg.DoModal() == IDOK)
	{
		m_BackGround.Color1 = dlg.GetColor();
		GetDlgItem(IDC_COLOR1)->Invalidate();
		SetModified(TRUE);
	}
}

void CGraphPropertiesGeneral::OnBnClickedColor2Select()
{
	CColorDialog dlg(m_BackGround.Color2, CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT, this);
	if (dlg.DoModal() == IDOK)
	{
		m_BackGround.Color2 = dlg.GetColor();
		GetDlgItem(IDC_COLOR2)->Invalidate();
		SetModified(TRUE);
	}
}

void CGraphPropertiesGeneral::UpdateControlsState(void)
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

	GetDlgItem(IDC_TITLE)->EnableWindow(m_ShowTitle);
	GetDlgItem(IDC_TITLEFONT)->EnableWindow(m_ShowTitle);
	GetDlgItem(IDC_SUBTITLE)->EnableWindow(m_ShowSubTitle);
	GetDlgItem(IDC_SUBTITLEFONT)->EnableWindow(m_ShowSubTitle);
	GetDlgItem(IDC_XLABEL)->EnableWindow(m_ShowXLabel);
	GetDlgItem(IDC_XLABELFONT)->EnableWindow(m_ShowXLabel);
	GetDlgItem(IDC_YLABEL)->EnableWindow(m_ShowYLabel);
	GetDlgItem(IDC_YLABELFONT)->EnableWindow(m_ShowYLabel);
}

BOOL CGraphPropertiesGeneral::OnApply()
{
	UpdateData(TRUE);

	if (m_pGraphCtrl)
	{
		m_pGraphCtrl->m_ShowTitle = m_ShowTitle;
		m_pGraphCtrl->m_ShowSubTitle = m_ShowSubTitle;
		m_pGraphCtrl->m_ShowXLabel = m_ShowXLabel;
		m_pGraphCtrl->m_ShowYLabel = m_ShowYLabel;
		m_pGraphCtrl->m_Title = m_Title;
		m_pGraphCtrl->m_SubTitle = m_SubTitle;
		m_pGraphCtrl->m_XLabel = m_XLabel;
		m_pGraphCtrl->m_YLabel = m_YLabel;
		m_pGraphCtrl->m_TitleFont = m_TitleFont;
		m_pGraphCtrl->m_SubTitleFont = m_SubTitleFont;
		m_pGraphCtrl->m_XLabelFont = m_XLabelFont;
		m_pGraphCtrl->m_YLabelFont = m_YLabelFont;
		m_BackGround.Type = (GRAPH_FILLTYPE)m_FillType;
		m_pGraphCtrl->m_BackGround = m_BackGround;
	}

	if (m_pView)
	{
		m_pView->Resize();
		m_pView->Invalidate();
	}

	return CPropertyPage::OnApply();
}

void CGraphPropertiesGeneral::OnCustomDrawColors(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	LPNMCUSTOMDRAW pNMDraw = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if (pNMDraw->hdr.idFrom != IDC_COLOR1 && pNMDraw->hdr.idFrom != IDC_COLOR2) 
		return;
	CWnd* pCtrl = GetDlgItem((int)pNMDraw->hdr.idFrom);
	if (pCtrl)
	{
		COLORREF color = pNMDraw->hdr.idFrom == IDC_COLOR1 ? m_BackGround.Color1 : m_BackGround.Color2;
		FillRect(pNMDraw->hdc, &pNMDraw->rc, CBrush(color));
	}
}
