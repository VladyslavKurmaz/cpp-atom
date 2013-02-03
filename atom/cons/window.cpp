#include "./pch.hpp"
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "./window.hpp"

window::window( logger::shared_ptr l, pref::shared_ptr p ) {
	atom::mount<window2logger>( this, l );
	atom::mount<window2pref>( this, p );

	atom::mount<window2frame>( this, frame::create( l, p ) );
	atom::mount<window2frame>( this, frame::create( l, p ) );
	atom::mount<window2frame>( this, frame::create( l, p ) );
	atom::mount<window2frame>( this, frame::create( l, p ) );
	atom::mount<window2frame>( this, frame::create( l, p ) );
	

	this->get_logger() << "create window" << std::endl;
}

window::~window() {
	this->get_logger() << "free window" << std::endl;
}

bool window::init() {
	RECT rect;
	DWORD const style = 0;//WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX | WS_THICKFRAME;
	DWORD const ex_style = WS_EX_TOPMOST;
	struct _ {
		static bool __( WNDCLASSEX& wcex, CREATESTRUCT& cs, RECT const& rect, DWORD const style, DWORD const ex_style ) {
			wcex.cbSize;
			wcex.style			=	CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc;
			wcex.cbClsExtra;
			wcex.cbWndExtra;
			wcex.hInstance;
			wcex.hIcon;
			wcex.hCursor;
			wcex.hbrBackground	=	(HBRUSH)GetStockObject( DKGRAY_BRUSH/*BLACK_BRUSH*/ );
			wcex.lpszMenuName;
			wcex.lpszClassName;
			wcex.hIconSm;
			//
			cs.lpCreateParams;
			cs.hInstance;
			cs.hMenu;
			cs.hwndParent;
			cs.cy				=	rect.bottom - rect.top;
			cs.cx				=	rect.right - rect.left;
			cs.y				=	rect.top;
			cs.x				=	rect.left;
			cs.style			=	style;
			cs.lpszName;
			cs.lpszClass;
			cs.dwExStyle		=	ex_style;
			return true;
		}
	};
	SystemParametersInfo( SPI_GETWORKAREA, 0, &rect, 0 );
	//rect.bottom = rect.top + ( rect.bottom - rect.top ) / 2;
	rect.right = rect.left + ( rect.right - rect.left ) / 2;
	SetRect( &rect, 0, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ) / 2 );
	//window::calc_rect( rect, style, ex_style, false, true );
	if ( atom::wwindow::init( boost::bind( _::__, _1, _2, boost::ref( rect ), style, ex_style ), true ) ) {
		SetWindowLong( this->get_hwnd(), GWL_STYLE, WS_OVERLAPPED );
		SetWindowLong( this->get_hwnd(), GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED );

		SetWindowPos( this->get_hwnd(), 0/*HWND_NOTOPMOST*/, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED );
		SetLayeredWindowAttributes( this->get_hwnd(), RGB( 0, 0, 0 ), 224, LWA_ALPHA );
		return true;
	}
	return false;
}
  
void window::run() {
	atom::wwindow::run();
}





