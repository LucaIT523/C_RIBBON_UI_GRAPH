// Graph.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\constants.h"
#include "..\include\graph.h"
#include "..\include\GraphView.h"
#include "..\MainFrm.h"
#include "..\include\graphctrl.h"
#include "..\include\tools.h"
#include <math.h>
#include <comutil.h>
#include "afxcoll.h"

// CGraph

CGraph::CGraph(HWND hParentWnd/* = NULL*/)
: m_pGraphWnd(NULL)
, m_ZoomFactor(2)
, m_MaxZoomFactor(256)
, m_MinZoomFactor(1)
, m_pDefaultTitle(NULL)
, m_hParentWnd(hParentWnd)
, m_bFFTButtonEnabled(TRUE)
, bSendClosingMessage(0)
{
	::CoInitialize(NULL);

	ppGraphAlive	=	NULL;
}

CGraph::~CGraph()
{
	bWindowOpen = 0;
	bSendClosingMessage = 0;
	ppGraphAlive = NULL;

	while (m_Graphs.GetCount() > 0)
	{
		CGraphCtrl* pGraph = (CGraphCtrl*)m_Graphs.RemoveHead();
		delete pGraph;
	}

	if (m_pGraphWnd) delete m_pGraphWnd;
	if (m_pDefaultTitle) free(m_pDefaultTitle);


	ppGraphAlive = NULL;

	::CoUninitialize();
}
// CGraph member functions


void CGraph::CloseGraph()
{
	if (m_pGraphWnd) m_pGraphWnd->SendMessage(WM_CLOSE);

	bWindowOpen = 0;
	bSendClosingMessage = 0;
	ppGraphAlive = NULL;
	if (m_hParentWnd)
	{
	}
}


//
//BOOL CGraph::Create(LPRECT lpInitRect, LPCTSTR lpWindowTitle /*  CGraph **ppBackPtr  */)
//{
////	ppGraphAlive = ppBackPtr;
//
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	if (m_pGraphWnd) return TRUE;
//
//
//	m_pGraphWnd = new CGraphWnd(this);
//	if (!m_pGraphWnd->LoadFrame(IDR_GRAPHFRAME, WS_TILEDWINDOW))
//		return FALSE;
//
////  m_pGraphWnd->ModifyStyleEx(WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE, WS_EX_TOOLWINDOW, 0);
//
//	TCHAR oldTitle[1024];
//	m_pGraphWnd->GetWindowText(oldTitle, 1024);
//	if (m_pDefaultTitle) free(m_pDefaultTitle);
//	m_pDefaultTitle = _tcsdup(oldTitle);
//
//	if (lpWindowTitle)
//		SetWindowTitle(lpWindowTitle);
//	if (lpInitRect)
//		m_pGraphWnd->MoveWindow(lpInitRect);
//
//	return TRUE;
//}

BOOL CGraph::GetGraphWindowVisible()
{
	if (!m_pGraphWnd)
	{
		bWindowOpen = 0;
		return FALSE;
	}
	if (m_pGraphWnd->IsWindowVisible())
	{
		bWindowOpen = 1;
		return TRUE;
	}
	return FALSE;
}


BOOL CGraph::Show(LPCTSTR lpWindowTitle)
{
	if (!m_pGraphWnd) return FALSE;

	bWindowOpen = 1;

	for (int i = 0; i < GetGraphsCount(); i++)
		GetGraphCtrl(i)->Prepare();

	if (lpWindowTitle)
		SetWindowTitle(lpWindowTitle);

	m_pGraphWnd->ShowWindow(SW_SHOW);
	m_pGraphWnd->m_pView->Resize();
	if (m_pGraphWnd->IsWindowVisible())
		m_pGraphWnd->m_pView->Invalidate();

	return TRUE;
}

int CGraph::GetGraphsCount(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (int)m_Graphs.GetCount();
}

CGraphCtrl* CGraph::GetGraphCtrl(int index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	POSITION pos = m_Graphs.FindIndex(index);
	if (pos)
		return (CGraphCtrl*)m_Graphs.GetAt(pos);
	else return NULL;
}

void CGraph::DeleteGraphCtrl(int index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	POSITION pos = m_Graphs.FindIndex(index);
	if (pos)
	{
		CGraphCtrl* pCtrl = (CGraphCtrl*)m_Graphs.GetAt(pos);
		m_Graphs.RemoveAt(pos);
		delete pCtrl;
	}
}

CGraphCtrl* CGraph::AddGraphCtrl(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (GetGraphsCount() == 4) return NULL;

	CGraphCtrl* pGraph = new CGraphCtrl(this);
	m_Graphs.AddTail(pGraph);
//	if (m_pGraphWnd && m_pGraphWnd->m_pView)
//		m_pGraphWnd->m_pView->Resize();
	return pGraph;
}

void CGraph::GetGraphRect(int index, LPRECT rect, LPRECT total)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (GetGraphsCount())
	{
	case 1: CopyRect(rect, total); break;
	case 2: 
		{
			rect->top = index * (int)ceil(double(total->bottom - total->top) / 2);
			rect->bottom = (index + 1) * (int)ceil(double(total->bottom - total->top) / 2) - 1;
			rect->left = total->left;
			rect->right = total->right;
		} break;
	case 3:
		{
			if (index == 2)
			{
				rect->top = 1 * (int)ceil(double(total->bottom - total->top) / 2);
				rect->bottom = 2 * (int)ceil(double(total->bottom - total->top) / 2) - 1;
				rect->left = total->left;
				rect->right = total->right;
			}
			else
			{
				rect->top = 0;
				rect->bottom = (int)ceil(double(total->bottom - total->top) / 2) - 1;
				rect->left = index * (int)ceil(double(total->right - total->left) / 2);
				rect->right = (index + 1) * (int)ceil(double(total->right - total->left) / 2) - 1;
			}
		} break;
	case 4:
	default: 
		{
			if (index < 4)
			{
				int y = (index < 2) ? 0 : 1;
				int x = (index == 0 || index == 2) ? 0 : 1;
				rect->top = y * (int)ceil(double(total->bottom - total->top) / 2);
				rect->bottom = (y + 1) * (int)ceil(double(total->bottom - total->top) / 2) - 1;
				rect->left = x * (int)ceil(double(total->right - total->left) / 2);
				rect->right = (x + 1) * (int)ceil(double(total->right - total->left) / 2) - 1;
			}
			else SetRectEmpty(rect);
		} break;
	}
}

void CGraph::GetGraphRect(CGraphCtrl* pGraph, LPRECT rect, LPRECT total)
{
	for (int i = 0; i < GetGraphsCount(); i++)
		if (GetGraphCtrl(i) == pGraph)
		{
			GetGraphRect(i, rect, total); 
			return;
		}
}

CGraphCtrl* CGraph::GetGraphCtrlForPoint(CPoint point, CPoint* localPoint)
{
	for (int i = 0; i < GetGraphsCount(); i++)
	{
		CRect rect, totalRect;
		m_pGraphWnd->m_pView->GetClientRect(totalRect);
		GetGraphRect(i, &rect, totalRect);
		if (rect.PtInRect(point))
		{
			if (localPoint)
				*localPoint = point, localPoint->Offset(-rect.left, -rect.top);
			return GetGraphCtrl(i);
		}
	}

	return NULL;
}

void CGraph::SendClosingMessage()
{
//	LRESULT SendMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	SendMessage(m_hParentWnd, WM_USER_GRAPH_GRAPHCLOSED, (WPARAM)ppGraphAlive, (LPARAM)NULL);
}


void CGraph::GraphWndDestroyed(void)
{
	m_pGraphWnd = NULL;
	bWindowOpen = 0;
	bSendClosingMessage = 0;
	ppGraphAlive = NULL;
}

void CGraph::GetRect(CRect* lpRect)
{
	ASSERT(lpRect);
	if (m_pGraphWnd)
		m_pGraphWnd->GetWindowRect(*lpRect);
	else lpRect->SetRect(0, 0, 0, 0);
}

BOOL CGraph::SavePreset(LPCTSTR lpFileName, CGraphCtrl* pGraphCtrl)
{
	CGraphCtrl* pCtrl = pGraphCtrl;
	if (!pCtrl && GetGraphsCount() > 0)
		pCtrl = GetGraphCtrl(0);
	IXMLDOMDocument2* pDocument = CreateXMLDocument();
	if (pDocument)
	{
		BOOL result = SavePresetToXML(pDocument, NULL, pCtrl);
		if (result)
		{
			_variant_t file_name = lpFileName;
			result = SUCCEEDED(pDocument->save(file_name));
		}
		FreeXMLDocument(pDocument);
		return result;
	}
	else return FALSE;
}

BOOL CGraph::LoadPreset(LPCTSTR lpFileName, CGraphCtrl* pGraphCtrl)
{
	CGraphCtrl* pCtrl = pGraphCtrl;
	if (!pCtrl && GetGraphsCount() > 0)
		pCtrl = GetGraphCtrl(0);

	IXMLDOMDocument2* pDocument = CreateXMLDocument();
	if (pDocument)
	{
		BOOL result = TRUE;
		VARIANT_BOOL loaded;

		pDocument->put_async(VARIANT_FALSE);
		_variant_t file_name = lpFileName;
		result = SUCCEEDED(pDocument->load(file_name, &loaded)) && loaded;
		if (result)
			result = LoadPresetFromXML(pDocument, NULL, pCtrl);

		FreeXMLDocument(pDocument);
		return result;
	}
	else return FALSE;
}

BOOL CGraph::SaveGraph(LPCTSTR lpFileName, CGraphCtrl* pGraphCtrl)
{
	return 0;
}

BOOL CGraph::LoadGraph(LPCTSTR lpFileName, CGraphCtrl* pGraphCtrl)
{
	return 0;
}

BOOL CGraph::SaveGraphData(LPCTSTR lpFileName, CGraphCtrl* pGraphCtrl)
{
	CGraphCtrl* pCtrl;

	IXMLDOMDocument2* pDocument = CreateXMLDocument();
	if (pDocument)
	{
		
		IXMLDOMElement* pRoot;
		BOOL result = SUCCEEDED(pDocument->createElement(CComBSTR(DATA_ROOT_NODE), &pRoot)) && pRoot &&
					  SUCCEEDED(pDocument->putref_documentElement(pRoot));
		
		if (result)
		{
			for (int i = 0; i < GetGraphsCount(); i++)
			{
				if (pGraphCtrl)
					pCtrl = pGraphCtrl;
				else pCtrl = GetGraphCtrl(i);
				result = SaveGraphDataToXML(pDocument, pRoot, pCtrl);
				if (pGraphCtrl) break;
			}
			
			if (result)
			{
				_variant_t file_name = lpFileName;
				result = SUCCEEDED(pDocument->save(file_name));
			}
		}
		FreeXMLDocument(pDocument);
		return result;
	}
	else return FALSE;
}

BOOL CGraph::LoadGraphData(LPCTSTR lpFileName, CGraphCtrl* pGraphCtrl)
{
	CGraphCtrl* pCtrl;

	IXMLDOMDocument2* pDocument = CreateXMLDocument();
	if (pDocument)
	{
		BOOL result = TRUE;
		VARIANT_BOOL loaded;

		pDocument->put_async(VARIANT_FALSE);
		_variant_t file_name = lpFileName;
		result = SUCCEEDED(pDocument->load(file_name, &loaded)) && loaded;

		if (result)
		{
			CString searchStr;
			searchStr.Format(_T("/%s/%s"), DATA_ROOT_NODE, DATA_GRAPH_ROOT_NODE);
			IXMLDOMNodeList* pGraphs;
			result = SUCCEEDED(pDocument->selectNodes(CComBSTR(searchStr), &pGraphs)) && pGraphs;
			if (result)
			{
				long graphsCount;
				int srcCount = GetGraphsCount();
				result = SUCCEEDED(pGraphs->get_length(&graphsCount));
				if (result)
				{
					if (graphsCount > GetGraphsCount())
					{
						while (graphsCount > GetGraphsCount())
							AddGraphCtrl();
					}
					else 
					{
						while (graphsCount < GetGraphsCount())
							DeleteGraphCtrl(GetGraphsCount() - 1);
					}

					for (int i = 0; i < graphsCount; i++)
					{
						pCtrl = GetGraphCtrl(i);
						IXMLDOMNode* pGraphNode;
						result = SUCCEEDED(pGraphs->get_item(i, &pGraphNode)) && pGraphNode;
						if (result)
						{
							IXMLDOMElement* pGraphElement;
							result = SUCCEEDED(pGraphNode->QueryInterface(IID_IXMLDOMElement, (void**)&pGraphElement)) && pGraphElement;
							if (result)
							{
								result = LoadGraphDataFromXML(pDocument, pGraphElement, pCtrl);
								pCtrl->Recenter();
								pGraphElement->Release();
							}
						}
						if (!result) break;
					}
					if (srcCount < GetGraphsCount())
					{
						int index = 0;
						for (int i = srcCount; i < GetGraphsCount(); i++)
						{
							GetGraphCtrl(i)->CopyPresetFrom(GetGraphCtrl(index));
							index++; 
							if (index >= srcCount) index = 0;
						}
					}
				}
			}
		}

		FreeXMLDocument(pDocument);
		return result;
	}
	else return FALSE;
}

IXMLDOMDocument2* CGraph::CreateXMLDocument(void)
{
	IXMLDOMDocument2* pDocument = NULL;

	if (!SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, 
		IID_IXMLDOMDocument2, (void**)&pDocument))) pDocument = NULL;

	return pDocument;
}

void CGraph::FreeXMLDocument(IXMLDOMDocument2* pDocument)
{
	if (pDocument) pDocument->Release();
}

BOOL CGraph::Initialize(void)
{
	return SUCCEEDED(CoInitialize(NULL));
}

void CGraph::Uninitialize(void)
{
	CoUninitialize();
}

VARIANT CGraph::GetXMLAttribute(IXMLDOMElement* pRoot, LPCTSTR lpNode, LPCTSTR lpAttribute)
{
	VARIANT result;
	VariantInit(&result);
	IXMLDOMNode* pOptions;
	if (SUCCEEDED(pRoot->selectSingleNode(CComBSTR(lpNode), &pOptions)) && pOptions)
	{
		IXMLDOMElement* pOptionsElem;
		if (SUCCEEDED(pOptions->QueryInterface(IID_IXMLDOMElement, (void**)&pOptionsElem)))
		{
			pOptionsElem->getAttribute(CComBSTR(lpAttribute), &result);
			pOptionsElem->Release();
		}
	}
	
	return result;
}

void CGraph::ReadXMLIntAttribute(IXMLDOMElement* pRoot, LPCTSTR lpNode, LPCTSTR lpAttribute, int* lpData)
{
	VARIANT data = GetXMLAttribute(pRoot, lpNode, lpAttribute);
	if (IsVariantINT(&data) || SUCCEEDED(VariantChangeType(&data, &data, 0, VT_INT)))
		*lpData = _variant_t(data);
	else *lpData = 0;
}

void CGraph::ReadXMLDoubleAttribute(IXMLDOMElement* pRoot, LPCTSTR lpNode, LPCTSTR lpAttribute, double* lpData)
{
	VARIANT data = GetXMLAttribute(pRoot, lpNode, lpAttribute);
	if (IsVariantDouble(&data) || SUCCEEDED(VariantChangeType(&data, &data, 0, VT_R8)))
		*lpData = _variant_t(data);
	else *lpData = 0;
}

void CGraph::ReadXMLStringAttribute(IXMLDOMElement* pRoot, LPCTSTR lpNode, LPCTSTR lpAttribute, CString* lpData)
{
	VARIANT data = GetXMLAttribute(pRoot, lpNode, lpAttribute);
	if (data.vt == VT_BSTR || SUCCEEDED(VariantChangeType(&data, &data, 0, VT_BSTR)))
	{
		char* strData = _com_util::ConvertBSTRToString(data.bstrVal);
		*lpData = strData;
		delete[] strData;
	}
	else lpData->Empty();
}

BOOL CGraph::SavePresetToXML(IXMLDOMDocument2* pDocument, IXMLDOMElement* lpRoot, CGraphCtrl* pCtrl)
{
	if (pDocument)
	{
		BOOL result = TRUE;
		IXMLDOMElement* pRoot;
		if (SUCCEEDED(pDocument->createElement(CComBSTR(PRESETS_ROOT_NODE), &pRoot)))
		{
			if (lpRoot)
				result = SUCCEEDED(lpRoot->appendChild(pRoot, NULL));
			else 
				result = SUCCEEDED(pDocument->putref_documentElement(pRoot));
		}
		else result = FALSE;

		if (pRoot)
		{
			IXMLDOMElement* pTitles;
			if (SUCCEEDED(pDocument->createElement(CComBSTR(TITLES_NODE), &pTitles)))
			{
				pRoot->appendChild(pTitles, NULL);
				IXMLDOMElement* pShowOptions;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(OPTIONS_NODE), &pShowOptions)))
				{
					pTitles->appendChild(pShowOptions, NULL);
					_variant_t showTitle = pCtrl->m_ShowTitle;
					_variant_t showSubTitle = pCtrl->m_ShowSubTitle;
					pShowOptions->setAttribute(CComBSTR(TITLE_ATTRIBUTE), showTitle);
					pShowOptions->setAttribute(CComBSTR(SUBTITLE_ATTRIBUTE), showSubTitle);
				}
			}

			IXMLDOMElement* pLabels;
			if (SUCCEEDED(pDocument->createElement(CComBSTR(LABELS_NODE), &pLabels)))
			{
				pRoot->appendChild(pLabels, NULL);
				IXMLDOMElement* pShowOptions;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(OPTIONS_NODE), &pShowOptions)))
				{
					pLabels->appendChild(pShowOptions, NULL);
					_variant_t xLabel = pCtrl->m_ShowXLabel;
					_variant_t yLabel = pCtrl->m_ShowYLabel;
					_variant_t xTicksLabel = pCtrl->m_ShowXTicksLabels;
					_variant_t yTicksLabel = pCtrl->m_ShowYTicksLabels;
					_variant_t xSubTicksLabel = pCtrl->m_ShowXSubTicksLabels;
					_variant_t ySubTicksLabel = pCtrl->m_ShowYSubTicksLabels;
					_variant_t zeroLabel = pCtrl->m_DrawZeroLabels;
					pShowOptions->setAttribute(CComBSTR(XLABEL_ATTRIBUTE), xLabel);
					pShowOptions->setAttribute(CComBSTR(YLABEL_ATTRIBUTE), yLabel);
					pShowOptions->setAttribute(CComBSTR(XTICKSLABEL_ATTRIBUTE), xTicksLabel);
					pShowOptions->setAttribute(CComBSTR(YTICKSLABEL_ATTRIBUTE), yTicksLabel);
					pShowOptions->setAttribute(CComBSTR(XSUBTICKSLABEL_ATTRIBUTE), xSubTicksLabel);
					pShowOptions->setAttribute(CComBSTR(YSUBTICKSLABEL_ATTRIBUTE), ySubTicksLabel);
					pShowOptions->setAttribute(CComBSTR(ZEROLABELS_ATTRIBUTE), zeroLabel);
				}
			}

			IXMLDOMElement* pGrids;
			if (SUCCEEDED(pDocument->createElement(CComBSTR(GRIDS_NODE), &pGrids)))
			{
				pRoot->appendChild(pGrids, NULL);
				IXMLDOMElement* pGridOptions;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(GRID_OPTIONS_NODE), &pGridOptions)))
				{
					pGrids->appendChild(pGridOptions, NULL);
					_variant_t show = pCtrl->m_ShowGrid;
					_variant_t xTicks = pCtrl->m_ShowXTicks;
					_variant_t yTicks = pCtrl->m_ShowYTicks;
					_variant_t color = pCtrl->m_GridColor;
					_variant_t xStable = pCtrl->m_StableXGridCount;
					_variant_t yStable = pCtrl->m_StableYGridCount;
					_variant_t lineStyle = pCtrl->m_GridLine;
					_variant_t xsize = pCtrl->m_DefaultXGridSize;
					_variant_t ysize = pCtrl->m_DefaultYGridSize;
					_variant_t xgrid = pCtrl->m_GridXChanging;
					_variant_t ygrid = pCtrl->m_GridYChanging;

					pGridOptions->setAttribute(CComBSTR(SHOW_ATTRIBUTE), show);
					pGridOptions->setAttribute(CComBSTR(XTICKS_ATTRIBUTE), xTicks);
					pGridOptions->setAttribute(CComBSTR(YTICKS_ATTRIBUTE), yTicks);
					pGridOptions->setAttribute(CComBSTR(COLOR_ATTRIBUTE), color);
					pGridOptions->setAttribute(CComBSTR(XSTABLE_ATTRIBUTE), xStable);
					pGridOptions->setAttribute(CComBSTR(YSTABLE_ATTRIBUTE), yStable);
					pGridOptions->setAttribute(CComBSTR(LINE_STYLE_ATTRIBUTE), lineStyle);
					pGridOptions->setAttribute(CComBSTR(XSIZE_ATTRIBUTE), xsize);
					pGridOptions->setAttribute(CComBSTR(YSIZE_ATTRIBUTE), ysize);
					pGridOptions->setAttribute(CComBSTR(GRIDX_CHANGING_ATTRIBUTE), xgrid);
					pGridOptions->setAttribute(CComBSTR(GRIDY_CHANGING_ATTRIBUTE), ygrid);
				}
				IXMLDOMElement* pSubGridOptions;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(SUBGRID_OPTIONS_NODE), &pSubGridOptions)))
				{
					pGrids->appendChild(pSubGridOptions, NULL);
					_variant_t show = pCtrl->m_ShowSubGrid;
					_variant_t xTicks = pCtrl->m_ShowXSubTicks;
					_variant_t yTicks = pCtrl->m_ShowYSubTicks;
					_variant_t color = pCtrl->m_SubGridColor;
					_variant_t lineStyle = pCtrl->m_SubGridLine;
					_variant_t xcount = pCtrl->m_SubXGridsCount;
					_variant_t ycount = pCtrl->m_SubYGridsCount;

					pSubGridOptions->setAttribute(CComBSTR(SHOW_ATTRIBUTE), show);
					pSubGridOptions->setAttribute(CComBSTR(XTICKS_ATTRIBUTE), xTicks);
					pSubGridOptions->setAttribute(CComBSTR(YTICKS_ATTRIBUTE), yTicks);
					pSubGridOptions->setAttribute(CComBSTR(COLOR_ATTRIBUTE), color);
					pSubGridOptions->setAttribute(CComBSTR(LINE_STYLE_ATTRIBUTE), lineStyle);
					pSubGridOptions->setAttribute(CComBSTR(XSUBGRIDS_COUNT_ATTRIBUTE), xcount);
					pSubGridOptions->setAttribute(CComBSTR(YSUBGRIDS_COUNT_ATTRIBUTE), ycount);
				}
			}

			IXMLDOMElement* pLegend;
			if (SUCCEEDED(pDocument->createElement(CComBSTR(LEGEND_NODE), &pLegend)))
			{
				pRoot->appendChild(pLegend, NULL);
				IXMLDOMElement* pOptions;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(OPTIONS_NODE), &pOptions)))
				{
					pLegend->appendChild(pOptions, NULL);
					_variant_t show = pCtrl->m_ShowLegend;
					_variant_t borderWidth = pCtrl->m_LegendBorderWidth;
					_variant_t borderColor = pCtrl->m_LegendBorderColor;
					_variant_t autoPos = pCtrl->m_bAutoLegendPos;
					_variant_t floating = pCtrl->m_bLegendFloating;
					pOptions->setAttribute(CComBSTR(SHOW_ATTRIBUTE), show);
					pOptions->setAttribute(CComBSTR(BORDERWIDTH_ATTRIBUTE), borderWidth);
					pOptions->setAttribute(CComBSTR(BORDER_COLOR_ATTRIBUTE), borderColor);
					pOptions->setAttribute(CComBSTR(AUTOPOS_ATTRIBUTE), autoPos);
					pOptions->setAttribute(CComBSTR(FLOATING_ATTRIBUTE), floating);
				}
				IXMLDOMElement* pBackground;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(BACKGROUND_NODE), &pBackground)))
				{
					pLegend->appendChild(pBackground, NULL);
					_variant_t style = pCtrl->m_LegendBackGround.Type;
					_variant_t gradient = pCtrl->m_LegendBackGround.grType;
					_variant_t color1 = pCtrl->m_LegendBackGround.Color1;
					_variant_t color2 = pCtrl->m_LegendBackGround.Color2;
					pBackground->setAttribute(CComBSTR(STYLE_ATTRIBUTE), style);
					pBackground->setAttribute(CComBSTR(GRADIENT_ATTRIBUTE), gradient);
					pBackground->setAttribute(CComBSTR(COLOR1_ATTRIBUTE), color1);
					pBackground->setAttribute(CComBSTR(COLOR2_ATTRIBUTE), color2);
				}
				IXMLDOMElement* pRect;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(POSITION_NODE), &pRect)))
				{
					pLegend->appendChild(pRect, NULL);
					CRect legendRect; pCtrl->GetLegendRect(&legendRect);
					_variant_t left = legendRect.left;
					_variant_t right = legendRect.right;
					_variant_t top = legendRect.top;
					_variant_t bottom = legendRect.bottom;
					pRect->setAttribute(CComBSTR(LEFT_ATTRIBUTE), left);
					pRect->setAttribute(CComBSTR(RIGHT_ATTRIBUTE), right);
					pRect->setAttribute(CComBSTR(TOP_ATTRIBUTE), top);
					pRect->setAttribute(CComBSTR(BOTTOM_ATTRIBUTE), bottom);
				}
			}

			IXMLDOMElement* pFonts;
			if (SUCCEEDED(pDocument->createElement(CComBSTR(FONTS_NODE), &pFonts)))
			{
				pRoot->appendChild(pFonts, NULL);
				IXMLDOMElement* pTitle;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(TITLE_NODE), &pTitle)))
				{
					pFonts->appendChild(pTitle, NULL);
					FONT* pFont = &pCtrl->m_TitleFont;

					_variant_t name = pFont->name;
					_variant_t size = pFont->size;
					_variant_t bold = pFont->bold;
					_variant_t italic = pFont->italic;
					_variant_t underline = pFont->underline;
					_variant_t strikeout = pFont->strikeout;
					_variant_t angle = pFont->angle;
					_variant_t color = pFont->color;

					pTitle->setAttribute(CComBSTR(NAME_ATTRIBUTE), name);
					pTitle->setAttribute(CComBSTR(SIZE_ATTRIBUTE), size);
					pTitle->setAttribute(CComBSTR(BOLD_ATTRIBUTE), bold);
					pTitle->setAttribute(CComBSTR(ITALIC_ATTRIBUTE), italic);
					pTitle->setAttribute(CComBSTR(UNDERLINE_ATTRIBUTE), underline);
					pTitle->setAttribute(CComBSTR(STRIKEOUT_ATTRIBUTE), strikeout);
					pTitle->setAttribute(CComBSTR(ANGLE_ATTRIBUTE), angle);
					pTitle->setAttribute(CComBSTR(COLOR_ATTRIBUTE), color);
				}

				IXMLDOMElement* pSubTitle;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(SUBTITLE_NODE), &pSubTitle)))
				{
					pFonts->appendChild(pSubTitle, NULL);
					FONT* pFont = &pCtrl->m_SubTitleFont;

					_variant_t name = pFont->name;
					_variant_t size = pFont->size;
					_variant_t bold = pFont->bold;
					_variant_t italic = pFont->italic;
					_variant_t underline = pFont->underline;
					_variant_t strikeout = pFont->strikeout;
					_variant_t angle = pFont->angle;
					_variant_t color = pFont->color;

					pSubTitle->setAttribute(CComBSTR(NAME_ATTRIBUTE), name);
					pSubTitle->setAttribute(CComBSTR(SIZE_ATTRIBUTE), size);
					pSubTitle->setAttribute(CComBSTR(BOLD_ATTRIBUTE), bold);
					pSubTitle->setAttribute(CComBSTR(ITALIC_ATTRIBUTE), italic);
					pSubTitle->setAttribute(CComBSTR(UNDERLINE_ATTRIBUTE), underline);
					pSubTitle->setAttribute(CComBSTR(STRIKEOUT_ATTRIBUTE), strikeout);
					pSubTitle->setAttribute(CComBSTR(ANGLE_ATTRIBUTE), angle);
					pSubTitle->setAttribute(CComBSTR(COLOR_ATTRIBUTE), color);
				}

				IXMLDOMElement* pLegendFont;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(LEGEND_NODE), &pLegendFont)))
				{
					pFonts->appendChild(pLegendFont, NULL);
					FONT* pFont = &pCtrl->m_LegendFont;

					_variant_t name = pFont->name;
					_variant_t size = pFont->size;
					_variant_t bold = pFont->bold;
					_variant_t italic = pFont->italic;
					_variant_t underline = pFont->underline;
					_variant_t strikeout = pFont->strikeout;
					_variant_t angle = pFont->angle;
					_variant_t color = pFont->color;

					pLegendFont->setAttribute(CComBSTR(NAME_ATTRIBUTE), name);
					pLegendFont->setAttribute(CComBSTR(SIZE_ATTRIBUTE), size);
					pLegendFont->setAttribute(CComBSTR(BOLD_ATTRIBUTE), bold);
					pLegendFont->setAttribute(CComBSTR(ITALIC_ATTRIBUTE), italic);
					pLegendFont->setAttribute(CComBSTR(UNDERLINE_ATTRIBUTE), underline);
					pLegendFont->setAttribute(CComBSTR(STRIKEOUT_ATTRIBUTE), strikeout);
					pLegendFont->setAttribute(CComBSTR(ANGLE_ATTRIBUTE), angle);
					pLegendFont->setAttribute(CComBSTR(COLOR_ATTRIBUTE), color);
				}

				IXMLDOMElement* pXLabel;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(XLABEL_NODE), &pXLabel)))
				{
					pFonts->appendChild(pXLabel, NULL);
					FONT* pFont = &pCtrl->m_XLabelFont;

					_variant_t name = pFont->name;
					_variant_t size = pFont->size;
					_variant_t bold = pFont->bold;
					_variant_t italic = pFont->italic;
					_variant_t underline = pFont->underline;
					_variant_t strikeout = pFont->strikeout;
					_variant_t angle = pFont->angle;
					_variant_t color = pFont->color;

					pXLabel->setAttribute(CComBSTR(NAME_ATTRIBUTE), name);
					pXLabel->setAttribute(CComBSTR(SIZE_ATTRIBUTE), size);
					pXLabel->setAttribute(CComBSTR(BOLD_ATTRIBUTE), bold);
					pXLabel->setAttribute(CComBSTR(ITALIC_ATTRIBUTE), italic);
					pXLabel->setAttribute(CComBSTR(UNDERLINE_ATTRIBUTE), underline);
					pXLabel->setAttribute(CComBSTR(STRIKEOUT_ATTRIBUTE), strikeout);
					pXLabel->setAttribute(CComBSTR(ANGLE_ATTRIBUTE), angle);
					pXLabel->setAttribute(CComBSTR(COLOR_ATTRIBUTE), color);
				}

				IXMLDOMElement* pYLabel;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(YLABEL_NODE), &pYLabel)))
				{
					pFonts->appendChild(pYLabel, NULL);
					FONT* pFont = &pCtrl->m_YLabelFont;

					_variant_t name = pFont->name;
					_variant_t size = pFont->size;
					_variant_t bold = pFont->bold;
					_variant_t italic = pFont->italic;
					_variant_t underline = pFont->underline;
					_variant_t strikeout = pFont->strikeout;
					_variant_t angle = pFont->angle;
					_variant_t color = pFont->color;

					pYLabel->setAttribute(CComBSTR(NAME_ATTRIBUTE), name);
					pYLabel->setAttribute(CComBSTR(SIZE_ATTRIBUTE), size);
					pYLabel->setAttribute(CComBSTR(BOLD_ATTRIBUTE), bold);
					pYLabel->setAttribute(CComBSTR(ITALIC_ATTRIBUTE), italic);
					pYLabel->setAttribute(CComBSTR(UNDERLINE_ATTRIBUTE), underline);
					pYLabel->setAttribute(CComBSTR(STRIKEOUT_ATTRIBUTE), strikeout);
					pYLabel->setAttribute(CComBSTR(ANGLE_ATTRIBUTE), angle);
					pYLabel->setAttribute(CComBSTR(COLOR_ATTRIBUTE), color);
				}

				IXMLDOMElement* pXTicksLabel;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(XTICKSLABEL_NODE), &pXTicksLabel)))
				{
					pFonts->appendChild(pXTicksLabel, NULL);
					FONT* pFont = &pCtrl->m_TicksXLabelsFont;

					_variant_t name = pFont->name;
					_variant_t size = pFont->size;
					_variant_t bold = pFont->bold;
					_variant_t italic = pFont->italic;
					_variant_t underline = pFont->underline;
					_variant_t strikeout = pFont->strikeout;
					_variant_t angle = pFont->angle;
					_variant_t color = pFont->color;

					pXTicksLabel->setAttribute(CComBSTR(NAME_ATTRIBUTE), name);
					pXTicksLabel->setAttribute(CComBSTR(SIZE_ATTRIBUTE), size);
					pXTicksLabel->setAttribute(CComBSTR(BOLD_ATTRIBUTE), bold);
					pXTicksLabel->setAttribute(CComBSTR(ITALIC_ATTRIBUTE), italic);
					pXTicksLabel->setAttribute(CComBSTR(UNDERLINE_ATTRIBUTE), underline);
					pXTicksLabel->setAttribute(CComBSTR(STRIKEOUT_ATTRIBUTE), strikeout);
					pXTicksLabel->setAttribute(CComBSTR(ANGLE_ATTRIBUTE), angle);
					pXTicksLabel->setAttribute(CComBSTR(COLOR_ATTRIBUTE), color);
				}
				
				IXMLDOMElement* pYTicksLabel;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(YTICKSLABEL_NODE), &pYTicksLabel)))
				{
					pFonts->appendChild(pYTicksLabel, NULL);
					FONT* pFont = &pCtrl->m_TicksYLabelsFont;

					_variant_t name = pFont->name;
					_variant_t size = pFont->size;
					_variant_t bold = pFont->bold;
					_variant_t italic = pFont->italic;
					_variant_t underline = pFont->underline;
					_variant_t strikeout = pFont->strikeout;
					_variant_t angle = pFont->angle;
					_variant_t color = pFont->color;

					pYTicksLabel->setAttribute(CComBSTR(NAME_ATTRIBUTE), name);
					pYTicksLabel->setAttribute(CComBSTR(SIZE_ATTRIBUTE), size);
					pYTicksLabel->setAttribute(CComBSTR(BOLD_ATTRIBUTE), bold);
					pYTicksLabel->setAttribute(CComBSTR(ITALIC_ATTRIBUTE), italic);
					pYTicksLabel->setAttribute(CComBSTR(UNDERLINE_ATTRIBUTE), underline);
					pYTicksLabel->setAttribute(CComBSTR(STRIKEOUT_ATTRIBUTE), strikeout);
					pYTicksLabel->setAttribute(CComBSTR(ANGLE_ATTRIBUTE), angle);
					pYTicksLabel->setAttribute(CComBSTR(COLOR_ATTRIBUTE), color);
				}
			}

			IXMLDOMElement* pSeries;
			if (SUCCEEDED(pDocument->createElement(CComBSTR(SERIES_NODE), &pSeries)))
			{
				pRoot->appendChild(pSeries, NULL);
				IXMLDOMElement* pOptions;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(OPTIONS_NODE), &pOptions)))
				{
					pSeries->appendChild(pOptions, NULL);
					_variant_t style = pCtrl->m_GraphStyle;
					_variant_t pointSize = pCtrl->m_PointSize;
					_variant_t xmin = pCtrl->m_GraphXSize.min;
					_variant_t ymin = pCtrl->m_GraphYSize.min;
					_variant_t xmax = pCtrl->m_GraphXSize.max;
					_variant_t ymax = pCtrl->m_GraphYSize.max;
					_variant_t intelligent = pCtrl->m_IntelligentAxisScale;
					_variant_t antialiasing = pCtrl->m_AntiAlaising;
					_variant_t borderWidth = pCtrl->m_GraphBorderWidth;
					_variant_t borderColor = pCtrl->m_GraphBorderColor;
					_variant_t symmetric = pCtrl->m_Symmetric;
					_variant_t graphx = pCtrl->m_GraphXChanging;
					_variant_t graphy = pCtrl->m_GraphYChanging;

					pOptions->setAttribute(CComBSTR(STYLE_ATTRIBUTE), style);
					pOptions->setAttribute(CComBSTR(ANTIALIASING_ATTRIBUTE), antialiasing);
					pOptions->setAttribute(CComBSTR(BORDERWIDTH_ATTRIBUTE), borderWidth);
					pOptions->setAttribute(CComBSTR(BORDER_COLOR_ATTRIBUTE), borderColor);
					pOptions->setAttribute(CComBSTR(POINTSIZE_ATTRIBUTE), pointSize);
					pOptions->setAttribute(CComBSTR(XMIN_ATTRIBUTE), xmin);
					pOptions->setAttribute(CComBSTR(YMIN_ATTRIBUTE), ymin);
					pOptions->setAttribute(CComBSTR(XMAX_ATTRIBUTE), xmax);
					pOptions->setAttribute(CComBSTR(YMAX_ATTRIBUTE), ymax);
					pOptions->setAttribute(CComBSTR(INTELLIGENT_ATTRIBUTE), intelligent);
					pOptions->setAttribute(CComBSTR(SYMMETRIC_ATTRIBUTE), symmetric);
					pOptions->setAttribute(CComBSTR(GRAPHX_CHANGING_ATTRIBUTE), graphx);
					pOptions->setAttribute(CComBSTR(GRAPHY_CHANGING_ATTRIBUTE), graphy);
				}

				IXMLDOMElement* pAdditional;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(ADDITIONAL_GRIDS_NODE), &pAdditional)))
				{
					pSeries->appendChild(pAdditional, NULL);
					_variant_t minx = pCtrl->m_AdditionalGridSteps.left;
					_variant_t maxx = pCtrl->m_AdditionalGridSteps.right;
					_variant_t miny = pCtrl->m_AdditionalGridSteps.bottom;
					_variant_t maxy = pCtrl->m_AdditionalGridSteps.top;

					pAdditional->setAttribute(CComBSTR(XMIN_ATTRIBUTE), minx);
					pAdditional->setAttribute(CComBSTR(XMAX_ATTRIBUTE), maxx);
					pAdditional->setAttribute(CComBSTR(YMIN_ATTRIBUTE), miny);
					pAdditional->setAttribute(CComBSTR(YMAX_ATTRIBUTE), maxy);
				}

				IXMLDOMElement* pLabelsFormat;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(LABELS_FORMAT_NODE), &pLabelsFormat)))
				{
					pSeries->appendChild(pLabelsFormat, NULL);
					_variant_t xFormat = pCtrl->m_XLabelsFormat;
					_variant_t yFormat = pCtrl->m_YLabelsFormat;
					_variant_t xAdditional = pCtrl->m_XLabelsFormatAdditional;
					_variant_t yAdditional = pCtrl->m_YLabelsFormatAdditional;

					pLabelsFormat->setAttribute(CComBSTR(XFORMAT_ATTRIBUTE), xFormat);
					pLabelsFormat->setAttribute(CComBSTR(YFORMAT_ATTRIBUTE), yFormat);
					pLabelsFormat->setAttribute(CComBSTR(XFORMAT_ADDITIONAL_ATTRIBUTE), xAdditional);
					pLabelsFormat->setAttribute(CComBSTR(YFORMAT_ADDITIONAL_ATTRIBUTE), yAdditional);
				}

				IXMLDOMElement* pBackground;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(BACKGROUND_NODE), &pBackground)))
				{
					pSeries->appendChild(pBackground, NULL);
					_variant_t style = pCtrl->m_GraphFieldBackGround.Type;
					_variant_t gradient = pCtrl->m_GraphFieldBackGround.grType;
					_variant_t color1 = pCtrl->m_GraphFieldBackGround.Color1;
					_variant_t color2 = pCtrl->m_GraphFieldBackGround.Color2;
					pBackground->setAttribute(CComBSTR(STYLE_ATTRIBUTE), style);
					pBackground->setAttribute(CComBSTR(GRADIENT_ATTRIBUTE), gradient);
					pBackground->setAttribute(CComBSTR(COLOR1_ATTRIBUTE), color1);
					pBackground->setAttribute(CComBSTR(COLOR2_ATTRIBUTE), color2);
				}

				for (int i = 0; i < pCtrl->GetSeriesCount(); i++)
				{
					GRAPHSERIES* pSerieData = pCtrl->GetSerie(i);

					IXMLDOMElement* pSerie;
					if (SUCCEEDED(pDocument->createElement(CComBSTR(SERIE_NODE), &pSerie)))
					{
						pSeries->appendChild(pSerie, NULL);
						_variant_t color = pSerieData->color;
						_variant_t markercolor = pSerieData->markercolor;
						_variant_t markerStyle = pSerieData->marker;
						_variant_t markerSize = pSerieData->markersize;
						_variant_t lineStyle = pSerieData->line;
						_variant_t lineWidth = pSerieData->linewidth > 0 ? pSerieData->linewidth : 1;
						_variant_t markers = pSerieData->markers;

						pSerie->setAttribute(CComBSTR(MARKER_STYLE_ATTRIBUTE), markerStyle);
						pSerie->setAttribute(CComBSTR(MARKER_SIZE_ATTRIBUTE), markerSize);
						pSerie->setAttribute(CComBSTR(COLOR_ATTRIBUTE), color);
						pSerie->setAttribute(CComBSTR(MARKER_COLOR_ATTRIBUTE), markercolor);
						pSerie->setAttribute(CComBSTR(LINE_STYLE_ATTRIBUTE), lineStyle);
						pSerie->setAttribute(CComBSTR(LINE_WIDTH_ATTRIBUTE), lineWidth);
						pSerie->setAttribute(CComBSTR(MARKERS_COUNT_ATTRIBUTE), markers);
					}
				}
			}

			IXMLDOMElement* pCommon;
			if (SUCCEEDED(pDocument->createElement(CComBSTR(COMMON_NODE), &pCommon)))
			{
				pRoot->appendChild(pCommon, NULL);
				IXMLDOMElement* pBackground;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(BACKGROUND_NODE), &pBackground)))
				{
					pCommon->appendChild(pBackground, NULL);
					_variant_t style = pCtrl->m_BackGround.Type;
					_variant_t gradient = pCtrl->m_BackGround.grType;
					_variant_t color1 = pCtrl->m_BackGround.Color1;
					_variant_t color2 = pCtrl->m_BackGround.Color2;
					pBackground->setAttribute(CComBSTR(STYLE_ATTRIBUTE), style);
					pBackground->setAttribute(CComBSTR(GRADIENT_ATTRIBUTE), gradient);
					pBackground->setAttribute(CComBSTR(COLOR1_ATTRIBUTE), color1);
					pBackground->setAttribute(CComBSTR(COLOR2_ATTRIBUTE), color2);
				}

				IXMLDOMElement* pMargins;
				if (SUCCEEDED(pDocument->createElement(CComBSTR(MARGINS_NODE), &pMargins)))
				{
					pCommon->appendChild(pMargins, NULL);
					_variant_t left = pCtrl->m_LeftBorder;
					_variant_t right = pCtrl->m_RightBorder;
					_variant_t top = pCtrl->m_TopBorder;
					_variant_t bottom = pCtrl->m_BottomBorder;

					pMargins->setAttribute(CComBSTR(LEFT_ATTRIBUTE), left);
					pMargins->setAttribute(CComBSTR(RIGHT_ATTRIBUTE), right);
					pMargins->setAttribute(CComBSTR(TOP_ATTRIBUTE), top);
					pMargins->setAttribute(CComBSTR(BOTTOM_ATTRIBUTE), bottom);
				}
			}
		}
		else result = FALSE;

		return result;
	}
	else return FALSE;
}

BOOL CGraph::LoadPresetFromXML(IXMLDOMDocument2* pDocument, IXMLDOMElement* lpRoot, CGraphCtrl* pCtrl)
{
	if (pDocument)
	{
		BOOL result = TRUE;
		IXMLDOMNode* pRootNode;
		IXMLDOMElement* pRoot = NULL;

		if (!lpRoot)
			result = SUCCEEDED(pDocument->selectSingleNode(CComBSTR(PRESETS_ROOT_NODE), &pRootNode)) && pRootNode;
		else
			result = SUCCEEDED(lpRoot->selectSingleNode(CComBSTR(PRESETS_ROOT_NODE), &pRootNode)) && pRootNode;

		if (result)
			result = SUCCEEDED(pRootNode->QueryInterface(IID_IXMLDOMElement, (void**)&pRoot)) && pRoot;

		if (result)
		{
			CString searchStr;

			searchStr.Format(_T("./%s/%s"), COMMON_NODE, BACKGROUND_NODE);
			ReadXMLIntAttribute(pRoot, searchStr, STYLE_ATTRIBUTE, (int*)&pCtrl->m_BackGround.Type);
			ReadXMLIntAttribute(pRoot, searchStr, GRADIENT_ATTRIBUTE, (int*)&pCtrl->m_BackGround.grType);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR1_ATTRIBUTE, (int*)&pCtrl->m_BackGround.Color1);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR2_ATTRIBUTE, (int*)&pCtrl->m_BackGround.Color2);

			searchStr.Format(_T("./%s/%s"), COMMON_NODE, MARGINS_NODE);
			ReadXMLIntAttribute(pRoot, searchStr, LEFT_ATTRIBUTE, &pCtrl->m_LeftBorder);
			ReadXMLIntAttribute(pRoot, searchStr, RIGHT_ATTRIBUTE, &pCtrl->m_RightBorder);
			ReadXMLIntAttribute(pRoot, searchStr, TOP_ATTRIBUTE, &pCtrl->m_TopBorder);
			ReadXMLIntAttribute(pRoot, searchStr, BOTTOM_ATTRIBUTE, &pCtrl->m_BottomBorder);

			searchStr.Format(_T("./%s/%s"), TITLES_NODE, OPTIONS_NODE);
			ReadXMLIntAttribute(pRoot, searchStr, TITLE_ATTRIBUTE, &pCtrl->m_ShowTitle);
			ReadXMLIntAttribute(pRoot, searchStr, SUBTITLE_ATTRIBUTE, &pCtrl->m_ShowSubTitle);

			searchStr.Format(_T("./%s/%s"), LABELS_NODE, OPTIONS_NODE);
			ReadXMLIntAttribute(pRoot, searchStr, XLABEL_ATTRIBUTE, &pCtrl->m_ShowXLabel);
			ReadXMLIntAttribute(pRoot, searchStr, YLABEL_ATTRIBUTE, &pCtrl->m_ShowYLabel);
			ReadXMLIntAttribute(pRoot, searchStr, XTICKSLABEL_ATTRIBUTE, &pCtrl->m_ShowXTicksLabels);
			ReadXMLIntAttribute(pRoot, searchStr, YTICKSLABEL_ATTRIBUTE, &pCtrl->m_ShowYTicksLabels);
			ReadXMLIntAttribute(pRoot, searchStr, XSUBTICKSLABEL_ATTRIBUTE, &pCtrl->m_ShowXSubTicksLabels);
			ReadXMLIntAttribute(pRoot, searchStr, YSUBTICKSLABEL_ATTRIBUTE, &pCtrl->m_ShowYSubTicksLabels);
			ReadXMLIntAttribute(pRoot, searchStr, ZEROLABELS_ATTRIBUTE, &pCtrl->m_DrawZeroLabels);

			searchStr.Format(_T("./%s/%s"), GRIDS_NODE, GRID_OPTIONS_NODE);
			ReadXMLIntAttribute(pRoot, searchStr, SHOW_ATTRIBUTE, &pCtrl->m_ShowGrid);
			ReadXMLIntAttribute(pRoot, searchStr, XTICKS_ATTRIBUTE, &pCtrl->m_ShowXTicks);
			ReadXMLIntAttribute(pRoot, searchStr, YTICKS_ATTRIBUTE, &pCtrl->m_ShowYTicks);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR_ATTRIBUTE, (int*)&pCtrl->m_GridColor);
			ReadXMLIntAttribute(pRoot, searchStr, XSTABLE_ATTRIBUTE, &pCtrl->m_StableXGridCount);
			ReadXMLIntAttribute(pRoot, searchStr, YSTABLE_ATTRIBUTE, &pCtrl->m_StableYGridCount);
			ReadXMLIntAttribute(pRoot, searchStr, LINE_STYLE_ATTRIBUTE, (int*)&pCtrl->m_GridLine);
			ReadXMLDoubleAttribute(pRoot, searchStr, XSIZE_ATTRIBUTE, &pCtrl->m_DefaultXGridSize);
			ReadXMLDoubleAttribute(pRoot, searchStr, YSIZE_ATTRIBUTE, &pCtrl->m_DefaultYGridSize);
			ReadXMLDoubleAttribute(pRoot, searchStr, GRIDX_CHANGING_ATTRIBUTE, &pCtrl->m_GridXChanging);
			ReadXMLDoubleAttribute(pRoot, searchStr, GRIDY_CHANGING_ATTRIBUTE, &pCtrl->m_GridYChanging);

			searchStr.Format(_T("./%s/%s"), GRIDS_NODE, SUBGRID_OPTIONS_NODE);
			ReadXMLIntAttribute(pRoot, searchStr, SHOW_ATTRIBUTE, &pCtrl->m_ShowSubGrid);
			ReadXMLIntAttribute(pRoot, searchStr, XTICKS_ATTRIBUTE, &pCtrl->m_ShowXSubTicks);
			ReadXMLIntAttribute(pRoot, searchStr, YTICKS_ATTRIBUTE, &pCtrl->m_ShowYSubTicks);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR_ATTRIBUTE, (int*)&pCtrl->m_SubGridColor);
			ReadXMLIntAttribute(pRoot, searchStr, LINE_STYLE_ATTRIBUTE, (int*)&pCtrl->m_SubGridLine);
			ReadXMLIntAttribute(pRoot, searchStr, XSUBGRIDS_COUNT_ATTRIBUTE, &pCtrl->m_SubXGridsCount);
			ReadXMLIntAttribute(pRoot, searchStr, YSUBGRIDS_COUNT_ATTRIBUTE, &pCtrl->m_SubYGridsCount);

			searchStr.Format(_T("./%s/%s"), LEGEND_NODE, POSITION_NODE);
			CRect legendRect;
			ReadXMLIntAttribute(pRoot, searchStr, LEFT_ATTRIBUTE, (int*)&legendRect.left);
			ReadXMLIntAttribute(pRoot, searchStr, RIGHT_ATTRIBUTE, (int*)&legendRect.right);
			ReadXMLIntAttribute(pRoot, searchStr, TOP_ATTRIBUTE, (int*)&legendRect.top);
			ReadXMLIntAttribute(pRoot, searchStr, BOTTOM_ATTRIBUTE, (int*)&legendRect.bottom);
			if (!legendRect.IsRectEmpty())
				pCtrl->SetLegendRect(legendRect);

			searchStr.Format(_T("./%s/%s"), LEGEND_NODE, OPTIONS_NODE);
			ReadXMLIntAttribute(pRoot, searchStr, SHOW_ATTRIBUTE, &pCtrl->m_ShowLegend);
			ReadXMLDoubleAttribute(pRoot, searchStr, BORDERWIDTH_ATTRIBUTE, &pCtrl->m_LegendBorderWidth);
			ReadXMLIntAttribute(pRoot, searchStr, BORDER_COLOR_ATTRIBUTE, (int*)&pCtrl->m_LegendBorderColor);
			ReadXMLIntAttribute(pRoot, searchStr, AUTOPOS_ATTRIBUTE, &pCtrl->m_bAutoLegendPos);
			ReadXMLIntAttribute(pRoot, searchStr, FLOATING_ATTRIBUTE, &pCtrl->m_bLegendFloating);

			searchStr.Format(_T("./%s/%s"), LEGEND_NODE, BACKGROUND_NODE);
			ReadXMLIntAttribute(pRoot, searchStr, STYLE_ATTRIBUTE, (int*)&pCtrl->m_LegendBackGround.Type);
			ReadXMLIntAttribute(pRoot, searchStr, GRADIENT_ATTRIBUTE, (int*)&pCtrl->m_LegendBackGround.grType);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR1_ATTRIBUTE, (int*)&pCtrl->m_LegendBackGround.Color1);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR2_ATTRIBUTE, (int*)&pCtrl->m_LegendBackGround.Color2);

			CString fontName;
			searchStr.Format(_T("./%s/%s"), FONTS_NODE, TITLE_NODE);
			ReadXMLStringAttribute(pRoot, searchStr, NAME_ATTRIBUTE, &fontName);
			_tcscpy(pCtrl->m_TitleFont.name, fontName.GetBuffer());
			ReadXMLIntAttribute(pRoot, searchStr, SIZE_ATTRIBUTE, &pCtrl->m_TitleFont.size);
			ReadXMLIntAttribute(pRoot, searchStr, BOLD_ATTRIBUTE, &pCtrl->m_TitleFont.bold);
			ReadXMLIntAttribute(pRoot, searchStr, ITALIC_ATTRIBUTE, &pCtrl->m_TitleFont.italic);
			ReadXMLIntAttribute(pRoot, searchStr, UNDERLINE_ATTRIBUTE, &pCtrl->m_TitleFont.underline);
			ReadXMLIntAttribute(pRoot, searchStr, STRIKEOUT_ATTRIBUTE, &pCtrl->m_TitleFont.strikeout);
			ReadXMLIntAttribute(pRoot, searchStr, ANGLE_ATTRIBUTE, &pCtrl->m_TitleFont.angle);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR_ATTRIBUTE, (int*)&pCtrl->m_TitleFont.color);

			searchStr.Format(_T("./%s/%s"), FONTS_NODE, SUBTITLE_NODE);
			ReadXMLStringAttribute(pRoot, searchStr, NAME_ATTRIBUTE, &fontName);
			_tcscpy(pCtrl->m_SubTitleFont.name, fontName.GetBuffer());
			ReadXMLIntAttribute(pRoot, searchStr, SIZE_ATTRIBUTE, &pCtrl->m_SubTitleFont.size);
			ReadXMLIntAttribute(pRoot, searchStr, BOLD_ATTRIBUTE, &pCtrl->m_SubTitleFont.bold);
			ReadXMLIntAttribute(pRoot, searchStr, ITALIC_ATTRIBUTE, &pCtrl->m_SubTitleFont.italic);
			ReadXMLIntAttribute(pRoot, searchStr, UNDERLINE_ATTRIBUTE, &pCtrl->m_SubTitleFont.underline);
			ReadXMLIntAttribute(pRoot, searchStr, STRIKEOUT_ATTRIBUTE, &pCtrl->m_SubTitleFont.strikeout);
			ReadXMLIntAttribute(pRoot, searchStr, ANGLE_ATTRIBUTE, &pCtrl->m_SubTitleFont.angle);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR_ATTRIBUTE, (int*)&pCtrl->m_SubTitleFont.color);

			searchStr.Format(_T("./%s/%s"), FONTS_NODE, LEGEND_NODE);
			ReadXMLStringAttribute(pRoot, searchStr, NAME_ATTRIBUTE, &fontName);
			_tcscpy(pCtrl->m_LegendFont.name, fontName.GetBuffer());
			ReadXMLIntAttribute(pRoot, searchStr, SIZE_ATTRIBUTE, &pCtrl->m_LegendFont.size);
			ReadXMLIntAttribute(pRoot, searchStr, BOLD_ATTRIBUTE, &pCtrl->m_LegendFont.bold);
			ReadXMLIntAttribute(pRoot, searchStr, ITALIC_ATTRIBUTE, &pCtrl->m_LegendFont.italic);
			ReadXMLIntAttribute(pRoot, searchStr, UNDERLINE_ATTRIBUTE, &pCtrl->m_LegendFont.underline);
			ReadXMLIntAttribute(pRoot, searchStr, STRIKEOUT_ATTRIBUTE, &pCtrl->m_LegendFont.strikeout);
			ReadXMLIntAttribute(pRoot, searchStr, ANGLE_ATTRIBUTE, &pCtrl->m_LegendFont.angle);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR_ATTRIBUTE, (int*)&pCtrl->m_LegendFont.color);

			searchStr.Format(_T("./%s/%s"), FONTS_NODE, XLABEL_NODE);
			ReadXMLStringAttribute(pRoot, searchStr, NAME_ATTRIBUTE, &fontName);
			_tcscpy(pCtrl->m_XLabelFont.name, fontName.GetBuffer());
			ReadXMLIntAttribute(pRoot, searchStr, SIZE_ATTRIBUTE, &pCtrl->m_XLabelFont.size);
			ReadXMLIntAttribute(pRoot, searchStr, BOLD_ATTRIBUTE, &pCtrl->m_XLabelFont.bold);
			ReadXMLIntAttribute(pRoot, searchStr, ITALIC_ATTRIBUTE, &pCtrl->m_XLabelFont.italic);
			ReadXMLIntAttribute(pRoot, searchStr, UNDERLINE_ATTRIBUTE, &pCtrl->m_XLabelFont.underline);
			ReadXMLIntAttribute(pRoot, searchStr, STRIKEOUT_ATTRIBUTE, &pCtrl->m_XLabelFont.strikeout);
			ReadXMLIntAttribute(pRoot, searchStr, ANGLE_ATTRIBUTE, &pCtrl->m_XLabelFont.angle);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR_ATTRIBUTE, (int*)&pCtrl->m_XLabelFont.color);

			searchStr.Format(_T("./%s/%s"), FONTS_NODE, YLABEL_NODE);
			ReadXMLStringAttribute(pRoot, searchStr, NAME_ATTRIBUTE, &fontName);
			_tcscpy(pCtrl->m_YLabelFont.name, fontName.GetBuffer());
			ReadXMLIntAttribute(pRoot, searchStr, SIZE_ATTRIBUTE, &pCtrl->m_YLabelFont.size);
			ReadXMLIntAttribute(pRoot, searchStr, BOLD_ATTRIBUTE, &pCtrl->m_YLabelFont.bold);
			ReadXMLIntAttribute(pRoot, searchStr, ITALIC_ATTRIBUTE, &pCtrl->m_YLabelFont.italic);
			ReadXMLIntAttribute(pRoot, searchStr, UNDERLINE_ATTRIBUTE, &pCtrl->m_YLabelFont.underline);
			ReadXMLIntAttribute(pRoot, searchStr, STRIKEOUT_ATTRIBUTE, &pCtrl->m_YLabelFont.strikeout);
			ReadXMLIntAttribute(pRoot, searchStr, ANGLE_ATTRIBUTE, &pCtrl->m_YLabelFont.angle);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR_ATTRIBUTE, (int*)&pCtrl->m_YLabelFont.color);

			searchStr.Format(_T("./%s/%s"), FONTS_NODE, XTICKSLABEL_NODE);
			ReadXMLStringAttribute(pRoot, searchStr, NAME_ATTRIBUTE, &fontName);
			_tcscpy(pCtrl->m_TicksXLabelsFont.name, fontName.GetBuffer());
			ReadXMLIntAttribute(pRoot, searchStr, SIZE_ATTRIBUTE, &pCtrl->m_TicksXLabelsFont.size);
			ReadXMLIntAttribute(pRoot, searchStr, BOLD_ATTRIBUTE, &pCtrl->m_TicksXLabelsFont.bold);
			ReadXMLIntAttribute(pRoot, searchStr, ITALIC_ATTRIBUTE, &pCtrl->m_TicksXLabelsFont.italic);
			ReadXMLIntAttribute(pRoot, searchStr, UNDERLINE_ATTRIBUTE, &pCtrl->m_TicksXLabelsFont.underline);
			ReadXMLIntAttribute(pRoot, searchStr, STRIKEOUT_ATTRIBUTE, &pCtrl->m_TicksXLabelsFont.strikeout);
			ReadXMLIntAttribute(pRoot, searchStr, ANGLE_ATTRIBUTE, &pCtrl->m_TicksXLabelsFont.angle);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR_ATTRIBUTE, (int*)&pCtrl->m_TicksXLabelsFont.color);

			searchStr.Format(_T("./%s/%s"), FONTS_NODE, YTICKSLABEL_NODE);
			ReadXMLStringAttribute(pRoot, searchStr, NAME_ATTRIBUTE, &fontName);
			_tcscpy(pCtrl->m_TicksYLabelsFont.name, fontName.GetBuffer());
			ReadXMLIntAttribute(pRoot, searchStr, SIZE_ATTRIBUTE, &pCtrl->m_TicksYLabelsFont.size);
			ReadXMLIntAttribute(pRoot, searchStr, BOLD_ATTRIBUTE, &pCtrl->m_TicksYLabelsFont.bold);
			ReadXMLIntAttribute(pRoot, searchStr, ITALIC_ATTRIBUTE, &pCtrl->m_TicksYLabelsFont.italic);
			ReadXMLIntAttribute(pRoot, searchStr, UNDERLINE_ATTRIBUTE, &pCtrl->m_TicksYLabelsFont.underline);
			ReadXMLIntAttribute(pRoot, searchStr, STRIKEOUT_ATTRIBUTE, &pCtrl->m_TicksYLabelsFont.strikeout);
			ReadXMLIntAttribute(pRoot, searchStr, ANGLE_ATTRIBUTE, &pCtrl->m_TicksYLabelsFont.angle);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR_ATTRIBUTE, (int*)&pCtrl->m_TicksYLabelsFont.color);

			searchStr.Format(_T("./%s/%s"), SERIES_NODE, BACKGROUND_NODE);
			ReadXMLIntAttribute(pRoot, searchStr, STYLE_ATTRIBUTE, (int*)&pCtrl->m_GraphFieldBackGround.Type);
			ReadXMLIntAttribute(pRoot, searchStr, GRADIENT_ATTRIBUTE, (int*)&pCtrl->m_GraphFieldBackGround.grType);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR1_ATTRIBUTE, (int*)&pCtrl->m_GraphFieldBackGround.Color1);
			ReadXMLIntAttribute(pRoot, searchStr, COLOR2_ATTRIBUTE, (int*)&pCtrl->m_GraphFieldBackGround.Color2);

			searchStr.Format(_T("./%s/%s"), SERIES_NODE, OPTIONS_NODE);
			ReadXMLIntAttribute(pRoot, searchStr, ANTIALIASING_ATTRIBUTE, &pCtrl->m_AntiAlaising);
			ReadXMLIntAttribute(pRoot, searchStr, STYLE_ATTRIBUTE, (int*)&pCtrl->m_GraphStyle);
			ReadXMLDoubleAttribute(pRoot, searchStr, BORDERWIDTH_ATTRIBUTE, &pCtrl->m_GraphBorderWidth);
			ReadXMLIntAttribute(pRoot, searchStr, BORDER_COLOR_ATTRIBUTE, (int*)&pCtrl->m_GraphBorderColor);
			ReadXMLIntAttribute(pRoot, searchStr, POINTSIZE_ATTRIBUTE, &pCtrl->m_PointSize);
			ReadXMLDoubleAttribute(pRoot, searchStr, XMIN_ATTRIBUTE, &pCtrl->m_GraphXSize.min);
			ReadXMLDoubleAttribute(pRoot, searchStr, YMIN_ATTRIBUTE, &pCtrl->m_GraphYSize.min);
			ReadXMLDoubleAttribute(pRoot, searchStr, XMAX_ATTRIBUTE, &pCtrl->m_GraphXSize.max);
			ReadXMLDoubleAttribute(pRoot, searchStr, YMAX_ATTRIBUTE, &pCtrl->m_GraphYSize.max);
			ReadXMLIntAttribute(pRoot, searchStr, INTELLIGENT_ATTRIBUTE, &pCtrl->m_IntelligentAxisScale);
			ReadXMLIntAttribute(pRoot, searchStr, SYMMETRIC_ATTRIBUTE, &pCtrl->m_Symmetric);
			ReadXMLDoubleAttribute(pRoot, searchStr, GRAPHX_CHANGING_ATTRIBUTE, &pCtrl->m_GraphXChanging);
			ReadXMLDoubleAttribute(pRoot, searchStr, GRAPHY_CHANGING_ATTRIBUTE, &pCtrl->m_GraphXChanging);

			searchStr.Format(_T("./%s/%s"), SERIES_NODE, ADDITIONAL_GRIDS_NODE);
			int minx, miny, maxx, maxy;
			ReadXMLIntAttribute(pRoot, searchStr, XMIN_ATTRIBUTE, &minx);
			ReadXMLIntAttribute(pRoot, searchStr, XMAX_ATTRIBUTE, &maxx);
			ReadXMLIntAttribute(pRoot, searchStr, YMIN_ATTRIBUTE, &miny);
			ReadXMLIntAttribute(pRoot, searchStr, YMAX_ATTRIBUTE, &maxy);
			pCtrl->m_AdditionalGridSteps.SetRect(minx, maxy, maxx, miny);

			searchStr.Format(_T("./%s/%s"), SERIES_NODE, LABELS_FORMAT_NODE);
			ReadXMLIntAttribute(pRoot, searchStr, XFORMAT_ATTRIBUTE, (int*)&pCtrl->m_XLabelsFormat);
			ReadXMLIntAttribute(pRoot, searchStr, YFORMAT_ATTRIBUTE, (int*)&pCtrl->m_YLabelsFormat);
			ReadXMLIntAttribute(pRoot, searchStr, XFORMAT_ADDITIONAL_ATTRIBUTE, &pCtrl->m_XLabelsFormatAdditional);
			ReadXMLIntAttribute(pRoot, searchStr, YFORMAT_ADDITIONAL_ATTRIBUTE, &pCtrl->m_YLabelsFormatAdditional);

			searchStr.Format(_T("./%s/%s"), SERIES_NODE, SERIE_NODE);
			IXMLDOMNodeList* pSeries;
			if (SUCCEEDED(pRoot->selectNodes(CComBSTR(searchStr), &pSeries)))
			{
				long seriesCount = 0;
				if (SUCCEEDED(pSeries->get_length(&seriesCount)) && seriesCount > 0)
				{
					int index = 0;
					for (int i = 0; i < pCtrl->GetSeriesCount(); i++)
					{
						GRAPHSERIES* pGraphSerie = pCtrl->GetSerie(i);
						IXMLDOMNode* pPresetSerie;
						if (SUCCEEDED(pSeries->get_item(index, &pPresetSerie)))
						{
							IXMLDOMElement* pSerieElement;
							if (SUCCEEDED(pPresetSerie->QueryInterface(IID_IXMLDOMElement, (void**)&pSerieElement)))
							{
								VARIANT style, size, color, markercolor, line, linewidth, markers;
								pSerieElement->getAttribute(CComBSTR(MARKER_STYLE_ATTRIBUTE), &style);
								pSerieElement->getAttribute(CComBSTR(MARKER_SIZE_ATTRIBUTE), &size);
								pSerieElement->getAttribute(CComBSTR(COLOR_ATTRIBUTE), &color);
								pSerieElement->getAttribute(CComBSTR(MARKER_COLOR_ATTRIBUTE), &markercolor);
								pSerieElement->getAttribute(CComBSTR(LINE_STYLE_ATTRIBUTE), &line);
								pSerieElement->getAttribute(CComBSTR(LINE_WIDTH_ATTRIBUTE), &linewidth);
								pSerieElement->getAttribute(CComBSTR(MARKERS_COUNT_ATTRIBUTE), &markers);

								if (IsVariantINT(&style) || SUCCEEDED(VariantChangeType(&style, &style, 0, VT_INT)))
									pGraphSerie->marker = (GRAPH_MARKERSTYLE )(int)_variant_t(style);
								else pGraphSerie->marker = MS_NONE;

								if (IsVariantINT(&size) || SUCCEEDED(VariantChangeType(&size, &size, 0, VT_INT)))
									pGraphSerie->markersize = _variant_t(size);
								else pGraphSerie->markersize = 0;

								if (IsVariantINT(&color) || SUCCEEDED(VariantChangeType(&color, &color, 0, VT_INT)))
									pGraphSerie->color = _variant_t(color);
								else pGraphSerie->color = RGB(0, 0, 0);

								if (IsVariantINT(&markercolor) || SUCCEEDED(VariantChangeType(&markercolor, &markercolor, 0, VT_INT)))
									pGraphSerie->markercolor = _variant_t(markercolor);
								else pGraphSerie->markercolor = RGB(0, 0, 0);

								if (IsVariantINT(&line) || SUCCEEDED(VariantChangeType(&line, &line, 0, VT_INT)))
									pGraphSerie->line = (GRAPH_LINESTYLE)(int)_variant_t(line);
								else pGraphSerie->line = LS_SOLID;

								if (IsVariantINT(&linewidth) || SUCCEEDED(VariantChangeType(&linewidth, &linewidth, 0, VT_INT)))
									pGraphSerie->linewidth = _variant_t(linewidth);
								else pGraphSerie->linewidth = 1;
								if (pGraphSerie->linewidth < 1) pGraphSerie->linewidth = 1;

								if (IsVariantINT(&markers) || SUCCEEDED(VariantChangeType(&markers, &markers, 0, VT_INT)))
									pGraphSerie->markers = (int)_variant_t(markers);
								else pGraphSerie->markers = 100;

								pSerieElement->Release();
							}
						}

						index++;
						if (index == seriesCount) index = 0;
					}
				}
			}
		}
		else result = FALSE;

		if (pRoot) pRoot->Release();

		return result;
	}
	else return FALSE;
}

void CGraph::ResetPreset(void)
{
	for (int i = 0; i < GetGraphsCount(); i++)
		GetGraphCtrl(i)->ResetPreset();
}

BOOL CGraph::SaveGraphDataToXML(IXMLDOMDocument2* pDocument, IXMLDOMElement* lpRoot, CGraphCtrl* pCtrl)
{
	BOOL result = pDocument != NULL;
	if (result)
	{
		IXMLDOMElement* pRoot;
		if (SUCCEEDED(pDocument->createElement(CComBSTR(DATA_GRAPH_ROOT_NODE), &pRoot)))
		{
			if (lpRoot)
				result = SUCCEEDED(lpRoot->appendChild(pRoot, NULL));
			else 
				result = SUCCEEDED(pDocument->putref_documentElement(pRoot));
		}
		else result = FALSE;

		if (result)
		{
			IXMLDOMElement* pLabels;
			if (SUCCEEDED(pDocument->createElement(CComBSTR(LABELS_NODE), &pLabels)))
			{
				pRoot->appendChild(pLabels, NULL);
				_variant_t title = pCtrl->m_Title;
				_variant_t subTitle = pCtrl->m_SubTitle;
				_variant_t xLabel = pCtrl->m_XLabel;
				_variant_t yLabel = pCtrl->m_YLabel;
				pLabels->setAttribute(CComBSTR(TITLE_ATTRIBUTE), title);
				pLabels->setAttribute(CComBSTR(SUBTITLE_ATTRIBUTE), subTitle);
				pLabels->setAttribute(CComBSTR(XLABEL_ATTRIBUTE), xLabel);
				pLabels->setAttribute(CComBSTR(YLABEL_ATTRIBUTE), yLabel);
			}

			IXMLDOMElement* pSeries;
			if (SUCCEEDED(pDocument->createElement(CComBSTR(SERIES_NODE), &pSeries)))
			{
				pRoot->appendChild(pSeries, NULL);
				for (int i = 0; i < pCtrl->GetSeriesCount(); i++)
				{
					GRAPHSERIES* pGraphSerie = pCtrl->GetSerie(i);
					IXMLDOMElement* pXMLSerie;
					if (SUCCEEDED(pDocument->createElement(CComBSTR(SERIE_NODE), &pXMLSerie)))
					{
						pSeries->appendChild(pXMLSerie, NULL);
						_variant_t name = pGraphSerie->lpName;
						_variant_t visible = pGraphSerie->visible;
						_variant_t legend = pGraphSerie->legend;
						pXMLSerie->setAttribute(CComBSTR(NAME_ATTRIBUTE), name);
						pXMLSerie->setAttribute(CComBSTR(VISIBLE_ATTRIBUTE), visible);
						pXMLSerie->setAttribute(CComBSTR(LEGEND_VISIBLE_ATTRIBUTE), legend);
						IXMLDOMElement* pData;
						if (SUCCEEDED(pDocument->createElement(CComBSTR(DATA_NODE), &pData)))
						{
							pXMLSerie->appendChild(pData, NULL);
							for (int p = 0; p < pGraphSerie->count; p++)
							{
								IXMLDOMElement* pPoint;
								if (SUCCEEDED(pDocument->createElement(CComBSTR(POINT_NODE), &pPoint)))
								{
									pData->appendChild(pPoint, NULL);
									_variant_t x = pGraphSerie->pData[p].x;
									_variant_t y = pGraphSerie->pData[p].y;
									pPoint->setAttribute(CComBSTR(X_ATTRIBUTE), x);
									pPoint->setAttribute(CComBSTR(Y_ATTRIBUTE), y);
								}
							}
						}
					}
				}
			}
		}
	}

	return result;
}

BOOL CGraph::LoadGraphDataFromXML(IXMLDOMDocument2* pDocument, IXMLDOMElement* lpRoot, CGraphCtrl* pCtrl)
{
	BOOL result = pDocument != NULL;
	
	if (result)
	{
		IXMLDOMElement* pRoot = lpRoot;
		IXMLDOMNode* pRootNode = NULL;
		if (!pRoot)
		{
			CString searchStr;
			searchStr.Format(_T("/%s/%s"), DATA_ROOT_NODE, DATA_GRAPH_ROOT_NODE);
			result = SUCCEEDED(pDocument->selectSingleNode(CComBSTR(searchStr), &pRootNode)) && pRootNode;
			if (result)
				result = SUCCEEDED(pRootNode->QueryInterface(IID_IXMLDOMElement, (void**)&pRoot)) && pRoot;
		}

		if (result)
		{
//			pCtrl->Clear();

			ReadXMLStringAttribute(pRoot, LABELS_NODE, TITLE_ATTRIBUTE, &pCtrl->m_Title);
			ReadXMLStringAttribute(pRoot, LABELS_NODE, SUBTITLE_ATTRIBUTE, &pCtrl->m_SubTitle);
			ReadXMLStringAttribute(pRoot, LABELS_NODE, XLABEL_ATTRIBUTE, &pCtrl->m_XLabel);
			ReadXMLStringAttribute(pRoot, LABELS_NODE, YLABEL_ATTRIBUTE, &pCtrl->m_YLabel);

			CString searchStr;
			searchStr.Format(_T("./%s/%s"), SERIES_NODE, SERIE_NODE);
			IXMLDOMNodeList* pSeries;
			result = SUCCEEDED(pRoot->selectNodes(CComBSTR(searchStr), &pSeries)) && pSeries;
			if (result)
			{
				long seriesCount;
				result = SUCCEEDED(pSeries->get_length(&seriesCount));
				if (result)
				{
					for (int i = 0; i < pCtrl->GetSeriesCount(); i++)
					{
						GRAPHSERIES* pGraphSerie = pCtrl->GetSerie(i);
						pGraphSerie->count = 0;
						delete[] pGraphSerie->pData;
						pGraphSerie->pData = NULL;
					}

					if (seriesCount < pCtrl->GetSeriesCount())
					{
						while (seriesCount < pCtrl->GetSeriesCount())
							pCtrl->DeleteSerie(pCtrl->GetSeriesCount() - 1);
					}
					else if (seriesCount > pCtrl->GetSeriesCount())
					{
						int index = 0;
						while (seriesCount > pCtrl->GetSeriesCount())
						{
							GRAPHSERIES* pGraphSerie = (pCtrl->GetSeriesCount() > 0) ? new GRAPHSERIES(*pCtrl->GetSerie(index)) : new GRAPHSERIES();
							pCtrl->AddSeries(pGraphSerie);
							index++;
							if (index >= pCtrl->GetSeriesCount()) index = 0;
						}
					}

					for (int i = 0; i < seriesCount; i++)
					{
						GRAPHSERIES* pGraphSerie = pCtrl->GetSerie(i);
						IXMLDOMNode* pSerieNode;
						result = SUCCEEDED(pSeries->get_item(i, &pSerieNode)) && pSerieNode;
						if (result)
						{
							IXMLDOMElement* pSerieElement;
							result = SUCCEEDED(pSerieNode->QueryInterface(IID_IXMLDOMElement, (void**)&pSerieElement)) && pSerieElement;
							if (result)
							{
								VARIANT nameVar, visibleVar, legendVar;
								pSerieElement->getAttribute(CComBSTR(NAME_ATTRIBUTE), &nameVar);
								pSerieElement->getAttribute(CComBSTR(VISIBLE_ATTRIBUTE), &visibleVar);
								pSerieElement->getAttribute(CComBSTR(LEGEND_VISIBLE_ATTRIBUTE), &legendVar);

								if (nameVar.vt == VT_BSTR || SUCCEEDED(VariantChangeType(&nameVar, &nameVar, 0, VT_BSTR)))
								{
									_bstr_t nameStr = nameVar.bstrVal;
									_tcscpy(pGraphSerie->lpName, nameStr);
									//delete[] nameStr;
								}
								else pGraphSerie->lpName[0] = 0;

								if (IsVariantINT(&visibleVar) || SUCCEEDED(VariantChangeType(&visibleVar, &visibleVar, 0, VT_INT)))
									pGraphSerie->visible = (_variant_t)visibleVar;
								else pGraphSerie->visible = TRUE;

								if (IsVariantINT(&legendVar) || SUCCEEDED(VariantChangeType(&legendVar, &legendVar, 0, VT_INT)))
									pGraphSerie->legend = (_variant_t)legendVar;
								else pGraphSerie->legend = TRUE;

								searchStr.Format(_T("./%s/%s"), DATA_NODE, POINT_NODE);
								IXMLDOMNodeList* pPoints;
								if (SUCCEEDED(pSerieElement->selectNodes(CComBSTR(searchStr), &pPoints)) && pPoints)
								{
									long pointsCount;
									if (SUCCEEDED(pPoints->get_length(&pointsCount)))
									{
										pGraphSerie->count = pointsCount;
										pGraphSerie->pData = new GRAPHPOINT[pointsCount];
										for (int p = 0; p < pointsCount; p++)
										{
											IXMLDOMNode* pPointNode;
											if (SUCCEEDED(pPoints->get_item(p, &pPointNode)) && pPointNode)
											{
												IXMLDOMElement* pPointElement;
												if (SUCCEEDED(pPointNode->QueryInterface(IID_IXMLDOMElement, (void**)&pPointElement)) && pPointElement)
												{
													VARIANT xVar, yVar;
													pPointElement->getAttribute(CComBSTR(X_ATTRIBUTE), &xVar);
													pPointElement->getAttribute(CComBSTR(Y_ATTRIBUTE), &yVar);

													if (VariantToDouble(&xVar))
														pGraphSerie->pData[p].x = _variant_t(xVar);
													else
														pGraphSerie->pData[p].x = 0;
													if (VariantToDouble(&yVar))
														pGraphSerie->pData[p].y = _variant_t(yVar);
													else
														pGraphSerie->pData[p].y = 0;
													
													pPointElement->Release();
												}
											}
										}
									}
								}
//								pCtrl->AddSeries(pGraphSerie);
								pSerieElement->Release();
							}
						}
						if (!result) break;
					}
				}
			}
		}

		if (pRoot && pRootNode)
			pRoot->Release();
	}

	return result;
}
void CGraph::SetWindowTitle(LPCTSTR lpWindowTitle)
{
	if (lpWindowTitle)
	{
		TCHAR newTitle[1024];
		_stprintf(newTitle, _T("%s: %s"), m_pDefaultTitle, lpWindowTitle);
		m_pGraphWnd->SetWindowText(newTitle);
	}
	else
		m_pGraphWnd->SetWindowText(m_pDefaultTitle);
}

CGraphView* CGraph::GetView(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_pGraphWnd)
		return m_pGraphWnd->m_pView;
	else return NULL;
}

void CGraph::OnEndPrintPreview()
{
	if (GetView())
		GetView()->m_bPrintPreview = FALSE;
}

void CGraph::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
{
    ASSERT(pDC && pInfo);

	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(1);
	pInfo->m_nCurPage = 1;

	pDC->SetMapMode(MM_TEXT);

	for (int i = 0; i < GetGraphsCount(); i++)
		GetGraphCtrl(i)->SetPrintingDC(pDC);
}

void CGraph::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{
    if (!pDC || !pInfo) return;

	CSize m_PaperSize = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	CRect windowRect;
	m_pGraphWnd->m_pView->GetClientRect(windowRect);
	double xScale = (double)m_PaperSize.cx / windowRect.Width();
	double yScale = (double)m_PaperSize.cy / windowRect.Height();

//	CRect clientRect(CPoint(0, 0), m_PaperSize);
	CRect clientRect(pInfo->m_rectDraw);

	int caps = GetDeviceCaps(pDC->m_hAttribDC, LOGPIXELSY);

	for (int i = 0; i < GetGraphsCount(); i++)
	{
		CSize sz = GetGraphCtrl(i)->GetGraphSize();

		GetGraphCtrl(i)->Resize((int)_round(sz.cx * xScale, 0), (int)_round(sz.cy * yScale, 0));
		CRect rect;
		GetGraphRect(i, rect, clientRect);
		GetGraphCtrl(i)->Draw();
		CDC* tempDC = GetGraphCtrl(i)->GetDC();
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), tempDC, 0, 0, SRCCOPY);

		GetGraphCtrl(i)->Resize(sz.cx, sz.cy);
	}

//	CRect drawRect;
//	drawRect = clientRect;

//	pDC->BitBlt(drawRect.left, drawRect.top, drawRect.Width(), drawRect.Height(), clientDIB.GetDC(), drawRect.left, drawRect.top, SRCCOPY);
}

void CGraph::OnEndPrinting(CDC *pDC, CPrintInfo *pInfo)
{
	for (int i = 0; i < GetGraphsCount(); i++)
		GetGraphCtrl(i)->SetPrintingDC(NULL);

	m_pGraphWnd->m_pView->Resize();
}

void CGraph::Print()
{
	CDC dc;
    CPrintDialog printDlg(FALSE, PD_ALLPAGES | PD_NOPAGENUMS | PD_NOSELECTION/* | PD_PRINTSETUP*/ | PD_RETURNDC);

	AfxGetApp()->GetPrinterDeviceDefaults(&printDlg.m_pd);

	printDlg.m_pd.nMinPage = printDlg.m_pd.nFromPage = 1;
	printDlg.m_pd.nMaxPage = printDlg.m_pd.nToPage = 1;

	LPDEVMODE lpDevMode = (LPDEVMODE)::GlobalLock(printDlg.m_pd.hDevMode);
	if (lpDevMode)
	{
		lpDevMode->dmFields |= DM_ORIENTATION;
		lpDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	}
	::GlobalUnlock(printDlg.m_pd.hDevMode);

    if (printDlg.DoModal() != IDOK)             // Get printer settings from user
        return;

    dc.Attach(printDlg.GetPrinterDC());         // attach a printer DC
    dc.m_bPrinting = TRUE;

    CString strTitle;
    strTitle.LoadString(AFX_IDS_APP_TITLE);

    DOCINFO di;                                 // Initialise print doc details
    memset(&di, 0, sizeof (DOCINFO));
    di.cbSize = sizeof (DOCINFO);
    di.lpszDocName = strTitle;

    BOOL bPrintingOK = dc.StartDoc(&di);        // Begin a new print job

    CPrintInfo Info;
    Info.m_rectDraw.SetRect(0,0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));

    OnBeginPrinting(&dc, &Info);                // Initialise printing
	int nMin = Info.GetMinPage();
	int nMax = Info.GetMaxPage();
    for (UINT page = Info.GetMinPage(); page <= Info.GetMaxPage() && bPrintingOK; page++)
    {
        dc.StartPage();                         // begin new page
        Info.m_nCurPage = page;
        OnPrint(&dc, &Info);                    // Print page
        bPrintingOK = (dc.EndPage() > 0);       // end page
    }
    OnEndPrinting(&dc, &Info);                  // Clean up after printing

    if (bPrintingOK)
        dc.EndDoc();                            // end a print job
    else
        dc.AbortDoc();                          // abort job.

    dc.Detach();                                // detach the printer DC
}

HWND CGraph::GetParent()
{
  return m_hParentWnd;
}
