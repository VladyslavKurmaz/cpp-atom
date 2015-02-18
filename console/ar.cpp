#include "./pch.hpp"
#include "./classes.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./ar.hpp"
#include "./window.hpp"

ar::ar( logger_ptr l, pref_ptr p, window_ptr w ):
		mode( l, p, w )
	,	ctrl()
	,	hStatic( NULL ) {
}

ar::~ar() {
}

void ar::activate( bool const state ) {
	if ( !hStatic ) {
		window_ptr w = this->getWindow();
		placement p;
		w->getPlacement( p );
		hStatic = CreateWindowEx( WS_EX_TOOLWINDOW, "EDIT", "Control panel", WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX, p.destination.right - 256, 0, 256, RECT_HEIGHT( p.destination ) / 2 , w->get_hwnd(), NULL, (HINSTANCE)GetModuleHandle(NULL), NULL );
		LONG clst = GetClassLong( hStatic, GCL_STYLE );
		SetClassLong( hStatic, GCL_STYLE, clst | CS_NOCLOSE );
	}
	ShowWindow( hStatic, ( state )?( SW_SHOW ):( SW_HIDE ) );
}

void ar::show( bool const state ) {
	ShowWindow( hStatic, ( state )?( SW_SHOW ):( SW_HIDE ) );
}

bool ar::command( int const id ) {
	return false;
}

void ar::key( KEY_EVENT_RECORD const& k ) {
}

void ar::mouselbdown( bool dblclick, int x, int y, unsigned int state ) {
	window_ptr w = this->getWindow();
	if ( !dblclick ) {
		ctrl.start( x, y );
		w->inputCapture( window::keyboard );
		w->inputCapture( window::mouse );
		w->invalidate();
	}
}

void ar::mouselbup( int x, int y, unsigned int state ) {
	window_ptr w = this->getWindow();
	RECT r;
	if ( w->inputIsCaptured( window::mouse ) ) {
		ctrl.getRect( r );
		//bitmapSave( "desktop.bmp", this->paintParam.dcb.dc, this->paintParam.dcb.bitmap );
		HDC dc = GetDC( NULL );
		size_t const cx = RECT_WIDTH( r );
		size_t const cy = RECT_HEIGHT( r );
		if ( ( cx > 8 ) && ( cy > 8 ) ) {
			dcb_t dcb;
			dcb.updateDC( cx, cy );
			BitBlt( dcb.dc,
				0,
				0,
				RECT_WIDTH( r ),
				RECT_HEIGHT( r ),
				dc,
				r.left,
				r.top,
				SRCCOPY );
			bitmapSave( "desktop.bmp", dcb.dc, dcb.bitmap );
			ReleaseDC( NULL, dc );
			//
			atom::proc<TCHAR> proc;
			if ( proc.run( "tesseract desktop.bmp out", false ) ) {
				proc.join();
			}

			std::ifstream t("out.txt");
			std::string str;

			t.seekg(0, std::ios::end);   
			str.reserve(t.tellg());
			t.seekg(0, std::ios::beg);

			str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			//
			SetWindowText( hStatic, str.c_str() );
		}
	}
	w->inputRelease( window::mouse );
}

void ar::mousemove( int x, int y, unsigned int state ){
	window_ptr w = this->getWindow();
	if ( w->inputIsCaptured( window::mouse ) ) {
		RECT r;
		GetClientRect( w->get_hwnd(), &r );
		ctrl.update( x, y, ( GetKeyState( VK_SPACE ) & 0x80 ) > 0, r );
		w->invalidate();
	}
}

void ar::paint( paint_param_t& paintParam, RECT const& rect ) {
	window_ptr w = this->getWindow();
	if ( w->inputIsCaptured(window::mouse ) ) {
		RECT rt;
		ctrl.getRect( rt );
		FrameRect( paintParam.dcb.dc, &rt, (HBRUSH)GetStockObject( WHITE_BRUSH ) );
	}
}

void ar::clear() {
	base_t::clear();
}


