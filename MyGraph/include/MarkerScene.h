#pragma once

#include "gl\cgl.h"
#include "types.h"

class CGL;

class CMarkerScene :
	public CGL
{
public:
	CMarkerScene(CGraphCtrl* pGraph, GRAPH_MARKERSTYLE  style, int size, COLORREF color);
	virtual ~CMarkerScene(void);
protected:
	virtual BOOL OnResize(int cx, int cy);
	virtual BOOL OnInit(void);
	virtual BOOL OnRender(void);
private:
	CGraphCtrl* m_pGraph;
	int m_Width;
	int m_Height;
	int m_MarkerSize;
	GRAPH_MARKERSTYLE  m_MarkerStyle;
	COLORREF m_Color;
public:
	void SetMarker(int size, GRAPH_MARKERSTYLE  style, COLORREF color);
};
