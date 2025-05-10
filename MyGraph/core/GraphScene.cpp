#include "StdAfx.h"
#include "..\include\graphscene.h"
#include "..\include\graph.h"
#include "..\include\graphctrl.h"
#include "..\include\graphview.h"
#include "..\include\tools.h"
#include "gl/gl.h" 			
#include "gl/glu.h" 		
#include "..\include\constants.h"
#include "..\include\\bitmaps\CSimpleDIB.h"
#include "..\include\gl\cgl.h"
#include <math.h>


CGraphScene::CGraphScene(CGraphCtrl* pGraph)
: m_pGraph(pGraph)
, m_Width(0)
, m_Height(0)
{
}

CGraphScene::~CGraphScene()
{
}

BOOL CGraphScene::OnResize(int cx, int cy)
{
	m_Width = cx;
	m_Height = cy;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, cx, cy);
	glOrtho(0, cx, cy, 0, -1000, 1000);
	glDisable(GL_LIGHTING);

	return TRUE ;
}

BOOL CGraphScene::OnInit(void)
{
	int params[4];
	glGetIntegerv(GL_VIEWPORT, params);
	m_Width = params[2];
	m_Height = params[3];

	return TRUE;
}

BOOL CGraphScene::OnRender(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor((double)GetRValue(TRANSPARENTCOLOR) / 255, 
		(double)GetGValue(TRANSPARENTCOLOR) / 255, 
		(double)GetBValue(TRANSPARENTCOLOR) / 255, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	CRect rect(0, 0, m_Width, m_Height);
	switch (m_pGraph->m_GraphFieldBackGround.Type)
	{
	case GRAPH_FT_SOLID: RenderSolid(rect, m_pGraph->m_GraphFieldBackGround.Color1); break;
	case GRAPH_FT_GRADIENT: RenderGradient(rect, m_pGraph->m_GraphFieldBackGround.grType, 
						  m_pGraph->m_GraphFieldBackGround.Color1, 
						  m_pGraph->m_GraphFieldBackGround.Color2); break;
	}

	RenderGraphField();

	return TRUE;
}

CGraphCtrl* CGraphScene::GetGraph(void)
{
	return m_pGraph;
}

void CGraphScene::RenderGraphField(void)
{
	CRect rect(0, 0, m_Width, m_Height);
	CRect graphRect;
	m_pGraph->GetGraphFieldRect(&graphRect);
	double startX = m_pGraph->GetStartX();
	double startY = m_pGraph->GetStartY();
	double endX = m_pGraph->GetEndX();
	double endY = m_pGraph->GetEndY();

	BOOL drawZeroLabel = 
		m_pGraph->m_ShowXTicksLabels || 
		m_pGraph->m_ShowYTicksLabels ||
		m_pGraph->m_ShowXSubTicksLabels ||
		m_pGraph->m_ShowYSubTicksLabels;

	double lastXPos = MINDOUBLE;
	double lastYPos = MINDOUBLE;
	double maxXSize = CalcMaxXTicksLabelSize();
	double maxYSize = CalcMaxYTicksLabelSize();

	if (m_pGraph->GetXGridSize() > 0)
	{
		{
			for (double x = m_pGraph->m_LogX?floor(startX):_nice_start(startX, endX, m_pGraph->GetXGridSize()); compare(x, m_pGraph->m_LogX?ceil(endX):endX) <= 0 && x <= endX; x += m_pGraph->GetXGridSize())
			{
				CRect r;
				GRAPHPOINT glt(x, endY);
				GRAPHPOINT grb(x, startY);
				m_pGraph->GraphToScreenRect(glt, grb, &r);

				if (m_pGraph->m_ShowXTicksLabels && r.left >= rect.left && r.left <= rect.right)
				{
					if ((compare(x, startX) != 0 || m_pGraph->m_DrawZeroLabels && drawZeroLabel) &&
						((compare(lastXPos, 0) < 0) || (compare(r.left - lastXPos, maxXSize + m_pGraph->CalcXDistance(LABELSTOBORDER)) >= 0)))
					{
						DrawXLabel(x, &m_pGraph->m_TicksXLabelsFont, graphRect);
						lastXPos = r.left;
					}
				}

				if (m_pGraph->GetXSubGridSize() > 0)
				{
					if (m_pGraph->m_LogX)
					{
						for (int i = 1; i < m_pGraph->m_SubXGridsCount+1; i++)
						{
							double sx = _log10_nice(pow(10.0, x)+(pow(10.0, x+m_pGraph->GetXGridSize()) - pow(10.0, x))/(m_pGraph->m_SubXGridsCount+1)*i);
							CRect r;
							GRAPHPOINT glt(sx, endY);
							GRAPHPOINT grb(sx, startY);
							m_pGraph->GraphToScreenRect(glt, grb, &r);

							if (r.left < rect.left || r.left > rect.right) continue;

							if (m_pGraph->m_ShowSubGrid && m_pGraph->m_ShowGrid)
								DrawXSubGridLine(r.left, r);
							if (m_pGraph->m_ShowXSubTicks && m_pGraph->m_ShowXTicks)
								DrawXSubTick(r.left, r);
						}
					}
					else
					{
						for (double sx = x + m_pGraph->GetXSubGridSize(); 
							compare(sx, x + m_pGraph->GetXGridSize()) < 0 && 
							compare(sx, endX) < 0;
							sx += m_pGraph->GetXSubGridSize())
						{
							CRect r;
							GRAPHPOINT glt(sx, endY);
							GRAPHPOINT grb(sx, startY);
							m_pGraph->GraphToScreenRect(glt, grb, &r);

							if (r.left < rect.left || r.left > rect.right) continue;

							if (m_pGraph->m_ShowSubGrid && m_pGraph->m_ShowGrid)
								DrawXSubGridLine(r.left, r);
							if (m_pGraph->m_ShowXSubTicks && m_pGraph->m_ShowXTicks)
								DrawXSubTick(r.left, r);
		//					if (m_pGraph->m_ShowXSubTicksLabels)
		//						DrawXLabel(sx, &m_pGraph->m_TicksXLabelsFont, graphRect, lastXPos);
						}
					}
				}
				if (r.left < rect.left || r.left > rect.right) continue;
				if (compare(x, startX) == 0) continue;

				if (m_pGraph->m_ShowGrid)
					DrawXGridLine(r.left, r);
				if (m_pGraph->m_ShowXTicks)
					DrawXTick(r.left, r);
			}
		}
	}
	if (m_pGraph->GetYGridSize() > 0)
	{
		{
			for (double y = m_pGraph->m_LogY?floor(startY):_nice_start(startY, endY, m_pGraph->GetYGridSize()); compare(y, m_pGraph->m_LogY?ceil(endY):endY) <=0 && y <= endY; y += m_pGraph->GetYGridSize())
			{
				CRect r;
				GRAPHPOINT glt(startX, y);
				GRAPHPOINT grb(endX, y);
				m_pGraph->GraphToScreenRect(glt, grb, &r);

				if (m_pGraph->m_ShowYTicksLabels && r.top >= rect.top && r.top <= rect.bottom)
				{
					if ((compare(y, startY) != 0 || m_pGraph->m_DrawZeroLabels && drawZeroLabel) &&
					((compare(lastYPos, 0) < 0) || (compare(lastYPos - r.top, maxYSize + 3) >= 0)))
					{
						DrawYLabel(y, &m_pGraph->m_TicksYLabelsFont, graphRect);
						lastYPos = r.top;
					}
				}

				if (m_pGraph->GetYSubGridSize() > 0)
				{
					if (m_pGraph->m_LogY)
					{
						for (int i = 1; i < m_pGraph->m_SubYGridsCount+1; i++)
						{
							double sy = _log10_nice(pow(10.0, y)+(pow(10.0, y+m_pGraph->GetYGridSize()) - pow(10.0, y))/(m_pGraph->m_SubYGridsCount+1)*i);
							CRect r;
							GRAPHPOINT glt(startX, sy);
							GRAPHPOINT grb(endX, sy);
							m_pGraph->GraphToScreenRect(glt, grb, &r);

							if (r.top < rect.top || r.top > rect.bottom) continue;

							if (m_pGraph->m_ShowSubGrid && m_pGraph->m_ShowGrid)
								DrawYSubGridLine(r.top, r);
							if (m_pGraph->m_ShowYSubTicks && m_pGraph->m_ShowYTicks)
								DrawYSubTick(r.top, r);
						}
					}
					else
					{
						for (double sy = y + m_pGraph->GetYSubGridSize(); 
							compare(sy, y + m_pGraph->GetYGridSize()) < 0 &&
							compare(sy, endY) < 0;
							sy += m_pGraph->GetYSubGridSize())
						{
							CRect r;
							GRAPHPOINT glt(startX, sy);
							GRAPHPOINT grb(endX, sy);
							m_pGraph->GraphToScreenRect(glt, grb, &r);

							if (r.top < rect.top || r.top > rect.bottom) continue;

							if (m_pGraph->m_ShowSubGrid && m_pGraph->m_ShowGrid)
								DrawYSubGridLine(r.top, r);
							if (m_pGraph->m_ShowYSubTicks && m_pGraph->m_ShowYTicks)
								DrawYSubTick(r.top, r);
		//					if (m_pGraph->m_ShowYSubTicksLabels)
		//						DrawYLabel(sy, &m_pGraph->m_TicksYLabelsFont, graphRect, lastYPos);
						}
					}
				}

				if (r.top < rect.top || r.top > rect.bottom) continue;

				if (compare(y, startY) == 0) continue;

				if (m_pGraph->m_ShowGrid)
					DrawYGridLine(r.top, r);
				if (m_pGraph->m_ShowYTicks)
					DrawYTick(r.top, r);
			}
		}
	}
	RenderGraphData();

	DrawBorder(rect);
}

void CGraphScene::RenderGraphData(void)
{
	for (int i = 0; i < m_pGraph->GetSeriesCount(); i++)
	{
		GRAPHSERIES* lpSerie = m_pGraph->GetSerie(i);
		if (lpSerie && lpSerie->visible)
		{
			if (m_pGraph->m_AntiAlaising)
			{
				switch (m_pGraph->m_GraphStyle)
				{
				case GS_SCATTER: glEnable(GL_POINT_SMOOTH); break;
				case GS_LINES: glEnable(GL_LINE_SMOOTH); break;
				}
			}

			switch (m_pGraph->m_GraphStyle)
			{
			case GS_SCATTER: 
				{
					SetGLColor(lpSerie->color);
					glPointSize((float)m_pGraph->m_PointSize);
					glBegin(GL_POINTS);
				} break;
			case GS_LINES: 
				{
					m_pGraph->SetLineStyle(lpSerie->linewidth > 0 ? lpSerie->linewidth : 1, lpSerie->color, lpSerie->line);
					glBegin(GL_LINE_STRIP);
				} break;
			case GS_STEM:
				{
					m_pGraph->SetLineStyle(lpSerie->linewidth > 0 ? lpSerie->linewidth : 1, lpSerie->color, lpSerie->line);
					glBegin(GL_LINES);
				} break;
			default: continue;
			}
			CPoint pt_min;
			GRAPHPOINT gpt = GRAPHPOINT(m_pGraph->GetStartX(), m_pGraph->GetStartY());
			if (m_pGraph->m_LogX)
				gpt.x = _log10_nice(gpt.x);
			if (m_pGraph->m_LogY)
				gpt.y = _log10_nice(gpt.y);
			m_pGraph->GraphToScreenPoint(gpt, &pt_min);
			for (int p = 0; p < lpSerie->count; p++)
			{
				CPoint pt;
				GRAPHPOINT gpt = lpSerie->pData[p];
				if (m_pGraph->m_LogX)
				{
					if (gpt.x <= 0)
					{
						if (m_pGraph->m_GraphStyle == GS_LINES)
						{
							glEnd();
							glBegin(GL_LINE_STRIP);
						}
						continue;
					}
					gpt.x = _log10_nice(gpt.x);
				}
				if (m_pGraph->m_LogY)
				{
					if (gpt.y <= 0 && !m_pGraph->m_ShowLogZero)
					{
						if (m_pGraph->m_GraphStyle == GS_LINES)
						{
							glEnd();
							glBegin(GL_LINE_STRIP);
						}
						continue;
					}
					gpt.y = _log10_nice(gpt.y);
				}
				CPoint pt_0;
				m_pGraph->GraphToScreenPoint(GRAPHPOINT(0, 0), &pt_0);
				m_pGraph->GraphToScreenPoint(gpt, &pt);
				glVertex2d(pt.x, pt.y);
				if (m_pGraph->m_GraphStyle == GS_STEM)
					glVertex2d(pt.x, pt_0.y);
			}
			glEnd();

			if (m_pGraph->m_AntiAlaising)
			{
				switch (m_pGraph->m_GraphStyle)
				{
				case GS_SCATTER: glDisable(GL_POINT_SMOOTH); break;
				case GS_LINES: glDisable(GL_LINE_SMOOTH); break;
				}
			}

			double nMarkers = _round(lpSerie->count * lpSerie->markers / 100 , 0);
			double nSkipStep = (lpSerie->count - nMarkers) / nMarkers;
			SetGLColor(lpSerie->markercolor);
			double curSkip = 0;
			for (int p = 0; p < lpSerie->count; p++)
			{
				if (compare(curSkip, 1) == -1)
				{
					CPoint pt;
					GRAPHPOINT gpt = lpSerie->pData[p];
					if (m_pGraph->m_LogX)
					{
						if (gpt.x <= 0)
							continue;
						gpt.x = _log10_nice(gpt.x);
					}
					if (m_pGraph->m_LogY)
					{
						if (gpt.y <= 0 && !m_pGraph->m_ShowLogZero)
							continue;
						gpt.y = _log10_nice(gpt.y);
					}
					m_pGraph->GraphToScreenPoint(gpt, &pt);
					m_pGraph->DrawMarker(lpSerie->marker, lpSerie->markersize, pt.x, pt.y);
					curSkip += nSkipStep;
				}
				else curSkip--;
			}
		}
	}
}

void CGraphScene::DrawBorder(LPRECT lpRect)
{
	CRect rect = lpRect;

	if (m_pGraph->m_GraphBorderWidth > 0)
	{
		m_pGraph->SetLineStyle((int)m_pGraph->m_GraphBorderWidth, m_pGraph->m_GraphBorderColor, LS_SOLID);
		glBegin(GL_LINE_LOOP);
		glVertex2d(rect.left, rect.top);
		glVertex2d(rect.right - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth) / 2, rect.top);
		glVertex2d(rect.right - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth) / 2, rect.bottom - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth) / 2);
		glVertex2d(rect.right, rect.bottom);
		glVertex2d(rect.right - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth) / 2, rect.bottom - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth) / 2);
		glVertex2d(rect.left, rect.bottom - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth) / 2);
		glEnd();
	}
}

void CGraphScene::DrawXGridLine(double x, LPRECT lpRect)
{
	m_pGraph->SetLineStyle(1, m_pGraph->m_GridColor, m_pGraph->m_GridLine);
	glBegin(GL_LINES);
	glVertex2d(x, lpRect->top + m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth));
	glVertex2d(x, lpRect->bottom - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth));
	glEnd();
}

void CGraphScene::DrawYGridLine(double y, LPRECT lpRect)
{
	m_pGraph->SetLineStyle(1, m_pGraph->m_GridColor, m_pGraph->m_GridLine);
	glBegin(GL_LINES);
	glVertex2d(lpRect->left + m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth), y);
	glVertex2d(lpRect->right - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth), y);
	glEnd();
}

void CGraphScene::DrawXSubGridLine(double x, LPRECT lpRect)
{
	m_pGraph->SetLineStyle(1, m_pGraph->m_SubGridColor, m_pGraph->m_SubGridLine);
	glBegin(GL_LINES);
	glVertex2d(x, lpRect->top + m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth));
	glVertex2d(x, lpRect->bottom - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth));
	glEnd();
}

void CGraphScene::DrawYSubGridLine(double y, LPRECT lpRect)
{
	m_pGraph->SetLineStyle(1, m_pGraph->m_SubGridColor, m_pGraph->m_SubGridLine);
	glBegin(GL_LINES);
	glVertex2d(lpRect->left + m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth), y);
	glVertex2d(lpRect->right - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth), y);
	glEnd();
}

void CGraphScene::DrawXTick(double x, LPRECT lpRect)
{
	m_pGraph->SetLineStyle((int)m_pGraph->m_GraphBorderWidth, m_pGraph->m_GraphBorderColor, LS_SOLID);
	glBegin(GL_LINES);
	glVertex2d(x, lpRect->top + m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth));
	glVertex2d(x, lpRect->top + m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth + 5));
	glVertex2d(x, lpRect->bottom - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth));
	glVertex2d(x, lpRect->bottom - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth - 5));
	glEnd();
}

void CGraphScene::DrawYTick(double y, LPRECT lpRect)
{
	m_pGraph->SetLineStyle((int)m_pGraph->m_GraphBorderWidth, m_pGraph->m_GraphBorderColor, LS_SOLID);
	glBegin(GL_LINES);
	glVertex2d(lpRect->left + m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth), y);
	glVertex2d(lpRect->left + m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth + 5), y);
	glVertex2d(lpRect->right - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth), y);
	glVertex2d(lpRect->right - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth - 5), y);
	glEnd();
}

void CGraphScene::DrawXSubTick(double x, LPRECT lpRect)
{
	m_pGraph->SetLineStyle((int)m_pGraph->m_GraphBorderWidth, m_pGraph->m_GraphBorderColor, LS_SOLID);
	glBegin(GL_LINES);
	glVertex2d(x, lpRect->top + m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth));
	glVertex2d(x, lpRect->top + m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth + 3));
	glVertex2d(x, lpRect->bottom - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth));
	glVertex2d(x, lpRect->bottom - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth - 3));
	glEnd();
}

void CGraphScene::DrawYSubTick(double y, LPRECT lpRect)
{
	m_pGraph->SetLineStyle((int)m_pGraph->m_GraphBorderWidth, m_pGraph->m_GraphBorderColor, LS_SOLID);
	glBegin(GL_LINES);
	glVertex2d(lpRect->left + m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth), y);
	glVertex2d(lpRect->left + m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth + 3), y);
	glVertex2d(lpRect->right - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth), y);
	glVertex2d(lpRect->right - m_pGraph->CalcLineWidth(m_pGraph->m_GraphBorderWidth - 3), y);
	glEnd();
}

void CGraphScene::DrawXLabel(double x , FONT* lpFont, LPRECT lpRect)
{
	CString labelText = m_pGraph->GetXTicksLabelText(x);

	CPoint pt;
	m_pGraph->GraphToScreenPoint(GRAPHPOINT(x, 0), &pt);

	FONTDRAWDATA fontData;
	memset(&fontData, 0, sizeof(FONTDRAWDATA));
	fontData.top = lpRect->bottom + m_pGraph->CalcYDistance(LABELSTOBORDER);
	fontData.centerx = lpRect->left + pt.x;
	fontData.dc = GetDC();
	_tcscpy(fontData.text, labelText);
	memcpy(&fontData.font, lpFont, sizeof(FONT));

	m_pGraph->AddDrawTextData(&fontData);
}

void CGraphScene::DrawYLabel(double y , FONT* lpFont, LPRECT lpRect)
{
	CString labelText = m_pGraph->GetYTicksLabelText(y);

	CPoint pt;
	m_pGraph->GraphToScreenPoint(GRAPHPOINT(0, y), &pt);

	FONTDRAWDATA fontData;
	memset(&fontData, 0, sizeof(FONTDRAWDATA));
	fontData.centery = lpRect->top + pt.y;
	fontData.right = lpRect->left - m_pGraph->CalcXDistance(LABELSTOBORDER);
	fontData.dc = GetDC();
	_tcscpy(fontData.text, labelText);
	memcpy(&fontData.font, lpFont, sizeof(FONT));

	m_pGraph->AddDrawTextData(&fontData);
}

double CGraphScene::CalcMaxXTicksLabelSize(void)
{
	double result = 0;
	double startX = m_pGraph->GetStartX();
	double endX = m_pGraph->GetEndX();
	for (double x = startX; _round(x, 4) <= endX && x <= endX; x += m_pGraph->GetXGridSize())
	{
		CSize sz = m_pGraph->GetTextExtent(m_pGraph->GetXTicksLabelText(x), &m_pGraph->m_TicksXLabelsFont);
		if (result < sz.cx)
			result = sz.cx;
	}
	return result;
}

double CGraphScene::CalcMaxYTicksLabelSize(void)
{
	double result = 0;
	double startY = m_pGraph->GetStartY();
	double endY = m_pGraph->GetEndY();
	for (double y = startY; _round(y, 4) <= endY && y <= endY; y += m_pGraph->GetYGridSize())
	{
		CSize sz = m_pGraph->GetTextExtent(m_pGraph->GetXTicksLabelText(y), &m_pGraph->m_TicksYLabelsFont);
		if (result < sz.cy)
			result = sz.cy;
	}
	return result;
}

