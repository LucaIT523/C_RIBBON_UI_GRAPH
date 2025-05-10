#pragma once

#include <afxtempl.h>
#include "types.h"
#include ".\bitmaps\csimpledib.h"

// CGraphCtrl command target

class CBGScene;
class CGraphScene;
class CLegendScene;
class CSimpleDIB;

class CGraph;

class DLL_EXP CGraphCtrl : public CObject
{
	friend class CGraph;
public:
	CGraphCtrl(CGraph* pGraph = NULL);
	virtual ~CGraphCtrl();
protected:
	CPtrList m_GraphSeries;
	int m_Width;
	int m_Height;
	CPtrList m_DrawTextData;
	void DrawText(CDC* dc);
	CGraph* m_pGraph;
	HFONT CreateFont(FONT* lpFont, HDC dc = NULL);
	CDC* m_pPrintingDC;

	CRect m_BGRect;
	CRect m_GraphRect;
	CRect m_LegendRect;
	CRect m_TitleRect;
	CRect m_SubTitleRect;
	CRect m_XLabelRect;
	CRect m_YLabelRect;
	CRect m_XTicksLabelRect;
	CRect m_YTicksLabelRect;
	CRect m_BackupLegendRect;

	void CalcTitleRect();
	void CalcSubTitleRect();
	void CalcLegendRect();
	void CalcXLabelRect();
	void CalcYLabelRect();
	void CalcXTicksLabelRect();
	void CalcYTicksLabelRect();
	void CalcGraphFieldRect();

protected:
	BACKGROUND m_BackGround_Default;
	BOOL m_AntiAlaising_Default;
	double m_GraphBorderWidth_Default;
	BOOL m_ShowTitle_Default;
	BOOL m_ShowSubTitle_Default;
	BOOL m_ShowXLabel_Default;
	BOOL m_ShowYLabel_Default;
	BOOL m_ShowXTicksLabels_Default;
	BOOL m_ShowYTicksLabels_Default;
	BOOL m_ShowXSubTicksLabels_Default;
	BOOL m_ShowYSubTicksLabels_Default;
	BOOL m_DrawZeroLabels_Default;
	BOOL m_ShowGrid_Default;
	double m_GridXSize_Default;
	double m_GridYSize_Default;
	BOOL m_ShowXTicks_Default;
	BOOL m_ShowYTicks_Default;
	COLORREF m_GridColor_Default;
	int m_StableXGridCount_Default;
	int m_StableYGridCount_Default;
	BOOL m_ShowSubGrid_Default;
	double m_SubGridXSize_Default;
	double m_SubGridYSize_Default;
	BOOL m_ShowXSubTicks_Default;
	BOOL m_ShowYSubTicks_Default;
	COLORREF m_SubGridColor_Default;
	BOOL m_ShowLegend_Default;
	double m_LegendBorderWidth_Default;
	BACKGROUND m_LegendBackGround_Default;
	FONT m_TicksXLabelsFont_Default;
	FONT m_TicksYLabelsFont_Default;
	FONT m_LegendFont_Default;
	FONT m_TitleFont_Default;
	FONT m_SubTitleFont_Default;
	FONT m_XLabelFont_Default;
	FONT m_YLabelFont_Default;
	BACKGROUND m_GraphFieldBackGround_Default;
	GRAPH_GRAPHSTYLE m_GraphStyle_Default;
	int m_LineWidth_Default;
	int m_PointSize_Default;
	GRAPHSIZE m_GraphXSize_Default;
	GRAPHSIZE m_GraphYSize_Default;
	BOOL m_IntelligentAxisScale_Default;
	COLORREF m_MarkerColor_Default;
	GRAPH_MARKERSTYLE  m_MarkerStyle_Default;
	int m_MarkerSize_Default;
	BOOL m_bAutoLegendPos_Default;
	BOOL m_bLegendFloating_Default;
	CRect m_LegendRect_Default;
	GRAPH_LINESTYLE m_GridLine_Default;
	GRAPH_LINESTYLE m_SubGridLine_Default;
	BOOL m_Symmetric_Default;
	double m_DefaultXGridSize_Default;
	double m_DefaultYGridSize_Default;
	double m_GridXChanging_Default;
	double m_GridYChanging_Default;
	int m_SubXGridsCount_Default;
	int m_SubYGridsCount_Default;
	double m_GraphXChanging_Default;
	double m_GraphYChanging_Default;
	GRAPH_LABELSFORMAT m_XLabelsFormat_Default;
	GRAPH_LABELSFORMAT m_YLabelsFormat_Default;
	int m_XLabelsFormatAdditional_Default;
	int m_YLabelsFormatAdditional_Default;
	CRect m_AdditionalGridSteps_Default;
	COLORREF m_GraphBorderColor_Default;
	COLORREF  m_LegendBorderColor_Default;

	double m_SubGridXSize;
	double m_SubGridYSize;
	double m_GridXSize;
	double m_GridYSize;

public:
	BOOL m_LogX;
	BOOL m_LogY;
	BOOL m_ShowLogZero;
	BOOL m_ShowLegend;
	BOOL m_ShowTitle;
	BOOL m_ShowXLabel;
	BOOL m_ShowYLabel;
	BOOL m_ShowSubTitle;
	BOOL m_ShowXTicks;
	BOOL m_ShowYTicks;
	BOOL m_ShowXSubTicks;
	BOOL m_ShowYSubTicks;
	BOOL m_ShowGrid;
	BOOL m_ShowSubGrid;
	double m_GraphBorderWidth;
	double m_LegendBorderWidth;
	COLORREF m_GridColor;
	COLORREF m_SubGridColor;
	BACKGROUND m_BackGround;
	BACKGROUND m_GraphFieldBackGround;
	BACKGROUND m_LegendBackGround;
	GRAPHSIZE m_GraphXSize;
	GRAPHSIZE m_GraphYSize;
	BOOL m_ShowXTicksLabels;
	BOOL m_ShowYTicksLabels;
	BOOL m_ShowXSubTicksLabels;
	BOOL m_ShowYSubTicksLabels;
	BOOL m_DrawZeroLabels;
	BOOL m_AntiAlaising;
	GRAPH_GRAPHSTYLE m_GraphStyle;
	int m_PointSize;
	CString m_Title;
	CString m_SubTitle;
	CString m_XLabel;
	CString m_YLabel;
	FONT m_TicksXLabelsFont;
	FONT m_TicksYLabelsFont;
	FONT m_LegendFont;
	FONT m_TitleFont;
	FONT m_SubTitleFont;
	FONT m_XLabelFont;
	FONT m_YLabelFont;
	GRAPH_LINESTYLE m_GridLine;
	GRAPH_LINESTYLE m_SubGridLine;
	GRAPH_LABELSFORMAT m_XLabelsFormat;
	GRAPH_LABELSFORMAT m_YLabelsFormat;
	int m_XLabelsFormatAdditional;
	int m_YLabelsFormatAdditional;
	CRect m_AdditionalGridSteps;
	COLORREF m_GraphBorderColor;
	COLORREF  m_LegendBorderColor;

	BOOL m_bAutoLegendPos;
	BOOL m_bLegendFloating;

	void CalculateAxisScale(void);
	int GetSeriesCount(void);
	void DeleteSerie(int index);
	void AddSeries(GRAPHSERIES* pSeries, BOOL redraw = TRUE);
	int IndexOfSerie(LPCTSTR lpName);
	GRAPHSERIES* GetSerie(int index);
	void Draw(CDC* pDC = NULL);

	void ClearDrawTextData(void);
	void AddDrawTextData(FONTDRAWDATA* lpData);

	void GetLegendRect(CRect* lpRect);
	void GetGraphFieldRect(CRect* lpRect);
	void DrawMarker(GRAPH_MARKERSTYLE  style, int size, double x, double y);
	GRAPH_GRAPHCONTROL GetControlForPoint(CPoint point, CPoint* localPoint);
	void Zoom(double zoomXFactor, double zoomYFactor, CPoint center);
protected:
	BOOL m_Prepared;
	GRAPHPOINT m_GraphCenter;
public:
	double m_CurrentXZoomFactor;
	double m_CurrentYZoomFactor;
	void Prepare(void);

	void GraphToScreenPoint(GRAPHPOINT lpGraphPt, CPoint* lpScreenPt);
	void GraphToScreenRect(GRAPHPOINT lpLeftTopPt, GRAPHPOINT lpRightBottom, CRect* lpScreenRect);
	void ScreenToGraphPoint(CPoint lpScreenPt, GRAPHPOINT* lpGraphPt, bool bCalcLog = true);
	void ScreenToGraphRect(CRect lpScreenRect, GRAPHPOINT* lpLeftTopPt, GRAPHPOINT* lpRightBottom);
	void SetGraphCenter(GRAPHPOINT pt, bool bCalcGridSizes = true);
	void GetGraphCenter(GRAPHPOINT* pGraphCenter);
	void GetVisibleSize(GRAPHPOINT* lpSize);
	void GetSize(GRAPHPOINT* lpSize);
	void GetGraphDataLimits(GRAPHSIZE* x, GRAPHSIZE* y);
	void GraphToScreenSize(GRAPHPOINT lpSize, CSize* lpScreenSize);

	CList<double,double> m_ZoomFactorXList;
	CList<double,double> m_ZoomFactorYList;
protected:
	CSimpleDIB m_DIB;
	CSimpleDIB m_GraphDIB;
	CSimpleDIB m_LegendDIB;
public:
	CDC* GetDC(void);
	void Resize(int cx, int cy);
protected:
	CBGScene* m_pBGScene;
	CGraphScene* m_pGraphScene;
	CLegendScene* m_pLegendScene;
public:
	void MoveGraph(double cx, double cy);
	double GetMinXValue(void);
	double GetMaxXValue(void);
	double GetMinYValue(void);
	double GetMaxYValue(void);
	CSize GetTextExtent(LPCTSTR lpText, FONT* lpFont, int maxWidth = 0);
protected:
	void DrawTitles(void);
public:
	BOOL m_IntelligentAxisScale;
	int m_StableXGridCount;
	int m_StableYGridCount;
protected:
	void CalculateGridSizes(void);
	CRect m_DefaultGraphRect;
public:
	CRect GetDefaultRect();
	void RecalcPositions(void);
	void ResetPreset(void);
	void MarkAsDefaultPreset(void);
	void Clear(void);
	void CopyPresetFrom(CGraphCtrl* pCtrl);
protected:
public:
	void SetLegendRect(LPRECT lpRect);
protected:
	int GetGraphFieldRightPos(void);
public:
	void Recenter(void);
	void AddSerieData(GRAPHPOINT pt, int nSerieIndex, BOOL redraw = TRUE);
	void SetLineStyle(int width, COLORREF color, GRAPH_LINESTYLE style);
	CString GetXTicksLabelText(double x);
	CString GetYTicksLabelText(double y);
protected:
	double CalculateGridSize(double& min, double& max, double defaultSize, double screenSize, double gridChanging, double graphChanging);
public:
	double GetStartX(void);
	double GetStartY(void);
	double GetEndX(void);
	double GetEndY(void);
	BOOL m_Symmetric;
	double m_DefaultXGridSize;
	double m_DefaultYGridSize;
	double m_GridXChanging;
	double m_GridYChanging;
	int m_SubXGridsCount;
	int m_SubYGridsCount;
	double m_GraphXChanging;
	double m_GraphYChanging;
	double GetXSubGridSize(void);
	double GetYSubGridSize(void);
	double GetXGridSize(void);
	double GetYGridSize(void);
	int m_LeftBorder;
	int m_RightBorder;
	int m_TopBorder;
	int m_BottomBorder;

	CSize GetGraphSize(void);
	void SetPrintingDC(CDC* pDC);
	int CalcXDistance(int value);
	int CalcYDistance(int value);
	int CalcLineWidth(int value);
	double CalcLineWidth(double value);
};


