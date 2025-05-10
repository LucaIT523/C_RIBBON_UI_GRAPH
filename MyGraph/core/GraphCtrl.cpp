// GraphCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\Graph.h"
#include "..\include\constants.h"
#include "atltypes.h"
#include "..\include\gl\cgl.h"
#include  <math.h>
#include "..\include\GraphCtrl.h"
#include "..\include\GraphScene.h"
#include "..\include\BGScene.h"
#include "..\include\LegendScene.h"
#include "..\include\graph.h"
#include "..\include\bitmaps\DDB.h"
#include "..\include\tools.h"
#include "..\include\graphview.h"
#include <gl\GLU.h>

// CGraphCtrl
#define PI 3.141592653589793f

CGraphCtrl::CGraphCtrl(CGraph* pGraph)
: m_pGraph(pGraph)
, m_ShowLegend(TRUE)
, m_ShowTitle(TRUE)
, m_ShowSubTitle(FALSE)
, m_ShowXLabel(TRUE)
, m_ShowYLabel(TRUE)
, m_ShowXTicks(TRUE)
, m_ShowYTicks(TRUE)
, m_ShowXSubTicks(FALSE)
, m_ShowYSubTicks(FALSE)
, m_ShowGrid(TRUE)
, m_ShowSubGrid(FALSE)
, m_GridXSize(DEFAULTXGRIDSIZE)
, m_GridYSize(DEFAULTYGRIDSIZE)
, m_GraphBorderWidth(2)
, m_LegendBorderWidth(2)
, m_GridColor(RGB(0xC0, 0xC0, 0xC0))
, m_SubGridColor(RGB(0xC0, 0xC0, 0xC0))
, m_BackGround(GRAPH_FT_NONE, GR_NONE, RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF))
, m_GraphFieldBackGround(GRAPH_FT_NONE, GR_NONE, RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF))
, m_LegendBackGround(GRAPH_FT_NONE, GR_NONE, RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF))
, m_ShowXTicksLabels(TRUE)
, m_ShowYTicksLabels(TRUE)
, m_ShowXSubTicksLabels(FALSE) // KARTIK ASKED FOR FALSE
, m_ShowYSubTicksLabels(FALSE) // KARTIK ASKED FOR FALSE
, m_AntiAlaising(TRUE)
, m_GraphStyle(GS_LINES)
, m_PointSize(3)
, m_TicksXLabelsFont()
, m_TicksYLabelsFont()
, m_LegendFont()
, m_TitleFont()
, m_SubTitleFont()
, m_XLabelFont()
, m_YLabelFont()
, m_DrawZeroLabels(TRUE)
, m_CurrentXZoomFactor(1)
, m_CurrentYZoomFactor(1)
, m_Prepared(FALSE)
, m_IntelligentAxisScale(TRUE)
, m_StableXGridCount(-1)
, m_StableYGridCount(-1)
, m_DefaultGraphRect(CRect(0, 0, 0, 0))
, m_bAutoLegendPos(TRUE)
, m_bLegendFloating(FALSE)
, m_GridLine(LS_SOLID)
, m_SubGridLine(LS_SOLID)
, m_Symmetric(FALSE)
, m_DefaultXGridSize(DEFAULTXGRIDSIZE)
, m_DefaultYGridSize(DEFAULTYGRIDSIZE)
, m_GridXChanging(25)
, m_GridYChanging(25)
, m_SubXGridsCount(2)
, m_SubYGridsCount(2)
, m_SubGridXSize(m_DefaultXGridSize / m_SubXGridsCount)
, m_SubGridYSize(m_DefaultYGridSize / m_SubYGridsCount)
, m_GraphXChanging(25)
, m_GraphYChanging(25)
, m_LeftBorder(20)
, m_RightBorder(20)
, m_TopBorder(10)
, m_BottomBorder(10)
, m_XLabelsFormat(LF_DOUBLE)
, m_YLabelsFormat(LF_DOUBLE)
, m_XLabelsFormatAdditional(2)
, m_YLabelsFormatAdditional(2)
, m_AdditionalGridSteps(CRect(0, 0, 0, 0))
, m_GraphBorderColor(RGB(0, 0, 0))
, m_LegendBorderColor(RGB(0, 0, 0))
, m_LogX(FALSE)
, m_LogY(FALSE)
, m_ShowLogZero(FALSE)
, m_pPrintingDC(NULL)
, m_BackupLegendRect(CRect(0, 0, 0, 0))
{
	m_pBGScene = new CBGScene(this);
	m_pGraphScene = new CGraphScene(this);
	m_pLegendScene = new CLegendScene(this);
	m_YLabelFont.angle = 900;
	MarkAsDefaultPreset();
}

CGraphCtrl::~CGraphCtrl()
{
	ClearDrawTextData();

	Clear();
	
	delete m_pBGScene;
	delete m_pGraphScene;
	delete m_pLegendScene;
}

// CGraphCtrl member functions

void CGraphCtrl::CalculateAxisScale(void)
{
	if (m_IntelligentAxisScale)
	{
		GetGraphDataLimits(&m_GraphXSize, &m_GraphYSize);

/*
		double xExp = floor(log10(fabs(m_GraphXSize.min))) > floor(log10(fabs(m_GraphXSize.max))) ?
			floor(log10(fabs(m_GraphXSize.min))) : floor(log10(fabs(m_GraphXSize.max)));
		double yExp = floor(log10(fabs(m_GraphYSize.min))) > floor(log10(fabs(m_GraphYSize.max))) ?
			floor(log10(fabs(m_GraphYSize.min))) : floor(log10(fabs(m_GraphYSize.max)));
*/		
		m_GraphXSize.min = m_LogX?floor(m_GraphXSize.min):_floor_nice(m_GraphXSize.min);
		m_GraphYSize.min = m_LogY?floor(m_GraphYSize.min):_floor_nice(m_GraphYSize.min);
		m_GraphXSize.max = m_LogX?ceil(m_GraphXSize.max):_ceil_nice(m_GraphXSize.max);
		m_GraphYSize.max = m_LogY?ceil(m_GraphYSize.max):_ceil_nice(m_GraphYSize.max);
/*
		m_GraphXSize.min = _round(m_GraphXSize.min / pow(10.0, xExp), 1) * pow(10.0, xExp);
		m_GraphXSize.max = _round(m_GraphXSize.max / pow(10.0, xExp), 1) * pow(10.0, xExp);
		m_GraphYSize.min = _round(m_GraphYSize.min / pow(10.0, yExp), 1) * pow(10.0, yExp);
		m_GraphYSize.max = _round(m_GraphYSize.max / pow(10.0, yExp), 1) * pow(10.0, yExp);
*/
	}
}

int CGraphCtrl::GetSeriesCount(void)
{
	return (int)m_GraphSeries.GetCount();
}

void CGraphCtrl::AddSeries(GRAPHSERIES* pSeries, BOOL redraw)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_GraphSeries.AddTail(pSeries);
	if (redraw && m_pGraph->GetView() && m_pGraph->GetView()->IsWindowVisible())
	{
		m_pGraph->GetView()->Resize();
		m_pGraph->GetView()->Invalidate();
	}
}

int CGraphCtrl::IndexOfSerie(LPCTSTR lpName)
{
	for (int i = 0; i < m_GraphSeries.GetCount(); i++)
		if (_tcsicmp(lpName, ((GRAPHSERIES*)m_GraphSeries.GetAt(m_GraphSeries.FindIndex(i)))->lpName) == 0)
			return i;
	return -1;
}

GRAPHSERIES* CGraphCtrl::GetSerie(int index)
{
	if (index < m_GraphSeries.GetCount())
		return (GRAPHSERIES*)m_GraphSeries.GetAt(m_GraphSeries.FindIndex(index));
	else return NULL;
}

void CGraphCtrl::Draw(CDC* pDC)
{
	if (m_Width <= 0 || m_Height <= 0) return;

	Recenter();
//	if (m_IntelligentAxisScale) CalculateAxisScale();

	if (!pDC)
		pDC = GetDC();
	m_pBGScene->Render();

	m_pGraphScene->Render();
	if (m_GraphFieldBackGround.Type != GRAPH_FT_NONE)
		pDC->BitBlt(m_GraphRect.left, m_GraphRect.top, m_GraphRect.Width(), m_GraphRect.Height(), 
			m_GraphDIB.GetDC(), 0, 0, SRCCOPY);
	else G_TransparentBlt(*pDC, m_GraphRect.left, m_GraphRect.top, m_GraphRect.Width(), m_GraphRect.Height(), 
			*m_GraphDIB.GetDC(), 0, 0, m_GraphRect.Width(), m_GraphRect.Height(), TRANSPARENTCOLOR);

	if (m_ShowLegend)
	{
		m_pLegendScene->Render();
		if (m_LegendBackGround.Type != GRAPH_FT_NONE)
			pDC->BitBlt(m_LegendRect.left, m_LegendRect.top, m_LegendRect.Width(), m_LegendRect.Height(), 
				m_LegendDIB.GetDC(), 0, 0, SRCCOPY);
		else G_TransparentBlt(*pDC, m_LegendRect.left, m_LegendRect.top, m_LegendRect.Width(), m_LegendRect.Height(), 
				*m_LegendDIB.GetDC(), 0, 0, m_LegendRect.Width(), m_LegendRect.Height(), TRANSPARENTCOLOR);
	}

	DrawTitles();
	DrawText(pDC);
}

void CGraphCtrl::GetGraphFieldRect(CRect* lpRect)
{
	*lpRect = m_GraphRect;
}

void CGraphCtrl::GetLegendRect(CRect* lpRect)
{
	*lpRect = m_LegendRect;
}

void CGraphCtrl::ClearDrawTextData(void)
{
	while (m_DrawTextData.GetCount() > 0)
	{
		FONTDRAWDATA* lpData = (FONTDRAWDATA*)m_DrawTextData.RemoveHead();
		delete lpData;
	}
}

void CGraphCtrl::AddDrawTextData(FONTDRAWDATA* lpData)
{
	FONTDRAWDATA* data = new FONTDRAWDATA;
	memcpy(data, lpData, sizeof(FONTDRAWDATA));
	m_DrawTextData.AddTail(data);
}

void CGraphCtrl::DrawText(CDC* dc)
{
	int mode = dc->SetBkMode(TRANSPARENT);

	while (m_DrawTextData.GetCount() > 0)
	{
		FONTDRAWDATA* pData = (FONTDRAWDATA*)m_DrawTextData.RemoveHead();
		
		HFONT font = CreateFont(&pData->font, m_pPrintingDC ? m_pPrintingDC->m_hAttribDC : dc->m_hAttribDC);
		if (font)
		{
			CFont m_curFont; m_curFont.Attach(font);
			CFont* oldFont = dc->SelectObject(&m_curFont);
			COLORREF oldColor = dc->SetTextColor(pData->font.color);
			CString str = pData->text;
			CSize sz = GetTextExtent(str, &pData->font);
			CRect rect;
			UINT xFormat = 0, yFormat = 0;
			if (pData->left != 0)
			{
				xFormat = DT_LEFT;
				rect.left = pData->left;
				if (pData->right != 0)
					rect.right = pData->right;
				else rect.right = pData->left + sz.cx;
			}
			else if (pData->right != 0)
			{
				xFormat = DT_RIGHT;
				rect.right = pData->right;
				rect.left = pData->right - sz.cx;
			}
			else if (pData->centerx != 0)
			{
				xFormat = DT_CENTER;
				rect.left = pData->centerx - sz.cx / 2 - 1;
				rect.right = pData->centerx + sz.cx / 2 + 1;
			}
			
			if (pData->top != 0)
			{
				yFormat = DT_TOP;
				rect.top = pData->top;
				if (pData->bottom != 0)
					rect.bottom = pData->bottom;
				else rect.bottom = pData->top + sz.cy;
			}
			else if (pData->bottom != 0)
			{
				yFormat = DT_BOTTOM;
				rect.bottom = pData->bottom;
				rect.top = pData->bottom - sz.cy;
			}
			else if (pData->centery != 0)
			{
				yFormat = DT_VCENTER;
				rect.top = pData->centery - sz.cy / 2 - 1;
				rect.bottom = pData->centery + sz.cy / 2 + 1;
			}

			double a = DegreesToRadians(pData->font.angle);

			FONT tempFont(pData->font);
			tempFont.angle = 0;
			CSize _sz = GetTextExtent(pData->text, &tempFont);
			if (pData->font.angle % 360 == 0)
			{
				dc->SetTextAlign(TA_LEFT | TA_TOP);
				dc->DrawText(pData->text, (int)_tcslen(pData->text), rect, 
					xFormat | yFormat | DT_WORDBREAK | DT_END_ELLIPSIS | DT_WORD_ELLIPSIS);
			}
			else if (pData->font.angle == 90)
			{
				dc->SetTextAlign(TA_RIGHT | TA_BOTTOM);
				dc->ExtTextOut(rect.right, rect.top, ETO_CLIPPED, rect, pData->text, (int)_tcslen(pData->text), NULL);
			}
			else if (pData->font.angle == 180)
			{
				dc->SetTextAlign(TA_LEFT | TA_BOTTOM);
				dc->ExtTextOut(rect.right, rect.top, ETO_CLIPPED, rect, pData->text, (int)_tcslen(pData->text), NULL);
			}
			else if (pData->font.angle == 270)
			{
				dc->SetTextAlign(TA_RIGHT | TA_BOTTOM);
				dc->ExtTextOut(rect.left, rect.bottom, ETO_CLIPPED, rect, pData->text, (int)_tcslen(pData->text), NULL);
			}
			else if (pData->font.angle < 90)
			{
				double _cos = cos(a / 2);
				double _sin = sin(a / 2);

				double _x = _sz.cy * _sin;
				_x = floor(_x);
				double _y = _sz.cy * _sin;
				_y = floor(_y);

				int x = rect.left + rect.Width() / 2;
//				int y = rect.bottom - (int)fabs(_y);
//				int y = rect.top + (int)fabs(_y);;
				int y = rect.bottom - rect.Height() / 2;
				dc->SetTextAlign(TA_CENTER | TA_BASELINE | TA_NOUPDATECP);
				dc->ExtTextOut(x, y, /*ETO_CLIPPED*/0, rect, pData->text, (int)_tcslen(pData->text), NULL);
//				dc->ExtTextOut(rect.left + (int)fabs(floor(_sz.cy * cos(pData->font.angle / 2) / 2)), rect.bottom, /*ETO_CLIPPED*/0, rect, pData->text, (int)_tcslen(pData->text), NULL);
			}
			else /*if (pData->font.angle == 90)*/
			{
				dc->SetTextAlign(TA_LEFT | TA_TOP);
				dc->ExtTextOut(rect.left + (int)fabs(floor(_sz.cy * cos(a / 2) / 2)), rect.bottom, /*ETO_CLIPPED*/0, rect, pData->text, (int)_tcslen(pData->text), NULL);
//				dc->ExtTextOut(rect.left, (int)_round(rect.bottom - fabs(sz.cy * sin (pData->font.angle / 2)), 0), /*ETO_CLIPPED*/0, rect, pData->text, (int)_tcslen(pData->text), NULL);
			}
//				dc->ExtTextOut(rect.left, (int)_round(rect.bottom - sz.cy * sin (pData->font.angle / 2), 0), ETO_CLIPPED, rect, pData->text, (int)_tcslen(pData->text), NULL);
/*
				double pi = 3.141592654;
				double radian = pi * 2 / 360 * pData->font.angle;

				CPoint center;
				center.x = sz.cx / 2;
				center.y = sz.cy / 2;
					
				CPoint rcenter;
				rcenter.x = long(cos(radian) * center.x - sin(radian) * center.y);
				rcenter.y = long(sin(radian) * center.x + cos(radian) * center.y);
					
				dc->SetTextAlign(TA_BASELINE);
				dc->ExtTextOut(rect.left + rect.Width() / 2 - rcenter.x, 
								rect.top + rect.Height() / 2 + rcenter.y,
								ETO_CLIPPED, rect, pData->text, (int)_tcslen(pData->text), NULL);
*/
//			dc->DrawFocusRect(rect);
			dc->SetTextColor(oldColor);
			dc->SelectObject(oldFont);
			m_curFont.Detach();
			DeleteObject(font);
		}

		delete pData;
	}
	dc->SetBkMode(mode);
}

HFONT CGraphCtrl::CreateFont(FONT* lpFont, HDC dc)
{
	LOGFONT font;
	FONTtoLOGFONT(*lpFont, font, dc);
	return ::CreateFontIndirect(&font);
}

void CGraphCtrl::DrawMarker(GRAPH_MARKERSTYLE  style, int size, double x, double y)
{
	double xsize = CalcXDistance(size);
	double ysize = CalcYDistance(size);

	glLineWidth((GLfloat)CalcLineWidth((double)size/8));

	switch (style)
	{
	case MS_RHOMB:
	case MS_RHOMB_E:
		{
			if (style == MS_RHOMB_E)
				glBegin(GL_LINE_LOOP);
			else
				glBegin(GL_QUADS);
			glVertex2d(x - xsize / 2, y);
			glVertex2d(x, y - ysize / 2);
			glVertex2d(x + xsize / 2, y);
			glVertex2d(x, y + ysize / 2);
			glEnd();
		} break;
	case MS_SQUARE: 
	case MS_SQUARE_E: 
		{
			if (style == MS_SQUARE_E)
				glBegin(GL_LINE_LOOP);
			else
				glBegin(GL_QUADS);
			glVertex2d(x - xsize / 2, y - ysize / 2);
			glVertex2d(x + xsize / 2, y - ysize / 2);
			glVertex2d(x + xsize / 2, y + ysize / 2);
			glVertex2d(x - xsize / 2, y + ysize / 2);
			glEnd();
		} break;
	case MS_TRIANGLE: 
	case MS_TRIANGLE_E: 
		{
			if (style == MS_TRIANGLE_E)
				glBegin(GL_LINE_LOOP);
			else
				glBegin(GL_TRIANGLES);
			glVertex2d(x - xsize / 2, y + ysize / 2);
			glVertex2d(x, y - ysize / 2);
			glVertex2d(x + xsize / 2, y + ysize / 2);
			glEnd();
		} break;
	case MS_TRIANGLEDOWN: 
	case MS_TRIANGLEDOWN_E: 
		{
			if (style == MS_TRIANGLEDOWN_E)
				glBegin(GL_LINE_LOOP);
			else
				glBegin(GL_TRIANGLES);
			glVertex2d(x - xsize / 2, y - ysize / 2);
			glVertex2d(x, y + ysize / 2);
			glVertex2d(x + xsize / 2, y - ysize / 2);
			glEnd();
		} break;
	case MS_CIRCLE:	
	case MS_CIRCLE_E:	
		{
			glPushMatrix();
			GLUquadricObj* nobj = gluNewQuadric();
			if (style == MS_CIRCLE_E)
				gluQuadricDrawStyle(nobj, GLU_SILHOUETTE);
			else
				gluQuadricDrawStyle(nobj, GLU_FILL);
			if (m_AntiAlaising)
				gluQuadricNormals(nobj, GLU_NONE);
			else gluQuadricNormals(nobj, GLU_SMOOTH);
			glTranslated(x, y, 0);
			gluDisk(nobj, 0, (xsize < ysize ? xsize : ysize) / 2, 100, 1);
			gluDeleteQuadric(nobj);
			glPopMatrix();
/*
			glEnable(GL_POINT_SMOOTH);
			glPointSize((float)size); 
			glBegin(GL_POINTS); 
			glVertex2d(x, y);
			glEnd();
*/
		} break;
	case MS_STAR: 
	case MS_STAR_E: 
		{
			if (style == MS_STAR_E)
			{
				glBegin(GL_LINE_LOOP);
				for (int v = 0; v < 5; v++)
				{
					glVertex2d(x + xsize * 1.2 / 2 * sin(PI*v/2.5), y - ysize * 1.2 / 2 * cos(PI*v/2.5)); 
					glVertex2d(x + xsize * 1.2 / 5 * sin(PI*(v/2.5+0.2)), y - ysize * 1.2 / 5 * cos(PI*(v/2.5+0.2))); 
				}
			}
			else
			{
				glBegin(GL_TRIANGLES);
				for (double v = 0; v < 5; v++)
				{
					glVertex2d(x + xsize * 1.2 / 2 * sin((double)PI*v/2.5), y - ysize * 1.2 / 2 * cos((double)PI*v/2.5)); 
					glVertex2d(x + xsize * 1.2 / 5 * sin((double)PI*(v/2.5+0.6)), y - ysize * 1.2 / 5 * cos((double)PI*(v/2.5+0.6))); 
					glVertex2d(x + xsize * 1.2 / 5 * sin((double)PI*(v/2.5-0.6)), y - ysize * 1.2 / 5 * cos((double)PI*(v/2.5-0.6))); 
				}
			}
			//glVertex2d(x - (double)size / 2, y + (double)size / 2);
			//glVertex2d(x, y - (double)size / 2);
			//glVertex2d(x + (double)size / 4, y);
			//glVertex2d(x - (double)size / 2, y - (double)size * 0.16);
			//glVertex2d(x + (double)size / 2, y - (double)size * 0.16);
			//glVertex2d(x, y + (double)size * 0.16);
			//glVertex2d(x + (double)size / 2, y + (double)size / 2);
			//glVertex2d(x - (double)size / 2, y - (double)size * 0.16);
			//glVertex2d(x + (double)size * 0.16, y - (double)size * 0.16);
			glEnd();
		} break;
	case MS_PLUS: 
		{
			glBegin(GL_LINES);
			glVertex2d(x - xsize / 2, y);
			glVertex2d(x + xsize / 2, y);
			glVertex2d(x, y - ysize / 2);
			glVertex2d(x, y + ysize / 2);
			glEnd();
		} break;
	case MS_X: 
		{
			glBegin(GL_LINES);
			glVertex2d(x - xsize / 2, y - ysize / 2);
			glVertex2d(x + xsize / 2, y + ysize / 2);
			glVertex2d(x + xsize / 2, y - ysize / 2);
			glVertex2d(x - xsize / 2, y + ysize / 2);
			glEnd();
		} break;
	case MS_ARROWUP: 
		{
			glBegin(GL_TRIANGLES);
			glVertex2d(x - xsize / 2, y);
			glVertex2d(x, y - ysize / 2);
			glVertex2d(x + xsize / 2, y);
			glEnd();
			glBegin(GL_LINES);
			glVertex2d(x, y);
			glVertex2d(x, y + ysize / 2);
			glEnd();
		} break;
	case MS_ARROWDOWN: 
		{
			glBegin(GL_TRIANGLES);
			glVertex2d(x + xsize / 2, y);
			glVertex2d(x, y + ysize / 2);
			glVertex2d(x - xsize / 2, y);
			glEnd();
			glBegin(GL_LINES);
			glVertex2d(x, y);
			glVertex2d(x, y - ysize / 2);
			glEnd();
		} break;
	}
	glDisable(GL_POINT_SMOOTH);
}

GRAPH_GRAPHCONTROL CGraphCtrl::GetControlForPoint(CPoint point, CPoint* localPoint)
{
	CRect graphRect, legendRect;

	GetLegendRect(&legendRect);
	if (m_ShowLegend && legendRect.PtInRect(point))
	{
		if (localPoint)
			*localPoint = point, localPoint->Offset(-legendRect.left, -legendRect.top);
		return GC_LEGEND;
	}

	GetGraphFieldRect(&graphRect);
	if (graphRect.PtInRect(point))
	{
		if (localPoint)
			*localPoint = point, localPoint->Offset(-graphRect.left, -graphRect.top);
		return GC_GRAPH;
	}
	
	if (point.x >= 0 && point.y >= 0 && point.x <= m_Width && point.y <= m_Height)
	{
		return GC_BACKGROUND;
	}
	else return GC_NONE;
}

void CGraphCtrl::Zoom(double zoomXFactor, double zoomYFactor, CPoint center)
{
	GRAPHPOINT grCenter;
	ScreenToGraphPoint(center, &grCenter, false);
	m_CurrentXZoomFactor *= zoomXFactor;
	m_CurrentYZoomFactor *= zoomYFactor;
	if (m_CurrentXZoomFactor < m_pGraph->m_MinZoomFactor) m_CurrentXZoomFactor = m_pGraph->m_MinZoomFactor;
	if (m_CurrentXZoomFactor > m_pGraph->m_MaxZoomFactor) m_CurrentXZoomFactor = m_pGraph->m_MaxZoomFactor;

	if (m_CurrentYZoomFactor < m_pGraph->m_MinZoomFactor) m_CurrentYZoomFactor = m_pGraph->m_MinZoomFactor;
	if (m_CurrentYZoomFactor > m_pGraph->m_MaxZoomFactor) m_CurrentYZoomFactor = m_pGraph->m_MaxZoomFactor;
	SetGraphCenter(grCenter);
}

void CGraphCtrl::Prepare(void)
{
	SetGraphCenter(GRAPHPOINT(m_GraphXSize.min + (m_GraphXSize.max - m_GraphXSize.min) / 2, 
		m_GraphYSize.min + (m_GraphYSize.max - m_GraphYSize.min) / 2));

	m_Prepared = TRUE;
}

void CGraphCtrl::GraphToScreenPoint(GRAPHPOINT lpGraphPt, CPoint* lpScreenPt)
{
	CRect rect;
	GetGraphFieldRect(&rect);
	GRAPHPOINT grCenter, size;
	GetGraphCenter(&grCenter);
	GetSize(&size);

	double screenXSize = rect.Width();
	double screenYSize = rect.Height();
	GRAPHPOINT screenCenter(screenXSize / 2, screenYSize / 2);
	lpScreenPt->x = (lpGraphPt.x > grCenter.x) ? 
		(int)(screenCenter.x + (lpGraphPt.x - grCenter.x) * screenXSize / size.x * m_CurrentXZoomFactor) :
		(int)(screenCenter.x - (grCenter.x - lpGraphPt.x) * screenXSize / size.x * m_CurrentXZoomFactor);
	lpScreenPt->y = (lpGraphPt.y > grCenter.y) ? 
		(int)(screenCenter.y - (lpGraphPt.y - grCenter.y) * screenYSize / size.y * m_CurrentYZoomFactor) :
		(int)(screenCenter.y + (grCenter.y - lpGraphPt.y) * screenYSize / size.y * m_CurrentYZoomFactor);
}

void CGraphCtrl::GraphToScreenRect(GRAPHPOINT lpLeftTopPt, GRAPHPOINT lpRightBottom, CRect* lpScreenRect)
{
	CPoint lefttop, rightbottom;
	GraphToScreenPoint(lpLeftTopPt, &lefttop);    
	GraphToScreenPoint(lpRightBottom, &rightbottom);
	lpScreenRect->SetRect(lefttop, rightbottom);
}

void CGraphCtrl::GraphToScreenSize(GRAPHPOINT lpSize, CSize* lpScreenSize)
{
	CRect rect;
	GetGraphFieldRect(&rect);
	GRAPHPOINT size;
	GetSize(&size);

	double screenXSize = rect.Width();
	double screenYSize = rect.Height();

	lpScreenSize->cx = (int)(lpSize.x * screenXSize / size.x * m_CurrentXZoomFactor);
	lpScreenSize->cy = (int)(lpSize.y * screenYSize / size.y * m_CurrentYZoomFactor);
}

void CGraphCtrl::ScreenToGraphPoint(CPoint lpScreenPt, GRAPHPOINT* lpGraphPt, bool bCalcLog)
{
	CRect rect;
	GetGraphFieldRect(&rect);
	GRAPHPOINT grCenter, size;
	GetGraphCenter(&grCenter);
	GetSize(&size);

	double screenXSize = rect.Width();
	double screenYSize = rect.Height();
	GRAPHPOINT screenCenter(screenXSize / 2, screenYSize / 2);
	lpGraphPt->x = (lpScreenPt.x > screenCenter.x) ?
		grCenter.x + ((double)lpScreenPt.x - screenCenter.x) * size.x / screenXSize / m_CurrentXZoomFactor :
		grCenter.x - (screenCenter.x - (double)lpScreenPt.x) * size.x / screenXSize / m_CurrentXZoomFactor;
	lpGraphPt->y = (lpScreenPt.y > screenCenter.y) ?
		grCenter.y - ((double)lpScreenPt.y - screenCenter.y) * size.y / screenYSize / m_CurrentYZoomFactor :
		grCenter.y + (screenCenter.y - (double)lpScreenPt.y) * size.y / screenYSize / m_CurrentYZoomFactor;
/* DVM fixed status display for log
	if (m_LogX&&bCalcLog)
		lpGraphPt->x = pow(10.0, lpGraphPt->x);
	if (m_LogY&&bCalcLog)
		lpGraphPt->y = pow(10.0, lpGraphPt->y);
*/
}

void CGraphCtrl::ScreenToGraphRect(CRect lpScreenRect, GRAPHPOINT* lpLeftTopPt, GRAPHPOINT* lpRightBottom)
{
	ScreenToGraphPoint(lpScreenRect.TopLeft(), lpLeftTopPt);
	ScreenToGraphPoint(lpScreenRect.BottomRight(), lpRightBottom);
}

void CGraphCtrl::SetGraphCenter(GRAPHPOINT pt, bool bCalcGridSizes)
{
	m_GraphCenter = pt;
	CalculateAxisScale();
	if (m_CurrentXZoomFactor == 1 && m_CurrentYZoomFactor == 1 && (m_StableXGridCount != 0 || m_StableYGridCount != 0)) CalculateGridSizes();

	GRAPHPOINT visibleSize;
	GetVisibleSize(&visibleSize);

	if (m_GraphCenter.x - visibleSize.x / 2 < m_GraphXSize.min) 
		m_GraphCenter.x = m_GraphXSize.min + visibleSize.x / 2;
	if (m_GraphCenter.x + visibleSize.x / 2 > m_GraphXSize.max) 
		m_GraphCenter.x = m_GraphXSize.max - visibleSize.x / 2;

	if (m_GraphCenter.y - visibleSize.y / 2 < m_GraphYSize.min) 
		m_GraphCenter.y = m_GraphYSize.min + visibleSize.y / 2;
	if (m_GraphCenter.y + visibleSize.y / 2 > m_GraphYSize.max)
		m_GraphCenter.y = m_GraphYSize.max - visibleSize.y / 2;

	if ((m_CurrentXZoomFactor != 1 || m_CurrentYZoomFactor != 1) && (m_StableXGridCount != 0 || m_StableYGridCount != 0) && bCalcGridSizes)
		CalculateGridSizes();

  // BUG WITH CENTERING
	if ((m_CurrentXZoomFactor != 1 || m_CurrentYZoomFactor != 1) && m_IntelligentAxisScale && !m_pGraph->GetView()->IsPanning())
	{
		double startX = GetStartX();
		double startY = GetStartY();
		double endX = GetEndX();
		double endY = GetEndY();

		double exp = 0;
		if (compare(startX, 0) != 0)
			exp = floor(log10(fabs(startX)));
		else exp = floor(log10(fabs(endX)));
		double newStartX = _round(startX / pow(10.0, exp), 1) * pow(10.0, exp);


		if (compare(startY, 0) != 0)
			exp = floor(log10(fabs(startY)));
		else exp = floor(log10(fabs(endY)));
		double newStartY = _round(startY / pow(10.0, exp), 1) * pow(10.0, exp);

		m_GraphCenter.x += newStartX - startX;
		m_GraphCenter.y += newStartY - startY;
	}
}

void CGraphCtrl::GetGraphCenter(GRAPHPOINT* pGraphCenter)
{
	*pGraphCenter = m_GraphCenter;
}

void CGraphCtrl::GetVisibleSize(GRAPHPOINT* lpSize)
{
	if (!lpSize) return;

	GRAPHPOINT size;
	GetSize(&size);

	lpSize->x = size.x / m_CurrentXZoomFactor;
	lpSize->y = size.y / m_CurrentYZoomFactor;
}

void CGraphCtrl::GetSize(GRAPHPOINT* lpSize)
{
	if (!lpSize) return;

	lpSize->x = m_GraphXSize.max - m_GraphXSize.min;
	lpSize->y = m_GraphYSize.max - m_GraphYSize.min;
}

CDC* CGraphCtrl::GetDC(void)
{
//	return m_pPrintingDC ? m_pPrintingDC : m_DIB.GetDC();
	return m_DIB.GetDC();
}

void CGraphCtrl::Resize(int cx, int cy)
{
	m_Width = cx; m_Height = cy;

	if (m_Width <= 0 || m_Height <= 0)
		return;

	if (m_DefaultGraphRect.IsRectNull())
		m_DefaultGraphRect.SetRect(0, 0, cx, cy);

	// ORDER SHOULD BE STABLE
	m_BGRect.SetRect(0, 0, cx, cy);	
	m_DIB.CheckDIBSize(m_BGRect.Width(), m_BGRect.Height(), 24);

	RecalcPositions();

	m_GraphDIB.CheckDIBSize(m_GraphRect.Width(), m_GraphRect.Height(), 24);
	if (m_ShowLegend)
		m_LegendDIB.CheckDIBSize(m_LegendRect.Width(), m_LegendRect.Height(), 24);

	m_pBGScene->Destroy();
	m_pBGScene->Create(GetDC());
	m_pBGScene->Init();
	m_pBGScene->Resize(m_BGRect.Width(), m_BGRect.Height());

	m_pGraphScene->Destroy();
	m_pGraphScene->Create(m_GraphDIB.GetDC());
	m_pGraphScene->Init();
	m_pGraphScene->Resize(m_GraphRect.Width(), m_GraphRect.Height());

	if (m_ShowLegend)
	{
		m_pLegendScene->Destroy();
		m_pLegendScene->Create(m_LegendDIB.GetDC());
		m_pLegendScene->Init();
		m_pLegendScene->Resize(m_LegendRect.Width(), m_LegendRect.Height());
	}
}

void CGraphCtrl::MoveGraph(double cx, double cy)
{
	CRect rect;
	GetGraphFieldRect(&rect);
	GRAPHPOINT grCenter, size;
	GetGraphCenter(&grCenter);
	GetSize(&size);
	double screenXSize = rect.Width();
	double screenYSize = rect.Height();
	grCenter.x += cx * size.x / screenXSize / m_CurrentXZoomFactor;
	grCenter.y -= cy * size.y / screenYSize / m_CurrentYZoomFactor;
	SetGraphCenter(grCenter);
}

void CGraphCtrl::CalcTitleRect()
{
	m_TitleRect = m_BGRect;
	m_TitleRect.left += CalcXDistance(m_LeftBorder);
	m_TitleRect.right -= CalcXDistance(m_RightBorder);
	m_TitleRect.top += CalcYDistance(m_TopBorder);
	if (m_ShowTitle && !m_Title.IsEmpty())
	{
		CSize sz = GetTextExtent(m_Title, &m_TitleFont, m_TitleRect.Width());
		m_TitleRect.bottom = m_TitleRect.top + sz.cy;
	}
	else m_TitleRect.bottom = m_TitleRect.top;
	m_TitleRect.NormalizeRect();
}

void CGraphCtrl::CalcSubTitleRect()
{
	m_SubTitleRect = m_TitleRect;
	if (m_ShowSubTitle && !m_SubTitle.IsEmpty())
	{
		m_SubTitleRect.top = m_SubTitleRect.bottom + CalcYDistance(LABELSTOBORDER);
		CSize sz = GetTextExtent(m_SubTitle, &m_SubTitleFont, m_SubTitleRect.Width());
		m_SubTitleRect.bottom = m_SubTitleRect.top + sz.cy;
	}
	else m_SubTitleRect.top = m_SubTitleRect.bottom;
	m_SubTitleRect.NormalizeRect();
}

void CGraphCtrl::CalcLegendRect()
{
	if (!m_bAutoLegendPos)
	{
		if (m_pPrintingDC && (m_LegendRect.EqualRect(m_BackupLegendRect) || m_BackupLegendRect.IsRectEmpty()))
		{
			m_BackupLegendRect = m_LegendRect;
			m_LegendRect.left = CalcXDistance(m_LegendRect.left);
			m_LegendRect.right = CalcXDistance(m_LegendRect.right);
			m_LegendRect.top = CalcYDistance(m_LegendRect.top);
			m_LegendRect.bottom = CalcYDistance(m_LegendRect.bottom);
		}
		else if (!m_BackupLegendRect.IsRectEmpty())
		{
			m_LegendRect = m_BackupLegendRect;
			m_BackupLegendRect = CRect(0, 0, 0, 0);
		}

		return;
	}
	if (m_ShowLegend)
	{
		int maxWidth = m_BGRect.Width() / 3;
		int nHeight = CalcYDistance(LABELSTOBORDER) + (int)m_GraphBorderWidth, nWidth = 0;
		int spacesWidth = CalcXDistance(LABELSTOBORDER) * 3 + (int)m_GraphBorderWidth * 2;
		for (int i = 0; i < GetSeriesCount(); i++)
		{
			GRAPHSERIES* pSerie = GetSerie(i);
			if (!pSerie->visible || !pSerie->legend) continue;
			CSize sz = GetTextExtent(pSerie->lpName, &m_LegendFont);
			if (sz.cy < CalcYDistance(pSerie->markersize)) sz.cy = CalcYDistance(pSerie->markersize);
			if (sz.cx + spacesWidth + CalcXDistance(pSerie->markersize) * 3 <= maxWidth)
			{
				if (nWidth < sz.cx + spacesWidth + CalcXDistance(pSerie->markersize) * 3)
					nWidth = sz.cx + spacesWidth + CalcXDistance(pSerie->markersize) * 3;
				nHeight += sz.cy;
			}
			else
			{
				double coef = ceil((double)sz.cx / (double)(maxWidth - spacesWidth - CalcXDistance(pSerie->markersize) * 3));
				nHeight += (int)((double)sz.cy * coef);
				nWidth = maxWidth;
			}
			nHeight += CalcYDistance(LABELSTOBORDER);
		}
		nHeight += (int)m_GraphBorderWidth;
		if (nWidth == 0) nWidth = spacesWidth;
		m_LegendRect.top = m_SubTitleRect.bottom + CalcYDistance(LABELSTOBORDER);
		m_LegendRect.right = m_BGRect.right - CalcXDistance(m_RightBorder);
		m_LegendRect.left = m_LegendRect.right - nWidth;
		m_LegendRect.bottom = m_LegendRect.top + nHeight;
		if (m_LegendRect.bottom > m_BGRect.bottom - CalcYDistance(m_BottomBorder))
			m_LegendRect.bottom = m_BGRect.bottom - CalcYDistance(m_BottomBorder);
	}
	else
	{
		m_LegendRect.top = m_SubTitleRect.bottom + CalcYDistance(LABELSTOBORDER);
		m_LegendRect.right = m_BGRect.right - CalcXDistance(m_RightBorder);
		m_LegendRect.left = m_LegendRect.right;
		m_LegendRect.bottom = m_LegendRect.top;
	}
	m_LegendRect.NormalizeRect();
}

void CGraphCtrl::CalcXLabelRect()
{
	m_XLabelRect = m_BGRect;
	m_XLabelRect.left = m_YTicksLabelRect.right + CalcXDistance(LABELSTOBORDER);
	m_XLabelRect.right = GetGraphFieldRightPos();
	m_XLabelRect.bottom -= CalcYDistance(m_BottomBorder);
	if (m_ShowXLabel && !m_XLabel.IsEmpty())
	{
		CSize sz = GetTextExtent(m_XLabel, &m_XLabelFont, m_XLabelRect.Width());
		m_XLabelRect.top = m_XLabelRect.bottom - sz.cy;
	}
	else m_XLabelRect.top = m_XLabelRect.bottom;
	m_XLabelRect.NormalizeRect();
}

void CGraphCtrl::CalcYLabelRect()
{
	m_YLabelRect = m_BGRect;
	m_YLabelRect.top = m_SubTitleRect.bottom + CalcYDistance(LABELSTOBORDER);
	m_YLabelRect.bottom -= CalcYDistance(m_BottomBorder);
	m_YLabelRect.left = CalcXDistance(m_LeftBorder);
	if (m_ShowYLabel && !m_YLabel.IsEmpty())
	{
		CSize sz = GetTextExtent(m_YLabel, &m_YLabelFont, m_YLabelRect.Height());
		m_YLabelRect.right = m_YLabelRect.left + sz.cx;
	}
	else m_YLabelRect.right = m_YLabelRect.left;
	m_YLabelRect.NormalizeRect();
}

void CGraphCtrl::CalcXTicksLabelRect()
{
	m_XTicksLabelRect = m_XLabelRect;
	if (m_ShowXTicks || m_ShowXSubTicks)
	{
		m_XTicksLabelRect.bottom = m_XTicksLabelRect.top - CalcYDistance(LABELSTOBORDER);
		double maxV = GetMaxXValue(); 
		double minV = GetMinXValue();
		CString maxStr = GetXTicksLabelText(maxV != MINDOUBLE ? maxV : 0);
		CString minStr = GetXTicksLabelText(minV != MAXDOUBLE ? minV : 0);
		CSize sz = GetTextExtent(maxStr.GetLength() > minStr.GetLength() ? maxStr : minStr, &m_TicksXLabelsFont);
		m_XTicksLabelRect.top = m_XTicksLabelRect.bottom - sz.cy;
	}
	else m_XTicksLabelRect.bottom = m_XTicksLabelRect.top;
	m_XTicksLabelRect.NormalizeRect();
}

void CGraphCtrl::CalcYTicksLabelRect()
{
	m_YTicksLabelRect = m_YLabelRect;
	if (m_ShowYTicks || m_ShowYSubTicks)
	{
		m_YTicksLabelRect.left = m_YTicksLabelRect.right + CalcXDistance(LABELSTOBORDER);
		double maxV = GetMaxYValue(); 
		double minV = GetMinYValue();
		CString maxStr = GetYTicksLabelText(maxV != MINDOUBLE ? maxV : 0);
		CString minStr = GetYTicksLabelText(minV != MAXDOUBLE ? minV : 0);
		CSize sz = GetTextExtent(maxStr.GetLength() > minStr.GetLength() ? maxStr : minStr, &m_TicksYLabelsFont);
		m_YTicksLabelRect.right = m_YTicksLabelRect.left + sz.cx;
	}
	else m_YTicksLabelRect.left = m_YTicksLabelRect.right;
	m_YTicksLabelRect.NormalizeRect();
}

void CGraphCtrl::CalcGraphFieldRect()
{
	m_GraphRect.left = m_YTicksLabelRect.right + CalcXDistance(LABELSTOBORDER);
	m_GraphRect.top = m_SubTitleRect.bottom + CalcYDistance(LABELSTOBORDER);
	m_GraphRect.bottom = m_XTicksLabelRect.top - CalcYDistance(LABELSTOBORDER);
	m_GraphRect.right = GetGraphFieldRightPos();
	m_GraphRect.NormalizeRect();
}

CSize CGraphCtrl::GetTextExtent(LPCTSTR lpText, FONT* lpFont, int maxWidth)
{
	CDC* pDC = m_pPrintingDC ? m_pPrintingDC : GetDC();
	if (!pDC) return CSize(0, 0);
	CSize sz;
	FONT tempFont(*lpFont);
	tempFont.angle = 0;
	HFONT font = CreateFont(&tempFont, pDC->m_hAttribDC);
	if (font)
	{
		CFont m_curFont; m_curFont.Attach(font);
		CFont* oldFont = pDC->SelectObject(&m_curFont);
		if (maxWidth != 0)
		{
			CRect rect(0, 0, maxWidth, 0);
			pDC->DrawText(lpText, (int)_tcslen(lpText), rect, DT_TOP | DT_LEFT | DT_WORDBREAK | DT_CALCRECT);
			sz.cx = rect.Width();
			sz.cy = rect.Height();
		}
		else sz = pDC->GetTextExtent(lpText);
		pDC->SelectObject(oldFont);
		m_curFont.Detach();
		DeleteObject(font);
	}

	if (sz != CSize(0, 0) && lpFont->angle % 180 != 0)
	{
		int x = sz.cx;
		int y = sz.cy;
		if (lpFont->angle % 90 == 0)
		{
			sz.cx = y;
			sz.cy = x;
		}
		else
		{	
			double a = 0;
			if (lpFont->angle < 90)
				a = lpFont->angle;
			else if (lpFont->angle < 180)
				a = 180 - lpFont->angle;
			else if (lpFont->angle < 270)
				a = lpFont->angle - 180;
			else a = 360 - lpFont->angle;

			a = DegreesToRadians(a);

			sz.cx = (int)fabs(_round(x * cos(a / 2) + y * sin( a / 2), 0));
			sz.cy = (int)fabs(_round(y * cos(a / 2) + x * sin( a / 2), 0));
		}
	}

	return sz;
}

double CGraphCtrl::GetMinXValue(void)
{
	double res = MAXDOUBLE;

	for (int serie = 0; serie < GetSeriesCount(); serie++)
	{
		GRAPHSERIES* lpSerie = GetSerie(serie);
		for (int i = 0; i < lpSerie->count; i++)
		{
			if (m_LogX)
			{
				if (lpSerie->pData[i].x <= 0)
					continue;
				if (_log10_nice(lpSerie->pData[i].x) < res)
					res = _log10_nice(lpSerie->pData[i].x);
			}
			else
			{
				if (lpSerie->pData[i].x < res)
					res = lpSerie->pData[i].x;
			}
		}
	}

	return res;
}

double CGraphCtrl::GetMaxXValue(void)
{
	double res = MINDOUBLE;

	for (int serie = 0; serie < GetSeriesCount(); serie++)
	{
		GRAPHSERIES* lpSerie = GetSerie(serie);
		for (int i = 0; i < lpSerie->count; i++)
		{
			if (m_LogX)
			{
				if (lpSerie->pData[i].x <= 0)
					continue;
				if (_log10_nice(lpSerie->pData[i].x) > res)
					res = _log10_nice(lpSerie->pData[i].x);
			}
			else
			{
				if (lpSerie->pData[i].x > res)
					res = lpSerie->pData[i].x;
			}
		}
	}

	return res;
}

double CGraphCtrl::GetMinYValue(void)
{
	double res = MAXDOUBLE;

	for (int serie = 0; serie < GetSeriesCount(); serie++)
	{
		GRAPHSERIES* lpSerie = GetSerie(serie);
		for (int i = 0; i < lpSerie->count; i++)
		{
			if (m_LogY)
			{
				if (lpSerie->pData[i].y <= 0)
					continue;
				if (_log10_nice(lpSerie->pData[i].y) < res)
					res = _log10_nice(lpSerie->pData[i].y);
			}
			else
			{
				if (lpSerie->pData[i].y < res)
					res = lpSerie->pData[i].y;
			}
		}
	}

	return res;
}

double CGraphCtrl::GetMaxYValue(void)
{
	double res = MINDOUBLE;

	for (int serie = 0; serie < GetSeriesCount(); serie++)
	{
		GRAPHSERIES* lpSerie = GetSerie(serie);
		for (int i = 0; i < lpSerie->count; i++)
		{
			if (m_LogY)
			{
				if (lpSerie->pData[i].y <= 0)
					continue;
				if (_log10_nice(lpSerie->pData[i].y) > res)
					res = _log10_nice(lpSerie->pData[i].y);
			}
			else
			{
				if (lpSerie->pData[i].y > res)
					res = lpSerie->pData[i].y;
			}
		}
	}

	return res;
}

void CGraphCtrl::DrawTitles(void)
{
	if (m_ShowTitle)
	{
		FONTDRAWDATA fontData;
		memset(&fontData, 0, sizeof(FONTDRAWDATA));
		fontData.top = m_TitleRect.top;
		fontData.bottom = m_TitleRect.bottom;
		fontData.centerx = m_TitleRect.left + m_TitleRect.Width() / 2;
		fontData.dc = GetDC();
		_tcscpy(fontData.text, m_Title.GetBuffer(0));
		memcpy(&fontData.font, &m_TitleFont, sizeof(FONT));
		AddDrawTextData(&fontData);
	}
	if (m_ShowSubTitle)
	{
		FONTDRAWDATA fontData;
		memset(&fontData, 0, sizeof(FONTDRAWDATA));
		fontData.top = m_SubTitleRect.top;
		fontData.bottom = m_SubTitleRect.bottom;
		fontData.centerx = m_SubTitleRect.left + m_SubTitleRect.Width() / 2;
		fontData.dc = GetDC();
		_tcscpy(fontData.text, m_SubTitle.GetBuffer(0));
		memcpy(&fontData.font, &m_SubTitleFont, sizeof(FONT));
		AddDrawTextData(&fontData);
	}
	if (m_ShowXLabel)
	{
		FONTDRAWDATA fontData;
		memset(&fontData, 0, sizeof(FONTDRAWDATA));
		fontData.top = m_XLabelRect.top;
		fontData.bottom = m_XLabelRect.bottom;
		fontData.centerx = m_XLabelRect.left + m_XLabelRect.Width() / 2;
		fontData.dc = GetDC();
		_tcscpy(fontData.text, m_XLabel.GetBuffer(0));
		memcpy(&fontData.font, &m_XLabelFont, sizeof(FONT));
		AddDrawTextData(&fontData);
	}
	if (m_ShowYLabel)
	{
		FONTDRAWDATA fontData;
		memset(&fontData, 0, sizeof(FONTDRAWDATA));
		fontData.centery = m_YLabelRect.top + m_YLabelRect.Height() / 2;
		fontData.left = m_YLabelRect.left;
		fontData.right = m_YLabelRect.right;
		fontData.dc = GetDC();
		_tcscpy(fontData.text, m_YLabel.GetBuffer(0));
		memcpy(&fontData.font, &m_YLabelFont, sizeof(FONT));
		AddDrawTextData(&fontData);
	}
}

void CGraphCtrl::CalculateGridSizes(void)
{
	if (m_IntelligentAxisScale)
	{
		GRAPHPOINT pt;
		GetSize(&pt);

		GRAPHPOINT zoomChange(1, 1);

		if (!m_LogX && m_StableXGridCount > 0)
			m_GridXSize = (pt.x / m_CurrentXZoomFactor) / m_StableXGridCount;
		else if (!m_LogX && m_StableXGridCount == -1)
		{
			if (m_CurrentXZoomFactor == 1)
				m_GridXSize = CalculateGridSize(m_GraphXSize.min, m_GraphXSize.max, m_DefaultXGridSize, m_GraphRect.Width(), m_GridXChanging, m_GraphXChanging);
			else
			{
				GRAPHPOINT vSize;
				GetVisibleSize(&vSize);
				vSize.y = 0;
				GRAPHPOINT oldSize = vSize;
				m_GridXSize = CalculateGridSize(vSize.y, vSize.x, m_DefaultXGridSize, m_GraphRect.Width(), m_GridXChanging, m_GraphXChanging);
				zoomChange.x = vSize.x / oldSize.x;
			}
		}
		if (!m_LogY && m_StableYGridCount > 0)
			m_GridYSize = (pt.y / m_CurrentYZoomFactor) / m_StableYGridCount;
		else if (!m_LogY && m_StableYGridCount == -1)
		{
			if (m_CurrentYZoomFactor == 1)
				m_GridYSize = CalculateGridSize(m_GraphYSize.min, m_GraphYSize.max, m_DefaultYGridSize, m_GraphRect.Height(), m_GridYChanging, m_GraphYChanging);
			else
			{
				GRAPHPOINT vSize;
				GetVisibleSize(&vSize);
				vSize.x = 0;
				GRAPHPOINT oldSize = vSize;
				m_GridYSize = CalculateGridSize(vSize.x, vSize.y, m_DefaultYGridSize, m_GraphRect.Height(), m_GridYChanging, m_GraphYChanging);
				zoomChange.y = vSize.y / oldSize.y;
			}
		}
		double zoomFactor = max(zoomChange.x, zoomChange.y); // CHECK IT FOR LOG
		if (!m_LogX && !m_LogY && compare(zoomFactor, 1) == 1)
		{
			m_CurrentXZoomFactor /= zoomChange.x;
			m_CurrentYZoomFactor /= zoomChange.y;
			SetGraphCenter(m_GraphCenter, false); 
		}
	}
	else
	{
		m_GridXSize = ((m_GraphXSize.max - m_GraphXSize.min) / m_Width * m_DefaultXGridSize) / m_CurrentXZoomFactor;
		m_GridYSize = ((m_GraphYSize.max - m_GraphYSize.min) / m_Height * m_DefaultYGridSize) / m_CurrentYZoomFactor;
	}
	if (m_LogX)
		m_GridXSize = 1;
	if (m_LogY)
		m_GridYSize = 1;

	if (m_SubXGridsCount <= 0)
		m_SubGridXSize = 0;
	else if (m_SubXGridsCount > 0)
		m_SubGridXSize = m_GridXSize / m_SubXGridsCount;

	if (m_SubYGridsCount <= 0)
		m_SubGridYSize = 0;
	else if (m_SubYGridsCount > 0)
		m_SubGridYSize = m_GridYSize / m_SubYGridsCount;

	GRAPHSIZE x, y;
	GetGraphDataLimits(&x, &y);
	if (compare(0, m_GridXSize) < 0)
	{
		while (compare(m_GraphXSize.min + m_AdditionalGridSteps.left * m_GridXSize, x.min) > 0)
			m_GraphXSize.min -= m_GridXSize;

		while (compare(m_GraphXSize.max - m_AdditionalGridSteps.right * m_GridXSize, x.max) < 0)
			m_GraphXSize.max += m_GridXSize;
	}

	if (compare(0, m_GridYSize) < 0)
	{
		while (compare(m_GraphYSize.min + m_AdditionalGridSteps.bottom * m_GridYSize, y.min) > 0)
			m_GraphYSize.min -= m_GridYSize;

		while (compare(m_GraphYSize.max - m_AdditionalGridSteps.top * m_GridYSize, y.max) < 0)
			m_GraphYSize.max += m_GridYSize;
	}
}

void CGraphCtrl::RecalcPositions(void)
{
	CalcTitleRect();				
	CalcSubTitleRect();				
	CalcLegendRect();				
	CalcYLabelRect();				
	CalcYTicksLabelRect();			
	CalcXLabelRect();				
	CalcXTicksLabelRect();			
	CalcGraphFieldRect();			
	m_YLabelRect.bottom = m_XTicksLabelRect.top - CalcYDistance(LABELSTOBORDER);
	m_YLabelRect.NormalizeRect();
	m_TitleRect.right = GetGraphFieldRightPos();
	m_TitleRect.left = m_GraphRect.left;
	m_TitleRect.NormalizeRect();
	m_SubTitleRect.right = GetGraphFieldRightPos();
	m_SubTitleRect.left = m_GraphRect.left;
	m_SubTitleRect.NormalizeRect();
}

void CGraphCtrl::ResetPreset(void)
{
	m_BackGround = m_BackGround_Default;
	m_AntiAlaising = m_AntiAlaising_Default;
	m_GraphBorderWidth = m_GraphBorderWidth_Default;
	m_ShowTitle = m_ShowTitle_Default;
	m_ShowSubTitle = m_ShowSubTitle_Default;
	m_ShowXLabel = m_ShowXLabel_Default;
	m_ShowYLabel = m_ShowYLabel_Default;
	m_ShowXTicksLabels = m_ShowXTicksLabels_Default;
	m_ShowYTicksLabels = m_ShowYTicksLabels_Default;
	m_ShowXSubTicksLabels = m_ShowXSubTicksLabels_Default;
	m_ShowYSubTicksLabels = m_ShowYSubTicksLabels_Default;
	m_DrawZeroLabels = m_DrawZeroLabels_Default;
	m_ShowGrid = m_ShowGrid_Default;
	m_GridXSize = m_GridXSize_Default;
	m_GridYSize = m_GridYSize_Default;
	m_ShowXTicks = m_ShowXTicks_Default;
	m_ShowYTicks = m_ShowYTicks_Default;
	m_GridColor = m_GridColor_Default;
	m_StableXGridCount = m_StableXGridCount_Default;
	m_StableYGridCount = m_StableYGridCount_Default;
	m_ShowSubGrid = m_ShowSubGrid_Default;
	m_SubGridXSize = m_SubGridXSize_Default;
	m_SubGridYSize = m_SubGridYSize_Default;
	m_ShowXSubTicks = m_ShowXSubTicks_Default;
	m_ShowYSubTicks = m_ShowYSubTicks_Default;
	m_SubGridColor = m_SubGridColor_Default;
	m_ShowLegend = m_ShowLegend_Default;
	m_LegendBorderWidth = m_LegendBorderWidth_Default;
	m_LegendBackGround = m_LegendBackGround_Default;
	m_TicksXLabelsFont = m_TicksXLabelsFont_Default;
	m_TicksYLabelsFont = m_TicksYLabelsFont_Default;
	m_LegendFont = m_LegendFont_Default;
	m_TitleFont = m_TitleFont_Default;
	m_SubTitleFont = m_SubTitleFont_Default;
	m_XLabelFont = m_XLabelFont_Default;
	m_YLabelFont = m_YLabelFont_Default;
	m_GraphFieldBackGround = m_GraphFieldBackGround_Default;
	m_GraphStyle = m_GraphStyle_Default;
	m_PointSize = m_PointSize_Default;
	m_GraphXSize = m_GraphXSize_Default;
	m_GraphYSize = m_GraphYSize_Default;
	m_IntelligentAxisScale = m_IntelligentAxisScale_Default;
	m_bAutoLegendPos = m_bAutoLegendPos_Default;
	m_bLegendFloating = m_bLegendFloating_Default;
	m_LegendRect = m_LegendRect_Default;
	m_GridLine = m_GridLine_Default;
	m_SubGridLine = m_SubGridLine_Default;
	m_Symmetric = m_Symmetric_Default;
	m_DefaultXGridSize = m_DefaultXGridSize_Default;
	m_DefaultYGridSize = m_DefaultYGridSize_Default;
	m_GridXChanging = m_GridXChanging_Default;
	m_GridYChanging = m_GridYChanging_Default;
	m_SubXGridsCount = m_SubXGridsCount_Default;
	m_SubYGridsCount = m_SubYGridsCount_Default;
	m_GraphXChanging = m_GraphXChanging_Default;
	m_GraphYChanging = m_GraphYChanging_Default;
	m_XLabelsFormat = m_XLabelsFormat_Default;
	m_YLabelsFormat = m_YLabelsFormat_Default;
	m_XLabelsFormatAdditional = m_XLabelsFormatAdditional_Default;
	m_YLabelsFormatAdditional = m_YLabelsFormatAdditional_Default;
	m_AdditionalGridSteps = m_AdditionalGridSteps_Default;
	m_GraphBorderColor = m_GraphBorderColor_Default;
	m_LegendBorderColor = m_LegendBorderColor_Default;

	m_YLabelFont.angle = 90; ///added by me

	for (int i = 0; i < GetSeriesCount(); i++)
	{
		GetSerie(i)->color = m_MarkerColor_Default;
		GetSerie(i)->marker = m_MarkerStyle_Default;
		GetSerie(i)->markersize = m_MarkerSize_Default;
		GetSerie(i)->linewidth = m_LineWidth_Default;
	}
}

void CGraphCtrl::MarkAsDefaultPreset(void)
{
	m_BackGround_Default = m_BackGround;
	m_AntiAlaising_Default = m_AntiAlaising;
	m_GraphBorderWidth_Default = m_GraphBorderWidth;
	m_ShowTitle_Default = m_ShowTitle;
	m_ShowSubTitle_Default = m_ShowSubTitle;
	m_ShowXLabel_Default = m_ShowXLabel;
	m_ShowYLabel_Default = m_ShowYLabel;
	m_ShowXTicksLabels_Default = m_ShowXTicksLabels;
	m_ShowYTicksLabels_Default = m_ShowYTicksLabels;
	m_ShowXSubTicksLabels_Default = m_ShowXSubTicksLabels;
	m_ShowYSubTicksLabels_Default = m_ShowYSubTicksLabels;
	m_DrawZeroLabels_Default = m_DrawZeroLabels;
	m_ShowGrid_Default = m_ShowGrid;
	m_GridXSize_Default = m_GridXSize;
	m_GridYSize_Default = m_GridYSize;
	m_ShowXTicks_Default = m_ShowXTicks;
	m_ShowYTicks_Default = m_ShowYTicks;
	m_GridColor_Default = m_GridColor;
	m_StableXGridCount_Default = m_StableXGridCount;
	m_StableYGridCount_Default = m_StableYGridCount;
	m_ShowSubGrid_Default = m_ShowSubGrid;
	m_SubGridXSize_Default = m_SubGridXSize;
	m_SubGridYSize_Default = m_SubGridYSize;
	m_ShowXSubTicks_Default = m_ShowXSubTicks;
	m_ShowYSubTicks_Default = m_ShowYSubTicks;
	m_SubGridColor_Default = m_SubGridColor;
	m_ShowLegend_Default = m_ShowLegend;
	m_LegendBorderWidth_Default = m_LegendBorderWidth;
	m_LegendBackGround_Default = m_LegendBackGround;
	m_TicksXLabelsFont_Default = m_TicksXLabelsFont;
	m_TicksYLabelsFont_Default = m_TicksYLabelsFont;
	m_LegendFont_Default = m_LegendFont;
	m_TitleFont_Default = m_TitleFont;
	m_SubTitleFont_Default = m_SubTitleFont;
	m_XLabelFont_Default = m_XLabelFont;
	m_YLabelFont_Default = m_YLabelFont;
	m_GraphFieldBackGround_Default = m_GraphFieldBackGround;
	m_GraphStyle_Default = m_GraphStyle;
	m_PointSize_Default = m_PointSize;
	m_GraphXSize_Default = m_GraphXSize;
	m_GraphYSize_Default = m_GraphYSize;
	m_IntelligentAxisScale_Default = m_IntelligentAxisScale;
	m_bAutoLegendPos_Default = m_bAutoLegendPos;
	m_bLegendFloating_Default = m_bLegendFloating;
	m_LegendRect_Default = m_LegendRect;
	m_GridLine_Default = m_GridLine;
	m_SubGridLine_Default = m_SubGridLine;
	m_Symmetric_Default = m_Symmetric;
	m_DefaultXGridSize_Default = m_DefaultXGridSize;
	m_DefaultYGridSize_Default = m_DefaultYGridSize;
	m_GridXChanging_Default = m_GridXChanging;
	m_GridYChanging_Default = m_GridYChanging;
	m_SubXGridsCount_Default = m_SubXGridsCount;
	m_SubYGridsCount_Default = m_SubYGridsCount;
	m_GraphXChanging_Default = m_GraphXChanging;
	m_GraphYChanging_Default = m_GraphYChanging;
	m_XLabelsFormat_Default = m_XLabelsFormat;
	m_YLabelsFormat_Default = m_YLabelsFormat;
	m_XLabelsFormatAdditional_Default = m_XLabelsFormatAdditional;
	m_YLabelsFormatAdditional_Default = m_YLabelsFormatAdditional;
	m_AdditionalGridSteps_Default = m_AdditionalGridSteps;
	m_GraphBorderColor_Default = m_GraphBorderColor;
	m_LegendBorderColor_Default = m_LegendBorderColor;

	if (GetSeriesCount() > 0)
	{
		m_MarkerColor_Default = GetSerie(0)->color;
		m_MarkerStyle_Default = GetSerie(0)->marker;
		m_MarkerSize_Default = GetSerie(0)->markersize;
		m_LineWidth_Default = GetSerie(0)->linewidth > 0 ? GetSerie(0)->linewidth : 1;
	}
	else
	{
		m_MarkerColor_Default = RGB(0, 0, 0);
		m_MarkerStyle_Default = MS_CIRCLE;
		m_MarkerSize_Default = 15;
		m_LineWidth_Default = 1;
	}
}

void CGraphCtrl::Clear(void)
{
	m_Title.Empty();
	m_SubTitle.Empty();
	m_XLabel.Empty();
	m_YLabel.Empty();

	while (m_GraphSeries.GetCount() > 0)
	{
		GRAPHSERIES* pSeries = (GRAPHSERIES*)m_GraphSeries.RemoveHead();
		if (pSeries->pData)
			delete[] pSeries->pData;
		delete pSeries;
	}
}

void CGraphCtrl::DeleteSerie(int index)
{
	POSITION pos = m_GraphSeries.FindIndex(index);
	if (pos)
	{
		GRAPHSERIES* pSeries = (GRAPHSERIES*)m_GraphSeries.GetAt(pos);
		m_GraphSeries.RemoveAt(pos); 
		if (pSeries->pData)
			delete[] pSeries->pData;
		delete pSeries;
	}
}

void CGraphCtrl::CopyPresetFrom(CGraphCtrl* pCtrl)
{
	m_BackGround = pCtrl->m_BackGround;
	m_AntiAlaising = pCtrl->m_AntiAlaising;
	m_GraphBorderWidth = pCtrl->m_GraphBorderWidth;
	m_ShowTitle = pCtrl->m_ShowTitle;
	m_ShowSubTitle = pCtrl->m_ShowSubTitle;
	m_ShowXLabel = pCtrl->m_ShowXLabel;
	m_ShowYLabel = pCtrl->m_ShowYLabel;
	m_ShowXTicksLabels = pCtrl->m_ShowXTicksLabels;
	m_ShowYTicksLabels = pCtrl->m_ShowYTicksLabels;
	m_ShowXSubTicksLabels = pCtrl->m_ShowXSubTicksLabels;
	m_ShowYSubTicksLabels = pCtrl->m_ShowYSubTicksLabels;
	m_DrawZeroLabels = pCtrl->m_DrawZeroLabels;
	m_ShowGrid = pCtrl->m_ShowGrid;
	m_GridXSize = pCtrl->m_GridXSize;
	m_GridYSize = pCtrl->m_GridYSize;
	m_ShowXTicks = pCtrl->m_ShowXTicks;
	m_ShowYTicks = pCtrl->m_ShowYTicks;
	m_GridColor = pCtrl->m_GridColor;
	m_StableXGridCount = pCtrl->m_StableXGridCount;
	m_StableYGridCount = pCtrl->m_StableYGridCount;
	m_ShowSubGrid = pCtrl->m_ShowSubGrid;
	m_SubGridXSize = pCtrl->m_SubGridXSize;
	m_SubGridYSize = pCtrl->m_SubGridYSize;
	m_ShowXSubTicks = pCtrl->m_ShowXSubTicks;
	m_ShowYSubTicks = pCtrl->m_ShowYSubTicks;
	m_SubGridColor = pCtrl->m_SubGridColor;
	m_ShowLegend = pCtrl->m_ShowLegend;
	m_LegendBorderWidth = pCtrl->m_LegendBorderWidth;
	m_LegendBackGround = pCtrl->m_LegendBackGround;
	m_TicksXLabelsFont = pCtrl->m_TicksXLabelsFont;
	m_TicksYLabelsFont = pCtrl->m_TicksYLabelsFont;
	m_LegendFont = pCtrl->m_LegendFont;
	m_TitleFont = pCtrl->m_TitleFont;
	m_SubTitleFont = pCtrl->m_SubTitleFont;
	m_XLabelFont = pCtrl->m_XLabelFont;
	m_YLabelFont = pCtrl->m_YLabelFont;
	m_GraphFieldBackGround = pCtrl->m_GraphFieldBackGround;
	m_GraphStyle = pCtrl->m_GraphStyle;
	m_PointSize = pCtrl->m_PointSize;
	m_GraphXSize = pCtrl->m_GraphXSize;
	m_GraphYSize = pCtrl->m_GraphYSize;
	m_IntelligentAxisScale = pCtrl->m_IntelligentAxisScale;
	m_DefaultGraphRect = pCtrl->GetDefaultRect();
	int index = 0;
	if (pCtrl->GetSeriesCount() > 0)
		for (int i = 0; i < GetSeriesCount(); i++)
		{
			GetSerie(i)->color = pCtrl->GetSerie(index)->color;
			GetSerie(i)->marker = pCtrl->GetSerie(index)->marker;
			GetSerie(i)->markersize = pCtrl->GetSerie(index)->markersize;
			GetSerie(i)->linewidth = pCtrl->GetSerie(index)->linewidth > 0 ? pCtrl->GetSerie(index)->linewidth : 1;

			index++;
			if (index >= pCtrl->GetSeriesCount()) index = 0;
		}
}

CRect CGraphCtrl::GetDefaultRect()
{
	return m_DefaultGraphRect;
}
void CGraphCtrl::SetLegendRect(LPRECT lpRect)
{
	m_LegendRect = lpRect;
	m_bAutoLegendPos = FALSE;
}

int CGraphCtrl::GetGraphFieldRightPos(void)
{
	int result = 0;
	if (!m_ShowLegend)
		result = m_BGRect.right -  CalcXDistance(m_RightBorder);
	else if (m_bAutoLegendPos || (m_BGRect.right - m_LegendRect.right - CalcXDistance(m_RightBorder) <= CalcXDistance(LABELSTOBORDER) * 3 && !m_bLegendFloating) || (m_bLegendFloating && m_BGRect.right - m_LegendRect.right - CalcXDistance(m_RightBorder) <= 0))
	{
		result = m_LegendRect.left - CalcXDistance(LABELSTOBORDER);
		m_bLegendFloating = FALSE;
	}
	else if (m_bLegendFloating || m_BGRect.right - (CalcXDistance(LABELSTOBORDER) + CalcXDistance(m_RightBorder) + 1.5 * (double)m_LegendRect.Width()) > m_LegendRect.left)
	{
		result = m_BGRect.right - CalcXDistance(m_RightBorder);
		m_bLegendFloating = TRUE;
	}
	else result = m_BGRect.right - (CalcXDistance(LABELSTOBORDER) + m_LegendRect.Width() + CalcXDistance(m_RightBorder));
	return result;
}

void CGraphCtrl::Recenter(void)
{
	SetGraphCenter(m_GraphCenter);
}

void CGraphCtrl::AddSerieData(GRAPHPOINT pt, int nSerieIndex, BOOL redraw)
{
	GRAPHSERIES* pSerie = GetSerie(nSerieIndex);
	if (pSerie)
	{
		pSerie->count++;
		pSerie->pData = (GRAPHPOINT*)realloc(pSerie->pData, pSerie->count * sizeof(GRAPHPOINT));
		pSerie->pData[pSerie->count - 1] = pt;

		if (redraw && m_pGraph->GetView() && m_pGraph->GetView()->IsWindowVisible())
		{
			m_pGraph->GetView()->Resize();
			m_pGraph->GetView()->Invalidate();
		}
	}
}

void CGraphCtrl::SetLineStyle(int width, COLORREF color, GRAPH_LINESTYLE style)
{
	glColor3d((double)GetRValue(color) / 255, 
		(double)GetGValue(color) / 255, 
		(double)GetBValue(color) / 255);
	glLineWidth((float)CalcLineWidth(width));
	glEnable(GL_LINE_STIPPLE);
	switch (style)
	{
	case LS_SOLID: glLineStipple(CalcLineWidth(1), 0xFFFF); break;
	case LS_DOT: glLineStipple(CalcLineWidth(width), 0x9249); break;
	case LS_DOT3: glLineStipple(CalcLineWidth(3 * width), 0xAAAA); break;
	case LS_DASHDOT: glLineStipple(CalcLineWidth(width), 0xE4E4); break;
	case LS_DASH: glLineStipple(CalcLineWidth(width), 0xF0F0); break;
	case LS_DASHLARGE: glLineStipple(CalcLineWidth(width), 0xF8F8); break;
	case LS_DASHDOTDOT: glLineStipple(CalcLineWidth(width), 0xE49C); break;
	}
}

void GetLabelText(CString& label, double value, GRAPH_LABELSFORMAT format, int additional)
{
	switch (format)
	{
	case LF_INT: label.Format(_T("%.0f"), value); break;
	case LF_DOUBLE: 
		{
			CString fmt;
			fmt.Format(_T("%%.%df"), additional);
			label.Format(fmt, _round(value, additional));
		} break;
	case LF_EXP:
		{
			int exp = _get_exp(value, 1);
			if (compare(_round(value / pow(10.0, exp), additional), 10) == 0)
				exp++;
			CString fmt;
			fmt.Format(_T("%%.%dfe%%d"), additional);
			label.Format(fmt, _round(value / pow(10.0, exp), additional), exp);
		} break;
	case LF_LOG:
	default:
		{ // TODO
			CString fmt;
			fmt.Format(_T("%%.%df"), 4);
			label.Format(fmt, _round(value, 4));
		} break;
	}
}


CString CGraphCtrl::GetXTicksLabelText(double x)
{
	CString result;
	GetLabelText(result, /* DVM fixed status display for log m_LogX?pow(10., x): */ x,
		m_XLabelsFormat, m_XLabelsFormatAdditional);
//	GetLabelText(result, m_LogX?pow(10., x):x, m_XLabelsFormat, m_XLabelsFormatAdditional); by Vitaly original
	return result;
}

CString CGraphCtrl::GetYTicksLabelText(double y)
{
	CString result;
	GetLabelText(result, /* DVM fixed status display for log m_LogY?pow(10., y):*/ y,
		m_YLabelsFormat, m_YLabelsFormatAdditional);
// 	GetLabelText(result, m_LogY?pow(10., y):y, m_YLabelsFormat, m_YLabelsFormatAdditional); by Vitaly, original
	return result;
}


double CGraphCtrl::CalculateGridSize(double& min, double& max, double defaultSize, double screenSize, double gridChanging, double graphChanging)
{
	if (screenSize == 0) return 0;
	double exp = _get_exp(min, max);
	double size = _round((max - min) / pow(10.0, exp), 1);

	double gridsCount = ceil(screenSize / defaultSize);
	double curGridSize = size / gridsCount;
	double minGridSize = _floor_nice(curGridSize, make_int(0 - _get_exp(curGridSize))); if (compare(minGridSize, 0) == 0) minGridSize = MINDOUBLE;
	double maxGridSize = _ceil_nice(curGridSize, make_int(0 - _get_exp(curGridSize))); if (compare(maxGridSize, 0) == 0) maxGridSize = MAXDOUBLE;
	double bestCurSize = MAXDOUBLE, bestCurGridSize = 0;
	if (compare(curGridSize - minGridSize, maxGridSize - curGridSize) <= 0)
		bestCurGridSize = minGridSize;
	else bestCurGridSize = maxGridSize;

//added start VB20060113
	exp = make_int(floor(log10(fabs(max - min))));
	curGridSize = fabs(max - min) / gridsCount;
	bestCurGridSize = pow(10.0, exp);
	double dif = fabs(bestCurGridSize - curGridSize);
	int n = 0;
	while (1)
	{
		double newBest;
		switch(n%4)
		{
		case 0:
			//0.5
			newBest = bestCurGridSize / 2;
			break;
		case 1:
			//0.25
			newBest = bestCurGridSize / 2;
			break;
		case 2:
			//0.2
			newBest = bestCurGridSize * 4 / 5;
			break;
		default:
			//0.1
			newBest = bestCurGridSize / 2;
			break;
		}
		n++;
		if (fabs(newBest - curGridSize) >= dif)
			break;
		dif = fabs(newBest - curGridSize);
		bestCurGridSize = newBest;
	}

	return bestCurGridSize;
//added end VB20060113

/*
	double startPos = floor(defaultSize * (100 - gridChanging) / 100);
	double endPos = ceil(defaultSize * (100 + gridChanging) / 100);
	double bestCurSize = MAXDOUBLE, bestCurGridSize = 0;
	double maxCurSize = 0, maxCurGridSize = 0, maxCount = 0;
	for (double pos = startPos; pos <= endPos; pos++)
	{
		double gridsCount = _round(screenSize / pos, 0);
		double curGridSize = _round(size / gridsCount, 2);
		double curSize = gridsCount * curGridSize;
		if (compare(curSize, size) == -1)
		{
			if (compare(curSize, maxCurSize) == 1)
			{
				maxCurSize = curSize;
				maxCurGridSize = curGridSize;
				maxCount = gridsCount;
			}
			continue;
		}
		if (compare(curSize, size) != 0)
		{
			double ___fabs = fabs(curSize - size);
			double ___log10 = log10(fabs(curSize - size));
			double ___floor = floor(log10(fabs(curSize - size)));
			if (floor(log10(fabs(curSize - size))) < -1) 
			{
				if (compare(curSize, maxCurSize) == -1 || compare(size, maxCurSize) == 1)
				{
					maxCurSize = curSize;
					maxCurGridSize = curGridSize;
					maxCount = gridsCount;
				}
				continue;
			}
		}
		if (compare(bestCurSize - size, curSize - size) == -1 && bestCurSize != MAXDOUBLE) continue;
		if (compare(bestCurSize - size, curSize - size) == 1 || bestCurSize == MAXDOUBLE) 
		{
			bestCurSize = curSize;
			bestCurGridSize = curGridSize;
			continue;
		}
		double bestExp = floor(log10(fabs(bestCurGridSize)));
		double curExp = floor(log10(fabs(curGridSize)));
		if (fabs(bestExp) < fabs(curExp)) continue;
		if (fabs(curExp) < fabs(bestExp))
		{
			bestCurSize = curSize;
			bestCurGridSize = curGridSize;
			continue;
		}

		double curGridBase = curGridSize / pow(10.0, curExp);
		double bestGridBase = bestCurGridSize / pow(10.0, bestExp);

		double curDiff[4];
		curDiff[0] = fabs(curGridBase - 0);
		curDiff[1] = fabs(5 - curGridBase);
		curDiff[2] = fabs(curGridBase - 5);
		curDiff[3] = fabs(10 - curGridBase);
		double minCur = 10;
		for (int i = 0; i < 4; i++)
			if (curDiff[i] < minCur) minCur = curDiff[i];

		double bestDiff[4];
		bestDiff[0] = fabs(bestGridBase - 0);
		bestDiff[1] = fabs(5 - bestGridBase);
		bestDiff[2] = fabs(bestGridBase - 5);
		bestDiff[3] = fabs(10 - bestGridBase);
		double minBest = 10;
		for (int i = 0; i < 4; i++)
			if (bestDiff[i] < minBest) minBest = bestDiff[i];
		if (minCur >= minBest) continue;

		bestCurSize = curSize;
		bestCurGridSize = curGridSize;
	}

	if (compare(bestCurGridSize, 0) == 0)
		bestCurGridSize = size / maxCount;

	double newGridSize = _round(bestCurGridSize, 1);
	double checkSource = bestCurGridSize - floor(bestCurGridSize);
	if (compare(checkSource, 0.2) == 1 && compare(checkSource, 0.3) == -1)
	{
		double diff2 = fabs(checkSource - 0.2);
		double diff25 = fabs(checkSource - 0.25);
		double diff3 = fabs(checkSource - 0.3);
		if (compare(diff25, diff2) == -1 && compare(diff25, diff3) == -1)
			newGridSize = floor(bestCurGridSize) + 0.25;
	}
	else if (compare(checkSource, 0.7) == 1 && compare(checkSource, 0.8) == -1)
	{
		double diff7 = fabs(checkSource - 0.7);
		double diff75 = fabs(checkSource - 0.75);
		double diff8 = fabs(checkSource - 0.8);
		if (compare(diff75, diff7) == -1 && compare(diff75, diff8) == -1)
			newGridSize = floor(bestCurGridSize) + 0.75;
	}

	bestCurGridSize = newGridSize;
*/
	if (exp <= 0)
		max = _round(max, 0 - make_int(exp));

	if (compare(min, 0) <= 0 && compare(max, 0) >= 0)
	{
		double gridSize = bestCurGridSize * pow(10.0, exp);
		min = _floor(min / gridSize , 0) * gridSize;
		max = _ceil(max / gridSize, 0) * gridSize;
		size = _round((max - min) / pow(10.0, exp), 1);
	}

	bestCurSize = bestCurGridSize * _ceil(size / bestCurGridSize);

/*
	if ((bestCurSize - size) / size * 100 <= graphChanging && m_Symmetric)
		max = min + bestCurSize * pow(10.0, exp);
*/

	if (compare(bestCurSize - size, 0) > 0 && m_Symmetric)
		max = min + bestCurSize * pow(10.0, exp);

//	ASSERT(bestCurGridSize > 0);

	return bestCurGridSize * pow(10.0, exp);
}

double CGraphCtrl::GetStartX(void)
{
	GRAPHPOINT sz, center;
	GetSize(&sz);
	
	GetGraphCenter(&center);
	sz.x /= m_CurrentXZoomFactor;

	return center.x - sz.x / 2;
}

double CGraphCtrl::GetStartY(void)
{
	GRAPHPOINT sz, center;
	GetSize(&sz);

	GetGraphCenter(&center);
	sz.y /= m_CurrentYZoomFactor;

	return center.y - sz.y / 2;
}

double CGraphCtrl::GetEndX(void)
{
	GRAPHPOINT sz, center;
	GetSize(&sz);
	GetGraphCenter(&center);
	sz.x /= m_CurrentXZoomFactor;
	sz.y /= m_CurrentYZoomFactor;

	return _ceil(center.x + sz.x / 2, 6);
}

double CGraphCtrl::GetEndY(void)
{
	GRAPHPOINT sz, center;
	GetSize(&sz);
	GetGraphCenter(&center);
	sz.x /= m_CurrentXZoomFactor;
	sz.y /= m_CurrentYZoomFactor;

	return _ceil(center.y + sz.y / 2, 6);
}

double CGraphCtrl::GetXSubGridSize(void)
{
	return m_SubGridXSize;
}

double CGraphCtrl::GetYSubGridSize(void)
{
	return m_SubGridYSize;
}

double CGraphCtrl::GetXGridSize(void)
{
	return m_GridXSize;
}

double CGraphCtrl::GetYGridSize(void)
{
	return m_GridYSize;
}

void CGraphCtrl::GetGraphDataLimits(GRAPHSIZE* x, GRAPHSIZE* y)
{
	x->min = MAXDOUBLE;
	x->max = MINDOUBLE;
	y->min = MAXDOUBLE;
	y->max = MINDOUBLE;
	for (int i = 0; i < GetSeriesCount(); i++)
	{
		GRAPHSERIES* pSerie = GetSerie(i);
		if (!pSerie->visible) continue;
		for (int j = 0; j < pSerie->count; j++)
		{
			GRAPHPOINT gp = pSerie->pData[j];
			if (m_LogX && gp.x > 0)
			{
				if (x->min > _log10_nice(gp.x))
					x->min = _log10_nice(gp.x);
				if (x->max < _log10_nice(gp.x))
					x->max = _log10_nice(gp.x);
			}
			else if (!m_LogX)
			{
				if (x->min > gp.x)
					x->min = gp.x;
				if (x->max < gp.x)
					x->max = gp.x;
			}

			if (m_LogY && gp.y > 0)
			{
				if (y->min > _log10_nice(gp.y))
					y->min = _log10_nice(gp.y);
				if (y->max < _log10_nice(gp.y))
					y->max = _log10_nice(gp.y);
			}
			else if (!m_LogY)
			{
				if (y->min > gp.y)
					y->min = gp.y;
				if (y->max < gp.y)
					y->max = gp.y;
			}
		}
	}
	if (!m_LogX && x->min == MAXDOUBLE) x->min = 0;
	if (!m_LogX && x->max == MINDOUBLE) x->max = 0;
	if (!m_LogY && y->min == MAXDOUBLE) y->min = 0;
	if (!m_LogY && y->max == MINDOUBLE) y->max = 0;

	if (compare(x->min, x->max) == 0)
	{
		if (compare(x->min, 0) > 0)
		{
			x->min = 0;
			x->max *= 2;
		}
		else if (compare(x->min, 0) < 0)
		{
			x->min *= 2;
			x->max = 0;
		}
		if (compare(x->min, 0) == 0)
		{
			x->min = -1;
			x->max = 1;
		}
	}

	if (compare(y->min, y->max) == 0)
	{
		if (compare(y->min, 0) > 0)
		{
			y->min = 0;
			y->max *= 2;
		}
		else if (compare(y->min, 0) < 0)
		{
			y->min *= 2;
			y->max = 0;
		}
		else if (compare(y->min, 0) == 0)
		{
			y->min = -1;
			y->max = 1;
		}
	}
}


CSize CGraphCtrl::GetGraphSize(void)
{
	return CSize(m_Width, m_Height);
}

void CGraphCtrl::SetPrintingDC(CDC* pDC)
{
	m_pPrintingDC = pDC;
}

int CGraphCtrl::CalcXDistance(int value)
{
	if (!m_pPrintingDC)
		return value;

	double scale = (double)m_pPrintingDC->GetDeviceCaps(LOGPIXELSX) / (double)GetDC()->GetDeviceCaps(LOGPIXELSX);
	return (int)_round(value * scale, 0);
}

int CGraphCtrl::CalcYDistance(int value)
{
	if (!m_pPrintingDC)
		return value;

	double scale = (double)m_pPrintingDC->GetDeviceCaps(LOGPIXELSY) / (double)GetDC()->GetDeviceCaps(LOGPIXELSY);
	return (int)_round(value * scale, 0);
}

int CGraphCtrl::CalcLineWidth(int value)
{
	if (!m_pPrintingDC)
		return value;

	double xscale = (double)m_pPrintingDC->GetDeviceCaps(LOGPIXELSX) / (double)GetDC()->GetDeviceCaps(LOGPIXELSX);
	double yscale = (double)m_pPrintingDC->GetDeviceCaps(LOGPIXELSY) / (double)GetDC()->GetDeviceCaps(LOGPIXELSY);
	double scale = xscale < yscale ? xscale : yscale;
	return (int)_round(value * scale, 0);
}

double CGraphCtrl::CalcLineWidth(double value)
{
	if (!m_pPrintingDC)
		return value;

	double xscale = (double)m_pPrintingDC->GetDeviceCaps(LOGPIXELSX) / (double)GetDC()->GetDeviceCaps(LOGPIXELSX);
	double yscale = (double)m_pPrintingDC->GetDeviceCaps(LOGPIXELSY) / (double)GetDC()->GetDeviceCaps(LOGPIXELSY);
	double scale = xscale < yscale ? xscale : yscale;
	return value * scale;
}
