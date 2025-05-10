#include "stdafx.h"
#include <math.h>
#include "..\include\tools.h"
#include "..\include\constants.h"
#include <string.h>
#include <comutil.h>
#include <limits.h>

double NICE_POINTS[] = {.20, .25, .40, .50, .60, .75, .80};

double _round(double data, int precision)
{
	if (precision < 0) return 0;
	double res = data;
	res *= pow(10.0, precision);
	if (res >= floor(res) + 0.5)
		res = make_int64(ceil(res));
	else
		res = make_int64(floor(res));

	res /= pow(10.0, precision);

	return res;
}

double _ceil(double data, int precision)
{
	ASSERT(precision >= 0);
	double res = data;
	res *= pow(10.0, precision);
	res =  ceil(res);
	if (compare(res - 1, data) == 0) res--;
	res /= pow(10.0, precision);

	return res;
}

double _floor(double data, int precision)
{
	ASSERT(precision >= 0);
	double res = data;
	res *= pow(10.0, precision);
	res =  floor(res);
	res /= pow(10.0, precision);

	return res;
}

BOOL IsVariantINT(VARIANT* lpData)
{
	return 	lpData && (lpData->vt == VT_I2 || lpData->vt == VT_I4 || lpData->vt == VT_I8 ||
				lpData->vt == VT_UI1 || lpData->vt == VT_BOOL || lpData->vt == VT_I1 ||
				lpData->vt == VT_UI2 || lpData->vt == VT_UI4 || lpData->vt == VT_UI8 ||
				lpData->vt == VT_INT || lpData->vt == VT_UINT);
}

BOOL IsVariantDouble(VARIANT* lpData)
{
	return 	lpData && (lpData->vt == VT_I2 || lpData->vt == VT_I4 || lpData->vt == VT_I8 ||
				lpData->vt == VT_UI1 || lpData->vt == VT_I1 ||lpData->vt == VT_UI2 || 
				lpData->vt == VT_UI4 || lpData->vt == VT_UI8 || lpData->vt == VT_INT || 
				lpData->vt == VT_UINT || lpData->vt == VT_R4 || lpData->vt == VT_R8);
}

void FONTtoLOGFONT(FONT& font, LOGFONT& logfont, HDC hdc)
{
	int caps = GetDeviceCaps(hdc, LOGPIXELSY);
	logfont.lfHeight = -MulDiv(font.size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
//	logfont.lfHeight = -16;
	logfont.lfWidth = 0;
	logfont.lfEscapement = font.angle * 10;
	logfont.lfOrientation = font.angle * 10;
	logfont.lfWeight = font.bold ? FW_BOLD : FW_NORMAL;
	logfont.lfItalic = font.italic;
	logfont.lfUnderline = font.underline;
	logfont.lfStrikeOut = font.strikeout;
	logfont.lfCharSet = ANSI_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	logfont.lfQuality = ANTIALIASED_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH;
	_tcscpy(logfont.lfFaceName, font.name);
}

void LOGFONTtoFONT(LOGFONT& logfont, FONT& font, HDC hdc)
{
	font.size = -MulDiv(logfont.lfHeight, 72, GetDeviceCaps(hdc, LOGPIXELSY));
	font.angle = logfont.lfEscapement / 10;
	font.bold = logfont.lfWeight >= FW_BOLD;
	font.italic = logfont.lfItalic;
	font.underline = logfont.lfUnderline;
	font.strikeout = logfont.lfStrikeOut;
	_tcscpy(font.name, logfont.lfFaceName);
}

int make_int(double value)
{
	if (value > INT_MAX)
		return INT_MAX;
	else if (value < INT_MIN)
		return INT_MIN;
	else return (int)value;
}

__int64 make_int64(double value)
{
	if (value > _I64_MAX)
		return _I64_MAX;
	else if (value < _I64_MIN)
		return _I64_MIN;
	else return (__int64)value;
}

int compare(double val1, double val2, int precision)
{

if (val1 == val2)
return 0;
else if (val1 > val2)
return 1;
else
return -1;

// below code commented out, Feb 07, 2008 after contacting Dubravko when 1e-5 data was not visible.
/*
	if (val1 == val2)
		return 0;
	__int64 intVal1 = make_int64(_round(val1 * pow(10.0, precision), 0));
	__int64 intVal2 = make_int64(_round(val2 * pow(10.0, precision), 0));
	if (intVal1 > intVal2) return 1;
	else if (intVal1 < intVal2) return -1;
	else return 0;

*/
}

BOOL CheckIntMinMax(int min, int max, int value, CDialog* pDialog, CWnd* pCtrl)
{
	if (value < min || value > max)
	{
		CString str;
		str.Format(RANGE_INT_ERROR_MESSAGE, min, max);
		MessageBox(0, str, APPLICATION_TITLE, MB_ICONSTOP | MB_OK);
		pDialog->GotoDlgCtrl(pCtrl);
		return FALSE;
	}
	else return TRUE;
}

double DegreesToRadians(double angle)
{
	double pi = 3.141592654;
	return (pi * 2 / 360 * angle);
}

int _get_exp(double value, double alt_value)
{
	int exp = 0;
	if (compare(value, 0) != 0)
		exp = make_int(floor(log10(fabs(value))));
	else exp = make_int(floor(log10(fabs(alt_value))));
	return exp;
}

double _nice_start(double start, double end, double step)
{
	double _pow = pow(10.0, _get_exp(fabs(end-start)));
	double _value;
	int n = 0;
	do
	{
		_value = floor(start / _pow) * _pow;
		if (_value < start)
			_value += _pow;
		switch(n%4)
		{
		case 0:
			//0.5
			_pow /= 2;
			break;
		case 1:
			//0.25
			_pow /= 2;
			break;
		case 2:
			//0.2
			_pow *= 4;
			_pow /= 5;
			break;
		default:
			//0.1
			_pow /= 2;
			break;
		}
		n++;
	}
	while(_value > end);
	while (_value >= start)
		_value -= step;

	return _value + step;
}

double _floor_nice(double value, int precision)
{
	if (compare(value, 0) == 0)
		return 0;
	else
	{
		double _pow = pow(10.0, _get_exp(value) + precision);
		double _value = value / _pow;
		double add = fabs(floor(_value) - _value);
		_value = floor(_value);

		int count = sizeof(NICE_POINTS) / sizeof(NICE_POINTS[0]);
		for (int i = 0; i < count; i++)
			if (compare(add, NICE_POINTS[i]) < 0)
			{
				if (i == 0) return _value * _pow;
				else return (_value + NICE_POINTS[i - 1]) * _pow;
			}
		return (_value + NICE_POINTS[count - 1]) * _pow;
	}
}

double _ceil_nice(double value, int precision)
{
	if (compare(value, 0) == 0)
		return 0;
	else
	{
		double _pow = pow(10.0, _get_exp(value) + precision);
		double _value = value / _pow;
		double add = 1 - fabs(ceil(_value) - _value);
		_value = ceil(_value);

		int count = sizeof(NICE_POINTS) / sizeof(NICE_POINTS[0]);
		for (int i = 0; i < count; i++)
			if (compare(add, NICE_POINTS[i]) <= 0)
				return (_value - (1 - NICE_POINTS[i])) * _pow;
		return (_value) * _pow;
	}
}

double _log10_nice(double value)
{
	return value>0.0?log10(value):MINDOUBLE;
}
BOOL VariantToDouble(VARIANT *pVar)
{
	if(IsVariantDouble(pVar))
		return TRUE;

	if(pVar->vt == VT_BSTR)
	{
		_bstr_t pStr = pVar->bstrVal;
		double dVal =	_tstof(pStr);
		//delete[] pStr;
		::SysFreeString(pVar->bstrVal);
		pVar->dblVal =	dVal;
		pVar->vt =	VT_R8;
		return TRUE;
	}

	return SUCCEEDED(VariantChangeType(pVar, pVar, 0, VT_R8));
}
