#include "StdAfx.h"
#include "..\include\graph.h"
#include "..\include\constants.h"
#include "..\include\legendscene.h"
#include "..\include\graphctrl.h"
#include "..\include\gl\cgl.h"
#include <gl\GL.h>
#include <gl\GLU.h>

CLegendScene::CLegendScene(CGraphCtrl* pGraph)
: m_pGraph(pGraph)
, m_Width(0)
, m_Height(0)
{
}

CLegendScene::~CLegendScene(void)
{
}

BOOL CLegendScene::OnResize(int cx, int cy)
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

BOOL CLegendScene::OnInit(void)
{
	int params[4];
	glGetIntegerv(GL_VIEWPORT, params);
	m_Width = params[2];
	m_Height = params[3];

	return TRUE;
}

BOOL CLegendScene::OnRender(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor((double)GetRValue(TRANSPARENTCOLOR) / 255, 
		(double)GetGValue(TRANSPARENTCOLOR) / 255, 
		(double)GetBValue(TRANSPARENTCOLOR) / 255, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	CRect rect(0, 0, m_Width, m_Height);

	switch (m_pGraph->m_LegendBackGround.Type)
	{
	case GRAPH_FT_SOLID: RenderSolid(rect, m_pGraph->m_LegendBackGround.Color1); break;
	case GRAPH_FT_GRADIENT: RenderGradient(rect, m_pGraph->m_LegendBackGround.grType, 
						  m_pGraph->m_LegendBackGround.Color1, 
						  m_pGraph->m_LegendBackGround.Color2); break;
	}

	DrawBorder(rect);
	DrawLegend(rect);
	
	return TRUE;
}

CGraphCtrl* CLegendScene::GetGraph(void)
{
	return m_pGraph;
}

void CLegendScene::DrawBorder(LPRECT lpRect)
{
	CRect rect = lpRect;

	m_pGraph->SetLineStyle((int)m_pGraph->m_LegendBorderWidth, m_pGraph->m_LegendBorderColor, LS_SOLID);
	glBegin(GL_LINE_LOOP);
	glVertex2d(rect.left, rect.top);
	glVertex2d(rect.right - m_pGraph->CalcLineWidth(m_pGraph->m_LegendBorderWidth) / 2, rect.top);
	glVertex2d(rect.right - m_pGraph->CalcLineWidth(m_pGraph->m_LegendBorderWidth) / 2, rect.bottom - m_pGraph->CalcLineWidth(m_pGraph->m_LegendBorderWidth) / 2);
	glVertex2d(rect.right, rect.bottom);
	glVertex2d(rect.right - m_pGraph->CalcLineWidth(m_pGraph->m_LegendBorderWidth) / 2, rect.bottom - m_pGraph->CalcLineWidth(m_pGraph->m_LegendBorderWidth) / 2);
	glVertex2d(rect.left, rect.bottom - m_pGraph->CalcLineWidth(m_pGraph->m_LegendBorderWidth) / 2);
	glEnd();
}

void CLegendScene::DrawLegend(LPRECT lpRect)
{
	CRect rect = lpRect, legendRect;
	m_pGraph->GetLegendRect(&legendRect);
	int nTop = rect.top + (int)m_pGraph->m_LegendBorderWidth + m_pGraph->CalcYDistance(LABELSTOBORDER);
	int stableWidth = (int)m_pGraph->m_LegendBorderWidth * 2 + m_pGraph->CalcXDistance(LABELSTOBORDER) * 3;
	
	for (int i = 0; i < m_pGraph->GetSeriesCount(); i++)
	{
		GRAPHSERIES* lpSerie = m_pGraph->GetSerie(i);
		if (lpSerie && lpSerie->visible && lpSerie->legend)
		{
			if (nTop + m_pGraph->m_LegendBorderWidth + m_pGraph->CalcYDistance(lpSerie->markersize) > rect.bottom)
				break;

			CSize sz = m_pGraph->GetTextExtent(lpSerie->lpName, &m_pGraph->m_LegendFont, rect.Width() - (stableWidth + m_pGraph->CalcXDistance(lpSerie->markersize) * 3));
			if (sz.cy < m_pGraph->CalcYDistance(lpSerie->markersize)) sz.cy = m_pGraph->CalcYDistance(lpSerie->markersize);

			m_pGraph->SetLineStyle(lpSerie->linewidth > 0 ? lpSerie->linewidth : 1, lpSerie->color, lpSerie->line);
			glBegin(GL_LINES);
			glVertex2d(rect.left + m_pGraph->CalcXDistance(LABELSTOBORDER), nTop + sz.cy / 2);
			glVertex2d(rect.left + m_pGraph->CalcXDistance(LABELSTOBORDER) + m_pGraph->CalcXDistance(lpSerie->markersize) * 3, nTop + sz.cy / 2);
			glEnd();

			SetGLColor(lpSerie->markercolor);
			m_pGraph->DrawMarker(lpSerie->marker, lpSerie->markersize, rect.left + m_pGraph->CalcXDistance(LABELSTOBORDER) + m_pGraph->CalcXDistance(lpSerie->markersize) * 1.5, nTop + sz.cy / 2);

			SetGLColor(lpSerie->color);

			FONTDRAWDATA fontData;
			memset(&fontData, 0, sizeof(FONTDRAWDATA));
			fontData.top = int(legendRect.top + nTop);
			fontData.bottom = fontData.top + sz.cy;
			fontData.left = legendRect.left + rect.left + m_pGraph->CalcXDistance(lpSerie->markersize) * 3 + 2 * m_pGraph->CalcXDistance(LABELSTOBORDER);
			fontData.right = fontData.left + sz.cx + m_pGraph->CalcXDistance(2);
			fontData.dc = GetDC();
			_tcscpy(fontData.text, lpSerie->lpName);
			memcpy(&fontData.font, &m_pGraph->m_LegendFont, sizeof(FONT));

			if (fontData.right > legendRect.right - m_pGraph->m_LegendBorderWidth)
				fontData.right = legendRect.right - (int)m_pGraph->m_LegendBorderWidth;

			if (fontData.bottom > legendRect.bottom - m_pGraph->m_LegendBorderWidth)
				fontData.bottom = legendRect.bottom - (int)m_pGraph->m_LegendBorderWidth;

			m_pGraph->AddDrawTextData(&fontData);

			nTop += sz.cy + m_pGraph->CalcYDistance(LABELSTOBORDER);
		}
	}

}
