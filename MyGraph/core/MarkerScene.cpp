#include "StdAfx.h"
#include "..\include\graph.h"
#include "..\include\constants.h"
#include "..\include\graphctrl.h"
#include "..\include\markerscene.h"
#include "..\include\gl\cgl.h"

CMarkerScene::CMarkerScene(CGraphCtrl* pGraph, GRAPH_MARKERSTYLE  style, int size, COLORREF color)
: m_Height(0)
, m_Width(0)
, m_MarkerStyle(style)
, m_MarkerSize(size)
, m_Color(color)
, m_pGraph(pGraph)
{
}

CMarkerScene::~CMarkerScene(void)
{
}

BOOL CMarkerScene::OnResize(int cx, int cy)
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

BOOL CMarkerScene::OnInit(void)
{
	int params[4];
	glGetIntegerv(GL_VIEWPORT, params);
	m_Width = params[2];
	m_Height = params[3];

	return TRUE;
}

BOOL CMarkerScene::OnRender(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor((double)GetRValue(TRANSPARENTCOLOR) / 255, 
		(double)GetGValue(TRANSPARENTCOLOR) / 255, 
		(double)GetBValue(TRANSPARENTCOLOR) / 255, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3d((double)GetRValue(m_Color) / 255, 
		(double)GetGValue(m_Color) / 255, 
		(double)GetBValue(m_Color) / 255);
	m_pGraph->DrawMarker(m_MarkerStyle, m_MarkerSize, (double)m_Width / 2, (double)m_Height / 2);
	return TRUE;
}

void CMarkerScene::SetMarker(int size, GRAPH_MARKERSTYLE  style, COLORREF color)
{
	m_MarkerStyle = style;
	m_MarkerSize = size;
	m_Color = color;
}
