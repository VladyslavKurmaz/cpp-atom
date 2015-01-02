#include "./pch.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./frame.hpp"
#include "./window.hpp"


window::window( logger_ptr l, pref_ptr p ) :
		wwindow( *this, INITLIST_9( &window::onKey, &window::onKey, &window::onChar, &window::onHotkey, &window::onPaint, &window::onClose, &window::onSettingChange, &window::onTimer, &window::onCommand ) )
	,	appearHotKey()
	,	accel()
	,	windowPlacement() {
	//
	atom::mount<window2logger>( this, l );
	atom::mount<window2pref>( this, p );
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
	base_node_t::clear();
}

void window::onKey( HWND hWnd, UINT vk, BOOL down, int repeat, UINT flags ){
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
		FillRect( dc, &rect, (HBRUSH)GetStockObject( BLACK_BRUSH ) );
		FrameRect( dc, &rect, (HBRUSH)GetStockObject( GRAY_BRUSH ) );
	}
	EndPaint( hWnd, &ps );
}

void window::onClose( HWND hWnd ) {
	PostThreadMessage( GetWindowThreadProcessId( hWnd, 0 ) , WM_QUIT, 0, 0 );
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
	switch( id ) {
	case CMDID_FULLSCREEN:
		this->toggleFullScreen();
		break;
	case CMDID_SPLIT:
		break;
	case CMDID_EXPAND:
		break;
	case CMDID_ROTATE:
		break;
	case CMDID_NEXT:
		break;
	case CMDID_PREV:
		break;
	case CMDID_CTRL_BREAK:
		break;
	case CMDID_CTRL_C:
		break;
	case CMDID_CLOSE:
		break;
	case CMDID_TTY1:
	case CMDID_TTY2:
	case CMDID_TTY3:
	case CMDID_TTY4:
	case CMDID_TTY5:
	case CMDID_TTY6:
		break;
	default:
		return;
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
	this->windowPlacement.visible = visible;
	this->windowPlacement.fullScreen = fullScreen;
	this->getPref().calculateDocks( this->windowPlacement );
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
		}
		MoveWindow( this->get_hwnd(), rt.left, rt.top, RECT_WIDTH( rt ), RECT_HEIGHT( rt ), TRUE );
		this->set_alpha( alpha );
	}
}



