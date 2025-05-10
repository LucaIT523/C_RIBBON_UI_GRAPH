#pragma once

#include "types.h"

extern double _round(double data, int precision = 0);
extern double _ceil(double data, int precision = 0);
extern double _floor(double data, int precision = 0);
extern BOOL IsVariantINT(VARIANT* lpData);
extern BOOL IsVariantDouble(VARIANT* lpData);
extern void FONTtoLOGFONT(FONT& font, LOGFONT& logfont, HDC hdc);
extern void LOGFONTtoFONT(LOGFONT& logfont, FONT& font, HDC hdc);
extern int compare(double val1, double val2, int precision = 4);
extern BOOL CheckIntMinMax(int min, int max, int value, CDialog* pDialog, CWnd* pCtrl);
extern double DegreesToRadians(double angle);
extern int _get_exp(double value, double alt_value = 1);
extern double _nice_start(double start, double end, double step);
extern double _floor_nice(double value, int precision = 0);
extern double _ceil_nice(double value, int precision = 0);
extern int make_int(double value);
extern __int64 make_int64(double value);
extern double _log10_nice(double value);
extern BOOL VariantToDouble(VARIANT *pVar);

