
#include "StdAfx.h"
#include "..\include\graph.h"
#include "..\include\constants.h"
#include "..\include\gl\cgl.h"
#include "..\include\bgscene.h"
#include "..\include\graphctrl.h"


CBGScene::CBGScene(CGraphCtrl* pGraph)
: m_pGraph(pGraph)
, m_Width(0)
, m_Height(0)
{
}

CBGScene::~CBGScene(void)
{
}

BOOL CBGScene::OnResize(int cx, int cy)
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

BOOL CBGScene::OnInit(void)
{
	int params[4];
	glGetIntegerv(GL_VIEWPORT, params);
	m_Width = params[2];
	m_Height = params[3];

	return TRUE;
}

BOOL CBGScene::OnRender(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor((double)GetRValue(TRANSPARENTCOLOR) / 255, 
		(double)GetGValue(TRANSPARENTCOLOR) / 255, 
		(double)GetBValue(TRANSPARENTCOLOR) / 255, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	CRect rect(0, 0, m_Width, m_Height);

	switch (m_pGraph->m_BackGround.Type)
	{
	case GRAPH_FT_SOLID: RenderSolid(rect, m_pGraph->m_BackGround.Color1); break;
	case GRAPH_FT_GRADIENT: RenderGradient(rect, m_pGraph->m_BackGround.grType, 
						  m_pGraph->m_BackGround.Color1, 
						  m_pGraph->m_BackGround.Color2); break;
	}

	return TRUE;
}

CGraphCtrl* CBGScene::GetGraph(void)
{
	return m_pGraph;
}
