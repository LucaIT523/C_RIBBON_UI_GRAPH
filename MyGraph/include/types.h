#pragma once

#if defined(_USRDLL)
#define DLL_EXP __declspec(dllexport)
#elif defined(_MSC_VER)&&(_MSC_VER<1200)
#define DLL_EXP __declspec(dllimport)
#else
#define DLL_EXP
#endif

#define WM_COORDINATES_UPDATE	(WM_USER + 1001)
#define WM_LISTVIEW_UPDATE		(WM_USER + 1002)
#define UM_SHOW_FFT       		(WM_USER + 1003)
#define WM_USER_GRAPH_GRAPHCLOSED	(WM_USER + 0x117)  // From Samarium, Sam_Messages.h, Sep 03 2023




enum GRAPH_GRADIENTTYPE {GR_NONE = 0, GR_LEFTTORIGHT, GR_RIGHTTOLEFT, GR_TOPTOBOTTOM,
		GR_BOTTOMTOTOP, GR_LEFTTOPTORIGHTBOTTOM, GR_LEFTBOTTOMTORIGHTOP,
		GR_RIGHTBOTTOMTOLEFTTOP, GR_RIGHTTOPTOLEFTBOTTOM};

enum GRAPH_FILLTYPE {GRAPH_FT_NONE = 0, GRAPH_FT_SOLID, GRAPH_FT_GRADIENT};
enum GRAPH_GRAPHSTYLE {GS_LINES = 1, GS_SCATTER, GS_STEM};
enum GRAPH_MARKERSTYLE {MS_NONE = 0, MS_SQUARE, MS_TRIANGLE, MS_TRIANGLEDOWN, MS_STAR, MS_CIRCLE, MS_RHOMB, MS_SQUARE_E, MS_TRIANGLE_E, MS_TRIANGLEDOWN_E, MS_STAR_E, MS_CIRCLE_E, MS_RHOMB_E, MS_PLUS, MS_X, MS_ARROWUP, MS_ARROWDOWN};
enum GRAPH_LINESTYLE {LS_SOLID = 0, LS_DOT, LS_DOT3, LS_DASH, LS_DASHLARGE, LS_DASHDOT, LS_DASHDOTDOT};
enum GRAPH_ZOOMMODE {ZM_NONE = 0, ZM_ZOOM, ZM_UNZOOM, ZM_100, ZM_PAN, ZM_CROSS};
enum GRAPH_GRAPHCONTROL {GC_NONE = 0, GC_BACKGROUND, GC_GRAPH, GC_LEGEND};
enum GRAPH_LABELSFORMAT {LF_INT = 0, LF_DOUBLE, LF_EXP, LF_LOG};

struct GRAPHPOINT
{
	double x;
	double y;
	GRAPHPOINT()
	{
		x = 0;
		y = 0;
	}
	GRAPHPOINT(double _x, double _y)
	{
		x = _x;
		y = _y;
	}
	GRAPHPOINT(GRAPHPOINT& pt)
	{
		x = pt.x;
		y = pt.y;
	}
};

struct GRAPHSERIES
{
	int count;
	TCHAR lpName[256];
	COLORREF color;
	COLORREF markercolor;
	GRAPH_MARKERSTYLE marker;
	int markersize;
	int markers;
	BOOL visible;
	BOOL legend;
	GRAPH_LINESTYLE line;
	int linewidth;
	GRAPHPOINT* pData;
	GRAPHSERIES()
	{
		count = 0;
		lpName[0] = 0;
		color = RGB(0, 0, 0);
		markercolor = RGB(0, 0, 0);
		marker = MS_CIRCLE;
		markersize = 15;
		line = LS_SOLID;
		linewidth = 1;
		visible = TRUE;
		legend = TRUE;
		pData = NULL;
		markers = 100;
	}
	GRAPHSERIES(GRAPHSERIES& gs)
	{
		count = gs.count;
		_tcscpy_s(lpName, gs.lpName);
//		_wcscpy_s(lpName, gs.lpName);
		color = gs.color;
		markercolor = gs.markercolor;
		marker = gs.marker;
		markersize = gs.markersize;
		visible = gs.visible;
		legend = gs.legend;
		line = gs.line;
		linewidth = gs.linewidth > 0 ? gs.linewidth : 1;
		pData = new GRAPHPOINT[gs.count];
		memcpy(pData, gs.pData, gs.count * sizeof(GRAPHPOINT));
		markers = gs.markers;
	}
};

struct GRAPHSIZE
{
	double min;
	double max;
	GRAPHSIZE()
	{
		min = 0;
		max = 0;
	}
	GRAPHSIZE(double _min, double _max)
	{
		min = _min;
		max = _max;
	}
};

struct BACKGROUND
{
	GRAPH_FILLTYPE Type;
	GRAPH_GRADIENTTYPE grType;
	COLORREF Color1;
	COLORREF Color2;
	BACKGROUND()
	{
		Type = GRAPH_FT_NONE;
		grType = GR_NONE;
		Color1 = RGB(0, 0, 0);
		Color2 = RGB(0, 0, 0);
	}
	BACKGROUND(GRAPH_FILLTYPE type, GRAPH_GRADIENTTYPE grtype, COLORREF color1, COLORREF color2)
	{
		Type = type;
		grType = grtype;
		Color1 = color1;
		Color2 = color2;
	}
	BACKGROUND(BACKGROUND& bg)
	{
		Type = bg.Type;
		grType = bg.grType;
		Color1 = bg.Color1;
		Color2 = bg.Color2;
	}
};

struct FONT
{
	int size;
	BOOL bold;
	BOOL italic;
	BOOL underline;
	BOOL strikeout;
	int angle;
	COLORREF color;
	TCHAR name[256];
	FONT(int _size, BOOL _bold, BOOL _italic, BOOL _underline, BOOL _strikeout, LPCTSTR _name, int _angle = 0, COLORREF _color = RGB(0, 0, 0))
	{
		size = _size;
		bold = _bold;
		italic = _italic;
		underline = _underline;
		strikeout = _strikeout;
		_tcscpy_s(name, _name);
		angle = _angle;
		color = _color;
	}
	FONT()
	{
		size = 8;
		bold = FALSE;
		italic = FALSE;
		underline = FALSE;
		strikeout = FALSE;
		_tcscpy_s(name, _T("Arial"));
		angle = 0;
		color = RGB(0, 0, 0);
	}
	FONT(FONT& font)
	{
		size = font.size;
		bold = font.bold;
		italic = font.italic;
		underline = font.underline;
		strikeout = font.strikeout;
		_tcscpy_s(name, font.name);
		angle = font.angle;
		color = font.color;
	}
};

struct FONTDRAWDATA
{
	int left;
	int right;
	int top;
	int bottom;
	int centerx;
	int centery;
	TCHAR text[256];
	FONT font;
	CDC* dc;
};

class CGraphCtrl;

struct LMOUSEDOWNINFO
{
	CPoint point;
	CGraphCtrl* graph;
	GRAPH_GRAPHCONTROL clickedon;
	CPoint graphPoint;
	CPoint controlPoint;
	LMOUSEDOWNINFO()
	{
		point = CPoint(0, 0);
		graph = NULL;
		clickedon = GC_NONE;
		graphPoint = CPoint(0, 0);
		controlPoint = CPoint(0, 0);
	}
};

struct DocType
{
public:
        int nID;
        BOOL bRead;
        BOOL bWrite;
        LPCTSTR description;
        LPCTSTR ext;
};

