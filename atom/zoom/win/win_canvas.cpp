#include "./win_canvas.hpp"

namespace atom { namespace zoom {

	win_canvas::win_canvas( logger_ptr l, stream_ptr s ) : 
			canvas( l, s )
		,	wwindow( *this, INITLIST_4( &win_canvas::onlbuttondown, &win_canvas::onlbuttonup, &win_canvas::onmousemove, &win_canvas::onclose ) )
		,	input_handler() {
	}

	win_canvas::~win_canvas(){
	}

	bool
	win_canvas::init(){
		if ( canvas::init() ) {
			RECT rect;
			DWORD const style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
			DWORD const ex_style = WS_EX_APPWINDOW;
			struct _
			{
				static bool __( WNDCLASSEX& wcex, CREATESTRUCT& cs, RECT const& rect, DWORD const style, DWORD const ex_style )
				{
					wcex.cbSize;
					wcex.style;
					wcex.lpfnWndProc;
					wcex.cbClsExtra;
					wcex.cbWndExtra;
					wcex.hInstance;
					wcex.hIcon;
					wcex.hCursor;
					wcex.hbrBackground;
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
			SetRect( &rect, 0, 0, 800, 600 );
			wwindow_base_t::calc_rect( rect, style, ex_style, false, true );
			if ( wwindow_base_t::init( boost::bind( _::__, _1, _2, boost::ref( rect ), style, ex_style ), true ) )
			{
				wwindow_base_t::show( true );
				return true;
			}
		}
		return false;
	}

	void*
	win_canvas::get_handle() {
		return static_cast<void*>( wwindow_base_t::get_hwnd() );
	}

	void
	win_canvas::run( boost::function< bool() > tick, boost::function< void( input_ptr ) > handler ) {
		this->input_handler = handler;
		wwindow_base_t::run( tick, 0 );
		this->input_handler = boost::function< void( input_ptr ) >();
	}
		
	void
	win_canvas::clear(){
		canvas::clear();
	}

	void
	win_canvas::onlbuttondown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
		//SetCapture( hWnd );
		////
		//POINT pt;
		//GetCursorPos( &pt );
		////
		//POINT c_pt;
		//get_center_pos( hWnd, c_pt );
		////
		//mouse_dx = 0;
		//mouse_dy = 0;
		////
		//SetCursorPos( c_pt.x, c_pt.y );
		this->input_handler( input_ptr( this->shared_from_this() ) );
	}

	void
	win_canvas::onlbuttonup(HWND hWnd, int x, int y, UINT keyFlags){
		//if ( GetCapture() == hWnd )
		//	ReleaseCapture();
		this->input_handler( input_ptr( this->shared_from_this() ) );
	}

	void
	win_canvas::onmousemove(HWND hWnd, int x, int y, UINT keyFlags){
		//if ( GetCapture() == hWnd ) {
		//	POINT c_pt;
		//	get_center_pos( hWnd, c_pt );
		//	//
		//	POINT pt;
		//	pt.x = x;
		//	pt.y = y;
		//	MapWindowPoints( hWnd, NULL, &pt, 1 );
		//	//
		//	mouse_dx += ( pt.x - c_pt.x );
		//	mouse_dy += ( pt.y - c_pt.y );
		//	//
		//	if ( ( c_pt.x != pt.x ) || ( c_pt.y != pt.y ) )
		//		SetCursorPos( c_pt.x, c_pt.y );
		//	SetCursor( NULL );
		//}
		this->input_handler( input_ptr( this->shared_from_this() ) );
	}

	void
	win_canvas::onclose( HWND hWnd ) {
		PostQuitMessage( 0 );
	}


} }
