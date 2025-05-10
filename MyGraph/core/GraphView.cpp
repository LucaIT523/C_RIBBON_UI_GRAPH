// GraphView.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\graph.h"
#include <afxcoll.h>
#include <atltypes.h>
#include "afxext.h"
#include "..\include\GraphView.h"
#include "..\include\GraphScene.h"
#include "..\core\bitmaps\cximage\ximage.h"
#include "..\include\graphpropertiesgeneral.h"
#include "..\include\graphpropertiesmargins.h"
#include "..\include\graphproperties.h"
#include "..\include\legendpropertiesgeneral.h"
#include "..\include\legendproperties.h"
#include "..\include\dataproperties.h"
#include "..\include\graphctrl.h"
#include "..\include\bitmaps\CSimpleDib.h"
#include "..\include\PathString.h"
#include "viewprintpreview.h"



#undef CXIMAGE_SUPPORT_GIF
#undef CXIMAGE_SUPPORT_MNG
#undef CXIMAGE_SUPPORT_TGA
#undef CXIMAGE_SUPPORT_PCX
#undef CXIMAGE_SUPPORT_WMF
#undef CXIMAGE_SUPPORT_J2K
#undef CXIMAGE_SUPPORT_JBG
#undef CXIMAGE_SUPPORT_JP2
#undef CXIMAGE_SUPPORT_JPC
#undef CXIMAGE_SUPPORT_PGX
#undef CXIMAGE_SUPPORT_RAS
#undef CXIMAGE_SUPPORT_PNM

DocType doctypes[CMAX_IMAGE_FORMATS] =
{
        { -1, TRUE, TRUE, _T("Supported files"), _T("*.bmp;*.jpg;*.jpeg;*.png;*.tif;*.tiff") },
#if CXIMAGE_SUPPORT_BMP
        { CXIMAGE_FORMAT_BMP, TRUE, TRUE, _T("BMP files"), _T("*.bmp") },
#endif
#if CXIMAGE_SUPPORT_GIF
        { CXIMAGE_FORMAT_GIF, TRUE, TRUE, _T("GIF files"), _T("*.gif") },
#endif
#if CXIMAGE_SUPPORT_JPG
        { CXIMAGE_FORMAT_JPG, TRUE, TRUE, _T("JPG files"), _T("*.jpg;*.jpeg") },
#endif
#if CXIMAGE_SUPPORT_PNG
        { CXIMAGE_FORMAT_PNG, TRUE, TRUE, _T("PNG files"), _T("*.png") },
#endif
#if CXIMAGE_SUPPORT_MNG
        { CXIMAGE_FORMAT_MNG, TRUE, TRUE, _T("MNG files"), _T("*.mng;*.jng;*.png") },
#endif
/*
#if CXIMAGE_SUPPORT_ICO
		{ CXIMAGE_FORMAT_ICO, TRUE, TRUE, _T("ICO CUR files"), _T("*.ico;*.cur") },
#endif
*/
#if CXIMAGE_SUPPORT_TIF
        { CXIMAGE_FORMAT_TIF, TRUE, TRUE, _T("TIFF files"), _T("*.tif;*.tiff") },
#endif
#if CXIMAGE_SUPPORT_TGA
        { CXIMAGE_FORMAT_TGA, TRUE, TRUE, _T("TGA files"), _T("*.tga") },
#endif
#if CXIMAGE_SUPPORT_PCX
        { CXIMAGE_FORMAT_PCX, TRUE, TRUE, _T("PCX files"), _T("*.pcx") },
#endif
/*
#if CXIMAGE_SUPPORT_WBMP
		{ CXIMAGE_FORMAT_WBMP, TRUE, TRUE, _T("WBMP files"), _T("*.wbmp") },
#endif
*/
#if CXIMAGE_SUPPORT_WMF
        { CXIMAGE_FORMAT_WMF, TRUE, FALSE, _T("WMF EMF files"), _T("*.wmf;*.emf") },
#endif
#if CXIMAGE_SUPPORT_J2K
        { CXIMAGE_FORMAT_J2K, TRUE, TRUE, _T("J2K files"), _T("*.j2k;*.jp2") },
#endif
#if CXIMAGE_SUPPORT_JBG
        { CXIMAGE_FORMAT_JBG, TRUE, TRUE, _T("JBG files"), _T("*.jbg") },
#endif
#if CXIMAGE_SUPPORT_JP2
        { CXIMAGE_FORMAT_JP2, TRUE, TRUE, _T("JP2 files"), _T("*.j2k;*.jp2") },
#endif
#if CXIMAGE_SUPPORT_JPC
        { CXIMAGE_FORMAT_JPC, TRUE, TRUE, _T("JPC files"), _T("*.j2c;*.jpc") },
#endif
#if CXIMAGE_SUPPORT_PGX
        { CXIMAGE_FORMAT_PGX, TRUE, TRUE, _T("PGX files"), _T("*.pgx") },
#endif
#if CXIMAGE_SUPPORT_RAS
        { CXIMAGE_FORMAT_RAS, TRUE, TRUE, _T("RAS files"), _T("*.ras") },
#endif
#if CXIMAGE_SUPPORT_PNM
        { CXIMAGE_FORMAT_PNM, TRUE, TRUE, _T("PNM files"), _T("*.pnm;*.pgm;*.ppm") }
#endif
};

class CSelectionDC : public CDC
{
private:
   int      m_iOldROP;
   CPen     m_Pen;
   CPen*    m_pPenOld;
   CBrush*  m_pBrushOld;
   HWND     m_hWnd;

private:
   void Init()
      {
         m_Pen.CreatePen(PS_SOLID,0,RGB(255,255,255));
         m_pPenOld   = SelectObject(&m_Pen);
         m_pBrushOld = (CBrush*)SelectStockObject(NULL_BRUSH);
         m_iOldROP   = SetROP2(R2_XORPEN);
      }

public:
   CSelectionDC(const CDC &cdc)
      {
         m_hWnd = NULL;
	      if( !Attach(cdc.m_hDC) )   AfxThrowResourceException();
         Init();
      }

    CSelectionDC(CWnd *pWnd)
      {
	      if( !Attach(::GetDC(m_hWnd = pWnd->GetSafeHwnd())) )  AfxThrowResourceException();
         Init();
      }

   virtual ~CSelectionDC()
      {
         SetROP2(m_iOldROP);
         SelectObject(m_pBrushOld);
         SelectObject(m_pPenOld);
         if( m_hWnd ){
            ::ReleaseDC(m_hWnd,Detach());
         }else{
	         Detach();
         }
      }
};

static void MulDivRect(LPRECT r1, LPRECT r2, int num, int div)
{
	r1->left = MulDiv(r2->left, num, div);
	r1->top = MulDiv(r2->top, num, div);
	r1->right = MulDiv(r2->right, num, div);
	r1->bottom = MulDiv(r2->bottom, num, div);
}

inline int roundleast(int n)
{
	int mod = n%10;
	n -= mod;
	if (mod >= 5)
		n += 10;
	else if (mod <= -5)
		n -= 10;
	return n;
}

static void RoundRect(LPRECT r1)
{
	r1->left = roundleast(r1->left);
	r1->right = roundleast(r1->right);
	r1->top = roundleast(r1->top);
	r1->bottom = roundleast(r1->bottom);
}

// CGraphView

CGraphView::CGraphView(CGraph* pGraph)
: m_pGraph(pGraph)
, m_ZoomMode(ZM_NONE)
, m_hZoomCursor(NULL)
, m_hUnZoomCursor(NULL)
, m_hStdCursor(NULL)
, m_hNoCursor(NULL)
, m_hHandCursor(NULL)
, m_hCrossCursor(NULL)
, m_hSizeAllCursor(NULL)
, m_Zooming(FALSE)
, m_Panning(FALSE)
, m_hContextMenu(NULL)
, m_pSelectedGraph(NULL)
, m_pTemplate(NULL)
, m_bPrintPreview(FALSE)
{
	m_LegendTracker.m_nStyle |= CRectTracker::resizeOutside | CRectTracker::hatchedBorder;
	m_LegendTracker.m_sizeMin = CSize(10, 10);
}

CGraphView::~CGraphView()
{
}

BEGIN_MESSAGE_MAP(CGraphView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CGraphView message handlers

void CGraphView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect clientRect;
	GetClientRect(clientRect);
	
	for (int i = 0; i < m_pGraph->GetGraphsCount(); i++)
	{
		CRect rect;
		m_pGraph->GetGraphRect(i, rect, clientRect);
		m_pGraph->GetGraphCtrl(i)->Draw();
		CDC* tempDC = m_pGraph->GetGraphCtrl(i)->GetDC();
		clientDIB.GetDC()->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), tempDC, 0, 0, SRCCOPY);
	}

	CRect drawRect;
//	GetUpdateRect(drawRect);
	drawRect = clientRect;

	dc.BitBlt(drawRect.left, drawRect.top, drawRect.Width(), drawRect.Height(), clientDIB.GetDC(), drawRect.left, drawRect.top, SRCCOPY);
	
	if (!m_LegendTracker.m_rect.IsRectEmpty())
		m_LegendTracker.Draw(&dc);
}

CGraph* CGraphView::GetGraph(void)
{
	return m_pGraph;
}

BOOL CGraphView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

GRAPH_ZOOMMODE CGraphView::GetZoomMode(void)
{
	return m_ZoomMode;
}

void CGraphView::SetZoomMode(GRAPH_ZOOMMODE mode)
{
	if (mode != ZM_CROSS && m_ZoomMode == ZM_CROSS)
		StopCrossing();
	else m_CrossPoint.SetPoint(0, 0);

	m_Panning = FALSE;
	m_Zooming = FALSE;
	m_ZoomMode = mode;
//	UpdateCursor();
}

void CGraphView::UpdateCursor(void)
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	LMOUSEDOWNINFO mouseInfo;
	GetLMouseInfoForPoint(&mouseInfo, point);


	if (m_ZoomMode != ZM_NONE && mouseInfo.clickedon == GC_GRAPH)
	{
		switch (m_ZoomMode)
		{
		case ZM_ZOOM: 
			{
				if (mouseInfo.graph->m_CurrentXZoomFactor < m_pGraph->m_MaxZoomFactor || mouseInfo.graph->m_CurrentYZoomFactor < m_pGraph->m_MaxZoomFactor)
					SetCursor(m_hZoomCursor); 
				else
					SetCursor(m_hNoCursor);
			} break;
		case ZM_UNZOOM: 
			{
				if (mouseInfo.graph->m_CurrentXZoomFactor > m_pGraph->m_MinZoomFactor || mouseInfo.graph->m_CurrentYZoomFactor > m_pGraph->m_MinZoomFactor)
					SetCursor(m_hUnZoomCursor); 
				else
					SetCursor(m_hNoCursor);
			} break;
		case ZM_100:
			{
				if (mouseInfo.graph->m_CurrentXZoomFactor != 1 || mouseInfo.graph->m_CurrentYZoomFactor != 1)
					SetCursor(m_hUnZoomCursor); 
				else
					SetCursor(m_hNoCursor);
			} break;
		case ZM_PAN:
			{
				if (mouseInfo.graph->m_CurrentXZoomFactor > m_pGraph->m_MinZoomFactor || mouseInfo.graph->m_CurrentYZoomFactor > m_pGraph->m_MinZoomFactor)
					SetCursor(m_hHandCursor); 
				else
					SetCursor(m_hNoCursor);
			} break;
		case ZM_CROSS:
			{
				SetCursor(m_hCrossCursor); 
			} break;
		}
		return;
	}
	
	SetCursor(m_hStdCursor);
}

int CGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	LoadCursors();	
	LoadMenus();	

	return 0;
}

void CGraphView::OnDestroy()
{
	CWnd::OnDestroy();
	DestroyCursors();
	DestroyMenus();
}

void CGraphView::LoadCursors(void)
{
	m_hZoomCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_GRAPH_ZOOM));
	m_hUnZoomCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_GRAPH_UNZOOM));
	m_hStdCursor = LoadCursor(NULL, IDC_ARROW);
	m_hNoCursor = LoadCursor(NULL, IDC_NO);
	m_hHandCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_GRAPH_HAND));
	m_hCrossCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_GRAPH_CROSS));
	m_hSizeAllCursor = LoadCursor(NULL, IDC_SIZEALL);
}

void CGraphView::DestroyCursors(void)
{
	if (m_hZoomCursor)
		DestroyCursor(m_hZoomCursor), m_hZoomCursor = NULL;
	if (m_hUnZoomCursor)
		DestroyCursor(m_hZoomCursor), m_hUnZoomCursor = NULL;
	if (m_hStdCursor)
		DestroyCursor(m_hStdCursor), m_hStdCursor = NULL;
	if (m_hNoCursor)
		DestroyCursor(m_hNoCursor), m_hNoCursor = NULL;
	if (m_hHandCursor)
		DestroyCursor(m_hHandCursor), m_hHandCursor = NULL;
	if (m_hCrossCursor)
		DestroyCursor(m_hCrossCursor), m_hCrossCursor = NULL;
	if (m_hSizeAllCursor)
		DestroyCursor(m_hSizeAllCursor), m_hSizeAllCursor = NULL;
}

void CGraphView::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);

	if (!m_LegendTracker.m_rect.IsRectEmpty() && m_LegendTracker.HitTest(point) >= 0)
		return;

	UpdateCursor();

	LMOUSEDOWNINFO mouseInfo;
	GetLMouseInfoForPoint(&mouseInfo, point);

	if (mouseInfo.clickedon == GC_GRAPH)
	{
		mouseInfo.graph->ScreenToGraphPoint(mouseInfo.controlPoint, &m_GraphMousePos);
		GetParent()->PostMessage(WM_COORDINATES_UPDATE, (WPARAM)mouseInfo.graph, (LPARAM)&m_GraphMousePos);
	}
	else GetParent()->PostMessage(WM_COORDINATES_UPDATE);

	if (m_Zooming)
	{
		CSelectionDC dc(this);
		DrawZoomRect(&dc);
		m_ZoomRect.right = point.x;
		m_ZoomRect.bottom = point.y;
		DrawZoomRect(&dc);
	}
	else if (m_Panning) 
	{
		mouseInfo.graph->MoveGraph((double)(m_PanPoint.x - point.x),
			(double)(m_PanPoint.y - point.y));
		m_PanPoint = point;
		Invalidate();
	}
	else if (m_ZoomMode == ZM_CROSS)
	{
		CRect rect;
		GetClientRect(rect);
		CSelectionDC dc(this);
		DrawCross(&dc, m_CrossPoint);

		if (rect.PtInRect(point))
		{
			SetCapture();
			if (mouseInfo.clickedon == GC_GRAPH)
			{
				m_CrossPoint = mouseInfo.point;
				DrawCross(&dc, m_CrossPoint);
			}
			else m_CrossPoint.SetPoint(0, 0);
		}
		else StopCrossing();
	}
}

void CGraphView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_LegendTracker.m_rect.IsRectEmpty())
	{
		if (m_LegendTracker.HitTest(point) < 0)
		{
			m_LegendTracker.m_rect.SetRectEmpty();
			Invalidate();
		}
		else if (m_LegendTracker.Track(this, point))
		{
			RecalcLegendRect(m_pSelectedGraph);
			Resize();
			Invalidate();
			return;
		}
	}

	GetLMouseInfoForPoint(&m_LButtonInfo, point);
	if (m_ZoomMode == ZM_ZOOM && m_LButtonInfo.graph && m_LButtonInfo.clickedon == GC_GRAPH)
	{
		CaptureMouse();
		m_ZoomRect.SetRect(point, point);
		CSelectionDC dc(this);
		DrawZoomRect(&dc);
		m_Zooming = TRUE;
	}
	else m_ZoomRect = CRect(0, 0, 0, 0);

	if (m_ZoomMode == ZM_PAN && m_LButtonInfo.graph && m_LButtonInfo.clickedon == GC_GRAPH)
	{
		CaptureMouse();
		m_Panning = TRUE;
		m_PanPoint = point;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CGraphView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_LegendTracker.m_rect.IsRectEmpty()) 
		return;

	if (m_Zooming)
	{
		CSelectionDC dc(this);
		DrawZoomRect(&dc);
		m_Zooming = FALSE;
	}

	ClipCursor(NULL);
	ReleaseCapture();

	LMOUSEDOWNINFO mouseInfo;
	GetLMouseInfoForPoint(&mouseInfo, point);

	if (mouseInfo.graph && mouseInfo.graph == m_LButtonInfo.graph)
	{
		if (m_ZoomMode != ZM_NONE && mouseInfo.clickedon == GC_GRAPH)
		{
			if (m_LButtonInfo.point == point)
			{
				if (m_ZoomMode == ZM_ZOOM)
				{
					mouseInfo.graph->Zoom(m_pGraph->m_ZoomFactor, m_pGraph->m_ZoomFactor, mouseInfo.controlPoint);
					mouseInfo.graph->m_ZoomFactorXList.AddTail(m_pGraph->m_ZoomFactor);
					mouseInfo.graph->m_ZoomFactorYList.AddTail(m_pGraph->m_ZoomFactor);
				}
				else if (m_ZoomMode == ZM_UNZOOM)
				{
					double x_zf = 1 / m_pGraph->m_ZoomFactor;
					double y_zf = 1 / m_pGraph->m_ZoomFactor;
					if (mouseInfo.graph->m_ZoomFactorXList.GetCount() > 0)
						x_zf = 1 / mouseInfo.graph->m_ZoomFactorXList.RemoveTail();
					if (mouseInfo.graph->m_ZoomFactorYList.GetCount() > 0)
						y_zf = 1 / mouseInfo.graph->m_ZoomFactorYList.RemoveTail();

					mouseInfo.graph->Zoom(x_zf, y_zf, mouseInfo.controlPoint);
				}
				else if (m_ZoomMode == ZM_100)
				{
					mouseInfo.graph->m_ZoomFactorXList.RemoveAll();
					mouseInfo.graph->m_ZoomFactorYList.RemoveAll();
					mouseInfo.graph->Zoom(1 / mouseInfo.graph->m_CurrentXZoomFactor, 1 / mouseInfo.graph->m_CurrentYZoomFactor, mouseInfo.controlPoint);
				}
				else if (m_ZoomMode == ZM_PAN) 
					m_Panning = FALSE;

				if (m_ZoomMode != ZM_CROSS)
					Invalidate();
			}
			else if (m_ZoomMode == ZM_ZOOM) 
			{
				CRect rcClient, rcTotal, rcLocal;
				GetClientRect(&rcTotal);
				m_pGraph->GetGraphRect(m_LButtonInfo.graph, rcClient, rcTotal);
				m_LButtonInfo.graph->GetGraphFieldRect(&rcLocal);
				rcLocal.left += rcClient.left;
				rcLocal.right += rcClient.left;
				rcLocal.top += rcClient.top;
				rcLocal.bottom += rcClient.top;

				double xFactor = (double)rcLocal.Width() / (double)m_ZoomRect.Width();
				double yFactor = (double)rcLocal.Height() / (double)m_ZoomRect.Height();
				m_ZoomRect.left -= rcLocal.left;
				m_ZoomRect.right -= rcLocal.left;
				m_ZoomRect.top -= rcLocal.top;
				m_ZoomRect.bottom -= rcLocal.top;
				mouseInfo.graph->Zoom(xFactor, yFactor, m_ZoomRect.CenterPoint());
				mouseInfo.graph->m_ZoomFactorXList.AddTail(xFactor);
				mouseInfo.graph->m_ZoomFactorYList.AddTail(yFactor);
				Invalidate();
			}
			else if (m_ZoomMode == ZM_PAN) 
				m_Panning = FALSE;
		}
	}

	m_LButtonInfo = LMOUSEDOWNINFO();
	UpdateCursor();

	CWnd::OnLButtonUp(nFlags, point);
}


void CGraphView::GetLMouseInfoForPoint(LMOUSEDOWNINFO* pInfo, CPoint point)
{
	if (!pInfo) return;
	*pInfo = LMOUSEDOWNINFO();
	pInfo->point = point;
	pInfo->graph = m_pGraph->GetGraphCtrlForPoint(point, &pInfo->graphPoint);
	if (pInfo->graph)
		pInfo->clickedon = pInfo->graph->GetControlForPoint(pInfo->graphPoint, &pInfo->controlPoint);
}

void CGraphView::DrawZoomRect(CDC* pDC)
{
	pDC->MoveTo(m_ZoomRect.left, m_ZoomRect.bottom);
	pDC->LineTo(m_ZoomRect.right, m_ZoomRect.bottom);
	pDC->LineTo(m_ZoomRect.right, m_ZoomRect.top);
	pDC->LineTo(m_ZoomRect.left, m_ZoomRect.top);
	pDC->LineTo(m_ZoomRect.left, m_ZoomRect.bottom);
}

void CGraphView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	Resize();
}

void CGraphView::CaptureMouse(void)
{
	CRect rcClient, rcTotal, rcLocal;
	GetClientRect(&rcTotal);
	m_pGraph->GetGraphRect(m_LButtonInfo.graph, rcClient, rcTotal);
	m_LButtonInfo.graph->GetGraphFieldRect(&rcLocal);
	rcLocal.left += rcClient.left;
	rcLocal.right += rcClient.left;
	rcLocal.top += rcClient.top;
	rcLocal.bottom += rcClient.top;
	MapWindowPoints(NULL,(LPPOINT)&rcLocal,2);
	ClipCursor(&rcLocal);
	SetCapture();
}

void CGraphView::StopCrossing(void)
{
	CSelectionDC dc(this);
	DrawCross(&dc, m_CrossPoint);
	ReleaseCapture();
	m_CrossPoint.SetPoint(0, 0);
}

void CGraphView::DrawCross(CDC* pDC, CPoint pt)
{
	if (pt.x == 0 && pt.y == 0) return;

	LMOUSEDOWNINFO mouseInfo;
	GetLMouseInfoForPoint(&mouseInfo, pt);
	if (mouseInfo.clickedon == GC_GRAPH && mouseInfo.graph)
	{
		CRect rcClient, rcTotal, rcLocal;
		GetClientRect(&rcTotal);
		m_pGraph->GetGraphRect(mouseInfo.graph, rcClient, rcTotal);
		mouseInfo.graph->GetGraphFieldRect(&rcLocal);
		rcLocal.left += rcClient.left;
		rcLocal.right += rcClient.left;
		rcLocal.top += rcClient.top;
		rcLocal.bottom += rcClient.top;

		pDC->MoveTo(rcLocal.left, pt.y);
		pDC->LineTo(rcLocal.right, pt.y);
		pDC->MoveTo(pt.x, rcLocal.top);
		pDC->LineTo(pt.x, rcLocal.bottom);

	}
}

void CGraphView::Resize(void)
{
	CRect clientRect;
	GetClientRect(clientRect);

	if (clientRect.Width() > 0 && clientRect.Height() > 0)
	{
		clientDIB.CheckDIBSize(clientRect.Width(), clientRect.Height(), 24);

		for (int i = 0; i < m_pGraph->GetGraphsCount(); i++)
		{
			CRect rect;
			m_pGraph->GetGraphRect(i, rect, clientRect);
			m_pGraph->GetGraphCtrl(i)->Resize(rect.Width(), rect.Height());
			m_pGraph->GetGraphCtrl(i)->Recenter();
		}
	} 

	if (m_pSelectedGraph && m_pSelectedGraph->m_ShowLegend && !m_LegendTracker.m_rect.IsRectEmpty())
		RecalcLegendTrackerRect(m_pSelectedGraph);
}


void CGraphView::CopyToClipboard(void)
{
	if (OpenClipboard())
	{
		EmptyClipboard();
		CBitmap * junk = new CBitmap();
		CClientDC cdc(this);
		CDC dc;
		dc.CreateCompatibleDC(&cdc);
		CSize size;
		clientDIB.GetSize(&size);
		junk->CreateCompatibleBitmap(&cdc, size.cx, size.cy);
		dc.SelectObject(junk);
		dc.BitBlt(0, 0, size.cx, size.cy, clientDIB.GetDC(), 0, 0, SRCCOPY);
		SetClipboardData(CF_BITMAP, junk->m_hObject);
		CloseClipboard();
		delete junk;
	}
}

void CGraphView::ExportToFile(void)
{
	CxImage img;
	img.CreateFromHBITMAP(clientDIB.GetHandle());

    int nDocType = img.GetType();
	CString filename;
    if (PromptForImageFileName(filename, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, &nDocType))
	{
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR fname[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];
		_tsplitpath(filename, drive, dir, fname, ext);
		CString fext = ext;
		int nFormat = FindType(fext);
		if (nFormat != CXIMAGE_FORMAT_UNKNOWN)
		{
//			if (nFormat == CXIMAGE_FORMAT_GIF && img.GetBpp()>8)
//				img.DecreaseBpp(8, true);
			img.Save(filename, nFormat);
		}
	}
}

void CGraphView::ExportToCSV(void)
{
	CString filename;
    if (PromptForCSVFileName(filename, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST))
	{
    if (m_pGraph)
    {
      CGraphCtrl* pGraph = (m_pSelectedGraph ? m_pSelectedGraph : (m_pGraph->GetGraphsCount() == 1 ? m_pGraph->GetGraphCtrl(0) : NULL));
      if (!pGraph)
        return;

      CStdioFile file;
      if (!file.Open(filename, CFile::modeCreate|CFile::modeWrite))
        return;

      file.WriteString(pGraph->m_Title);
      file.WriteString(_T("\n"));

      for (int nSerie = 0; nSerie < pGraph->GetSeriesCount(); nSerie++)
      {
        GRAPHSERIES *pSerie = pGraph->GetSerie(nSerie);
        if (!pSerie)
          continue;

        //Serie title
        file.WriteString(pSerie->lpName);
        file.WriteString(_T("\n"));

        //Labels
        file.WriteString(pGraph->m_XLabel);
        file.WriteString(_T("; "));
        file.WriteString(pGraph->m_YLabel);
        file.WriteString(_T("\n"));

        //Serie data
        for (int n = 0; n < pSerie->count; n++)
        {
          CString str;
          str.Format(_T("%4.12e; %4.12e\n"), pSerie->pData[n].x, pSerie->pData[n].y);
          file.WriteString(str);
        }
        file.WriteString(_T("\n"));
      }
      file.Close();
    }
	}
}

void CGraphView::ExportToXML(void)
{
  CString filename;
  if (PromptForXMLFileName(filename, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST))
  {
    if (m_pGraph)
    {
      CGraphCtrl* pGraph = (m_pSelectedGraph ? m_pSelectedGraph : (m_pGraph->GetGraphsCount() == 1 ? m_pGraph->GetGraphCtrl(0) : NULL));
      if (!pGraph)
        return;

      CStdioFile file;
      if (!file.Open(filename, CFile::modeCreate|CFile::modeWrite))
        return;

      CString str;
      file.WriteString(_T("<?xml version='1.0'?>\n"));
      str.Format(_T("<graph title='%s'>\n"), pGraph->m_Title);
      file.WriteString(str);

      for (int nSerie = 0; nSerie < pGraph->GetSeriesCount(); nSerie++)
      {
        GRAPHSERIES *pSerie = pGraph->GetSerie(nSerie);
        if (!pSerie)
          continue;

        str.Format(_T("\t<serie name='%s' xlabel='%s' ylabel='%s'>\n"), pSerie->lpName, pGraph->m_XLabel, pGraph->m_YLabel);
        file.WriteString(str);

        //Serie data
        for (int n = 0; n < pSerie->count; n++)
        {
          str.Format(_T("\t\t<data x='%4.12e' y='%4.12e' />\n"), pSerie->pData[n].x, pSerie->pData[n].y);
          file.WriteString(str);
        }
        file.WriteString(_T("\t</serie>\n"));
      }
      file.WriteString(_T("</graph>\n"));
      file.Close();
    }
  }
}

int CGraphView::FindType(CString& ext)
{
        int type = 0;
        if (ext == ".bmp") type = CXIMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
        else if (ext==".jpg"||ext==".jpeg") type = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_GIF
        else if (ext == ".gif") type = CXIMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_PNG
        else if (ext == ".png") type = CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_MNG
        else if (ext==".mng"||ext==".jng") type = CXIMAGE_FORMAT_MNG;
#endif
#if CXIMAGE_SUPPORT_ICO
        else if (ext == ".ico") type = CXIMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
        else if (ext==".tiff"||ext==".tif") type = CXIMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
        else if (ext==".tga") type = CXIMAGE_FORMAT_TGA;
#endif
#if CXIMAGE_SUPPORT_PCX
        else if (ext==".pcx") type = CXIMAGE_FORMAT_PCX;
#endif
#if CXIMAGE_SUPPORT_WBMP
        else if (ext==".wbmp") type = CXIMAGE_FORMAT_WBMP;
#endif
#if CXIMAGE_SUPPORT_WMF
        else if (ext==".wmf"||ext==".emf") type = CXIMAGE_FORMAT_WMF;
#endif
#if CXIMAGE_SUPPORT_J2K
        else if (ext==".j2k"||ext==".jp2") type = CXIMAGE_FORMAT_J2K;
#endif
#if CXIMAGE_SUPPORT_JBG
        else if (ext==".jbg") type = CXIMAGE_FORMAT_JBG;
#endif
#if CXIMAGE_SUPPORT_JP2
        else if (ext==".jp2"||ext==".j2k") type = CXIMAGE_FORMAT_JP2;
#endif
#if CXIMAGE_SUPPORT_JPC
        else if (ext==".jpc"||ext==".j2c") type = CXIMAGE_FORMAT_JPC;
#endif
#if CXIMAGE_SUPPORT_PGX
        else if (ext==".pgx") type = CXIMAGE_FORMAT_PGX;
#endif
#if CXIMAGE_SUPPORT_RAS
        else if (ext==".ras") type = CXIMAGE_FORMAT_RAS;
#endif
#if CXIMAGE_SUPPORT_PNM
        else if (ext==".pnm"||ext==".pgm"||ext==".ppm") type = CXIMAGE_FORMAT_PNM;
#endif
        else type = CXIMAGE_FORMAT_UNKNOWN;

        return type;
}

BOOL CGraphView::PromptForImageFileName(CString& fileName,
        DWORD dwFlags, BOOL bOpenFileDialog, int* pType)
{
	CFileDialog dlgFile(bOpenFileDialog);
	CString title;
	if (bOpenFileDialog) title="Open image file"; else title="Save image file";

	dlgFile.m_ofn.Flags |= dwFlags;

	int nDocType = (pType != NULL) ? *pType : CXIMAGE_FORMAT_BMP;
	if (nDocType==0) nDocType=1;

	int nIndex = GetIndexFromType(nDocType, bOpenFileDialog);
	if (nIndex == -1) nIndex = 0;

	dlgFile.m_ofn.nFilterIndex = nIndex +1;
	// strDefExt is necessary to hold onto the memory from GetExtFromType
	CString strDefExt = GetExtFromType(nDocType).Mid(2,3);
	dlgFile.m_ofn.lpstrDefExt = strDefExt;

	CString strFilter = GetFileTypes(bOpenFileDialog);
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();
	if (bRet)
	{
		if (pType != NULL)
		{
			int nIndex = (int)dlgFile.m_ofn.nFilterIndex - 1;
			ASSERT(nIndex >= 0);
			*pType = GetTypeFromIndex(nIndex, bOpenFileDialog);
		}
	}
	return bRet;
}

CString CGraphView::GetExtFromType(int nDocType)
{
    for (int i=0;i<CMAX_IMAGE_FORMATS;i++)
	{
        if (doctypes[i].nID == nDocType)
            return doctypes[i].ext;
    }
    return CString("");
}

CString CGraphView::GetFileTypes(BOOL bOpenFileDialog)
{
    CString str;
    for (int i=0;i<CMAX_IMAGE_FORMATS;i++)
	{
        if (bOpenFileDialog && doctypes[i].bRead)
		{
            str += doctypes[i].description;
			str += _T(" (");
			str += doctypes[i].ext;
			str += _T(")");
            str += (TCHAR)NULL;
            str += doctypes[i].ext;
            str += (TCHAR)NULL;
        }
		else if (!bOpenFileDialog && doctypes[i].bWrite)
		{
            str += doctypes[i].description;
			str += _T(" (");
			str += doctypes[i].ext;
			str += _T(")");
            str += (TCHAR)NULL;
            str += doctypes[i].ext;
            str += (TCHAR)NULL;
        }
    }
    return str;
}

int CGraphView::GetIndexFromType(int nDocType, BOOL bOpenFileDialog)
{
    int nCnt = 0;
    for (int i=0;i<CMAX_IMAGE_FORMATS;i++)
	{
        if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite)
		{
            if (doctypes[i].nID == nDocType) return nCnt;
            nCnt++;
        }
    }
    return -1;
}

int CGraphView::GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog)
{
    int nCnt = 0;
    for (int i=0;i<CMAX_IMAGE_FORMATS;i++)
	{
        if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite)
		{
            if (nCnt == nIndex)
//              return i; // PJO - Buglet ?
				return doctypes[i].nID;
            nCnt++;
        }
    }
    ASSERT(FALSE);
    return -1;
}

void CGraphView::SavePreset(void)
{
	if (m_pGraph->GetGraphsCount() != 1) return;
	CString fileName;
	if (PromptForPresetFileName(fileName, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE))
		if (!m_pGraph->SavePreset(fileName, m_pSelectedGraph))
		{
			CString str;
			str.Format(_T("Failed to save preset to %s."), fileName);
			MessageBox(str, _T("Graph Viewer"), MB_ICONSTOP | MB_OK);
		}
	m_pSelectedGraph = NULL;
}

void CGraphView::LoadPreset(LPCTSTR pchFile /* = NULL */)
{
	CString fileName;

	BOOL bContinue =	TRUE;
	if(pchFile == NULL)
		bContinue =	PromptForPresetFileName(fileName, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE);
	else
		fileName =	pchFile;
	if (bContinue)
	{
		BOOL result = TRUE;
		for (int i = 0; i < m_pGraph->GetGraphsCount(); i++)
		{
			CGraphCtrl* pCtrl;
			if (m_pSelectedGraph)
				pCtrl = m_pSelectedGraph;
			else 
				pCtrl = m_pGraph->GetGraphCtrl(i);
			result &= m_pGraph->LoadPreset(fileName, pCtrl);
			if (m_pSelectedGraph) break;
		}
		if (result)
		{
			StopMoving();
			Resize();
			Invalidate();
		}
		else
		{
			CString str;
			str.Format(_T("Failed to load preset from %s."), fileName);
			MessageBox(str, _T("Graph Viewer"), MB_ICONSTOP | MB_OK);
		}
	}
	m_pSelectedGraph = NULL;
}

void CGraphView::SaveData(void)
{
	CString fileName;
	if (PromptForDataFileName(fileName, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE))
		if (!m_pGraph->SaveGraphData(fileName, m_pSelectedGraph))
		{
			CString str;
			str.Format(_T("Failed to save data to %s."), fileName);
			MessageBox(str, _T("Graph Viewer"), MB_ICONSTOP | MB_OK);
		}

	m_pSelectedGraph = NULL;
}

void CGraphView::LoadData(void)
{
	CString fileName;
	if (PromptForDataFileName(fileName, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, TRUE))
		if (m_pGraph->LoadGraphData(fileName, m_pSelectedGraph))
		{
			CPathString path;
			path.setPath(fileName);
			
			CString strTitle;
			strTitle.LoadString(IDR_GRAPHFRAME);
			GetParent()->SetWindowText(strTitle + _T(" - ") + path.getFileName()+path.getExt());

			StopMoving();
			Resize();
			Invalidate();
		}
		else
		{
			CString str;
			str.Format(_T("Failed to load data from %s."), fileName);
			MessageBox(str, _T("Graph Viewer"), MB_ICONSTOP | MB_OK);
		}
	m_pSelectedGraph = NULL;
}

void CGraphView::SaveGraph(void)
{
	CString fileName;
	if (PromptForGraphFileName(fileName, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE))
		if (!m_pGraph->SaveGraph(fileName, m_pSelectedGraph))
		{
			CString str;
			str.Format(_T("Failed to save graph to %s."), fileName);
			MessageBox(str, _T("Graph Viewer"), MB_ICONSTOP | MB_OK);
		}
	m_pSelectedGraph = NULL;
}

void CGraphView::LoadGraph(void)
{
	CString fileName;
	if (PromptForGraphFileName(fileName, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, TRUE))
		if (!m_pGraph->LoadGraph(fileName, m_pSelectedGraph))
		{
			CString str;
			str.Format(_T("Failed to load graph from %s."), fileName);
			MessageBox(str, _T("Graph Viewer"), MB_ICONSTOP | MB_OK);
		}

	m_pSelectedGraph = NULL;
}

BOOL CGraphView::PromptForPresetFileName(CString& fileName,
        DWORD dwFlags, BOOL bOpenFileDialog)
{
	CFileDialog dlgFile(bOpenFileDialog);
	CString title;
	if (bOpenFileDialog) title=_T("Open preset file"); else title=_T("Save preset file");
	dlgFile.m_ofn.Flags |= dwFlags;
	dlgFile.m_ofn.lpstrFilter = _T("Graph Presets (*.gpf)\0*.gpf\0All Files\0*.*\0");
	dlgFile.m_ofn.nFilterIndex = 1;
	dlgFile.m_ofn.lpstrDefExt = _T("gpf");
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();
	return bRet;
}

BOOL CGraphView::PromptForDataFileName(CString& fileName,
        DWORD dwFlags, BOOL bOpenFileDialog)
{
	CFileDialog dlgFile(bOpenFileDialog);
	CString title;
	if (bOpenFileDialog) title=_T("Open graph data file"); else title=_T("Save graph data file");
	dlgFile.m_ofn.Flags |= dwFlags;
	dlgFile.m_ofn.lpstrFilter = _T("Graph Data Files (*.gdf)\0*.gdf\0All Files\0*.*\0");
	dlgFile.m_ofn.nFilterIndex = 1;
	dlgFile.m_ofn.lpstrDefExt = _T("gdf");
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();
	return bRet;
}

BOOL CGraphView::PromptForGraphFileName(CString& fileName,
        DWORD dwFlags, BOOL bOpenFileDialog)
{
	CFileDialog dlgFile(bOpenFileDialog);
	CString title;
	if (bOpenFileDialog) title=_T("Open graph file"); else title=_T("Save graph file");
	dlgFile.m_ofn.Flags |= dwFlags;
	dlgFile.m_ofn.lpstrFilter = _T("Graph Files (*.grf)\0*.grf\0All Files\0*.*\0");
	dlgFile.m_ofn.nFilterIndex = 1;
	dlgFile.m_ofn.lpstrDefExt = _T("grf");
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();
	return bRet;
}

BOOL CGraphView::PromptForCSVFileName(CString& fileName,
                                      DWORD dwFlags)
{
  CFileDialog dlgFile(FALSE);
  CString title;
  dlgFile.m_ofn.Flags |= dwFlags;
  dlgFile.m_ofn.lpstrFilter = _T("CSV Files (*.csv)\0*.csv\0All Files\0*.*\0");
  dlgFile.m_ofn.nFilterIndex = 1;
  dlgFile.m_ofn.lpstrDefExt = _T("csv");
  dlgFile.m_ofn.lpstrTitle = _T("Save CSV file");
  dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
  BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
  fileName.ReleaseBuffer();
  return bRet;
}

BOOL CGraphView::PromptForXMLFileName(CString& fileName,
        DWORD dwFlags)
{
	CFileDialog dlgFile(FALSE);
	CString title;
	dlgFile.m_ofn.Flags |= dwFlags;
	dlgFile.m_ofn.lpstrFilter = _T("XML Files (*.xml)\0*.xml\0All Files\0*.*\0");
	dlgFile.m_ofn.nFilterIndex = 1;
	dlgFile.m_ofn.lpstrDefExt = _T("xml");
	dlgFile.m_ofn.lpstrTitle = _T("Save XML file");
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();
	return bRet;
}

void CGraphView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	StopCrossing();

	m_LegendTracker.m_rect.SetRectEmpty();
	Invalidate();

	LMOUSEDOWNINFO mouseInfo;
	CPoint pt = point;
	ScreenToClient(&pt);
	GetLMouseInfoForPoint(&mouseInfo, pt);
	
	m_pSelectedGraph = mouseInfo.graph;
	if (m_pSelectedGraph != NULL)
	{
		HMENU hMenu = NULL;
		switch (mouseInfo.clickedon)
		{
		case GC_BACKGROUND: 
			{
				hMenu = GetSubMenu(m_hContextMenu, 0); 
				if (hMenu) SetMenuDefaultItem(hMenu, ID_BACKGROUND_GRAPH, FALSE);
			} break;
		case GC_LEGEND: 
			{
				hMenu = GetSubMenu(m_hContextMenu, 1); 
				if (hMenu) SetMenuDefaultItem(hMenu, ID_LEGEND_LEGEND, FALSE);
			} break;
		case GC_GRAPH:
			{
				hMenu = GetSubMenu(m_hContextMenu, 2); 
				if (hMenu) SetMenuDefaultItem(hMenu, ID_DATA_PLOTFIELD, FALSE);
			} break;
		}
		if (hMenu)
		{
			UpdateContextMenu(hMenu);
			UpdateCommonMenuItems(hMenu);
			UINT nCmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, point.x, point.y, NULL, m_hWnd, NULL);
			if (nCmd)
				GetParent()->PostMessage(WM_COMMAND, nCmd, 0);
		}
	}
}

void CGraphView::LoadMenus(void)
{
	m_hContextMenu = LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_CONTEXTMENU));
}

void CGraphView::DestroyMenus(void)
{
	if (m_hContextMenu) 
		DestroyMenu(m_hContextMenu), m_hContextMenu = NULL;
}

void CGraphView::UpdateContextMenu(HMENU hMenu)
{
	EnableMenuItem(hMenu, ID_BACKGROUND_GRAPH, MF_BYCOMMAND | MF_DISABLED);
	EnableMenuItem(hMenu, ID_LEGEND_LEGEND, MF_BYCOMMAND | MF_DISABLED);
	EnableMenuItem(hMenu, ID_DATA_PLOTFIELD, MF_BYCOMMAND | MF_DISABLED);

	EnableMenuItem(hMenu, ID_OPEN_DATA, MF_BYCOMMAND | (m_pGraph->GetGraphsCount() > 1 ? MF_GRAYED : MF_ENABLED));
}

void CGraphView::UpdateCommonMenuItems(HMENU hMenu)
{

	CheckMenuItem(hMenu, ID_ZOOM_ZOOM, MF_BYCOMMAND | (GetZoomMode() == ZM_ZOOM ? MF_CHECKED : MF_UNCHECKED));
	CheckMenuItem(hMenu, ID_ZOOM_UNZOOM, MF_BYCOMMAND | (GetZoomMode() == ZM_UNZOOM ? MF_CHECKED : MF_UNCHECKED));
	CheckMenuItem(hMenu, ID_ZOOM_ZOOM100, MF_BYCOMMAND | (GetZoomMode() == ZM_100 ? MF_CHECKED : MF_UNCHECKED));
	CheckMenuItem(hMenu, ID_TOOLS_PANTOOL, MF_BYCOMMAND | (GetZoomMode() == ZM_PAN ? MF_CHECKED : MF_UNCHECKED));
	CheckMenuItem(hMenu, ID_TOOLS_CROSSTOOL, MF_BYCOMMAND | (GetZoomMode() == ZM_CROSS ? MF_CHECKED : MF_UNCHECKED));
	CheckMenuItem(hMenu, ID_TOOLS_SELECTIONTOOL, MF_BYCOMMAND | (GetZoomMode() == ZM_NONE ? MF_CHECKED : MF_UNCHECKED));

	CGraphCtrl* pGraph = (m_pSelectedGraph ? m_pSelectedGraph : (m_pGraph->GetGraphsCount() == 1 ? m_pGraph->GetGraphCtrl(0) : NULL));

	EnableMenuItem(hMenu, ID_GRAPHCOMPONENTS_TITLE, MF_BYCOMMAND | (!pGraph ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, ID_GRAPHCOMPONENTS_SUBTITLE, MF_BYCOMMAND | (!pGraph  ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, ID_GRAPHCOMPONENTS_LEGEND, MF_BYCOMMAND | (!pGraph  ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, ID_GRAPHCOMPONENTS_XLABEL, MF_BYCOMMAND | (!pGraph  ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, ID_GRAPHCOMPONENTS_YLABEL, MF_BYCOMMAND | (!pGraph  ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, ID_GRAPHCOMPONENTS_XDATALABELS, MF_BYCOMMAND | (!pGraph  ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, ID_GRAPHCOMPONENTS_YDATALABELS, MF_BYCOMMAND | (!pGraph  ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, ID_GRAPHCOMPONENTS_GRIDS, MF_BYCOMMAND | (!pGraph  ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, ID_GRAPHCOMPONENTS_SUBGRIDS, MF_BYCOMMAND | (!pGraph  ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, ID_GRAPHCOMPONENTS_TICKS, MF_BYCOMMAND | (!pGraph  ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, ID_GRAPHCOMPONENTS_SUBTICKS, MF_BYCOMMAND | (!pGraph  ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, ID_EXPORT_TO_CSV, MF_BYCOMMAND | (!pGraph  ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, ID_EXPORT_TO_XML, MF_BYCOMMAND | (!pGraph  ? MF_GRAYED : MF_ENABLED));

	if (pGraph)
	{
		CheckMenuItem(hMenu, ID_GRAPHCOMPONENTS_TITLE, MF_BYCOMMAND | (pGraph->m_ShowTitle ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem(hMenu, ID_GRAPHCOMPONENTS_SUBTITLE, MF_BYCOMMAND | (pGraph->m_ShowSubTitle ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem(hMenu, ID_GRAPHCOMPONENTS_LEGEND, MF_BYCOMMAND | (pGraph->m_ShowLegend ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem(hMenu, ID_GRAPHCOMPONENTS_XLABEL, MF_BYCOMMAND | (pGraph->m_ShowXLabel ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem(hMenu, ID_GRAPHCOMPONENTS_YLABEL, MF_BYCOMMAND | (pGraph->m_ShowYLabel ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem(hMenu, ID_GRAPHCOMPONENTS_XDATALABELS, MF_BYCOMMAND | (pGraph->m_ShowXTicksLabels ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem(hMenu, ID_GRAPHCOMPONENTS_YDATALABELS, MF_BYCOMMAND | (pGraph->m_ShowYTicksLabels ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem(hMenu, ID_GRAPHCOMPONENTS_GRIDS, MF_BYCOMMAND | (pGraph->m_ShowGrid ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem(hMenu, ID_GRAPHCOMPONENTS_SUBGRIDS, MF_BYCOMMAND | (pGraph->m_ShowSubGrid ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem(hMenu, ID_GRAPHCOMPONENTS_TICKS, MF_BYCOMMAND | (pGraph->m_ShowXTicks && pGraph->m_ShowYTicks ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem(hMenu, ID_GRAPHCOMPONENTS_SUBTICKS, MF_BYCOMMAND | (pGraph->m_ShowXSubTicks && pGraph->m_ShowYSubTicks ? MF_CHECKED : MF_UNCHECKED));
	}
}

void CGraphView::DoGraphComponentsClick(UINT nID)
{
	CGraphCtrl* pGraph = (m_pSelectedGraph ? m_pSelectedGraph : (m_pGraph->GetGraphsCount() == 1 ? m_pGraph->GetGraphCtrl(0) : NULL));
	if (!pGraph) return;
	switch (nID)
	{
	case ID_GRAPHCOMPONENTS_TITLE: pGraph->m_ShowTitle = !pGraph->m_ShowTitle; break;
	case ID_GRAPHCOMPONENTS_SUBTITLE: pGraph->m_ShowSubTitle = !pGraph->m_ShowSubTitle; break;
	case ID_GRAPHCOMPONENTS_LEGEND: pGraph->m_ShowLegend = !pGraph->m_ShowLegend; break;
	case ID_GRAPHCOMPONENTS_XLABEL: pGraph->m_ShowXLabel = !pGraph->m_ShowXLabel; break;
	case ID_GRAPHCOMPONENTS_YLABEL: pGraph->m_ShowYLabel = !pGraph->m_ShowYLabel; break;
	case ID_GRAPHCOMPONENTS_XDATALABELS: pGraph->m_ShowXTicksLabels = !pGraph->m_ShowXTicksLabels; break;
	case ID_GRAPHCOMPONENTS_YDATALABELS: pGraph->m_ShowYTicksLabels = !pGraph->m_ShowYTicksLabels; break;
	case ID_GRAPHCOMPONENTS_GRIDS: pGraph->m_ShowGrid = !pGraph->m_ShowGrid; break;
	case ID_GRAPHCOMPONENTS_SUBGRIDS: pGraph->m_ShowSubGrid = !pGraph->m_ShowSubGrid; break;
	case ID_GRAPHCOMPONENTS_TICKS: pGraph->m_ShowXTicks = !pGraph->m_ShowXTicks; 
								   /*pGraph->m_ShowYTicks = !pGraph->m_ShowYTicks;*/ break;
	case ID_GRAPHCOMPONENTS_SUBTICKS: pGraph->m_ShowXSubTicks = !pGraph->m_ShowXSubTicks; 
									  /*pGraph->m_ShowYSubTicks = !pGraph->m_ShowYSubTicks;*/ break;
	default: return;
	}
	Resize();
	Invalidate();
	m_pSelectedGraph = NULL;
}

BOOL CGraphView::UpdateCmdUIGraphComponentsClick(UINT nID)
{
	BOOL		w_bSts = FALSE;

	CGraphCtrl* pGraph = (m_pSelectedGraph ? m_pSelectedGraph : (m_pGraph->GetGraphsCount() == 1 ? m_pGraph->GetGraphCtrl(0) : NULL));
	if (!pGraph) 
		return w_bSts;
	
	switch (nID)
	{
	case ID_GRAPHCOMPONENTS_TITLE: w_bSts = pGraph->m_ShowTitle; break;
	case ID_GRAPHCOMPONENTS_SUBTITLE: w_bSts = pGraph->m_ShowSubTitle; break;
	case ID_GRAPHCOMPONENTS_LEGEND: w_bSts = pGraph->m_ShowLegend; break;
	case ID_GRAPHCOMPONENTS_XLABEL: w_bSts = pGraph->m_ShowXLabel; break;
	case ID_GRAPHCOMPONENTS_YLABEL: w_bSts = pGraph->m_ShowYLabel; break;
	case ID_GRAPHCOMPONENTS_XDATALABELS:w_bSts = pGraph->m_ShowXTicksLabels; break;
	case ID_GRAPHCOMPONENTS_YDATALABELS: w_bSts = pGraph->m_ShowYTicksLabels; break;
	case ID_GRAPHCOMPONENTS_GRIDS: w_bSts = pGraph->m_ShowGrid; break;
	case ID_GRAPHCOMPONENTS_SUBGRIDS: w_bSts = pGraph->m_ShowSubGrid; break;
	case ID_GRAPHCOMPONENTS_TICKS: w_bSts = pGraph->m_ShowXTicks;
		/*pGraph->m_ShowYTicks = !pGraph->m_ShowYTicks; */ break;
	case ID_GRAPHCOMPONENTS_SUBTICKS: w_bSts = pGraph->m_ShowXSubTicks;
		/*pGraph->m_ShowYSubTicks = !pGraph->m_ShowYSubTicks;*/ break;
	default: return w_bSts;
	}
	return w_bSts;
}

void CGraphView::OnLegendMoveSize()
{
	CGraphCtrl* pGraph = (m_pSelectedGraph ? m_pSelectedGraph : (m_pGraph->GetGraphsCount() == 1 ? m_pGraph->GetGraphCtrl(0) : NULL));
	if (!pGraph) return;

	m_pSelectedGraph = pGraph;
	RecalcLegendTrackerRect(pGraph);
	Invalidate();
}

void CGraphView::OnLegendHide()
{
	CGraphCtrl* pGraph = (m_pSelectedGraph ? m_pSelectedGraph : (m_pGraph->GetGraphsCount() == 1 ? m_pGraph->GetGraphCtrl(0) : NULL));
	if (!pGraph) return;
	pGraph->m_ShowLegend = FALSE;
	Resize();
	Invalidate();
	m_pSelectedGraph = NULL;
}

void CGraphView::OnLegendProperties()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CGraphCtrl* pGraph = (m_pSelectedGraph ? m_pSelectedGraph : (m_pGraph->GetGraphsCount() == 1 ? m_pGraph->GetGraphCtrl(0) : NULL));
	if (!pGraph) return;

	CLegendProperties legendProperties(_T("Properties"), this);
	legendProperties.SetGraphCtrl(pGraph);
	legendProperties.SetGraphView(this);
	legendProperties.DoModal();
	m_pSelectedGraph = NULL;
}

void CGraphView::OnGraphProperties()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CGraphCtrl* pGraph = (m_pSelectedGraph ? m_pSelectedGraph : (m_pGraph->GetGraphsCount() == 1 ? m_pGraph->GetGraphCtrl(0) : NULL));
	if (!pGraph) 
		return;

	CGraphProperties graphProperties(_T("Properties"), this);
	graphProperties.SetGraphCtrl(pGraph);
	graphProperties.SetGraphView(this);
	graphProperties.DoModal();
	m_pSelectedGraph = NULL;
}

void CGraphView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk(nFlags, point);

	LMOUSEDOWNINFO mouseInfo;
	GetLMouseInfoForPoint(&mouseInfo, point);
	m_pSelectedGraph = mouseInfo.graph;
	if (m_pSelectedGraph && mouseInfo.clickedon == GC_LEGEND)
		OnLegendMoveSize();
}


void CGraphView::RecalcLegendTrackerRect(CGraphCtrl* pGraph)
{
	CRect graphRect, legendRect, totalRect;
	GetClientRect(totalRect);
	m_pGraph->GetGraphRect(pGraph, graphRect, totalRect);
	pGraph->GetLegendRect(&legendRect);
	legendRect.top += graphRect.top;
	legendRect.bottom += graphRect.top;
	legendRect.left += graphRect.left;
	legendRect.right += graphRect.left;
	m_LegendTracker.m_rect = legendRect;
}

BOOL CGraphView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (pWnd == this && m_LegendTracker.SetCursor(this, nHitTest))
		return TRUE;

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CGraphView::RecalcLegendRect(CGraphCtrl* pGraph)
{
	CRect graphRect, legendRect = m_LegendTracker.m_rect, totalRect;
	GetClientRect(totalRect);
	m_pGraph->GetGraphRect(pGraph, graphRect, totalRect);
	legendRect.top -= graphRect.top;
	legendRect.bottom -= graphRect.top;
	legendRect.left -= graphRect.left;
	legendRect.right -= graphRect.left;
	pGraph->SetLegendRect(legendRect);
}

void CGraphView::StopMoving(void)
{
	m_LegendTracker.m_rect.SetRectEmpty();
}

void CGraphView::OnDataProperties(void)
{
	CGraphCtrl* pGraph = (m_pSelectedGraph ? m_pSelectedGraph : (m_pGraph->GetGraphsCount() == 1 ? m_pGraph->GetGraphCtrl(0) : NULL));
	if (!pGraph) return;

	CDataProperties dataProperties(_T("Properties"), this);
	dataProperties.SetGraphCtrl(pGraph);
	dataProperties.SetGraphView(this);
	dataProperties.DoModal();
	m_pSelectedGraph = NULL;
}

void CGraphView::OnData100(void)
{
	if (m_pSelectedGraph)
	{
		m_pSelectedGraph->m_ZoomFactorXList.RemoveAll();
		m_pSelectedGraph->m_ZoomFactorYList.RemoveAll();
		m_pSelectedGraph->Zoom(1 / m_pSelectedGraph->m_CurrentXZoomFactor, 1 / m_pSelectedGraph->m_CurrentYZoomFactor, CPoint(0, 0));
		GetParent()->PostMessage(WM_COMMAND, ID_TOOLS_SELECTIONTOOL, 0);
		Invalidate();
	}
}

BOOL CGraphView::IsPanning(void)
{
	return m_Panning;
}

void CGraphView::OnGraphPageSetup(void)
{
	MessageBox(_T("Sorry. Not implemented yet"));
	return;

	CRect m_rectMargin(10, 10, 10, 10);
	CPageSetupDialog dlg;
	PAGESETUPDLG& psd = dlg.m_psd;
	BOOL bMetric = FALSE;//AfxGetApp()->GetUnits() == 1; //centimeters
	psd.Flags |= PSD_MARGINS | (bMetric ? PSD_INHUNDREDTHSOFMILLIMETERS :
		PSD_INTHOUSANDTHSOFINCHES);
	int nUnitsPerInch = bMetric ? 2540 : 1000;
	MulDivRect(&psd.rtMargin, m_rectMargin, nUnitsPerInch, 1440);
	RoundRect(&psd.rtMargin);
	// get the current device from the app
	PRINTDLG pd;
	pd.hDevNames = NULL;
	pd.hDevMode = NULL;
	AfxGetApp()->GetPrinterDeviceDefaults(&pd);
	psd.hDevNames = pd.hDevNames;
	psd.hDevMode = pd.hDevMode;
	if (dlg.DoModal() == IDOK)
	{
		RoundRect(&psd.rtMargin);
		MulDivRect(m_rectMargin, &psd.rtMargin, 1440, nUnitsPerInch);
//		AfxGetApp()->m_rectPageMargin = m_rectMargin;
		AfxGetApp()->SelectPrinter(psd.hDevNames, psd.hDevMode);
//		AfxGetApp()->NotifyPrinterChanged();
	}
}

void CGraphView::OnGraphPrintPreview(void)
{
	if (m_bPrintPreview)
	{
		AfxGetApp()->m_pMainWnd->SetFocus();
		return;
	}

	CFrameWnd* pOldFrame = (CFrameWnd*)GetParent();
	pOldFrame->ShowWindow(SW_HIDE); 

	if (!m_pTemplate)
	{
		m_pTemplate = new CSingleDocTemplate(
			IDR_GRAPHFRAME,
			NULL,
			RUNTIME_CLASS(CFrameWnd),
			RUNTIME_CLASS(CViewPrintPreview));
		AfxGetApp()->AddDocTemplate(m_pTemplate);
	}

	CFrameWnd* pFrameWnd = m_pTemplate->CreateNewFrame( NULL, NULL );
	m_bPrintPreview =TRUE;

	m_pTemplate->InitialUpdateFrame(pFrameWnd, NULL, FALSE);

	CViewPrintPreview* pView = (CViewPrintPreview*)pFrameWnd->GetActiveView();
	pView->m_pGraph = m_pGraph;
	if (AfxGetApp()->m_pMainWnd && AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		pView->m_pOldFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	else 
		pView->m_pOldFrame = pOldFrame;

	AfxGetApp()->m_pMainWnd = pFrameWnd;

	TCHAR old[1024], buf[1024];
	if (pOldFrame->GetWindowText(old, 1024) > 0)
		_stprintf(buf, _T("%s - Print Preview"), old);
	else _tcscpy(buf, _T("Graph Print Preview"));
	pFrameWnd->SetWindowText(buf);
	pFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);

	PRINTDLG m_pd;
	AfxGetApp()->GetPrinterDeviceDefaults(&m_pd);

	m_pd.nMinPage = m_pd.nFromPage = 1;
	m_pd.nMaxPage = m_pd.nToPage = 1;

	LPDEVMODE lpDevMode = (LPDEVMODE)::GlobalLock(m_pd.hDevMode);
	if (lpDevMode)
	{
		lpDevMode->dmFields |= DM_ORIENTATION;
		lpDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	}
	::GlobalUnlock(m_pd.hDevMode);

	pView->OnFilePrintPreview();
}

void CGraphView::OnGraphPrint(void)
{
	m_pGraph->Print();
}
