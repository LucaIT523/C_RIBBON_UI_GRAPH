#pragma once

#include "types.h"
#include "gl\cgl.h"

class CGL;

class CLineScene :
	public CGL
{
public:
	CLineScene(CGraphCtrl* pGraph, GRAPH_LINESTYLE style, int width, COLORREF color);
	virtual ~CLineScene(void);
	void SetLine(GRAPH_LINESTYLE style, int width, COLORREF color);
private:
	CGraphCtrl* m_pGraph;
	int m_Width;
	int m_Height;
	int m_LineWidth;
	GRAPH_LINESTYLE m_LineStyle;
	COLORREF m_Color;
protected:
	virtual BOOL OnResize(int cx, int cy);
	virtual BOOL OnInit(void);
	virtual BOOL OnRender(void);
};
