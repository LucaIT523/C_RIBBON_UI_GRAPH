#pragma once

#define LABELSTOBORDER	5
#define DEFAULTXGRIDSIZE 40
#define DEFAULTYGRIDSIZE 30
#define TRANSPARENTCOLOR RGB(255, 255, 255)
#define MAXDOUBLE (1.7e300)
#define MINDOUBLE (-1.7e300)

#define PRESETS_ROOT_NODE				_T("graph-preset")
#define TITLES_NODE						_T("titles")
#define TITLE_ATTRIBUTE					_T("title")
#define SUBTITLE_ATTRIBUTE				_T("subtitle")
#define LABELS_NODE						_T("labels")
#define XLABEL_ATTRIBUTE				_T("xlabel")
#define YLABEL_ATTRIBUTE				_T("ylabel")
#define XTICKSLABEL_ATTRIBUTE			_T("xtickslabel")
#define YTICKSLABEL_ATTRIBUTE			_T("ytickslabel")
#define XSUBTICKSLABEL_ATTRIBUTE		_T("xsubtickslabel")
#define YSUBTICKSLABEL_ATTRIBUTE		_T("ysubtickslabel")
#define ZEROLABELS_ATTRIBUTE			_T("zerolabels")
#define GRIDS_NODE						_T("grids")
#define GRID_OPTIONS_NODE				_T("grid")
#define SUBGRID_OPTIONS_NODE			_T("subgrid")
#define SHOW_ATTRIBUTE					_T("show")
#define XSIZE_ATTRIBUTE					_T("xsize")
#define YSIZE_ATTRIBUTE					_T("ysize")
#define XTICKS_ATTRIBUTE				_T("xticks")
#define YTICKS_ATTRIBUTE				_T("yticks")
#define COLOR_ATTRIBUTE					_T("color")
#define MARKER_COLOR_ATTRIBUTE			_T("markercolor")
#define XSTABLE_ATTRIBUTE				_T("xstable")
#define YSTABLE_ATTRIBUTE				_T("ystable")
#define XDEFAULT_ATTRIBUTE				_T("xdefault")
#define YDEFAULT_ATTRIBUTE				_T("ydefault")
#define LEGEND_NODE						_T("legend")
#define OPTIONS_NODE					_T("options")
#define BACKGROUND_NODE					_T("background")
#define BORDERWIDTH_ATTRIBUTE			_T("borderwidth")
#define STYLE_ATTRIBUTE					_T("style")
#define GRADIENT_ATTRIBUTE				_T("gradient")
#define COLOR1_ATTRIBUTE				_T("color1")
#define COLOR2_ATTRIBUTE				_T("color2")
#define FONTS_NODE						_T("fonts")
#define TITLE_NODE						_T("title")
#define SUBTITLE_NODE					_T("subtitle")
#define XLABEL_NODE						_T("xlabel")
#define YLABEL_NODE						_T("ylabel")
#define XTICKSLABEL_NODE				_T("xtickslabel")
#define YTICKSLABEL_NODE				_T("ytickslabel")
#define NAME_ATTRIBUTE					_T("name")
#define SIZE_ATTRIBUTE					_T("size")
#define BOLD_ATTRIBUTE					_T("bold")
#define ITALIC_ATTRIBUTE				_T("italic")
#define UNDERLINE_ATTRIBUTE				_T("underline")
#define STRIKEOUT_ATTRIBUTE				_T("strikeout")
#define ANGLE_ATTRIBUTE					_T("angle")
#define SERIES_NODE						_T("series")
#define SERIE_NODE						_T("serie")
#define XMIN_ATTRIBUTE					_T("xmin")
#define YMIN_ATTRIBUTE					_T("ymin")
#define XMAX_ATTRIBUTE					_T("xmax")
#define YMAX_ATTRIBUTE					_T("ymax")
#define INTELLIGENT_ATTRIBUTE			_T("intelligent")
#define LINE_WIDTH_ATTRIBUTE			_T("linewidth")
#define POINTSIZE_ATTRIBUTE				_T("pointsize")
#define COMMON_NODE						_T("common")
#define ANTIALIASING_ATTRIBUTE			_T("antialiasing")
#define MARKER_STYLE_ATTRIBUTE			_T("markerstyle")
#define MARKER_SIZE_ATTRIBUTE			_T("markersize")
#define AUTOPOS_ATTRIBUTE				_T("autopos")
#define FLOATING_ATTRIBUTE				_T("floating")
#define POSITION_NODE					_T("position")
#define LEFT_ATTRIBUTE					_T("left")
#define RIGHT_ATTRIBUTE					_T("right")
#define BOTTOM_ATTRIBUTE				_T("bottom")
#define TOP_ATTRIBUTE					_T("top")
#define VISIBLE_ATTRIBUTE				_T("visible")
#define LEGEND_VISIBLE_ATTRIBUTE		_T("legend")
#define LINE_STYLE_ATTRIBUTE			_T("linestyle")
#define SYMMETRIC_ATTRIBUTE				_T("symmetric")
#define GRAPHX_CHANGING_ATTRIBUTE		_T("graphx_changing")
#define GRAPHY_CHANGING_ATTRIBUTE		_T("graphy_changing")
#define GRIDX_CHANGING_ATTRIBUTE		_T("gridx_changing")
#define GRIDY_CHANGING_ATTRIBUTE		_T("gridy_changing")
#define XSUBGRIDS_COUNT_ATTRIBUTE		_T("xcount")
#define YSUBGRIDS_COUNT_ATTRIBUTE		_T("ycount")
#define MARKERS_COUNT_ATTRIBUTE			_T("markerscount")
#define MARGINS_NODE					_T("margins")
#define LEFT_ATTRIBUTE					_T("left")
#define RIGHT_ATTRIBUTE					_T("right")
#define TOP_ATTRIBUTE					_T("top")
#define BOTTOM_ATTRIBUTE				_T("bottom")
#define BORDER_COLOR_ATTRIBUTE			_T("border-color")
#define ADDITIONAL_GRIDS_NODE			_T("additional-grids")
#define LABELS_FORMAT_NODE				_T("labels-format")
#define XFORMAT_ATTRIBUTE				_T("xformat")
#define YFORMAT_ATTRIBUTE				_T("yformat")
#define XFORMAT_ADDITIONAL_ATTRIBUTE	_T("xprecision")
#define YFORMAT_ADDITIONAL_ATTRIBUTE	_T("yprecision")

#define DATA_ROOT_NODE					_T("graph-data")
#define DATA_GRAPH_ROOT_NODE			_T("graph")
#define DATA_NODE						_T("data")
#define POINT_NODE						_T("point")
#define X_ATTRIBUTE						_T("x")
#define Y_ATTRIBUTE						_T("y")

#define APPLICATION_TITLE				_T("Graph")
#define RANGE_INT_ERROR_MESSAGE			_T("Please enter data between %d and %d")