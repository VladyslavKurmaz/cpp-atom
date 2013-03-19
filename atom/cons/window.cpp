#include "./pch.hpp"
//#include <boost/function.hpp>
//#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./frame.hpp"
#include "./window.hpp"

#define RECT_WIDTH( r ) ( (r).right - (r).left )
#define RECT_HEIGHT( r ) ( (r).bottom - (r).top )

window::window( logger_ptr l, pref_ptr p ) :
wwindow( *this, INITLIST_7( &window::onchar, &window::onhotkey, &window::onpaint, &window::onclose, &window::onsettingchange, &window::ontimer, &window::oncommand ) )
	,	current_frame()
	,	expand_mode( false )
	,	appear_hk()
	,	accel()
	,	anchor()
	,	in_rect()
	,	slide_dir( 0 )
	,	slide_start_time()
	,	slide_timer_id( 1 )
	,	paint_param() {
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
	this->update_placement();
	if ( base_window_t::init( boost::bind( _::__, _1, _2, boost::ref( this->in_rect ), style, ex_style ), true ) ) {
		this->set_styles( WS_OVERLAPPED, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED ).set_alpha( get_pref().get< unsigned int >( po_ui_alpha ) );
		//
		atom::mount<window2frame>( this, this->head_frame = this->current_frame = frame::create( get_slot<window2logger>().item(), get_slot<window2pref>().item(), this->shared_from_this(), frame::frame_coord( 0, 1, 0, 1, 1, 1 ) ) );
		uni_string s(
			"cmd"
			//"d:\\work\\env\\cygwin\\bin\\bash.exe --login -i"
			//"powershell.exe"
			//"C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\msbuild.exe"
			);
		this->current_frame->run( s );
		//
		(*this)
			( po_hk_appear )
			( po_hk_split )
			( po_hk_expand )
			( po_hk_rotate )
			( po_hk_next )
			( po_hk_prev )
			( po_hk_close )
			( po_hk_tty1 )
			( po_hk_tty2 )
			( po_hk_tty3 )
			( po_hk_tty4 )
			( po_hk_tty5 )
			( po_hk_tty6 )
			( po_ui_timeout )
			( po_ui_alignment )
			( po_ui_width )
			( po_ui_height )
			( po_ui_clip )
			( po_ui_alpha )
			( po_ui_bk_color )
			( po_ui_font_text )
			( po_ui_font_sys )
			( po_ui_margin )
			( po_ui_border )
			( po_ui_padding )
			( po_ui_scroll );
		this->update_hotkeys();
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
		static bool __( frame_ptr& f ) {
			f->clear();
			return true;
		}
	};
	l.for_each( boost::bind( &_::__, _1 ) );
}

void window::onchar( HWND hWnd, TCHAR ch, int cRepeat ) {
	this->current_frame->onchar( ch );
	this->invalidate();
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
		paint_param_t&	pp;
		frame_ptr		current;
		bool			focused;

		context( HWND w, window& pw ) : pp( pw.paint_param ), wnd( w ) {
			dc = BeginPaint( wnd, &ps );
			GetClientRect( wnd, &rect );
			current		= pw.current_frame;
			focused		= ( GetFocus() == wnd );
			//
			FillRect( pp.mem_dc, &rect, pp.bk_brush );
		}
		~context() {
			BitBlt( dc,
					0,
					0,
					rect.right,
					rect.bottom,
					pp.mem_dc,
					0,
					0,
					SRCCOPY );
			EndPaint( wnd, &ps );
		}
	} c( hWnd, *this );
	//
	window2frame const & l = this->get_slot< window2frame >();
	struct _{
		static bool __( frame_ptr const& f, context const& cntx, bool const expand ) {
			HDC dc = cntx.pp.mem_dc;
			RECT rt;
			int const rw = cntx.rect.right - cntx.rect.left;
			int const rh = cntx.rect.bottom - cntx.rect.top;
			frame::frame_coord const& coord = ((expand)?(frame::frame_coord( 0, 1, 0, 1, 1, 1 )):(f->get_coord()));
			rt.left 	= cntx.rect.left + coord.left.get_n() * rw / coord.left.get_d();
			rt.top 		= cntx.rect.top + coord.top.get_n() * rh / coord.top.get_d();
			rt.right	= rt.left + rw / coord.width;
			rt.bottom	= rt.top + rh / coord.height;
			//
			InflateRect( &rt, -cntx.pp.margin_size, -cntx.pp.margin_size );
			//
			HBRUSH bb = (( cntx.focused && ( f == cntx.current ) )?(cntx.pp.border_brush):(cntx.pp.border_brush_inactive));
			for( int i = 0; i < cntx.pp.border_size; ++i ) {
				FrameRect( dc, &rt, bb );
				InflateRect( &rt, -1, -1 );
			}
			//
			InflateRect( &rt, -cntx.pp.padding_size, -cntx.pp.padding_size );
			//
			SetBkMode( dc, TRANSPARENT );
			//
			atom::shared_gdiobj<HRGN> rgn = CreateRectRgn( rt.left, rt.top, rt.right, rt.bottom );
			SelectClipRgn( dc, rgn );
			{
				SelectObject( dc, cntx.pp.font_sys );
				SetTextColor( dc, cntx.pp.font_sys_color );
				DrawText( dc, f->get_caption().c_str(), -1, &rt, DT_RIGHT | DT_TOP | DT_SINGLELINE );
				//
				SelectObject( dc, cntx.pp.font_text );
				SetTextColor( dc, cntx.pp.font_text_color );
				f->draw( dc, rt );
			}
			SelectClipRgn( dc, NULL );
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
		this->head_frame->reorder();
		this->current_frame->run( "cmd" );
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
	case CMDID_TTY1:
	case CMDID_TTY2:
	case CMDID_TTY3:
	case CMDID_TTY4:
	case CMDID_TTY5:
	case CMDID_TTY6:
		this->current_frame = this->current_frame->get_by_index( id - CMDID_TTY1 );
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
	//
	HDC dc = GetDC( NULL );
	{
		this->paint_param.mem_dc		= CreateCompatibleDC( dc );
		this->paint_param.mem_bitmap	= CreateCompatibleBitmap( dc, RECT_WIDTH( this->in_rect ), RECT_HEIGHT( this->in_rect ) );
		SelectObject( this->paint_param.mem_dc, this->paint_param.mem_bitmap );
	}
	ReleaseDC( NULL, dc );
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

window& window::operator()( atom::po::id_t const opt ) {
	switch( opt ) {
	//case po_hk_split:
	//	this->accel.add_accel( CMDID_SPLIT,		false, true, false, true, 'S' );
	//	this->accel.build();
	//	break;
	//case po_hk_expand:
	//	this->accel.add_accel( CMDID_EXPAND,	false, true, false, true, 'Q' );
	//	this->accel.build();
	//	break;
	//case po_hk_rotate:
	//	this->accel.add_accel( CMDID_ROTATE,	false, true, false, true, 'A' );
	//	this->accel.build();
	//	break;
	//case po_hk_next:
	//	this->accel.add_accel( CMDID_NEXT,		false, true, false, true, VK_TAB );
	//	this->accel.build();
	//	break;
	//case po_hk_prev:
	//	this->accel.add_accel( CMDID_PREV,		false, true, true, true, VK_TAB );
	//	this->accel.build();
	//	break;
	//case po_hk_close:
	//	this->accel.add_accel( CMDID_CLOSE,		false, true, false, true, VK_F4 );
	//	this->accel.build();
	//	break;
	case po_hk_tty1:
		this->accel.add_accel( CMDID_TTY1,		true, true, false, true, VK_F1 );
		this->accel.build();
		break;
	//case po_hk_tty2:
	//	this->accel.add_accel( CMDID_TTY2,		true, true, false, true, VK_F2 );
	//	this->accel.build();
	//	break;
	//case po_hk_tty3:
	//	this->accel.add_accel( CMDID_TTY3,		true, true, false, true, VK_F3 );
	//	this->accel.build();
	//	break;
	//case po_hk_tty4:
	//	this->accel.add_accel( CMDID_TTY4,		true, true, false, true, VK_F4 );
	//	this->accel.build();
	//	break;
	//case po_hk_tty5:
	//	this->accel.add_accel( CMDID_TTY5,		true, true, false, true, VK_F5 );
	//	this->accel.build();
	//	break;
	//case po_hk_tty6:
	//	this->accel.add_accel( CMDID_TTY6,		true, true, false, true, VK_F6 );
	//	this->accel.build();
	//	break;
	case po_ui_bk_color:
		this->paint_param.bk_brush = CreateSolidBrush( get_pref().get< unsigned int >( opt ) );
		break;
	case po_ui_font_text:
		{
			HFONT tfont = CreateFont(
				16,
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
				"Consolas"
				);
			if ( tfont != NULL ) {
				this->paint_param.font_text = tfont; 
				this->paint_param.font_text_color = 0xFFFFFF;
			} else {
				this->get_logger() << "Text font creation error: " << get_pref().get< std::string >( po_ui_font_text ) << std::endl;
			}
			break;
		}
	case po_ui_font_sys:
		{
			HFONT sfont = CreateFont(
				12,
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
				"Consolas"
				);
			if ( sfont != NULL ) {
				this->paint_param.font_sys = sfont; 
				this->paint_param.font_sys_color = 0x008000;
			} else {
				this->get_logger() << "Sys font creation error: " << get_pref().get< std::string >( po_ui_font_text ) << std::endl;
			}
			break;
		}
	case po_ui_margin:
		this->paint_param.margin_size			= 0;
		break;
	case po_ui_border:
		this->paint_param.border_brush			= CreateSolidBrush( 0xFFFFFF );
		this->paint_param.border_brush_inactive	= CreateSolidBrush( 0x404040 );
		this->paint_param.border_size			= 1;
		break;
	case po_ui_padding:
		this->paint_param.padding_size			= 2;
		break;
	case po_ui_scroll:
		this->paint_param.scroll_brush			= CreateSolidBrush( 0x00FF00 );
		this->paint_param.scroll_size			= 2;
		break;
	}
	return (*this);
}
