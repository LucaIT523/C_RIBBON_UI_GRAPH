#include "..\stdafx.h"
#include "..\..\include\gl\cgl.h"

#include "gl/gl.h" 			//OPENGL
#include "gl/glu.h" 		//OPENGL

//
// Constructor
//
CGL::CGL()
{
	m_hrc = NULL ;
	m_pdc = NULL ;
	m_pPal = NULL ;
	m_pOldPal = NULL ;

	m_bDoubleBuffer = FALSE ;
	m_bDrawToBitmap = FALSE ;
}

//
// Destructor
//
CGL::~CGL()
{
	Destroy() ;
}

//
// Destory: 
//
void CGL::Destroy() 
{
	if (m_pOldPal)
	{
		// Select in old palette.
	 	m_pdc->SelectPalette(m_pOldPal, 0); 
		m_pOldPal = NULL ;
	}

	if (m_pPal) 
	{		
		// Delete palette.
		delete m_pPal ;
		m_pPal = NULL ;
	}

	if (m_hrc) 
	{
		// Delete rendering context.
		if (m_hrc == wglGetCurrentContext())
			wglMakeCurrent(NULL,NULL) ; 

		wglDeleteContext(m_hrc) ;
		m_hrc = NULL ;
	}		

	if (m_pdc && !m_bDrawToBitmap) // m_pdc is not owned by us when drawing to bitmap
	{
		delete m_pdc ;
		m_pdc = NULL ;
	}
}

//
//	Create: use PFD_DRAW_TO_BITMAP
//
BOOL CGL::Create(CDC* pdcMemory, int iPixelType, DWORD dwFlags)
{
	TRACE0("CGL::Create (MemoryDC)\r\n") ;
#ifdef _DEBUG
	pdcMemory->AssertValid() ;
#endif

    m_pdc = pdcMemory ;
	CBitmap* pBitmap = m_pdc->GetCurrentBitmap() ;
	BITMAP bmInfo ;
	pBitmap->GetObject(sizeof(BITMAP), &bmInfo) ;

	ASSERT(bmInfo.bmPlanes == 1) ;
	ASSERT((bmInfo.bmBitsPixel == 1) || (bmInfo.bmBitsPixel == 8) || (bmInfo.bmBitsPixel == 16) || (bmInfo.bmBitsPixel == 24)) ;		

	//
	// Fill in the Pixel Format Descriptor
	//
   PIXELFORMATDESCRIPTOR pfd ;
   memset(&pfd,0, sizeof(PIXELFORMATDESCRIPTOR)) ;
   pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);  
   pfd.nVersion = 1 ;                           // Version number
   pfd.dwFlags =  dwFlags ;
   pfd.iPixelType = iPixelType ;
   pfd.cColorBits = (BYTE)bmInfo.bmBitsPixel ;
   pfd.cDepthBits = 32 ;						// 32-bit depth buffer
   pfd.iLayerType = PFD_MAIN_PLANE ;           // Layer type

	// Let children change creation.
	OnCreate(NULL, &pfd) ;

	ASSERT( (dwFlags & PFD_DRAW_TO_BITMAP)) ;
	ASSERT( !(dwFlags & PFD_DOUBLEBUFFER)) ;
	ASSERT( (iPixelType == PFD_TYPE_RGBA) || (iPixelType == PFD_TYPE_COLORINDEX)) ;

	m_bDrawToBitmap = TRUE ;

	// Determine double buffering state.
	m_bDoubleBuffer = FALSE ;


	// Chose the pixel format.
   int nPixelFormat = ChoosePixelFormat(m_pdc->m_hDC, &pfd);
   if (nPixelFormat == 0)
	{
		TRACE("ChoosePixelFormat Failed %d\r\n",GetLastError()) ;
		return FALSE ;
	}
	TRACE("Pixel Format %d\r\n", nPixelFormat) ;

	// Set the pixel format.
   BOOL bResult = SetPixelFormat(m_pdc->m_hDC, nPixelFormat, &pfd);
   if (!bResult)
	{
		TRACE("SetPixelFormat Failed %d\r\n",GetLastError()) ;
		return FALSE ;
	}
	
	// Create the palette
	CreatePalette() ;	
	
   //
   // Create a rendering context.
   //
   m_hrc = wglCreateContext(m_pdc->m_hDC);
	if (!m_hrc)
	{
		TRACE("wglCreateContext Failed %x\r\n", GetLastError()) ;
		return FALSE;
	}

	// Window size is 0,0 here. Don't wglMakeCurrent.
	// wglMakeCurrent will be called in Resize 
	
	// A dibsection should set the color palette after this call.
	//	m_pDibSurf->SetPalette(m_pPal) ; // DIBSECTION
	//	if (m_pPal) setPaletteDIB() ;

	return TRUE;
		
}

//
// Create
//
BOOL CGL::Create(CWnd* pWnd, int iPixelType, DWORD dwFlags)
{
	TRACE0("CGL::Create\r\n") ;
	ASSERT(pWnd) ;
	ASSERT( (iPixelType == PFD_TYPE_RGBA) || (iPixelType == PFD_TYPE_COLORINDEX)) ;


	//
	// Fill in the Pixel Format Descriptor
	//
   PIXELFORMATDESCRIPTOR pfd ;
	memset(&pfd,0, sizeof(PIXELFORMATDESCRIPTOR)) ;
   pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);  
   pfd.nVersion = 1 ;                           // Version number
	pfd.dwFlags =  dwFlags ;
	pfd.iPixelType = iPixelType ;
	pfd.cColorBits = 24 ;						// 24 of bits for color
	pfd.cDepthBits = 32 ;						// 32-bit depth buffer
   pfd.iLayerType = PFD_MAIN_PLANE ;           // Layer type

	// Let children change creation.
	OnCreate(pWnd, &pfd) ;

	//
	// Use the other Create function for rendering on bitmaps.
	//
	ASSERT( !(dwFlags & PFD_DRAW_TO_BITMAP)) ;
	ASSERT( (dwFlags & PFD_DRAW_TO_WINDOW)) ;

	// Determine double buffering state.
	m_bDoubleBuffer = pfd.dwFlags & PFD_DOUBLEBUFFER ;

	// Create a dc
	m_pdc = new CClientDC(pWnd);

	// Chose the pixel format.
   int nPixelFormat = ChoosePixelFormat(m_pdc->m_hDC, &pfd);
	if (nPixelFormat == 0)
	{
		TRACE("ChoosePixelFormat Failed %d\r\n",GetLastError()) ;
		return FALSE ;
	}
	TRACE("Pixel Format %d\r\n", nPixelFormat) ;

	// Set the pixel format.
   BOOL bResult = SetPixelFormat(m_pdc->m_hDC, nPixelFormat, &pfd);
	if (!bResult)
	{
		TRACE("SetPixelFormat Failed %d\r\n",GetLastError()) ;
		return FALSE ;
	}
	
	// Create the palette
	CreatePalette() ;

	//
   // Create a rendering context.
   //
   m_hrc = wglCreateContext(m_pdc->m_hDC);
	if (!m_hrc)
	{
		TRACE("wglCreateContext Failed %x\r\n", GetLastError()) ;
		return FALSE;
	}

	// Window size is 0,0 here. Don't wglMakeCurrent.
	// wglMakeCurrent will be called in Resize
	return TRUE;
}

//
// Resize
//
BOOL CGL::Resize(int cx, int cy) 
{
	if ( (cx <= 0) || (cy <= 0) ) return FALSE;
	MakeCurrent() ;	

	return OnResize(cx, cy) ;    
}

//
// Init
//
BOOL CGL::Init()
{
	MakeCurrent() ;
	return OnInit() ;
}

//
// Render
//
BOOL CGL::Render()
{
	// Make the HGLRC current
	MakeCurrent();

	// Draw	
	OnRender();

   // Glush OpenGL 
	glFlush();

   // Flush GDI
   GdiFlush();

	// Swap Buffers
	if (m_bDoubleBuffer)
   {
		SwapBuffers(m_pdc->m_hDC);
      GdiFlush();
   }

	return TRUE;
}

CDC* CGL::GetDC(void)
{
	return m_pdc;
}

void CGL::RenderGradient(LPRECT lpRect, GRAPH_GRADIENTTYPE grType, COLORREF color1, COLORREF color2)
{
	double r1 = (double)GetRValue(color1) / 0xFF;
	double g1 = (double)GetGValue(color1) / 0xFF;
	double b1 = (double)GetBValue(color1) / 0xFF;
	double r2 = (double)GetRValue(color2) / 0xFF;
	double g2 = (double)GetGValue(color2) / 0xFF;
	double b2 = (double)GetBValue(color2) / 0xFF;

	switch (grType)
	{
	case GR_RIGHTTOLEFT:
		{
			glBegin(GL_QUADS);
			glColor3d(r2, g2, b2);
			glVertex2i(lpRect->left, lpRect->top);
			glColor3d(r1, g1, b1);
			glVertex2i(lpRect->right, lpRect->top);
			glVertex2i(lpRect->right, lpRect->bottom);
			glColor3d(r2, g2, b2);
			glVertex2i(lpRect->left, lpRect->bottom);
			glEnd();
		} break;
	case GR_LEFTTORIGHT:
		{
			glBegin(GL_QUADS);
			glColor3d(r1, g1, b1);
			glVertex2i(lpRect->left, lpRect->top);
			glColor3d(r2, g2, b2);
			glVertex2i(lpRect->right, lpRect->top);
			glVertex2i(lpRect->right, lpRect->bottom);
			glColor3d(r1, g1, b1);
			glVertex2i(lpRect->left, lpRect->bottom);
			glEnd();
		} break;
	case GR_BOTTOMTOTOP:
		{
			glBegin(GL_QUADS);
			glColor3d(r2, g2, b2);
			glVertex2i(lpRect->left, lpRect->top);
			glVertex2i(lpRect->right, lpRect->top);
			glColor3d(r1, g1, b1);
			glVertex2i(lpRect->right, lpRect->bottom);
			glVertex2i(lpRect->left, lpRect->bottom);
			glEnd();
		} break;
	case GR_TOPTOBOTTOM:
		{
			glBegin(GL_QUADS);
			glColor3d(r1, g1, b1);
			glVertex2i(lpRect->left, lpRect->top);
			glVertex2i(lpRect->right, lpRect->top);
			glColor3d(r2, g2, b2);
			glVertex2i(lpRect->right, lpRect->bottom);
			glVertex2i(lpRect->left, lpRect->bottom);
			glEnd();
		} break;
	case GR_LEFTTOPTORIGHTBOTTOM:
		{
			glBegin(GL_QUADS);
			glColor3d(r1, g1, b1);
			glVertex2i(lpRect->left, lpRect->top);
			glColor3d(r1/2 + r2/2, g1/2 + g2/2, b1/2 + b2/2);
			glVertex2i(lpRect->right, lpRect->top);
			glColor3d(r2, g2, b2);
			glVertex2i(lpRect->right, lpRect->bottom);
			glColor3d(r1/2 + r2/2, g1/2 + g2/2, b1/2 + b2/2);
			glVertex2i(lpRect->left, lpRect->bottom);
			glEnd();
		} break;
	case GR_LEFTBOTTOMTORIGHTOP:
		{
			glBegin(GL_QUADS);
			glColor3d(r1/2 + r2/2, g1/2 + g2/2, b1/2 + b2/2);
			glVertex2i(lpRect->left, lpRect->top);
			glColor3d(r2, g2, b2);
			glVertex2i(lpRect->right, lpRect->top);
			glColor3d(r1/2 + r2/2, g1/2 + g2/2, b1/2 + b2/2);
			glVertex2i(lpRect->right, lpRect->bottom);
			glColor3d(r1, g1, b1);
			glVertex2i(lpRect->left, lpRect->bottom);
			glEnd();
		} break;
	case GR_RIGHTTOPTOLEFTBOTTOM:
		{
			glBegin(GL_QUADS);
			glColor3d(r1/2 + r2/2, g1/2 + g2/2, b1/2 + b2/2);
			glVertex2i(lpRect->left, lpRect->top);
			glColor3d(r1, g1, b1);
			glVertex2i(lpRect->right, lpRect->top);
			glColor3d(r1/2 + r2/2, g1/2 + g2/2, b1/2 + b2/2);
			glVertex2i(lpRect->right, lpRect->bottom);
			glColor3d(r2, g2, b2);
			glVertex2i(lpRect->left, lpRect->bottom);
			glEnd();
		} break;
	case GR_RIGHTBOTTOMTOLEFTTOP:
		{
			glBegin(GL_QUADS);
			glColor3d(r2, g2, b2);
			glVertex2i(lpRect->left, lpRect->top);
			glColor3d(r1/2 + r2/2, g1/2 + g2/2, b1/2 + b2/2);
			glVertex2i(lpRect->right, lpRect->top);
			glColor3d(r1, g1, b1);
			glVertex2i(lpRect->right, lpRect->bottom);
			glColor3d(r1/2 + r2/2, g1/2 + g2/2, b1/2 + b2/2);
			glVertex2i(lpRect->left, lpRect->bottom);
			glEnd();
		} break;
	}
}

void CGL::RenderSolid(LPRECT lpRect, COLORREF color)
{
	glBegin(GL_QUADS);
	SetGLColor(color);
	glVertex2i(lpRect->left, lpRect->top);
	glVertex2i(lpRect->right, lpRect->top);
	glVertex2i(lpRect->right, lpRect->bottom);
	glVertex2i(lpRect->left, lpRect->bottom);
	glEnd();
}

void CGL::SetGLColor(COLORREF color)
{
	double r = (double)GetRValue(color) / 0xFF;
	double g = (double)GetGValue(color) / 0xFF;
	double b = (double)GetBValue(color) / 0xFF;
	glColor3d(r, g, b);
}

