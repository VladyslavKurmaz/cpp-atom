#include "./pch.hpp"
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "./window.hpp"

window::window( logger::shared_ptr l, pref::shared_ptr p ) : wwindow( *this, INITLIST_4( &window::onchar, &window::onhotkey, &window::onpaint, &window::onclose ) ) {
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
			wcex.hbrBackground	=	(HBRUSH)GetStockObject( DKGRAY_BRUSH /*BLACK_BRUSH*/ );
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
	if ( base_window_t::init( boost::bind( _::__, _1, _2, boost::ref( rect ), style, ex_style ), true ) ) {
		this->set_styles( WS_OVERLAPPED, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED ).set_alpha( 240 );



		if ( RegisterHotKey( this->get_hwnd(), 1, MOD_WIN | MOD_NOREPEAT, VK_OEM_3 )) {
			this->get_logger() << "Hotkey registered, using MOD_NOREPEAT flag" << std::endl;
		} else {
			this->get_logger() << "Hotkey register error" << std::endl;
		}
		return true;
	}
	return false;
}
  
void window::run() {
	base_window_t::run();
}
std::basic_string< TCHAR > str;
void window::onchar( HWND hWnd, TCHAR ch, int cRepeat ) {
	str += ch;
	InvalidateRect( hWnd, NULL, TRUE );
	//this->get_logger() << ch << ":" << (unsigned int)(unsigned char)ch << " ";
}

void window::onhotkey( HWND hWnd, int idHotKey, UINT fuModifiers, UINT vk ) {
	this->show( !this->is_visible() );
	if ( this->is_visible() ) {
		this->activate();
	}
}

void window::onpaint( HWND hWnd ) {
	PAINTSTRUCT ps; 
	RECT rt;
	HDC hdc = BeginPaint( hWnd, &ps ); 
	GetClientRect( hWnd, &rt );
	InflateRect( &rt, -1, -1 );
	FrameRect( hdc, &rt, (HBRUSH)GetStockObject( WHITE_BRUSH ) );
	TextOut( hdc, 10, 10, str.c_str(), str.length() );
	EndPaint( hWnd, &ps ); 
}

void window::onclose( HWND ) {
	PostQuitMessage( 0 );
}





