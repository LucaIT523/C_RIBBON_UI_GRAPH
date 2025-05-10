// DataPropertiesLabels.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\graph.h"
#include "DataPropertiesLabels.h"
#include "..\include\graphctrl.h"
#include "..\include\graphview.h"
#include "..\include\tools.h"


// CDataPropertiesLabels dialog

IMPLEMENT_DYNAMIC(CDataPropertiesLabels, CPropertyPage)
CDataPropertiesLabels::CDataPropertiesLabels()
	: CPropertyPage(CDataPropertiesLabels::IDD)
	, m_pGraphCtrl(NULL)
	, m_pView(NULL)
	, m_XLabels(TRUE)
	, m_YLabels(TRUE)
	, m_XLabelAngle(0)
	, m_YLabelAngle(0)
	, m_XLabelPrecision(2)
	, m_YLabelPrecision(2)
	, m_XLabelFormat(0)
	, m_YLabelFormat(0)
{
}

CDataPropertiesLabels::~CDataPropertiesLabels()
{
}

void CDataPropertiesLabels::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CDataPropertiesLabels)
	DDX_Check(pDX, IDC_XLABELS, m_XLabels);
	DDX_Check(pDX, IDC_YLABELS, m_YLabels);
	DDX_Text(pDX, IDC_XLABEL_ANGLE, m_XLabelAngle);
	DDX_Text(pDX, IDC_YLABEL_ANGLE, m_YLabelAngle);
	DDX_Text(pDX, IDC_XLABEL_PRECISION, m_XLabelPrecision);
	DDX_Text(pDX, IDC_YLABEL_PRECISION, m_YLabelPrecision);
	DDX_CBIndex(pDX, IDC_XLABEL_FORMAT, m_XLabelFormat);
	DDX_CBIndex(pDX, IDC_YLABEL_FORMAT, m_YLabelFormat);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataPropertiesLabels, CPropertyPage)
	ON_EN_CHANGE(IDC_XLABEL_ANGLE, OnEnChange)
	ON_EN_CHANGE(IDC_YLABEL_ANGLE, OnEnChange)
	ON_EN_CHANGE(IDC_XLABEL_PRECISION, OnEnChange)
	ON_EN_CHANGE(IDC_YLABEL_PRECISION, OnEnChange)
	ON_BN_CLICKED(IDC_XLABELS, OnBnClickedChange)
	ON_BN_CLICKED(IDC_YLABELS, OnBnClickedChange)
	ON_BN_CLICKED(IDC_XFONT, OnBnClickedXFont)
	ON_BN_CLICKED(IDC_YFONT, OnBnClickedYFont)
	ON_CBN_SELENDOK(IDC_XLABEL_FORMAT, OnCbnSelendokLabelFormat)
	ON_CBN_SELENDOK(IDC_YLABEL_FORMAT, OnCbnSelendokLabelFormat)
END_MESSAGE_MAP()


// CDataPropertiesLabels message handlers

void CDataPropertiesLabels::OnEnChange()
{
	SetModified(TRUE);
}

void CDataPropertiesLabels::OnBnClickedChange()
{
	SetModified(TRUE);
	UpdateControlsState();
}

void CDataPropertiesLabels::UpdateControlsState()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_XFONT)->EnableWindow(m_XLabels);
	GetDlgItem(IDC_YFONT)->EnableWindow(m_YLabels);
	GetDlgItem(IDC_XLABEL_ANGLE)->EnableWindow(m_XLabels);
	GetDlgItem(IDC_XLABEL_ANGLE_SPIN)->EnableWindow(m_XLabels);
	GetDlgItem(IDC_YLABEL_ANGLE)->EnableWindow(m_YLabels);
	GetDlgItem(IDC_YLABEL_ANGLE_SPIN)->EnableWindow(m_YLabels);

	GetDlgItem(IDC_XLABEL_FORMAT)->EnableWindow(m_XLabels);
	GetDlgItem(IDC_YLABEL_FORMAT)->EnableWindow(m_YLabels);

	GetDlgItem(IDC_XLABEL_PRECISION)->EnableWindow(m_XLabels && ((GRAPH_LABELSFORMAT)m_XLabelFormat == LF_DOUBLE || (GRAPH_LABELSFORMAT)m_XLabelFormat == LF_EXP));
	GetDlgItem(IDC_XLABEL_PRECISION_SPIN)->EnableWindow(m_XLabels && ((GRAPH_LABELSFORMAT)m_XLabelFormat == LF_DOUBLE || (GRAPH_LABELSFORMAT)m_XLabelFormat == LF_EXP));
	GetDlgItem(IDC_YLABEL_PRECISION)->EnableWindow(m_YLabels && ((GRAPH_LABELSFORMAT)m_YLabelFormat == LF_DOUBLE || (GRAPH_LABELSFORMAT)m_YLabelFormat == LF_EXP));
	GetDlgItem(IDC_YLABEL_PRECISION_SPIN)->EnableWindow(m_YLabels && ((GRAPH_LABELSFORMAT)m_YLabelFormat == LF_DOUBLE || (GRAPH_LABELSFORMAT)m_YLabelFormat == LF_EXP));
}

void CDataPropertiesLabels::OnBnClickedXFont()
{
	if (!m_pView) return;
	LOGFONT font;
	FONTtoLOGFONT(m_XFont, font, *m_pView->GetDC());
	CFontDialog dlg(&font, CF_EFFECTS | CF_SCREENFONTS, NULL, this);
	dlg.m_cf.rgbColors = m_XFont.color;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&font);
		LOGFONTtoFONT(font, m_XFont, *m_pView->GetDC());
		m_XFont.color = dlg.GetColor();
		SetModified(TRUE);
	}
}

void CDataPropertiesLabels::OnBnClickedYFont()
{
	if (!m_pView) return;
	LOGFONT font;
	FONTtoLOGFONT(m_YFont, font, *m_pView->GetDC());
	CFontDialog dlg(&font, CF_EFFECTS | CF_SCREENFONTS, NULL, this);
	dlg.m_cf.rgbColors = m_YFont.color;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&font);
		LOGFONTtoFONT(font, m_YFont, *m_pView->GetDC());
		m_YFont.color = dlg.GetColor();
		SetModified(TRUE);
	}
}

BOOL CDataPropertiesLabels::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	GetDlgItem(IDC_XLABEL_ANGLE_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)90, (short)0));
	GetDlgItem(IDC_YLABEL_ANGLE_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)90, (short)0));
	GetDlgItem(IDC_XLABEL_PRECISION_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)20, (short)0));
	GetDlgItem(IDC_YLABEL_PRECISION_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)20, (short)0));

	FillDataFormatList((CComboBox*)GetDlgItem(IDC_XLABEL_FORMAT));
	FillDataFormatList((CComboBox*)GetDlgItem(IDC_YLABEL_FORMAT));

	if (m_pGraphCtrl)
	{
		m_XFont = m_pGraphCtrl->m_TicksXLabelsFont;
		m_YFont = m_pGraphCtrl->m_TicksYLabelsFont;
		m_XLabels = m_pGraphCtrl->m_ShowXTicksLabels;
		m_YLabels = m_pGraphCtrl->m_ShowYTicksLabels;
		m_XLabelAngle = m_XFont.angle;
		m_YLabelAngle = m_YFont.angle;
		m_XLabelFormat = m_pGraphCtrl->m_XLabelsFormat;
		m_YLabelFormat = m_pGraphCtrl->m_YLabelsFormat;
		m_XLabelPrecision = m_pGraphCtrl->m_XLabelsFormatAdditional;
		m_YLabelPrecision = m_pGraphCtrl->m_YLabelsFormatAdditional;
	}
	UpdateData(FALSE);
	UpdateControlsState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDataPropertiesLabels::OnApply()
{
	if (!Validate()) return FALSE;

	if (m_pGraphCtrl)
	{
		m_XFont.angle = m_XLabelAngle;
		m_YFont.angle = m_YLabelAngle;
		m_pGraphCtrl->m_TicksXLabelsFont = m_XFont;
		m_pGraphCtrl->m_TicksYLabelsFont = m_YFont;
		m_pGraphCtrl->m_ShowXTicksLabels = m_XLabels;
		m_pGraphCtrl->m_ShowYTicksLabels = m_YLabels;
		m_pGraphCtrl->m_XLabelsFormat = (GRAPH_LABELSFORMAT)m_XLabelFormat;
		m_pGraphCtrl->m_YLabelsFormat = (GRAPH_LABELSFORMAT)m_YLabelFormat;
		m_pGraphCtrl->m_XLabelsFormatAdditional = m_XLabelPrecision;
		m_pGraphCtrl->m_YLabelsFormatAdditional = m_YLabelPrecision;

		if (m_pView)
		{
			m_pView->Resize();
			m_pView->Invalidate();
		}
	}

	return CPropertyPage::OnApply();
}

BOOL CDataPropertiesLabels::Validate(void)
{
	if (!CheckIntMinMax(0, 90, m_XLabelAngle, this, GetDlgItem(IDC_XLABEL_ANGLE))) return FALSE;
	if (!CheckIntMinMax(0, 90, m_YLabelAngle, this, GetDlgItem(IDC_YLABEL_ANGLE))) return FALSE;

	if (!CheckIntMinMax(0, 20, m_XLabelPrecision, this, GetDlgItem(IDC_XLABEL_PRECISION))) return FALSE;
	if (!CheckIntMinMax(0, 20, m_YLabelPrecision, this, GetDlgItem(IDC_YLABEL_PRECISION))) return FALSE;

	return TRUE;
}

void CDataPropertiesLabels::FillDataFormatList(CComboBox* pCombo)
{
	pCombo->AddString(_T("Integer"));
	pCombo->AddString(_T("Double"));
	pCombo->AddString(_T("Exponent"));
	pCombo->AddString(_T("Log"));
}

void CDataPropertiesLabels::OnCbnSelendokLabelFormat()
{
	SetModified(TRUE);
	UpdateControlsState();
}
