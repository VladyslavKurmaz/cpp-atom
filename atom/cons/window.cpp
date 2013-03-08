#include "./pch.hpp"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include "./window.hpp"

#define RECT_WIDTH( r ) ( (r).right - (r).left )
#define RECT_HEIGHT( r ) ( (r).bottom - (r).top )

window::window( logger::shared_ptr l, pref::shared_ptr p ) :
wwindow( *this, INITLIST_7( &window::onchar, &window::onhotkey, &window::onpaint, &window::onclose, &window::onsettingchange, &window::ontimer, &window::oncommand ) )
	,	current_frame()
	,	expand_mode( false )
	,	child()
	,	appear_hk()
	,	accel()
	,	anchor()
	,	in_rect()
	,	slide_dir( 0 )
	,	slide_start_time()
	,	slide_timer_id( 1 )
	,	font( NULL ) {
	//
	atom::mount<window2logger>( this, l );
	atom::mount<window2pref>( this, p );

	this->head_frame = 
	this->current_frame = frame::create( l, p, frame::frame_coord( 0, 1, 0, 1, 1, 1 ) );
	child = process::create( l, this->head_frame );
	atom::mount<window2frame>( this, this->head_frame );
 
	child->run( "cmd.exe" );
	//child->run( "msbuild.exe" );
	//child->run( "c:\\work\\env\\cygwin\\bin\\bash.exe --login -i" );
	//child->run( "powershell.exe" );
	//child->run( "cmd /c \"powershell.exe\"" );

	this->get_logger() << "create window" << std::endl;
}

window::~window() {
	delete_font();
	this->get_logger() << "free window" << std::endl;
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
	this->update_placement();
	if ( base_window_t::init( boost::bind( _::__, _1, _2, boost::ref( this->in_rect ), style, ex_style ), true ) ) {
		this->set_styles( WS_OVERLAPPED, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED ).set_alpha( get_pref().get< unsigned int >( po_ui_alpha ) );
		//
		this->update_hotkeys();
		this->update_accels();
		this->update_font();
		//
		return true;
	}
	return false;
}
  
void window::run() {
	struct _ {
		static bool __( HWND hWnd, MSG* msg, atom::accel& accel ) {
			return accel.translate( hWnd, msg );
		}
	};
	base_window_t::run( boost::bind( _::__, _1, _2, boost::ref( this->accel ) ) );
}

void window::clear() {
	window2frame& l = this->get_value( boost::mpl::identity< window2frame >() );
	struct _{
		static bool __( frame::shared_ptr& f ) {
			f->clear();
			return true;
		}
	};
	l.for_each( boost::bind( &_::__, _1 ) );
}

void window::onchar( HWND hWnd, TCHAR ch, int cRepeat ) {
	if ( ch == VK_RETURN ) {
		child->write( "\x0D\x0A" );
	} else {
		child->write( ch );
	}
	InvalidateRect( hWnd, NULL, TRUE );
}

void window::onhotkey( HWND hWnd, int idHotKey, UINT fuModifiers, UINT vk ) {
	if ( idHotKey == this->appear_hk.id ) {
		if ( slide_dir ) {
			slide_dir *= -1;
			slide_start_time += 2 * ( timeGetTime() - slide_start_time ) - get_pref().get< unsigned int >( po_ui_timeout );
		} else {
			slide_dir = ((this->is_visible())?(-1):(1));
			slide_start_time = timeGetTime();
			SetTimer( hWnd, slide_timer_id, USER_TIMER_MINIMUM, NULL );
			this->show( true ).activate();
		}
	}
}

void window::onpaint( HWND hWnd ){
	struct context {
		HWND			wnd;
		PAINTSTRUCT 	ps; 
		HDC 			dc; 
		RECT 			rect;
		unsigned int	margin;
		COLORREF		margin_color;
		unsigned int	border;
		COLORREF		border_color;
		unsigned int	padding;
		COLORREF		padding_color;
		HFONT			font;
		COLORREF		font_color;
		frame::shared_ptr
						current;

		context( HWND w, window& pw ) : wnd( w ) {
			dc = BeginPaint( wnd, &ps ); 
			GetClientRect( wnd, &rect );
			//
			font		= pw.font;
			font_color	= pw.get_pref().get< unsigned int >( po_ui_font_color );
			current		= pw.current_frame;
		}
		~context() {
			EndPaint( wnd, &ps );
		}
	} c( hWnd, *this );
	//
	window2frame const & l = this->get_value( boost::mpl::identity< window2frame >() );
	struct _{
		static bool __( frame::shared_ptr const& f, context const& cntx, bool const expand ) {
			RECT rt;
			int const rw = cntx.rect.right - cntx.rect.left;
			int const rh = cntx.rect.bottom - cntx.rect.top;
			frame::frame_coord const& coord = ((expand)?(frame::frame_coord( 0, 1, 0, 1, 1, 1 )):(f->get_coord()));
			rt.left 	= cntx.rect.left + coord.left.get_n() * rw / coord.left.get_d();
			rt.top 		= cntx.rect.top + coord.top.get_n() * rh / coord.top.get_d();
			rt.right	= rt.left + rw / coord.width;
			rt.bottom	= rt.top + rh / coord.height;
			//
			//HBRUSH hbr = CreateSolidBrush( RGB( 0, 255, 0 ) );
			//FrameRect( cntx.dc, &rt, hbr );
			//DeleteObject( hbr );
			FrameRect( cntx.dc, &rt, (HBRUSH)GetStockObject( WHITE_BRUSH ) );
			//
			SelectObject( cntx.dc, cntx.font );
			
			//SelectObject( cntx.dc, (HFONT)GetStockObject( SYSTEM_FIXED_FONT/*OEM_FIXED_FONT*//*ANSI_FIXED_FONT*/ ) );

			SetTextColor( cntx.dc, cntx.font_color );
			SetBkMode( cntx.dc, TRANSPARENT );
			InflateRect( &rt, -1, -1 );
			//
			if ( f == cntx.current ) {
				std::stringstream ss;
				ss << f.get();
				DrawText( cntx.dc, ss.str().c_str(), -1, &rt, DT_LEFT | DT_TOP );
			}
			return true;
		}
	};
	if ( this->expand_mode ) {
		_::__( this->current_frame, c, true );
	} else {
		l.for_each( boost::bind( &_::__, _1, boost::ref( c ), false ) );
	}
}

void window::onclose( HWND ) {
	child->close();
	PostQuitMessage( 0 );
}

void window::onsettingchange( HWND hWnd, UINT uiAction, LPCTSTR lpName ) {
	switch( uiAction ) {
	case SPI_SETWORKAREA:
	case SPI_ICONVERTICALSPACING:
		update_placement();
		if ( !slide_dir ) {
			update_position( hWnd, this->is_visible(), 1.f );
		}
		break;
	}
}

void window::ontimer( HWND hWnd, UINT id ){
	if ( id == slide_timer_id ) {
		DWORD const dt = timeGetTime() - slide_start_time;
		DWORD const total = get_pref().get< unsigned int >( po_ui_timeout );
		bool const in_dir = ( slide_dir > 0 );
		float mult = 1.f;
		if ( dt > total ) {
			this->show( in_dir );
			slide_dir = 0;
			KillTimer( hWnd, id );
		} else {
			mult = (float)dt / (float)total;
		}
		update_position( hWnd, in_dir, mult );
	};
}

void window::oncommand( HWND hWnd, int id, HWND hwndCtl, UINT codeNotify ) {
	switch( id ) {
	case CMDID_SPLIT:
		atom::mount<window2frame>( this, this->current_frame = this->current_frame->split( RECT_WIDTH( this->in_rect ) > RECT_HEIGHT( this->in_rect ) ) );
		break;
	case CMDID_EXPAND:
		this->expand_mode = !this->expand_mode;
		break;
	case CMDID_ROTATE:
		break;
	case CMDID_NEXT:
		this->current_frame = this->current_frame->get_next();
		break;
	case CMDID_PREV:
		this->current_frame = this->current_frame->get_prev();
		break;
	case CMDID_CLOSE:
		break;
	default:
		return;
	}
	this->invalidate();
}

/*
       template<class T>
       T& as() {
           return boost::any_cast<T&>(v);
       }

	          template<class T>
       const T& as() const {
           return boost::any_cast<const T&>(v);
       }

	   */

void
window::update_hotkeys() {
	hotkey_t new_hk;
	std::vector<std::string> strs;
	std::string appear_hk = get_pref().get< std::string >( po_hk_appear );
	boost::split( strs, appear_hk, boost::is_any_of("+") );
	//
	size_t check_cnt = 0;
	for ( size_t i = 0; i < strs.size(); ++i ) {
		std::string const& s = strs[i];
		//
		check_cnt++;
		if ( s == "win" ){
			new_hk.mods |= MOD_WIN;
		} else if ( s == "ctrl" ) {
			new_hk.mods |= MOD_CONTROL;
		} else if ( s == "alt" ) {
			new_hk.mods |= MOD_ALT;
		} else if ( s == "shift" ) {
			new_hk.mods |= MOD_SHIFT;
		} else {
			try {
				std::stringstream ss;
				ss << s;
				ss >> new_hk.vk;
				if ( !new_hk.vk ) {
					std::stringstream ss;
					ss << s;
					ss >> std::hex >> new_hk.vk;
				}
			} catch( std::exception& e ){
				this->get_logger() << "Invalid hotkey format " << e.what()<< std::endl;
				check_cnt--;
			}
		}
	}
	if ( check_cnt == strs.size() ) {
		new_hk.mods |= MOD_NOREPEAT;
		if ( !( new_hk == this->appear_hk )) {
			new_hk.id++;
			if ( RegisterHotKey( this->get_hwnd(), new_hk.id, new_hk.mods, new_hk.vk )) {
				if ( this->appear_hk.id && !UnregisterHotKey( this->get_hwnd(), this->appear_hk.id ) ) {
					this->get_logger() << "Hotkey unregister error" << std::endl;
				}
				this->appear_hk = new_hk; 
			} else {
				this->get_logger() << "Hotkey register error" << std::endl;
			}
		}
	} else {
		this->get_logger() << "Invalid hotkey format " << appear_hk << std::endl;
	}
}
//
void window::update_placement(){
	std::string const alig_str	= get_pref().get< std::string >( po_ui_alignment );
	unsigned int const width	= get_pref().get< unsigned int >( po_ui_width );
	unsigned int const height	= get_pref().get< unsigned int >( po_ui_height );
	bool const clip				= get_pref().get< bool >( po_ui_clip );
	//
	alignment::type align = alignment::client;
	std::vector<std::string> strs;
	boost::split( strs, alig_str, boost::is_any_of("+") );
	//
	size_t check_cnt = 0;
	for ( size_t i = 0; i < strs.size(); ++i ) {
		std::string const& s = strs[i];
		//
		check_cnt++;
		if ( s == "client" ){
			align |= alignment::client;
		} else if ( s == "top" ){
			align |= alignment::top;
		} else if ( s == "bottom" ) {
			align |= alignment::bottom;
		} else if ( s == "vcenter" ) {
			align |= alignment::vcenter;
		} else if ( s == "left" ) {
			align |= alignment::left;
		} else if ( s == "right" ) {
			align |= alignment::right;
		} else if ( s == "hcenter" ) {
			align |= alignment::hcenter;
		} else if ( s == "center" ) {
			align |= alignment::center;
		} else {
			check_cnt--;
		}
	}
	//
	if ( check_cnt == strs.size() ) {
	} else {
		this->get_logger() << "Invalid alignment format " << alig_str << std::endl;
		align = alignment::top | alignment::vcenter;
	}
	//
	RECT rt;
	RECT wnd_rt;
	//
	SetRect( &rt, 0, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ) );
	if ( clip ) {
		SystemParametersInfo( SPI_GETWORKAREA, 0, &rt, 0 );
	}
	//
	alignment::type const h_align = align & alignment::hcenter;
	alignment::type const v_align = align & alignment::vcenter;
	//
	int const rt_w = rt.right - rt.left;
	int const rt_h = rt.bottom - rt.top;
	int const wnd_rt_w = ((h_align)?(rt_w * width / 100):(rt_w));
	int const wnd_rt_h = ((v_align)?(rt_h * height / 100):(rt_h));
	SetRect( &wnd_rt, rt.left, rt.top, rt.left + wnd_rt_w, rt.top + wnd_rt_h );
	//
	int offs_dx = 0;
	int offs_dy = 0;
	if ( h_align == alignment::hcenter ) {
		offs_dx = ( rt_w - wnd_rt_w ) / 2;
	} else if ( h_align == alignment::right ) {
		offs_dx = ( rt_w - wnd_rt_w );
	}
	if ( v_align == alignment::vcenter ) {
		offs_dy = ( rt_h - wnd_rt_h ) / 2;
	} else if ( v_align == alignment::bottom ) {
		offs_dy = ( rt_h - wnd_rt_h );
	}
	OffsetRect( &wnd_rt, offs_dx, offs_dy );
	//
	this->in_rect = wnd_rt;
	//
	int anchor_dx = 0;
	int anchor_dy = 0;
	this->anchor.x = wnd_rt.left;
	this->anchor.y = wnd_rt.top;
	//
	if ( h_align ) {
		if ( ( h_align & alignment::left ) == h_align ) {
			anchor_dx = -wnd_rt_w;
		} else if ( ( h_align & alignment::right ) == h_align ) {
			anchor_dx = wnd_rt_w;
		}
	}
	if ( v_align ) {
		if ( ( v_align & alignment::top ) == v_align ) {
			anchor_dy = -wnd_rt_h;
		} else if ( ( v_align & alignment::bottom ) == v_align ) {
			anchor_dy = wnd_rt_h;
		}
	}
	//
	this->anchor.x += anchor_dx;
	this->anchor.y += anchor_dy;
}

void window::update_position( HWND hWnd, bool dir, float mult ) {
	if ( dir ) {
		mult = 1.f - mult;
	};
	int x = in_rect.left + (int)( (float)( this->anchor.x - in_rect.left ) * mult );
	int y = in_rect.top + (int)( (float)( this->anchor.y - in_rect.top ) * mult );
	MoveWindow( hWnd, x, y, in_rect.right - in_rect.left, in_rect.bottom - in_rect.top, TRUE );
	//
	this->set_alpha( (BYTE)( (float)get_pref().get< unsigned int >( po_ui_alpha ) * ( 1.f - mult ) ) );
}

void window::update_accels() {

	this->accel.add_accel( CMDID_SPLIT,		true, false, false, false, 's' );
	this->accel.add_accel( CMDID_EXPAND,	true, false, false, false, 'w' );
	this->accel.add_accel( CMDID_ROTATE,	true, false, false, false, 'r' );
	this->accel.add_accel( CMDID_NEXT,		false, true, false, true, VK_TAB );
	this->accel.add_accel( CMDID_PREV,		false, true, true, true, VK_TAB );
	this->accel.add_accel( CMDID_CLOSE,		false, true, false, true, VK_F4 );
	this->accel.build();
}

void window::delete_font(){
	if ( this->font != NULL ) {
		DeleteObject( this->font );
	}
	this->font = NULL;
}

void window::update_font() {
	HFONT nfont = CreateFont(
						get_pref().get< unsigned int >( po_ui_font_height ),
						0,
						0,
						0,
						FW_NORMAL,
						FALSE,
						FALSE,
						FALSE,
						DEFAULT_CHARSET,
						OUT_OUTLINE_PRECIS,
						CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY,
						FIXED_PITCH,
						get_pref().get< std::string >( po_ui_font_name ).c_str()
						);
	if ( nfont != NULL ) {
		delete_font();
		this->font = nfont; 
	} else {
		this->get_logger() << "Font creation error: " << get_pref().get< std::string >( po_ui_font_name ) << std::endl;
	}
}
