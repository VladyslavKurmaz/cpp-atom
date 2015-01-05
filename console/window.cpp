#include "./pch.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./shell.hpp"
#include "./window.hpp"


window::window( logger_ptr l, pref_ptr p ) :
		wwindow( *this, INITLIST_9( &window::onKey, &window::onKey, &window::onChar, &window::onHotkey, &window::onPaint, &window::onClose, &window::onSettingChange, &window::onTimer, &window::onCommand ) )
	,	appearHotKey()
	,	accel()
	,	windowPlacement()
	,	paintParam()
	,	sh() {
	//
	atom::mount<window2logger>( this, l );
	atom::mount<window2pref>( this, p );
	//
	sh = shell::create( l, p );
}

window::~window() {
}

bool window::init() {
	DWORD const style = 0;
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
			wcex.hbrBackground	=	NULL;
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
	// prepare paint objects
	//
	std::string const d1( DELIM1 );
	std::string const d2( DELIM2 );
	//
	static const struct font_t {
		atom::po::id_t	opt;
	} fonts[] = {
		po_ui_font_text,
		po_ui_font_sys
	};
	//
	BOOST_FOREACH( font_t const& font, fonts )
	{
		std::string s = this->getPref().get< std::string >( font.opt );
		atom::attributes< TCHAR > a( s, d1, d2 );
		HFONT f = CreateFont(
			a.as<unsigned int>(_T("height") ),
			0,
			0,
			0,
			FW_NORMAL,
			FALSE,
			FALSE,
			FALSE,
			OEM_CHARSET,
			OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			FIXED_PITCH,
			a.as<std::string>(_T("name") ).c_str()
			);
		unsigned int color = a.as_color( _T("color") );
		//
		if ( f != NULL ) {
			switch( font.opt ) {
			case po_ui_font_text:
				this->paintParam.textFont	= f;
				this->paintParam.textColor	= color;
				break;
			case po_ui_font_sys:
				this->paintParam.sysFont	= f; 
				this->paintParam.sysColor	= color;
				break;
			};
		} else {
			this->getLogger() << "Text font creation error: " << s << std::endl;
		}
	}
	//
	this->paintParam.bk = CreateSolidBrush( this->getPref().get< unsigned int >( po_ui_bk_color ) );

	atom::attributes< TCHAR > padding( this->getPref().get< std::string >( po_ui_padding ), d1, d2 );
	unsigned int const p = padding.as<unsigned int>(_T("size") );
	SetRect( &this->paintParam.padding, p, p, p, p );

	atom::attributes< TCHAR > border( this->getPref().get< std::string >( po_ui_border ), d1, d2 );
	this->paintParam.borderActive			= CreateSolidBrush( border.as_color( _T("color") ) );
	this->paintParam.borderInactive			= CreateSolidBrush( border.as_color( _T("inactive") ) );
	//
	this->updatePlacement( false, false );
	if ( base_window_t::init( boost::bind( _::__, _1, _2, boost::ref( this->windowPlacement.destination ), style, ex_style ), true ) ) {
		this->set_styles( WS_OVERLAPPED, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED ).set_alpha( this->getPref().get< unsigned int >( po_ui_alpha ) );
		//
		hotkey new_hk;
		if ( this->getPref().parseHotkey( po_hk_appear, new_hk ) ) {
			//
			if ( new_hk != this->appearHotKey ) {
				new_hk.id++;
				if ( RegisterHotKey( this->get_hwnd(), new_hk.id, new_hk.mods, new_hk.vk )) {
					if ( this->appearHotKey.id && !UnregisterHotKey( this->get_hwnd(), this->appearHotKey.id ) ) {
						this->getLogger() << "Hotkey unregister error" << std::endl;
					}
					this->appearHotKey = new_hk; 
				} else {
					this->getLogger() << "Hotkey register error" << std::endl;
				}
			}
		}
		//
		pref::opt2cmd_t tags[] = {
			{ po_hk_entire_screen,	CMDID_FULLSCREEN },
			{ po_hk_frame_split,	CMDID_SPLIT },
			{ po_hk_frame_minmax,	CMDID_EXPAND },
			{ po_hk_frame_rotate,	CMDID_ROTATE },
			{ po_hk_frame_next,		CMDID_NEXT },
			{ po_hk_frame_prev,		CMDID_PREV },
			{ po_hk_frame_close,	CMDID_CLOSE },
			{ po_hk_ctrl_break,		CMDID_CTRL_BREAK },
			{ po_hk_ctrl_c,			CMDID_CTRL_C },
			{ po_hk_tty1,			CMDID_TTY1 },
			{ po_hk_tty2,			CMDID_TTY2 },
			{ po_hk_tty3,			CMDID_TTY3 },
			{ po_hk_tty4,			CMDID_TTY4 },
			{ po_hk_tty5,			CMDID_TTY5 },
			{ po_hk_tty6,			CMDID_TTY6 }
		};
		this->getPref().parseAccel( tags, this->accel );
		this->accel.build();
		//
		return true;
	}
	return false;
}
  
void window::run() {
	struct _ {
		static bool __( HWND hWnd, MSG* msg, atom::accel& accel, window& w ) {
			return accel.translate( hWnd, msg );
		}
	};
	this->toggleVisibility();
	base_window_t::run( boost::bind( _::__, _1, _2, boost::ref( this->accel ), boost::ref( *this ) ) );
}

void window::clear() {
	this->sh->clear();
	base_node_t::clear();
}

void window::onKey( HWND hWnd, UINT vk, BOOL down, int repeat, UINT flags ) {
	KEY_EVENT_RECORD key;
	key.bKeyDown			=	down;
	key.wRepeatCount		=	repeat;
	key.wVirtualKeyCode		=	vk;
	key.wVirtualScanCode	=	MapVirtualKey( vk, MAPVK_VK_TO_VSC );
	BYTE kbrd[256] = { 0 };
	WORD c = 0;
	GetKeyboardState( kbrd );
	ToAscii(
		key.wVirtualKeyCode,
		key.wVirtualScanCode,
		kbrd,
		&c,
		0
		);
	key.uChar.UnicodeChar	=	c;
	key.uChar.AsciiChar;
	key.dwControlKeyState	=
		( ( GetKeyState( VK_CAPITAL ) & 0x01 ) ? ( CAPSLOCK_ON ) : ( 0 ) ) |
		//ENHANCED_KEY
		( ( GetKeyState( VK_LMENU ) & 0x80 ) ? ( LEFT_ALT_PRESSED ) : ( 0 ) ) |
		( ( GetKeyState( VK_LCONTROL ) & 0x80 ) ? ( LEFT_CTRL_PRESSED ) : ( 0 ) ) |
		( ( GetKeyState( VK_NUMLOCK ) & 0x01 ) ? ( NUMLOCK_ON ) : ( 0 ) ) |
		( ( GetKeyState( VK_RMENU ) & 0x80 ) ? ( RIGHT_ALT_PRESSED ) : ( 0 ) ) |
		( ( GetKeyState( VK_RCONTROL ) & 0x80 ) ? ( RIGHT_CTRL_PRESSED ) : ( 0 ) ) |
		( ( GetKeyState( VK_SCROLL ) & 0x01 ) ? ( SCROLLLOCK_ON ) : ( 0 ) ) |
		( ( GetKeyState( VK_SHIFT ) & 0x80 ) ? ( SHIFT_PRESSED ) : ( 0 ) ) ;

	//this->getLogger() << vk << ((down)?(" down"):(" up")) << std::endl;
	this->sh->key( key );
	this->invalidate();
}

void window::onChar( HWND hWnd, TCHAR ch, int cRepeat ) {
}

void window::onHotkey( HWND hWnd, int idHotKey, UINT fuModifiers, UINT vk ) {
	if ( idHotKey == this->appearHotKey.id ) {
		this->toggleVisibility();
	}
}

void window::onPaint( HWND hWnd ) {
	PAINTSTRUCT	ps; 
	RECT 		rect;
	HDC			dc = BeginPaint( hWnd, &ps );
	{
		GetClientRect( hWnd, &rect );
		if ( this->windowPlacement.sliding ) {
			//SIZE bmsz = { 0 };
			//GetBitmapDimensionEx( this->paintParam.bitmap, &bmsz );
			//StretchBlt( dc,
			//	0,
			//	0,
			//	rect.right,
			//	rect.bottom,
			//	this->paintParam.dc,
			//	0,
			//	0,
			//	bmsz.cx,
			//	bmsz.cy,
			//	SRCCOPY );
			FillRect( dc, &rect, this->paintParam.bk );
		} else {
			FillRect( this->paintParam.dc, &rect, this->paintParam.bk );
			this->sh->paint( this->paintParam, rect );
			BitBlt( dc,
				0,
				0,
				rect.right,
				rect.bottom,
				this->paintParam.dc,
				0,
				0,
				SRCCOPY );
		}
	}
	EndPaint( hWnd, &ps );
}

void window::onClose( HWND hWnd ) {
	PostQuitMessage( 0 );
}

void window::onSettingChange( HWND hWnd, UINT uiAction, LPCTSTR lpName ) {
	switch( uiAction ) {
	case SPI_SETWORKAREA:
	case SPI_ICONVERTICALSPACING:
		this->updatePlacement( this->windowPlacement.visible, this->windowPlacement.fullScreen );
		this->slideBegin();
		break;
	}
}

void window::onTimer( HWND hWnd, UINT id ){
	if ( id == this->windowPlacement.timerId ) {
		slideUpdate();
	};
}

void window::onCommand( HWND hWnd, int id, HWND hwndCtl, UINT codeNotify ) {
	if ( !sh->command( id ) ) {
		switch( id ) {
		case CMDID_FULLSCREEN:
			this->toggleFullScreen();
			break;
		}
	}
	this->invalidate();
}

void window::toggleVisibility() {
	this->updatePlacement( !this->windowPlacement.visible, this->windowPlacement.fullScreen );
	this->slideBegin();
}

void window::toggleFullScreen(){
	this->updatePlacement( this->windowPlacement.visible, !this->windowPlacement.fullScreen );
	this->slideBegin();
}

void window::updatePlacement( bool const visible, bool const fullScreen ) {
	RECT rt = this->windowPlacement.destination;
	this->windowPlacement.visible = visible;
	this->windowPlacement.fullScreen = fullScreen;
	this->getPref().calculateDocks( this->windowPlacement );
	// update mem dc
	SIZE sz;
	sz.cx = RECT_WIDTH( this->windowPlacement.destination );
	sz.cy = RECT_HEIGHT( this->windowPlacement.destination );
	if ( ( RECT_WIDTH( rt ) != sz.cx ) || ( RECT_HEIGHT( rt ) != sz.cy ) ) {
		this->paintParam.updareDC( sz );
	}
}

void window::slideBegin() {
	if ( this->windowPlacement.sliding ) {
		this->windowPlacement.startTime -= this->windowPlacement.timeout - 2 * ( timeGetTime() - this->windowPlacement.startTime );
	} else {
		this->windowPlacement.sliding = true;
		//
		this->windowPlacement.startTime =
		this->windowPlacement.lastTime = timeGetTime();
		SetTimer( this->get_hwnd(), this->windowPlacement.timerId, USER_TIMER_MINIMUM, NULL );
		this->show( true ).activate();
	}
}

bool operator==( RECT const& l, RECT const& r ) {
	return ( ( l.left == r.left ) && ( l.top == r.top ) &&  ( l.right == r.right ) &&  ( l.bottom == r.bottom ) );
}

bool operator!=( RECT const& l, RECT const& r ) {
	return ( !( l == r ) );
}

template < typename T >
struct slider {
	slider< T > const& operator()( T &r, T const v1, T const v2, LONG const left, DWORD const dt ) const {
		float const x = (float)( left - (LONG)dt);
		float const a = (float)( v2 - v1 ) / (float)( left * left );
		r = v2 - (T)( a * x * x );
		return (*this);
	}
};

void window::slideUpdate() {
	if ( this->windowPlacement.sliding ) {
		DWORD const total = this->windowPlacement.timeout;
		bool const show = this->windowPlacement.sliding;
		RECT srect;
		RECT drect = this->windowPlacement.destination;
		GetWindowRect( this->get_hwnd(), &srect );
		//
		BYTE salpha = 0;
		BYTE dalpha = this->windowPlacement.alpha;
		this->getAlpha( salpha );
		//
		RECT rt = drect;
		BYTE alpha = dalpha;
		//
		DWORD const ctime = timeGetTime();
		DWORD const dt = ctime - this->windowPlacement.lastTime;
		LONG const left = (LONG)this->windowPlacement.startTime + (LONG)this->windowPlacement.timeout - ctime;
		this->windowPlacement.lastTime += dt;
		//
		if ( left > 0 ) {
			slider< LONG > s;
			s	( rt.left,		srect.left,		drect.left, left, dt )
				( rt.top,		srect.top,		drect.top, left, dt )
				( rt.right,		srect.right,	drect.right, left, dt )
				( rt.bottom,	srect.bottom,	drect.bottom, left, dt );
			slider< BYTE > a;
			a	( alpha,		salpha,			dalpha,	left,	dt );
		} else {
			// stop
			this->show( show );
			if ( show ) {
				this->activate().focus();
			}
			KillTimer( this->get_hwnd(), this->windowPlacement.timerId );
			this->windowPlacement.sliding = false;
			this->invalidate();
		}
		MoveWindow( this->get_hwnd(), rt.left, rt.top, RECT_WIDTH( rt ), RECT_HEIGHT( rt ), TRUE );
		this->set_alpha( alpha );
	}
}



