// DataPropertiesGrids.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\graph.h"
#include "..\include\constants.h"
#include "..\include\DataPropertiesGrids.h"
#include "..\include\tools.h"
#include "..\include\linescene.h"
#include "..\include\graphctrl.h"
#include "..\include\graphview.h"


// CDataPropertiesGrids dialog

IMPLEMENT_DYNAMIC(CDataPropertiesGrids, CPropertyPage)
CDataPropertiesGrids::CDataPropertiesGrids()
	: CPropertyPage(CDataPropertiesGrids::IDD)
	, m_Grid(TRUE)
	, m_SubGrid(TRUE)
	, m_Ticks(TRUE)
	, m_SubTicks(TRUE)
	, m_Intelligent(TRUE)
	, m_Symmetric(TRUE)
	, m_GridLine(LS_SOLID)
	, m_SubGridLine(LS_SOLID)
	, m_GridColor(RGB(0, 0, 0))
	, m_SubGridColor(RGB(0, 0, 0))
	, m_XGridSize(DEFAULTXGRIDSIZE)
	, m_YGridSize(DEFAULTYGRIDSIZE)
	, m_XSubGrids(2)
	, m_YSubGrids(2)
	, m_pGraphCtrl(NULL)
	, m_pView(NULL)
	, m_pLineScene(NULL)
	, m_MinX(0)
	, m_MinY(0)
	, m_MaxX(0)
	, m_MaxY(0)
{
}

CDataPropertiesGrids::~CDataPropertiesGrids()
{
	if (m_pLineScene) delete m_pLineScene;
}

void CDataPropertiesGrids::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CDataPropertiesGrids)
	DDX_Check(pDX, IDC_GRID, m_Grid);
	DDX_Check(pDX, IDC_SUBGRID, m_SubGrid);
	DDX_Check(pDX, IDC_TICKS, m_Ticks);
	DDX_Check(pDX, IDC_SUBTICKS, m_SubTicks);
	DDX_Check(pDX, IDC_INTELLIGENT, m_Intelligent);
	DDX_Check(pDX, IDC_SYMMETRIC, m_Symmetric);
	DDX_CBIndex(pDX, IDC_GRID_LINE, m_GridLine);
	DDX_CBIndex(pDX, IDC_SUBGRID_LINE, m_SubGridLine);
	DDX_Control(pDX, IDC_GRID_LINE, m_cGridLine);
	DDX_Control(pDX, IDC_SUBGRID_LINE, m_cSubGridLine);
	DDX_Text(pDX, IDC_XGRID_SIZE, m_XGridSize);
	DDX_Text(pDX, IDC_YGRID_SIZE, m_YGridSize);
	DDX_Text(pDX, IDC_XSUBGRIDS, m_XSubGrids);
	DDX_Text(pDX, IDC_YSUBGRIDS, m_YSubGrids);
	DDX_Text(pDX, IDC_MINX, m_MinX);
	DDX_Text(pDX, IDC_MINY, m_MinY);
	DDX_Text(pDX, IDC_MAXX, m_MaxX);
	DDX_Text(pDX, IDC_MAXY, m_MaxY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataPropertiesGrids, CPropertyPage)
	ON_BN_CLICKED(IDC_GRID_COLOR_SELECT, OnBnClickedGridColorSelect)
	ON_BN_CLICKED(IDC_SUBGRID_COLOR_SELECT, OnBnClickedSubgridColorSelect)
	ON_CBN_SELENDOK(IDC_GRID_LINE, OnCbnSelendok)
	ON_CBN_SELENDOK(IDC_SUBGRID_LINE, OnCbnSelendok)
	ON_BN_CLICKED(IDC_GRID, OnCheckClicked)
	ON_BN_CLICKED(IDC_SUBGRID, OnCheckClicked)
	ON_BN_CLICKED(IDC_TICKS, OnCheckClicked)
	ON_BN_CLICKED(IDC_SUBTICKS, OnCheckClicked)
	ON_BN_CLICKED(IDC_INTELLIGENT, OnCheckClicked)
	ON_BN_CLICKED(IDC_SYMMETRIC, OnCheckClicked)
	ON_EN_CHANGE(IDC_XGRID_SIZE, OnEnChange)
	ON_EN_CHANGE(IDC_YGRID_SIZE, OnEnChange)
	ON_EN_CHANGE(IDC_XSUBGRIDS, OnEnChange)
	ON_EN_CHANGE(IDC_YSUBGRIDS, OnEnChange)
	ON_EN_CHANGE(IDC_MINX, OnEnChange)
	ON_EN_CHANGE(IDC_MINY, OnEnChange)
	ON_EN_CHANGE(IDC_MAXX, OnEnChange)
	ON_EN_CHANGE(IDC_MAXY, OnEnChange)
	ON_MESSAGE(WM_DRAWITEM, OnDrawItem)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_GRID_COLOR, OnCustomDrawColors)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SUBGRID_COLOR, OnCustomDrawColors)
END_MESSAGE_MAP()


// CDataPropertiesGrids message handlers

void CDataPropertiesGrids::OnBnClickedGridColorSelect()
{
	CColorDialog dlg(m_GridColor, CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT, this);
	if (dlg.DoModal() == IDOK)
	{
		m_GridColor = dlg.GetColor();
		GetDlgItem(IDC_GRID_COLOR)->Invalidate();
		SetModified(TRUE);
	}
}

void CDataPropertiesGrids::OnBnClickedSubgridColorSelect()
{
	CColorDialog dlg(m_SubGridColor, CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT, this);
	if (dlg.DoModal() == IDOK)
	{
		m_SubGridColor = dlg.GetColor();
		GetDlgItem(IDC_SUBGRID_COLOR)->Invalidate();
		SetModified(TRUE);
	}
}

void CDataPropertiesGrids::OnCbnSelendok()
{
	SetModified(TRUE);
}

void CDataPropertiesGrids::OnCheckClicked()
{
	SetModified(TRUE);
	UpdateControlsState();
}

void CDataPropertiesGrids::OnEnChange()
{
	SetModified(TRUE);
}

BOOL CDataPropertiesGrids::OnApply()
{
	UpdateData(TRUE);

	if (!Validate()) return FALSE;

	if (m_pGraphCtrl)
	{
		m_pGraphCtrl->m_ShowGrid = m_Grid;
		m_pGraphCtrl->m_ShowSubGrid = m_SubGrid;
		m_pGraphCtrl->m_ShowXTicks = m_Ticks;
		m_pGraphCtrl->m_ShowYTicks = m_Ticks;
		m_pGraphCtrl->m_ShowXSubTicks = m_SubTicks;
		m_pGraphCtrl->m_ShowYSubTicks = m_SubTicks;
		m_pGraphCtrl->m_IntelligentAxisScale = m_Intelligent;
		m_pGraphCtrl->m_GridLine = (GRAPH_LINESTYLE)m_GridLine;
		m_pGraphCtrl->m_SubGridLine = (GRAPH_LINESTYLE)m_SubGridLine;
		m_pGraphCtrl->m_GridColor = m_GridColor;
		m_pGraphCtrl->m_SubGridColor = m_SubGridColor;
		m_pGraphCtrl->m_Symmetric = m_Symmetric;
		m_pGraphCtrl->m_DefaultXGridSize = m_XGridSize;
		m_pGraphCtrl->m_DefaultYGridSize = m_YGridSize;
		m_pGraphCtrl->m_SubXGridsCount = m_XSubGrids;
		m_pGraphCtrl->m_SubYGridsCount = m_YSubGrids;
		m_pGraphCtrl->m_AdditionalGridSteps.SetRect(m_MinX, m_MaxY, m_MaxX, m_MinY);

		if (m_pView)
			m_pView->Invalidate();

	}

	return CPropertyPage::OnApply();
}

BOOL CDataPropertiesGrids::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	GetDlgItem(IDC_XGRID_SIZE_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)1000, (short)1));
	GetDlgItem(IDC_YGRID_SIZE_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)1000, (short)1));
	GetDlgItem(IDC_XSUBGRIDS_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)10, (short)0));
	GetDlgItem(IDC_YSUBGRIDS_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)10, (short)0));
	GetDlgItem(IDC_MINX_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)100, (short)0));
	GetDlgItem(IDC_MINY_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)100, (short)0));
	GetDlgItem(IDC_MAXX_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)100, (short)0));
	GetDlgItem(IDC_MAXY_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)100, (short)0));

	FillLineStyleCombo(m_cGridLine);
	FillLineStyleCombo(m_cSubGridLine);
	
	if (m_pGraphCtrl)
	{
		CRect rect;
		m_cGridLine.GetClientRect(rect);
		rect.right -= GetSystemMetrics(SM_CXVSCROLL);
		rect.DeflateRect(5, 5);
		m_LineDIB.Create(rect.Width(), rect.Height(), 24);
		if (!m_pLineScene)
			m_pLineScene = new CLineScene(m_pGraphCtrl, LS_SOLID, 1, COLORREF(RGB(0, 0, 0)));
		else m_pLineScene->Destroy();
		m_pLineScene->Create(m_LineDIB.GetDC());
		m_pLineScene->Resize(rect.Width(), rect.Height());

		m_Grid = m_pGraphCtrl->m_ShowGrid;
		m_SubGrid = m_pGraphCtrl->m_ShowSubGrid;
		m_Ticks = m_pGraphCtrl->m_ShowXTicks && m_pGraphCtrl->m_ShowYTicks;
		m_SubTicks = m_pGraphCtrl->m_ShowXSubTicks && m_pGraphCtrl->m_ShowYSubTicks;
		m_Intelligent = m_pGraphCtrl->m_IntelligentAxisScale;
		m_GridLine = m_pGraphCtrl->m_GridLine;
		m_SubGridLine = m_pGraphCtrl->m_SubGridLine;
		m_GridColor = m_pGraphCtrl->m_GridColor;
		m_SubGridColor = m_pGraphCtrl->m_SubGridColor;
		m_Symmetric = m_pGraphCtrl->m_Symmetric;
		m_XGridSize = (int)m_pGraphCtrl->m_DefaultXGridSize;
		m_YGridSize = (int)m_pGraphCtrl->m_DefaultYGridSize;
		m_XSubGrids = (int)m_pGraphCtrl->m_SubXGridsCount;
		m_YSubGrids = (int)m_pGraphCtrl->m_SubYGridsCount;
		m_MinX = m_pGraphCtrl->m_AdditionalGridSteps.left;
		m_MaxX = m_pGraphCtrl->m_AdditionalGridSteps.right;
		m_MinY = m_pGraphCtrl->m_AdditionalGridSteps.bottom;
		m_MaxY = m_pGraphCtrl->m_AdditionalGridSteps.top;
	}

	UpdateData(FALSE);
	UpdateControlsState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDataPropertiesGrids::UpdateControlsState(void)
{
	UpdateData(TRUE);

	GetDlgItem(IDC_GRID_LINE)->EnableWindow(m_Grid);
	GetDlgItem(IDC_GRID_COLOR)->EnableWindow(m_Grid);
	GetDlgItem(IDC_GRID_COLOR_SELECT)->EnableWindow(m_Grid);

	GetDlgItem(IDC_SUBGRID)->EnableWindow(m_Grid);
	GetDlgItem(IDC_SUBGRID_LINE)->EnableWindow(m_SubGrid && m_Grid);
	GetDlgItem(IDC_SUBGRID_COLOR)->EnableWindow(m_SubGrid && m_Grid);
	GetDlgItem(IDC_SUBGRID_COLOR_SELECT)->EnableWindow(m_SubGrid && m_Grid);

	GetDlgItem(IDC_SUBTICKS)->EnableWindow(m_Ticks);

	GetDlgItem(IDC_SYMMETRIC)->EnableWindow(m_Intelligent);

	GetDlgItem(IDC_MINX)->EnableWindow(m_Intelligent);
	GetDlgItem(IDC_MINY)->EnableWindow(m_Intelligent);
	GetDlgItem(IDC_MAXX)->EnableWindow(m_Intelligent);
	GetDlgItem(IDC_MAXY)->EnableWindow(m_Intelligent);

}

void CDataPropertiesGrids::FillLineStyleCombo(CComboBox& combo)
{
	combo.AddString(_T("Solid"));
	combo.AddString(_T("Dot"));
	combo.AddString(_T("Dot3"));
	combo.AddString(_T("Dash"));
	combo.AddString(_T("DashLarge"));
	combo.AddString(_T("DashDot"));
	combo.AddString(_T("DashDotDot"));
}

LRESULT CDataPropertiesGrids::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lParam;
	if (lpDrawItem->CtlID == IDC_GRID_LINE || lpDrawItem->CtlID == IDC_SUBGRID_LINE)
	{
		CRect rect(lpDrawItem->rcItem);
		FillRect(lpDrawItem->hDC, rect, (HBRUSH)(COLOR_WINDOW + 1));
		if (lpDrawItem->itemState & ODS_SELECTED ||
			lpDrawItem->itemState & ODS_FOCUS)
			DrawFocusRect(lpDrawItem->hDC, rect);
		rect.DeflateRect(2, 2);
		CSize size;
		m_LineDIB.GetSize(&size);
		m_pLineScene->SetLine((GRAPH_LINESTYLE)lpDrawItem->itemID, 1, lpDrawItem->CtlID == IDC_GRID_LINE ? m_GridColor : m_SubGridColor);
		m_pLineScene->Render();
		int left = rect.left + (rect.Width() - size.cx) / 2;
		int top = rect.top + (rect.Height() - size.cy) / 2;
		BitBlt(lpDrawItem->hDC, left, top, size.cx, size.cy, *m_LineDIB.GetDC(), 0, 0, SRCCOPY);
	}

	return TRUE;
}

void CDataPropertiesGrids::OnCustomDrawColors(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	LPNMCUSTOMDRAW pNMDraw = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if (pNMDraw->hdr.idFrom != IDC_GRID_COLOR && pNMDraw->hdr.idFrom != IDC_SUBGRID_COLOR) return;
	::FillRect(pNMDraw->hdc, &pNMDraw->rc, CBrush(pNMDraw->hdr.idFrom == IDC_GRID_COLOR ? m_GridColor : m_SubGridColor));
}

BOOL CDataPropertiesGrids::Validate(void)
{
	if (!CheckIntMinMax(1, 1000, m_XGridSize, this, GetDlgItem(IDC_XGRID_SIZE))) return FALSE;
	if (!CheckIntMinMax(1, 1000, m_YGridSize, this, GetDlgItem(IDC_YGRID_SIZE))) return FALSE;
	if (!CheckIntMinMax(0, 10, m_XSubGrids, this, GetDlgItem(IDC_XSUBGRIDS))) return FALSE;
	if (!CheckIntMinMax(0, 10, m_YSubGrids, this, GetDlgItem(IDC_YSUBGRIDS))) return FALSE;

	if (!CheckIntMinMax(0, 100, m_MinX, this, GetDlgItem(IDC_MINX))) return FALSE;
	if (!CheckIntMinMax(0, 100, m_MinY, this, GetDlgItem(IDC_MINY))) return FALSE;
	if (!CheckIntMinMax(0, 100, m_MaxX, this, GetDlgItem(IDC_MAXX))) return FALSE;
	if (!CheckIntMinMax(0, 100, m_MaxY, this, GetDlgItem(IDC_MAXY))) return FALSE;

	return TRUE;
}
