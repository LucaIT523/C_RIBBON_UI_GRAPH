#pragma once

class CGL;

class CBGScene :
	public CGL
{
public:
	CBGScene(CGraphCtrl* pGraph = NULL);
	~CBGScene(void);
	CGraphCtrl* GetGraph(void);
protected:
	virtual BOOL OnResize(int cx, int cy);
	virtual BOOL OnInit(void);
	virtual BOOL OnRender(void);
private:
	CGraphCtrl* m_pGraph;
	int m_Width;
	int m_Height;
};
