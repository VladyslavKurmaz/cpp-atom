#include "./pch.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./frame.hpp"
#include "./window.hpp"

//?????????// add processing for WM_QUERYENDSESSION
#define RECT_WIDTH( r ) ( (r).right - (r).left )
#define RECT_HEIGHT( r ) ( (r).bottom - (r).top )

			//::string_t s(
			//	"cmd.exe"
			//	//"d:\\work\\env\\cygwin\\bin\\bash.exe --login -i"
			//	//"powershell.exe"
			//	//"C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\msbuild.exe"
			//	);

DWORD thread_id = 0;
window::window( logger_ptr l, pref_ptr p ) :
		wwindow( *this, INITLIST_9( &window::onkey, &window::onkey, &window::onchar, &window::onhotkey, &window::onpaint, &window::onclose, &window::onsettingchange, &window::ontimer, &window::oncommand ) )
	,	head_area()
	,	current_frame()
	,	expand_mode( false )
	,	appear_hk()
	,	accel()
	,	rect_active()
	,	rect_inactive()
	,	sliding( 0 )
	,	slide_start_time()
	,	slide_last_time()
	,	slide_timer_id( 1 )
	,	paint_param() {
	//
	atom::mount<window2logger>( this, l );
	atom::mount<window2pref>( this, p );
	//???????? workaround
	thread_id = GetCurrentThreadId();
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
	this->calculate_docks();
	if ( base_window_t::init( boost::bind( _::__, _1, _2, boost::ref( this->rect_inactive ), style, ex_style ), true ) ) {
		this->set_styles( WS_OVERLAPPED, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED ).set_alpha( get_pref()->get< unsigned int >( po_ui_alpha ) );
		//
		get_pref()->register_process_callback( pref::pgEmpty, boost::bind( &window::process_empty, this->shared_from_this() ) );
		//
		get_pref()->register_process_callback( pref::pgAutostart, boost::bind( &window::process_autostart, this->shared_from_this() ) );
		this->process_autostart();
		get_pref()->register_process_callback( pref::pgHotkeys, boost::bind( &window::process_hotkeys, this->shared_from_this() ) );
		this->process_hotkeys();
		get_pref()->register_process_callback( pref::pgFonts, boost::bind( &window::process_fonts, this->shared_from_this() ) );
		this->process_fonts();
		get_pref()->register_process_callback( pref::pgWindow, boost::bind( &window::process_window, this->shared_from_this() ) );
		this->process_window();
		get_pref()->register_process_callback( pref::pgUI, boost::bind( &window::process_ui, this->shared_from_this() ) );
		this->process_ui();
		//
		this->current_frame = frame::create( this->get_logger(), this->get_pref(), this->shared_from_this() );
		this->head_area = area::create( area_ptr(), this->current_frame, true );
		//

		return true;
	}
	return false;
}
  
void window::run() {
	struct _ {
		static bool __( HWND hWnd, MSG* msg, atom::accel& accel, window& w ) {
			if ( msg->message == WM_FRAMEEXIT ) {
				w.close_frame( (frame_id_t)msg->wParam );
				return true;
			}
			return accel.translate( hWnd, msg );
		}
	};
	base_window_t::run( boost::bind( _::__, _1, _2, boost::ref( this->accel ), boost::ref( *this ) ) );
}

void window::clear() {
	this->head_area = area_ptr();
	this->current_frame = frame_ptr();
	base_node_t::clear();
}

void window::onkey( HWND hWnd, UINT vk, BOOL down, int repeat, UINT flags ){
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

	//this->get_logger() << vk << ((down)?(" down"):(" up")) << std::endl;
	if ( this->current_frame ) {
		this->current_frame->process( bridge_msg::bmKbrd, &key );
	}
	this->invalidate();

}

void window::onchar( HWND hWnd, TCHAR ch, int cRepeat ) {
}

void window::onhotkey( HWND hWnd, int idHotKey, UINT fuModifiers, UINT vk ) {
	if ( idHotKey == this->appear_hk.id ) {
		slide_begin( true );
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
	struct _{
		static void __( frame_ptr const& f, frame_coord const& coord, context const& cntx ) {
			HDC dc = cntx.pp.mem_dc;
			RECT rt;
			int const rw = cntx.rect.right - cntx.rect.left;
			int const rh = cntx.rect.bottom - cntx.rect.top;
			rt.left 	= cntx.rect.left + coord.left.get_n() * rw / coord.left.get_d();
			rt.top 		= cntx.rect.top + coord.top.get_n() * rh / coord.top.get_d();
			rt.right	= rt.left + rw / coord.width;
			rt.bottom	= rt.top + rh / coord.height;
			//
			InflateRect( &rt, -(int)cntx.pp.margin_size, -(int)cntx.pp.margin_size );
			//
			HBRUSH bb = (( cntx.focused && ( f == cntx.current ) )?(cntx.pp.border_brush):(cntx.pp.border_brush_inactive));
			for( unsigned int i = 0; i < cntx.pp.border_size; ++i ) {
				FrameRect( dc, &rt, bb );
				InflateRect( &rt, -1, -1 );
			}
			//
			InflateRect( &rt, -(int)cntx.pp.padding_size, -(int)cntx.pp.padding_size );
			//
			SetBkMode( dc, TRANSPARENT );
			//
			atom::shared_gdiobj<HRGN> rgn = CreateRectRgn( rt.left, rt.top, rt.right, rt.bottom );
			SelectClipRgn( dc, rgn );
			{
				SelectObject( dc, cntx.pp.font_sys );
				SetTextColor( dc, cntx.pp.font_sys_color );
				std::stringstream ss;
				ss << " #" << f->get_index();
				DrawText( dc, ss.str().c_str(), -1, &rt, DT_RIGHT | DT_TOP | DT_SINGLELINE );
				//
				SelectObject( dc, cntx.pp.font_text );
				SetTextColor( dc, cntx.pp.font_text_color );
				f->draw( dc, rt );
			}
			SelectClipRgn( dc, NULL );
		}
	};
	frame_coord coord( 0, 1, 0, 1, 1, 1 );
	if ( this->expand_mode ) {
		_::__( this->current_frame, coord, c );
	} else {
		this->head_area->draw( boost::bind( &_::__, _1, _2, boost::ref( c ) ), coord );
	}
}

void window::onclose( HWND ) {
	assert( false );
	//frames_t fs;
	//this->head_area->collect( fs );
	//BOOST_FOREACH( frame_ptr f, fs ) { f->close(); }
}

void window::onsettingchange( HWND hWnd, UINT uiAction, LPCTSTR lpName ) {
	switch( uiAction ) {
	case SPI_SETWORKAREA:
	case SPI_ICONVERTICALSPACING:
		this->calculate_docks();
		this->slide_begin( false );
		break;
	}
}

void window::ontimer( HWND hWnd, UINT id ){
	if ( id == slide_timer_id ) {
		slide_update();
	};
}

void window::oncommand( HWND hWnd, int id, HWND hwndCtl, UINT codeNotify ) {
	switch( id ) {
	case CMDID_SPLIT:
		{
#ifndef STANDALONE
			area_ptr a = this->head_area->find( this->current_frame );
			a->split( this->current_frame = this->current_frame->split() );
#endif
			break;
		}
	case CMDID_EXPAND:
		this->expand_mode = !this->expand_mode;
		break;
	case CMDID_ROTATE:
		if ( !this->expand_mode ) {
			this->head_area->find( this->current_frame )->rotate();
		}
		break;
	case CMDID_NEXT:
		this->current_frame = this->head_area->find( this->current_frame )->next();
		break;
	case CMDID_PREV:
		this->current_frame = this->head_area->find( this->current_frame )->prev();
		break;
	case CMDID_CTRL_BREAK:
		this->current_frame->process( bridge_msg::bmCtrlBreak, NULL );
		break;
	case CMDID_CTRL_C:
		this->current_frame->process( bridge_msg::bmCtrlC, NULL );
		break;
	case CMDID_CLOSE:
		this->current_frame->process( bridge_msg::bmExit, NULL );
		break;
	case CMDID_TTY1:
	case CMDID_TTY2:
	case CMDID_TTY3:
	case CMDID_TTY4:
	case CMDID_TTY5:
	case CMDID_TTY6:
		{
			frame_ptr f = this->head_area->get_by_index( id - CMDID_TTY1 + 1 );
			if ( f ) {
				this->current_frame = f;
			}
			break;
		}
	default:
		return;
	}
	this->invalidate();
}

void
window::parse_conf( TCHAR const* conf ) {
	::string_t s( conf );
	//
	for( size_t i = 0; i < conf_cmds_cnt; ++i ) {
		if ( ( s == conf_cmds[i].cmd ) && ( conf_cmds[i].id ) ) {
			SendMessage( this->get_hwnd(), WM_COMMAND, MAKEWPARAM( conf_cmds[i].id, 0 ), 0 );
			return;
		}
	}
	// check command line options
	try {
		this->get_pref()->parse( ::string_t( conf ) );
	}
	catch ( std::exception& /*e*/ ) {
		//send exception description back to the console process
	}
}

void
window::close_frame( frame_id_t const id ) {
	frame_ptr f = this->head_area->get_by_id( id );
	if ( f ) {
		//
		if ( this->current_frame == f ) {
			this->expand_mode = false;
			this->current_frame = this->head_area->find( f )->next();
			if ( this->current_frame == f ) {
				this->current_frame = frame_ptr();
			}
		}
		//
		this->head_area->find( f )->close();
		f->clear();
		//
		// exit application
		if ( !this->current_frame ) {
			PostThreadMessage( thread_id, WM_QUIT, 0, 0 );
		}
	}
	this->invalidate();
}

atom::parse_tag< TCHAR, UINT > hotkey_tags[] = {
	{ "win",	MOD_WIN },
	{ "ctrl",	MOD_CONTROL },
	{ "alt",	MOD_ALT },
	{ "shift",	MOD_SHIFT }
};
size_t hotkey_tags_count = sizeof( hotkey_tags ) / sizeof( hotkey_tags[0] );

void
window::update_hotkeys() {
	atom::parse_result< TCHAR, UINT > result = atom::parse_tags( get_pref()->get< ::string_t >( po_hk_appear ), hotkey_tags, hotkey_tags_count, ::string_t( "+" ) );
	if ( ( result.total_found > 1 ) && ( result.unparsed.size() == 1 ) ) {
		hotkey_t new_hk;
		new_hk.mods = MOD_NOREPEAT | result.result;
		try {
			std::stringstream ss;
			ss << result.unparsed[0];
			ss >> new_hk.vk;
			if ( !new_hk.vk ) {
				std::stringstream ss;
				ss << result.unparsed[0];
				ss >> std::hex >> new_hk.vk;
			}
			//
			if ( !( new_hk == this->appear_hk )) {
				new_hk.id++;
				if ( RegisterHotKey( this->get_hwnd(), new_hk.id, new_hk.mods, new_hk.vk )) {
					if ( this->appear_hk.id && !UnregisterHotKey( this->get_hwnd(), this->appear_hk.id ) ) {
						*(this->get_logger()) << "Hotkey unregister error" << std::endl;
					}
					this->appear_hk = new_hk; 
				} else {
					*(this->get_logger()) << "Hotkey register error" << std::endl;
				}
			}
		}
		catch( std::exception& e ){
			*(this->get_logger()) << "Invalid hotkey format " << e.what()<< std::endl;
		}
	}
}
//
atom::parse_tag< TCHAR, alignment::type > alignment_tags[] = {
	{ "hcenter",	alignment::hcenter },
	{ "left",		alignment::left },
	{ "right",		alignment::right },
	{ "hclient",	alignment::hclient },
	{ "hmask",		alignment::hmask },
	{ "vcenter",	alignment::vcenter },
	{ "top",		alignment::top },
	{ "bottom",		alignment::bottom },
	{ "vclient",	alignment::vclient },
	{ "vmask",		alignment::vmask },
	{ "client",		alignment::client },
	{ "center",		alignment::center }
};
size_t alignment_tags_count = sizeof( alignment_tags ) / sizeof( alignment_tags[0] );

void window::calculate_docks(){
	::string_t const alig_str	= get_pref()->get< ::string_t >( po_ui_alignment );
	unsigned int const width	= get_pref()->get< unsigned int >( po_ui_width );
	unsigned int const height	= get_pref()->get< unsigned int >( po_ui_height );
	bool const clip				= get_pref()->get< bool >( po_ui_clip );
	//
	alignment::type align = alignment::client;
	atom::parse_result< TCHAR, alignment::type > result = atom::parse_tags( alig_str, alignment_tags, alignment_tags_count, ::string_t( "+" ) );
	//
	if ( ( result.total_found > 0 ) && ( result.unparsed.size() == 0 ) ) {
		align = result.result;
	} else {
		*(this->get_logger()) << "Invalid alignment format " << alig_str << std::endl;
		align = alignment::hcenter | alignment::top;
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
	alignment::type const h_align = align & alignment::hmask;
	alignment::type const v_align = align & alignment::vmask;
	//
	int const rt_w = rt.right - rt.left;
	int const rt_h = rt.bottom - rt.top;
	int const wnd_rt_w = ((h_align == alignment::hclient )?(rt_w):(rt_w * width / 100));
	int const wnd_rt_h = ((v_align == alignment::vclient )?(rt_h):(rt_h * height / 100));
	//
	SetRect( &rect_active, 0, 0, wnd_rt_w, wnd_rt_h );
	rect_inactive = rect_active;
	OffsetRect( &rect_inactive, 0, -wnd_rt_h );
	//
	HDC dc = GetDC( NULL );
	{
		this->paint_param.mem_dc		= CreateCompatibleDC( dc );
		this->paint_param.mem_bitmap	= CreateCompatibleBitmap( dc, RECT_WIDTH( this->rect_active ), RECT_HEIGHT( this->rect_active ) );
		SelectObject( this->paint_param.mem_dc, this->paint_param.mem_bitmap );
	}
	ReleaseDC( NULL, dc );
}

void window::slide_begin( bool const update ) {
	if ( sliding ) {
		slide_start_time += get_pref()->get< unsigned int >( po_ui_timeout ) - ( timeGetTime() - slide_start_time );
		slide_last_time = slide_start_time;
		sliding *= -1;
	} else {
		sliding = ((this->is_visible())?(-1):(1));
		if ( update ) {
			sliding *= -1;
		}
		//
		slide_start_time = slide_start_time = timeGetTime();
		SetTimer( this->get_hwnd(), slide_timer_id, USER_TIMER_MINIMUM, NULL );
		this->show( true ).activate();
	}
}

bool operator==( RECT const& l, RECT const& r ) {
	return ( ( l.left == r.left ) && ( l.top == r.top ) &&  ( l.right == r.right ) &&  ( l.bottom == r.bottom ) );
}

bool operator!=( RECT const& l, RECT const& r ) {
	return ( !( l == r ) );
}

void window::slide_update() {
#if 0
	if ( this->sliding ) {
		DWORD const t = timeGetTime();
		DWORD const total = get_pref()->get< unsigned int >( po_ui_timeout );
		RECT srect;
		RECT drect;
		//
		GetWindowRect( this->get_hwnd(), &srect );
		drect = ( this->sliding > 0 )?( this->rect_active ):( this->rect_inactive );
		//
		if ( ( t - slide_start_time ) < total ) {
			float const mult = ( (float)dt / (float)total );
			//
			slide_start_time = t;


		} else
		{
			// stop
			bool const s = ( rect_inactive != slide_rect_dest );
			this->show( s );
			if ( s ) {
				this->activate();
			}
			KillTimer( this->get_hwnd(), slide_timer_id );
			sliding = false;
		}
	}



 - 
slide_start_time


		mult = 1.f - mult;
	//
	RECT rt;
	struct val {
		val const& operator()( LONG &r, int const v1, LONG const v2, float const f ) const {
			r = v1 - (LONG)( (float)( v1 - v2 ) * f );
			return (*this);
		}
	} v;
	v	( rt.left, slide_rect_dest.left, slide_rect_src.left, mult )
		( rt.top, slide_rect_dest.top, slide_rect_src.top, mult )
		( rt.right, slide_rect_dest.right, slide_rect_src.right, mult )
		( rt.bottom, slide_rect_dest.bottom, slide_rect_src.bottom, mult );
	MoveWindow( this->get_hwnd(), rt.left, rt.top, RECT_WIDTH( rt ), RECT_HEIGHT( rt ), TRUE );
	//
	// this->set_alpha( (BYTE)( (float)get_pref()->get< unsigned int >( po_ui_alpha ) * ( 1.f - mult ) ) );
	//
	if ( mult == 1.f ) {
	}
#endif
}




atom::parse_tag< TCHAR, BYTE > accel_tags[] = {
	{ "alt",		FALT },
	{ "ctrl",		FCONTROL },
	{ "shift",		FSHIFT }
};
size_t accel_tags_count = sizeof( accel_tags ) / sizeof( accel_tags[0] );

atom::parse_tag< TCHAR, WORD > vk_tags[] = {
	{ "tab",		VK_TAB },
	{ "break",		VK_CANCEL },
	{ "f1",			VK_F1 },
	{ "f2",			VK_F2 },
	{ "f3",			VK_F3 },
	{ "f4",			VK_F4 },
	{ "f5",			VK_F5 },
	{ "f6",			VK_F6 },
	{ "f7",			VK_F7 },
	{ "f8",			VK_F8 },
	{ "f9",			VK_F9 },
	{ "f10",		VK_F10 }
};
size_t vk_tags_count = sizeof( vk_tags ) / sizeof( vk_tags[0] );

void window::process_autostart() {
	//Registry.SetValue(@"HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run",
	//          "MyStartUp",
	//          @"C:\StartUpApp.exe");
}

void window::process_hotkeys() {
	static const struct hk_tags_t {
		atom::po::id_t	opt;
		WORD			cmd;
	} hk_tags[] = {
		{ po_hk_split, CMDID_SPLIT },
		{ po_hk_expand, CMDID_EXPAND },
		{ po_hk_rotate, CMDID_ROTATE },
		{ po_hk_next, CMDID_NEXT },
		{ po_hk_prev, CMDID_PREV },
		{ po_hk_ctrl_break, CMDID_CTRL_BREAK },
		{ po_hk_ctrl_c, CMDID_CTRL_C },
		{ po_hk_close, CMDID_CLOSE },
		{ po_hk_tty1, CMDID_TTY1 },
		{ po_hk_tty2, CMDID_TTY2 },
		{ po_hk_tty3, CMDID_TTY3 },
		{ po_hk_tty4, CMDID_TTY4 },
		{ po_hk_tty5, CMDID_TTY5 },
		{ po_hk_tty6, CMDID_TTY6 }
	};
	//
	BOOST_FOREACH( hk_tags_t const& tag, hk_tags )
	{
		::string_t s = get_pref()->get< ::string_t >( tag.opt );
		atom::parse_result< TCHAR, BYTE > mods = atom::parse_tags( s, accel_tags, accel_tags_count, ::string_t( "+" ) );
		//
		if ( ( mods.total_found > 1 ) && ( mods.unparsed.size() == 1 ) ) {
			atom::parse_result< TCHAR, WORD > vk = atom::parse_tags( s, vk_tags, vk_tags_count, mods.unparsed );
			if ( ( vk.total_found = 1 ) && ( vk.unparsed.size() == 0 ) ) {
			} else {
				vk.result = (WORD)mods.unparsed[0].c_str()[0];
			}
			this->accel.add_accel( tag.cmd, mods.result | FVIRTKEY, vk.result );

		} else {
			// throw exception
			*(this->get_logger()) << "Invalid accelerator format format " << s << std::endl;
		}
	}
	//
	this->accel.build();
	this->update_hotkeys();
}

void window::process_fonts() {
	static const struct fonts_t {
		atom::po::id_t	opt;
	} fonts[] = {
		po_ui_font_text,
		po_ui_font_sys
	};
	//
	::string_t const d1( DELIM1 );
	::string_t const d2( DELIM2 );
	//
	BOOST_FOREACH( fonts_t const& font, fonts )
	{
		::string_t s = get_pref()->get< ::string_t >( font.opt );
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
			a.as<::string_t>(_T("name") ).c_str()
			);
		unsigned int color = a.as_color( _T("color") );
		//
		if ( f != NULL ) {
			switch( font.opt ) {
			case po_ui_font_text:
				this->paint_param.font_text = f;
				this->paint_param.font_text_color = color;
				break;
			case po_ui_font_sys:
				this->paint_param.font_sys = f; 
				this->paint_param.font_sys_color = color;
				break;
			};
		} else {
			*(this->get_logger()) << "Text font creation error: " << s << std::endl;
		}
	}
}

void window::process_window(){
	this->calculate_docks();
	this->slide_begin( false );
}

void window::process_ui() {
	::string_t const d1( DELIM1 );
	::string_t const d2( DELIM2 );
	//
	this->paint_param.bk_brush = CreateSolidBrush( get_pref()->get< unsigned int >( po_ui_bk_color ) );
	// po_ui_margin
	this->paint_param.margin_size			= 0;
	//
	atom::attributes< TCHAR > border( get_pref()->get< ::string_t >( po_ui_border ), d1, d2 );
	this->paint_param.border_brush			= CreateSolidBrush( border.as_color( _T("color") ) );
	this->paint_param.border_brush_inactive	= CreateSolidBrush( border.as_color( _T("inactive") ) );
	this->paint_param.border_size			= border.as<unsigned int>(_T("size") );
	//
	atom::attributes< TCHAR > padding( get_pref()->get< ::string_t >( po_ui_padding ), d1, d2 );
	this->paint_param.padding_size			= padding.as<unsigned int>(_T("size") );
	//
	atom::attributes< TCHAR > scroll( get_pref()->get< ::string_t >( po_ui_scroll ), d1, d2 );
	this->paint_param.scroll_brush			= CreateSolidBrush( scroll.as_color( _T("color") ) );
	this->paint_param.scroll_size			= scroll.as<unsigned int>(_T("size") );
	//
	this->invalidate();
}
