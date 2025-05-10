#include "StdAfx.h"
#include "..\include\graph.h"
#include "..\include\constants.h"
#include "..\include\linescene.h"
#include "..\include\graphctrl.h"

CLineScene::CLineScene(CGraphCtrl* pGraph, GRAPH_LINESTYLE style, int width, COLORREF color)
	: m_pGraph(pGraph)
	, m_Width(0)
	, m_Height(0)
	, m_LineWidth(width)
	, m_LineStyle(style)
	, m_Color(color)
{
}

CLineScene::~CLineScene(void)
{
}

void CLineScene::SetLine(GRAPH_LINESTYLE style, int width, COLORREF color)
{
	m_LineStyle = style;
	m_LineWidth = width;
	m_Color = color;
}

BOOL CLineScene::OnResize(int cx, int cy)
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

BOOL CLineScene::OnInit(void)
{
	int params[4];
	glGetIntegerv(GL_VIEWPORT, params);
	m_Width = params[2];
	m_Height = params[3];

	return TRUE;
}

BOOL CLineScene::OnRender(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor((double)GetRValue(TRANSPARENTCOLOR) / 255, 
		(double)GetGValue(TRANSPARENTCOLOR) / 255, 
		(double)GetBValue(TRANSPARENTCOLOR) / 255, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	m_pGraph->SetLineStyle(m_LineWidth, m_Color, m_LineStyle);
	glBegin(GL_LINES);
	glVertex2d(0, (double)m_Height / 2);
	glVertex2d(m_Width, (double)m_Height / 2);
	glEnd();

	return TRUE;
}

