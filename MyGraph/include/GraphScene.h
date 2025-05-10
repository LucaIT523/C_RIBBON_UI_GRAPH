#pragma once

#include "types.h"
#include "gl\cgl.h"

class CGraphView;
class CGL;

class CGraphScene :
	public CGL
{
public:
	CGraphScene(CGraphCtrl* pGraph = NULL);
	virtual ~CGraphScene();
protected:
	virtual BOOL OnResize(int cx, int cy);
	virtual BOOL OnInit(void);
	virtual BOOL OnRender(void);
private:
	CGraphCtrl* m_pGraph;
	int m_Width;
	int m_Height;
public:
	CGraphCtrl* GetGraph(void);
protected:
	void RenderGraphField(void);
	void RenderGraphData(void);
	void DrawBorder(LPRECT lpRect);
protected:
	void DrawXGridLine(double x, LPRECT lpRect);
	void DrawYGridLine(double y, LPRECT lpRect);
	void DrawXSubGridLine(double x, LPRECT lpRect);
	void DrawYSubGridLine(double y, LPRECT lpRect);
	void DrawXTick(double x, LPRECT lpRect);
	void DrawYTick(double y, LPRECT lpRect);
	void DrawXSubTick(double x, LPRECT lpRect);
	void DrawYSubTick(double y, LPRECT lpRect);
	void DrawXLabel(double x , FONT* lpFont, LPRECT lpRect);
	void DrawYLabel(double y , FONT* lpFont, LPRECT lpRect);
	double CalcMaxXTicksLabelSize(void);
	int CompareX(double val1, double val2, int precision = 4);
	int CompareY(double val1, double val2, int precision = 4);
public:
	double CalcMaxYTicksLabelSize(void);
};
