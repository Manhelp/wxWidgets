/////////////////////////////////////////////////////////////////////////////
// Name:        dcclient.cpp
// Purpose:     wxClientDC class
// Author:      AUTHOR
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "dcclient.h"
#endif

#include "wx/dcclient.h"
#include "wx/dcmemory.h"
#include "wx/region.h"
#include "wx/window.h"
#include <math.h>

//-----------------------------------------------------------------------------
// constants
//-----------------------------------------------------------------------------

#define RAD2DEG 57.2957795131

//-----------------------------------------------------------------------------
// wxPaintDC
//-----------------------------------------------------------------------------

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxWindowDC, wxDC)
IMPLEMENT_DYNAMIC_CLASS(wxClientDC, wxWindowDC)
IMPLEMENT_DYNAMIC_CLASS(wxPaintDC, wxWindowDC)
#endif

/*
 * wxWindowDC
 */

#include "wx/mac/uma.h"

wxWindowDC::wxWindowDC() 
{
}

wxWindowDC::wxWindowDC(wxWindow *the_canvas) 
{
	WindowRef windowref ;
	wxWindowMac* rootwindow ;
	
	// this time it is really the full window
	Rect clipRect ;
	the_canvas->MacGetPortParams(&m_macLocalOrigin, &clipRect , &windowref , &rootwindow );
	SetRectRgn( m_macBoundaryClipRgn , clipRect.left , clipRect.top , clipRect.right , clipRect.bottom ) ;
	SectRgn( m_macBoundaryClipRgn , the_canvas->MacGetVisibleRegion().GetWXHRGN() , m_macBoundaryClipRgn ) ;
	CopyRgn( m_macBoundaryClipRgn , m_macCurrentClipRgn ) ;
	m_macPort = UMAGetWindowPort( windowref ) ;
	m_minY = m_minX =  0;
	wxSize size = the_canvas->GetSize() ;
	m_maxX = size.x  ;
	m_maxY = size.y ; 

 	m_ok = TRUE ;
  SetBackground(the_canvas->MacGetBackgroundBrush());
}

wxWindowDC::~wxWindowDC()
{
}

/*
 * wxClientDC
 */

wxClientDC::wxClientDC()
{
}

wxClientDC::wxClientDC(wxWindow *window)
{
	WindowRef windowref ;
	wxWindowMac* rootwindow ;
	
	Rect clipRect ;
	wxPoint origin = window->GetClientAreaOrigin() ;
	
	window->MacGetPortClientParams(&m_macLocalOrigin, &clipRect , &windowref , &rootwindow );
	SetRectRgn( m_macBoundaryClipRgn , clipRect.left + origin.x , clipRect.top + origin.y , clipRect.right + origin.x , clipRect.bottom + origin.y ) ;
	SectRgn( m_macBoundaryClipRgn , window->MacGetVisibleRegion().GetWXHRGN() , m_macBoundaryClipRgn ) ;
	OffsetRgn( m_macBoundaryClipRgn , -origin.x , -origin.y ) ;
	CopyRgn( m_macBoundaryClipRgn , m_macCurrentClipRgn ) ;
	m_macPort = UMAGetWindowPort( windowref ) ;
	m_minY = m_minX =  0;
	wxSize size = window->GetSize() ;
	m_maxX = size.x  ;
	m_maxY = size.y ; 
 	m_ok = TRUE ;
  SetBackground(window->MacGetBackgroundBrush());
	SetFont( window->GetFont() ) ;
}

wxClientDC::~wxClientDC()
{
}

/*
 * wxPaintDC
 */

wxPaintDC::wxPaintDC()
{
}

wxPaintDC::wxPaintDC(wxWindow *window)
{
	WindowRef windowref ;
	wxWindowMac* rootwindow ;
	
	Rect clipRect ;
	wxPoint origin = window->GetClientAreaOrigin() ;
	
	window->MacGetPortClientParams(&m_macLocalOrigin, &clipRect , &windowref , &rootwindow );
	SetRectRgn( m_macBoundaryClipRgn , clipRect.left + origin.x , clipRect.top + origin.y , clipRect.right + origin.x , clipRect.bottom + origin.y ) ;
	SectRgn( m_macBoundaryClipRgn , window->MacGetVisibleRegion().GetWXHRGN() , m_macBoundaryClipRgn ) ;
	OffsetRgn( m_macBoundaryClipRgn , -origin.x , -origin.y ) ;
  SectRgn( m_macBoundaryClipRgn  , window->GetUpdateRegion().GetWXHRGN() , m_macBoundaryClipRgn ) ;
	CopyRgn( m_macBoundaryClipRgn , m_macCurrentClipRgn ) ;
	m_macPort = UMAGetWindowPort( windowref ) ;
 	m_ok = TRUE ;
	/*
	wxCoord x , y ,w , h ;
	window->GetUpdateRegion().GetBox( x , y , w , h ) ;
	m_minY = m_minX =  0;
	wxSize size = window->GetSize() ;
	m_maxX = size.x  ;
	m_maxY = size.y ; 
	SetClippingRegion( x , y , w , h ) ;
  	*/
  SetBackground(window->MacGetBackgroundBrush());
  SetFont(window->GetFont() ) ;
}

wxPaintDC::~wxPaintDC()
{
}
