// DataPropertiesSeries.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\graph.h"
#include "..\include\DataPropertiesSeries.h"
#include "..\include\markerscene.h"
#include "..\include\linescene.h"
#include "..\include\graphctrl.h"
#include "..\include\graphview.h"
#include "..\include\tools.h"
#include "..\include\bitmaps\CSimpleDib.h"

// CDataPropertiesSeries dialog

IMPLEMENT_DYNAMIC(CDataPropertiesSeries, CPropertyPage)
CDataPropertiesSeries::CDataPropertiesSeries()
	: CPropertyPage(CDataPropertiesSeries::IDD)
	, m_nCurrentSerie(-1)
	, m_curVisible(TRUE)
	, m_curLegend(TRUE)
	, m_curMarkerStyle(0)
	, m_curLineStyle(0)
	, m_curMarkerSize(15)
	, m_curColor(RGB(0, 0, 0))
	, m_curMarkerColor(RGB(0, 0, 0))
	, m_pMarkerScene(NULL)
	, m_pLineScene(NULL)
	, m_ItemChangingHandling(FALSE)
	, m_cLineWidth(1)
{
}

CDataPropertiesSeries::~CDataPropertiesSeries()
{
	if (m_pMarkerScene) delete m_pMarkerScene;
	if (m_pLineScene) delete m_pLineScene;
	m_Series.RemoveAll();
}

void CDataPropertiesSeries::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CDataPropertiesSeries)
	DDX_Control(pDX, IDC_SERIES, m_cSeriesList);
	DDX_Control(pDX, IDC_MARKER_STYLE, m_cMarkerStyle);
	DDX_Control(pDX, IDC_LINE_STYLE, m_cLineStyle);
	DDX_Check(pDX, IDC_VISIBLE, m_curVisible);
	DDX_Check(pDX, IDC_VISIBLE_LEGEND, m_curLegend);
	DDX_Text(pDX, IDC_MARKER_SIZE, m_curMarkerSize);
	DDX_Text(pDX, IDC_MARKERS_COUNT, m_curMarkers);
	DDX_Text(pDX, IDC_NAME, m_curName);
	DDX_CBIndex(pDX, IDC_MARKER_STYLE, m_curMarkerStyle);
	DDX_CBIndex(pDX, IDC_LINE_STYLE, m_curLineStyle);
	DDX_Text(pDX, IDC_LINE_WIDTH, m_cLineWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataPropertiesSeries, CPropertyPage)
	ON_BN_CLICKED(IDC_VISIBLE, OnBnClickedVisible)
	ON_BN_CLICKED(IDC_VISIBLE_LEGEND, OnBnClickedVisibleLegend)
	ON_EN_CHANGE(IDC_MARKER_SIZE, OnEnChangeMarkerSize)
	ON_BN_CLICKED(IDC_COLOR_SELECT, OnBnClickedColorSelect)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SERIES, OnLvnItemchangedSeries)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COLOR, OnCustomDrawColors)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MARKER_COLOR, OnCustomDrawColors)
	ON_EN_CHANGE(IDC_NAME, OnEnChangeName)
	ON_CBN_SELENDOK(IDC_MARKER_STYLE, OnCbnSelendokMarkerStyle)
	ON_CBN_SELENDOK(IDC_LINE_STYLE, OnCbnSelendokLineStyle)
	ON_MESSAGE(WM_DRAWITEM, OnDrawItem)
	ON_BN_CLICKED(IDC_MARKER_COLOR_SELECT, OnBnClickedMarkerColorSelect)
	ON_EN_CHANGE(IDC_MARKERS_COUNT, OnEnChangeMarkersCount)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_SERIES, OnLvnItemchangingSeries)
	ON_MESSAGE(WM_LISTVIEW_UPDATE, OnListViewUpdate)
	ON_EN_CHANGE(IDC_LINE_WIDTH, OnEnChangeLineWidth)
  ON_BN_CLICKED(IDC_BUTTON_FFT, OnBnClickedButtonFft)
END_MESSAGE_MAP()


// CDataPropertiesSeries message handlers


void CDataPropertiesSeries::OnBnClickedVisible()
{
	SetModified(TRUE);
	UpdateControls();
}

void CDataPropertiesSeries::OnBnClickedVisibleLegend()
{
	SetModified(TRUE);
}

void CDataPropertiesSeries::OnEnChangeMarkerSize()
{
	SetModified(TRUE);
}

void CDataPropertiesSeries::OnBnClickedColorSelect()
{
	CColorDialog dlg(m_curColor, CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT, this);
	if (dlg.DoModal() == IDOK)
	{
		m_curColor = dlg.GetColor();
		GetDlgItem(IDC_COLOR)->Invalidate();
		GetDlgItem(IDC_LINE_STYLE)->Invalidate();
		SetModified(TRUE);
	}
}

void CDataPropertiesSeries::OnLvnItemchangedSeries(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	SelectSerie(pNMLV->iItem);
	*pResult = 0;
}

void CDataPropertiesSeries::OnCustomDrawColors(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	LPNMCUSTOMDRAW pNMDraw = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if (pNMDraw->hdr.idFrom != IDC_COLOR && pNMDraw->hdr.idFrom != IDC_MARKER_COLOR) return;
	::FillRect(pNMDraw->hdc, &pNMDraw->rc, CBrush(pNMDraw->hdr.idFrom == IDC_COLOR ? m_curColor : m_curMarkerColor));
}

BOOL CDataPropertiesSeries::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	GetDlgItem(IDC_MARKER_SIZE_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)50, (short)1));
	GetDlgItem(IDC_MARKERS_COUNT_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)100, (short)1));
	GetDlgItem(IDC_LINE_WIDTH_SPIN)->SendMessage(UDM_SETRANGE, 0, MAKELONG((short)10, (short)1));

	m_MarkerDIB.Create(13, 13, 24);
	if (!m_pMarkerScene)
		m_pMarkerScene = new CMarkerScene(m_pGraphCtrl, MS_NONE, 11, COLORREF(RGB(0, 0, 0)));
	else m_pMarkerScene->Destroy();
	m_pMarkerScene->Create(m_MarkerDIB.GetDC());
	m_pMarkerScene->Resize(13, 13);

	m_cMarkerStyle.AddString(_T("None"));
	m_cMarkerStyle.AddString(_T("Square"));
	m_cMarkerStyle.AddString(_T("Triangle"));
	m_cMarkerStyle.AddString(_T("TriangleDown"));
	m_cMarkerStyle.AddString(_T("Star"));
	m_cMarkerStyle.AddString(_T("Circle"));
	m_cMarkerStyle.AddString(_T("Rhomb"));
	m_cMarkerStyle.AddString(_T("EmptySquare"));
	m_cMarkerStyle.AddString(_T("EmptyTriangle"));
	m_cMarkerStyle.AddString(_T("EmptyTriangleDown"));
	m_cMarkerStyle.AddString(_T("EmptyStar"));
	m_cMarkerStyle.AddString(_T("EmptyCircle"));
	m_cMarkerStyle.AddString(_T("EmptyRhomb"));
	m_cMarkerStyle.AddString(_T("Plus"));
	m_cMarkerStyle.AddString(_T("X"));
	m_cMarkerStyle.AddString(_T("ArrowUp"));
	m_cMarkerStyle.AddString(_T("ArrowDown"));


	CRect rect;
	m_cLineStyle.GetClientRect(rect);
	rect.right -= GetSystemMetrics(SM_CXVSCROLL);
	rect.DeflateRect(5, 5);
	m_LineDIB.Create(rect.Width(), rect.Height(), 24);
	if (!m_pLineScene)
		m_pLineScene = new CLineScene(m_pGraphCtrl, LS_SOLID, 1, COLORREF(RGB(0, 0, 0)));
	else m_pLineScene->Destroy();
	m_pLineScene->Create(m_LineDIB.GetDC());
	m_pLineScene->Resize(rect.Width(), rect.Height());

	m_cLineStyle.AddString(_T("Solid"));
	m_cLineStyle.AddString(_T("Dot"));
	m_cLineStyle.AddString(_T("Dot3"));
	m_cLineStyle.AddString(_T("Dash"));
	m_cLineStyle.AddString(_T("DashLarge"));
	m_cLineStyle.AddString(_T("DashDot"));
	m_cLineStyle.AddString(_T("DashDotDot"));

	m_cSeriesList.GetClientRect(rect);
	m_cSeriesList.DeleteAllItems();
	m_cSeriesList.InsertColumn(0, _T(""), LVCFMT_LEFT, rect.Width());
	if (m_pGraphCtrl)
	{
		for (int i = 0; i < m_pGraphCtrl->GetSeriesCount(); i++)
		{
			GRAPHSERIES* pGraphSerie = m_pGraphCtrl->GetSerie(i);
			SERIEINFO serieInfo;
			serieInfo.color = pGraphSerie->color;
			serieInfo.markercolor = pGraphSerie->markercolor;
			serieInfo.legend = pGraphSerie->legend;
			serieInfo.marker = pGraphSerie->marker;
			serieInfo.size = pGraphSerie->markersize;
			serieInfo.visible = pGraphSerie->visible;
			serieInfo.line = pGraphSerie->line;
			serieInfo.linewidth = pGraphSerie->linewidth > 0 ? pGraphSerie->linewidth : 1;
			serieInfo.markers = pGraphSerie->markers;
			_tcscpy(serieInfo.name, pGraphSerie->lpName);
			m_Series.AddTail(serieInfo);
			m_cSeriesList.InsertItem(m_cSeriesList.GetItemCount(), serieInfo.name, m_cSeriesList.GetItemCount());
		}
		if (m_pGraphCtrl->GetSeriesCount() > 0)
			m_cSeriesList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

	UpdateData(FALSE);
	UpdateControls();

	CreateImageList();
//	m_cSeriesList.SetImageList(&m_ListImages, LVSIL_SMALL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDataPropertiesSeries::OnEnChangeName()
{
	SetModified(TRUE);
}

BOOL CDataPropertiesSeries::SelectSerie(int nIndex)
{
	if (nIndex < 0/* || nIndex == m_nCurrentSerie*/) return FALSE;
	if (m_nCurrentSerie >= 0 && !SaveData(m_nCurrentSerie)) return FALSE;
	LoadData(nIndex);
	m_nCurrentSerie = nIndex;
	UpdateControls();
	CreateImageList();

	return TRUE;
}

BOOL CDataPropertiesSeries::SaveData(int nIndex)
{
	UpdateData(TRUE);

	if (!Validate()) return FALSE;

	POSITION pos = m_Series.FindIndex(nIndex);
	if (pos)
	{
		m_Series.GetAt(pos).color = m_curColor;
		m_Series.GetAt(pos).markercolor = m_curMarkerColor;
		m_Series.GetAt(pos).legend = m_curLegend;
		m_Series.GetAt(pos).marker = m_curMarkerStyle;
		m_Series.GetAt(pos).line = m_curLineStyle;
		m_Series.GetAt(pos).linewidth = m_cLineWidth > 0 ? m_cLineWidth : 1;
		m_Series.GetAt(pos).size = m_curMarkerSize;
		m_Series.GetAt(pos).visible = m_curVisible;
		m_Series.GetAt(pos).markers = m_curMarkers;
		if (m_Series.GetAt(pos).name != m_curName)
		{
			_tcscpy(m_Series.GetAt(pos).name, m_curName);
			m_cSeriesList.SetItemText(nIndex, 0, m_curName);
		}
	}

	return TRUE;
}

void CDataPropertiesSeries::LoadData(int nIndex)
{
	POSITION pos = m_Series.FindIndex(nIndex);
	if (pos)
	{
		m_curColor = m_Series.GetAt(pos).color;
		m_curMarkerColor = m_Series.GetAt(pos).markercolor;
		m_curLegend = m_Series.GetAt(pos).legend;
		m_curMarkerStyle = m_Series.GetAt(pos).marker;
		m_curLineStyle = m_Series.GetAt(pos).line;
		m_cLineWidth = m_Series.GetAt(pos).linewidth > 0 ? m_Series.GetAt(pos).linewidth : 1;
		m_curMarkerSize = m_Series.GetAt(pos).size;
		m_curVisible = m_Series.GetAt(pos).visible;
		m_curName = m_Series.GetAt(pos).name;
		m_curMarkers = m_Series.GetAt(pos).markers;
	}
	UpdateData(FALSE);
	GetDlgItem(IDC_COLOR)->Invalidate();
	GetDlgItem(IDC_MARKER_COLOR)->Invalidate();
}

BOOL CDataPropertiesSeries::OnApply()
{
	if (!SaveData(m_nCurrentSerie)) return FALSE;

	if (m_pGraphCtrl)
	{
		for (int i = 0; i < m_pGraphCtrl->GetSeriesCount(); i++)
		{
			GRAPHSERIES* pGraphSerie = m_pGraphCtrl->GetSerie(i);
			POSITION pos = m_Series.FindIndex(i);
			if (!pos) continue;
			SERIEINFO serieInfo = m_Series.GetAt(pos);
			pGraphSerie->markercolor = serieInfo.markercolor;
			pGraphSerie->color = serieInfo.color;
			pGraphSerie->legend = serieInfo.legend;
			pGraphSerie->marker = (GRAPH_MARKERSTYLE )serieInfo.marker;
			pGraphSerie->markersize = serieInfo.size;
			pGraphSerie->visible = serieInfo.visible;
			pGraphSerie->line = (GRAPH_LINESTYLE)serieInfo.line;
			pGraphSerie->linewidth = serieInfo.linewidth > 0 ? serieInfo.linewidth : 1;
			pGraphSerie->markers = serieInfo.markers;
			_tcscpy(pGraphSerie->lpName, serieInfo.name);
		}
	}

	if (m_pView)
	{
		m_pView->Resize();
		m_pView->Invalidate();
	}

	return CPropertyPage::OnApply();
}

void CDataPropertiesSeries::UpdateControls(void)
{
	UpdateData(TRUE);
	CWnd* pCtrl = GetDlgItem(IDC_VISIBLE_LEGEND);
	if (pCtrl) pCtrl->EnableWindow(m_curVisible);

  GetDlgItem(IDC_BUTTON_FFT)->EnableWindow(m_pView && m_pView->GetGraph() && m_pView->GetGraph()->m_bFFTButtonEnabled);
}

void CDataPropertiesSeries::OnCbnSelendokMarkerStyle()
{
	SetModified(TRUE);
}

void CDataPropertiesSeries::OnCbnSelendokLineStyle()
{
	SetModified(TRUE);
}

LRESULT CDataPropertiesSeries::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lParam;
	if (lpDrawItem->CtlID == IDC_LINE_STYLE)
	{
		CRect rect(lpDrawItem->rcItem);
		FillRect(lpDrawItem->hDC, rect, (HBRUSH)(COLOR_WINDOW + 1));
		if (lpDrawItem->itemState & ODS_SELECTED ||
			lpDrawItem->itemState & ODS_FOCUS)
			DrawFocusRect(lpDrawItem->hDC, rect);
		rect.DeflateRect(2, 2);
		CSize size;
		m_LineDIB.GetSize(&size);
		m_pLineScene->SetLine((GRAPH_LINESTYLE)lpDrawItem->itemID, m_cLineWidth, m_curColor);
		m_pLineScene->Render();
		int left = rect.left + (rect.Width() - size.cx) / 2;
		int top = rect.top + (rect.Height() - size.cy) / 2;
		BitBlt(lpDrawItem->hDC, left, top, size.cx, size.cy, *m_LineDIB.GetDC(), 0, 0, SRCCOPY);
	}
	else if (lpDrawItem->CtlID == IDC_MARKER_STYLE)
	{
		CRect rect(lpDrawItem->rcItem);
		FillRect(lpDrawItem->hDC, rect, (HBRUSH)(COLOR_WINDOW + 1));
		if (lpDrawItem->itemState & ODS_SELECTED ||
			lpDrawItem->itemState & ODS_FOCUS)
			DrawFocusRect(lpDrawItem->hDC, rect);
		rect.DeflateRect(1, 1);
		CSize size;
		m_MarkerDIB.GetSize(&size);
		m_pMarkerScene->SetMarker(12, (GRAPH_MARKERSTYLE )lpDrawItem->itemID, m_curMarkerColor);
		m_pMarkerScene->Render();
		int left = rect.left + (rect.Width() - size.cx) / 2;
		int top = rect.top + (rect.Height() - size.cy) / 2;
		BitBlt(lpDrawItem->hDC, left, top, size.cx, size.cy, *m_MarkerDIB.GetDC(), 0, 0, SRCCOPY);
	}

	return TRUE;
}

void CDataPropertiesSeries::CreateImageList(void)
{
/*
	CSize size;
	m_MarkerDIB.GetSize(&size);

	m_ListImages.DeleteImageList();
	m_ListImages.Create(size.cx, size.cy, ILC_COLOR24, 0, 1);
	for (int i = 0; i < m_Series.GetCount(); i++)
	{
		POSITION pos = m_Series.FindIndex(i);
		if (!pos) continue;
		m_pMarkerScene->SetMarker(11, (GRAPH_MARKERSTYLE )m_Series.GetAt(pos).marker, m_Series.GetAt(pos).color);
		m_pMarkerScene->Render();

		CBitmap bmp;
		CDC dc;
		dc.CreateCompatibleDC(m_pMarkerScene->GetDC());
		bmp.CreateCompatibleBitmap(m_pMarkerScene->GetDC(), size.cx, size.cy);
		dc.SelectObject(&bmp);
		dc.BitBlt(0, 0, size.cx, size.cy, m_pMarkerScene->GetDC(), 0, 0, SRCCOPY);
		m_ListImages.Add(&bmp, TRANSPARENTCOLOR);

		EmptyClipboard();
		SetClipboardData(CF_BITMAP, bmp.m_hObject);
		CloseClipboard();
	}
*/
}

void CDataPropertiesSeries::OnBnClickedMarkerColorSelect()
{
	CColorDialog dlg(m_curMarkerColor, CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT, this);
	if (dlg.DoModal() == IDOK)
	{
		m_curMarkerColor = dlg.GetColor();
		GetDlgItem(IDC_MARKER_COLOR)->Invalidate();
		GetDlgItem(IDC_MARKER_STYLE)->Invalidate();
		SetModified(TRUE);
	}
}

void CDataPropertiesSeries::OnEnChangeMarkersCount()
{
	SetModified(TRUE);
}

BOOL CDataPropertiesSeries::Validate(void)
{
	if (!CheckIntMinMax(1, 50, m_curMarkerSize, this, GetDlgItem(IDC_MARKER_SIZE))) return FALSE;
	if (!CheckIntMinMax(1, 100, m_curMarkers, this, GetDlgItem(IDC_MARKERS_COUNT))) return FALSE;
	if (!CheckIntMinMax(1, 10, m_cLineWidth, this, GetDlgItem(IDC_LINE_WIDTH))) return FALSE;
	return TRUE;
}

void CDataPropertiesSeries::OnLvnItemchangingSeries(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState != 0 && !m_ItemChangingHandling)
	{
		m_ItemChangingHandling = TRUE;
		*pResult = !SelectSerie(pNMLV->iItem);
		::PostMessage(m_hWnd, WM_LISTVIEW_UPDATE, 0, 0);
	}
	else *pResult = 0;
}

LRESULT CDataPropertiesSeries::OnListViewUpdate(WPARAM wParam, LPARAM lParam)
{
	m_ItemChangingHandling = FALSE;
	return 0;
}


void CDataPropertiesSeries::OnEnChangeLineWidth()
{
	SetModified(TRUE);
}

void CDataPropertiesSeries::OnBnClickedButtonFft()
{
	if (m_nCurrentSerie < 0 || m_nCurrentSerie > m_pGraphCtrl->GetSeriesCount()-1)
		return;

	if (m_pView && m_pView->GetGraph() && m_pView->GetGraph()->GetParent())
	{
		::SendMessage(m_pView->GetGraph()->GetParent(), 
			UM_SHOW_FFT,
			(WPARAM)m_pGraphCtrl->GetSerie(m_nCurrentSerie), 
			NULL);

//		AfxMessageBox(_T("Message sent from Graph to Parent App"));
	}
}
