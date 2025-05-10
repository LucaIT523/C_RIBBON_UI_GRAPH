#pragma once

#include "gl\cgl.h"

class CGL;

class CLegendScene :
	public CGL
{
public:
	CLegendScene(CGraphCtrl* pGraph = NULL);
	~CLegendScene(void);
	CGraphCtrl* GetGraph(void);
protected:
	virtual BOOL OnResize(int cx, int cy);
	virtual BOOL OnInit(void);
	virtual BOOL OnRender(void);
private:
	CGraphCtrl* m_pGraph;
	int m_Width;
	int m_Height;
protected:
	void DrawBorder(LPRECT lpRect);
	void DrawLegend(LPRECT lpRect);
};
