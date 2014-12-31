#include "./pch.hpp"

#pragma comment(lib, "winmm.lib")

// https://connect.microsoft.com/PowerShell/feedback/details/572313/powershell-exe-can-hang-if-stdin-is-redirected
// http://en.wikipedia.org/wiki/ANSI_escape_code
// http://support.microsoft.com/default.aspx?scid=kb;EN-US;q190351&wa=wsignin1.0
//SetWinEventHook.
// http://stackoverflow.com/questions/2175665/createprocess-process-do-not-terminate-when-redirecting-std-out-in-err

const TCHAR CMDID_SPLIT_NAME[]		= _T( "split" );
const TCHAR CMDID_EXPAND_NAME[]		= _T( "expand" );
const TCHAR CMDID_ROTATE_NAME[]		= _T( "rotate" );
const TCHAR CMDID_NEXT_NAME[]		= _T( "next" );
const TCHAR CMDID_PREV_NAME[]		= _T( "prev" );

const conf_cmd_t conf_cmds[] = {
	{ CMDID_SPLIT_NAME,		CMDID_SPLIT,	_T("split current frame") },
	{ CMDID_EXPAND_NAME,	CMDID_EXPAND,	_T("maximize/restore frame") },
	{ CMDID_ROTATE_NAME,	CMDID_ROTATE,	_T("rotate frames") },
	{ CMDID_NEXT_NAME,		CMDID_NEXT,		_T("focus next frame") },
	{ CMDID_PREV_NAME,		CMDID_PREV,		_T("focus prev frame") },
};
const size_t conf_cmds_cnt = sizeof( conf_cmds ) / sizeof( conf_cmds[0] );

const TCHAR DELIM1[] = _T( ";" );
const TCHAR DELIM2[] = _T( ":" );

int atom::node_cnt = 0;


/*
class area : public boost::enable_shared_from_this< area > {
	public:
		static area_ptr create( area_ptr p, frame_ptr f, bool const r ) {
			area_ptr a = area_ptr( new area( p, f ) );
			if ( a ) {
				a->reorder();
			} 
			return ( a );
		};
		///
		~area() {
		}
		//
		void close() {
			if ( this->parent ){
				area_ptr a = area_ptr(); 
				if ( this->parent->first != this->shared_from_this() ) { a = this->parent->first; }
				if ( this->parent->second != this->shared_from_this() ) { a = this->parent->second; }
				//
				this->parent->vert = a->vert;
				this->parent->first = a->first; if ( this->parent->first ) { this->parent->first->parent = this->parent; }
				this->parent->second = a->second; if ( this->parent->second ) { this->parent->second->parent = this->parent; }
				this->parent->frame = a->frame;
				//
				a->clear();
				this->parent->reorder();
			}
			this->clear();
		}
		//
		area_ptr find( frame_ptr f ) {
			area_ptr result = area_ptr();
			if ( this->frame == f ) { result = this->shared_from_this(); }
			if ( !result && this->first ) { result = this->first->find( f ); }
			if ( !result && this->second ) { result = this->second->find( f ); }
			return ( result );
		}
		//
		frame_ptr get_by_index( unsigned int const i ) {
			frame_ptr result = frame_ptr();
			if ( this->frame && ( this->frame->get_index() == i ) ) { result = this->frame; }
			if ( !result && this->first ) { result = this->first->get_by_index( i ); }
			if ( !result && this->second ) { result = this->second->get_by_index( i ); }
			return ( result );
		}
		//
		frame_ptr get_by_id( frame_id_t const id ) {
			frame_ptr result = frame_ptr();
			if ( this->frame && ( this->frame->get_id() == id ) ) { result = this->frame; }
			if ( !result && this->first ) { result = this->first->get_by_id( id ); }
			if ( !result && this->second ) { result = this->second->get_by_id( id ); }
			return ( result );
		}
		//
		void collect( frames_t& fs ) const {
			if ( this->frame ) {
				fs.push_back( this->frame );
			} else {
				this->first->collect( fs );
				this->second->collect( fs );
			}
		}
		void split( frame_ptr f ) {
			this->vert = true;
			this->first = create( this->shared_from_this(), this->frame, false );
			this->second = create( this->shared_from_this(), f, false );
			this->frame = frame_ptr();
			this->reorder();
		}
		void rotate() {
			if ( this->parent ) {
				this->parent->vert = !this->parent->vert;  
				if ( this->parent->vert ) {
					std::swap( this->parent->first, this->parent->second );
				}
			}
			this->reorder();
		}
		frame_ptr next() {
			if ( this->parent ) {
				return ( this->parent->search( this->shared_from_this(), true, true ) );
			}
			return ( this->frame );
		}
		frame_ptr prev() {
			if ( this->parent ) {
				return ( this->parent->search( this->shared_from_this(), true, false) );
			}
			return ( this->frame );
		}
		void draw( boost::function< void( frame_ptr const&, frame_coord const& ) > func, frame_coord const& c ) const {
			if ( this->frame ) {
				func( this->frame, c );
			} else {
				fraction const l = ((this->vert)?( c.left + fraction( 1, c.width * 2 ) ):( c.left ) );
				fraction const t = ((this->vert)?( c.top ):( c.top + fraction( 1, c.height * 2 ) ) );
				unsigned int const w = ((this->vert)?(c.width * 2):(c.width));
				unsigned int const h = ((this->vert)?(c.height):(c.height * 2));
				//
				if ( this->first ) {
					this->first->draw( func, frame_coord( c.left.get_n(), c.left.get_d(), c.top.get_n(), c.top.get_d(), w, h ) );
				}
				if ( this->second ) {
					this->second->draw( func, frame_coord( l.get_n(), l.get_d(), t.get_n(), t.get_d(), w, h ) );
				}
			}
		}

	protected:
		///
		area_ptr get( bool const f ) const {
			if ( f ) {
				return ( this->first );
			}
			return ( this->second );
		}
		///
		frame_ptr search( area_ptr a, bool const up, bool const n ) {
			if ( this->frame ) {
				return ( this->frame );
			} else {
				if ( up ) {
					// moving up
					if ( ( this->parent ) && ( this->get( !n ) == a ) ) {
						return ( this->parent->search( this->shared_from_this(), up, n ) );
					}
					return ( this->get( !( this->get( true ) == a ) )->search( area_ptr(), false, n ) );
				} else {
					// moving down
					return ( this->get( n )->search( area_ptr(), up, n ) );
				}
			}
		}
		///
		void reorder() {
			unsigned int i = 0;
			area_ptr a = this->shared_from_this();
			while( a->parent ) {
				a = a->parent;
			}
			a->reorder( i );
		}
		///
		void reorder( unsigned int& i ) const {
			if ( this->frame ) {
				this->frame->set_index( ++i );
			} else {
				this->first->reorder( i );
				this->second->reorder( i );
			}
		}
	private:
		area_ptr
			parent;
		bool
			vert;
		area_ptr
			first;
		area_ptr
			second;
		frame_ptr
			frame;
		///
		area( area_ptr p, frame_ptr f ) :
				parent( p )
			,	vert( false )
			,	first()
			,	second()
			,	frame( f ) {
		}
		///
		void clear() {
			this->parent = this->first = this->second = area_ptr();
			this->frame = frame_ptr();
		}
	};




	void window::onKey( HWND hWnd, UINT vk, BOOL down, int repeat, UINT flags ){
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
	if ( this->current_frame ) {
		this->current_frame->process( bridge_msg::bmKbrd, &key );
	}
	this->invalidate();

}



void window::onPaint( HWND hWnd ){
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

HDC dc = GetDC( NULL );
	{
		this->paint_param.mem_dc		= CreateCompatibleDC( dc );
		this->paint_param.mem_bitmap	= CreateCompatibleBitmap( dc, RECT_WIDTH( this->windowPlacement.active ), RECT_HEIGHT( this->windowPlacement.active ) );
		SelectObject( this->paint_param.mem_dc, this->paint_param.mem_bitmap );
	}
	ReleaseDC( NULL, dc );




	void window::processFonts() {
	static const struct fonts_t {
		atom::po::id_t	opt;
	} fonts[] = {
		po_ui_font_text,
		po_ui_font_sys
	};
	//
	std::string const d1( DELIM1 );
	std::string const d2( DELIM2 );
	//
	BOOST_FOREACH( fonts_t const& font, fonts )
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
				this->paint_param.font_text = f;
				this->paint_param.font_text_color = color;
				break;
			case po_ui_font_sys:
				this->paint_param.font_sys = f; 
				this->paint_param.font_sys_color = color;
				break;
			};
		} else {
			this->getLogger() << "Text font creation error: " << s << std::endl;
		}
	}
}



	struct paint_param_t {
		atom::shared_dc
			mem_dc;
		atom::shared_gdiobj< HBITMAP >
			mem_bitmap;
		atom::shared_gdiobj< HBRUSH >
			bk_brush;
		unsigned int
			margin_size;
		atom::shared_gdiobj< HBRUSH >
			border_brush;
		atom::shared_gdiobj< HBRUSH >
			border_brush_inactive;
		unsigned int
			border_size;
		unsigned int
			padding_size;
		atom::shared_gdiobj< HBRUSH >
			scroll_brush;
		unsigned int
			scroll_size;
		atom::shared_gdiobj< HFONT >
			font_text;
		COLORREF
			font_text_color;
		atom::shared_gdiobj< HFONT >
			font_sys;
		COLORREF
			font_sys_color;
	} paint_param;


	void window::processUI() {
	std::string const d1( DELIM1 );
	std::string const d2( DELIM2 );
	//
	this->paint_param.bk_brush = CreateSolidBrush( this->getPref().get< unsigned int >( po_ui_bk_color ) );
	// po_ui_margin
	this->paint_param.margin_size			= 0;
	//
	atom::attributes< TCHAR > border( this->getPref().get< std::string >( po_ui_border ), d1, d2 );
	this->paint_param.border_brush			= CreateSolidBrush( border.as_color( _T("color") ) );
	this->paint_param.border_brush_inactive	= CreateSolidBrush( border.as_color( _T("inactive") ) );
	this->paint_param.border_size			= border.as<unsigned int>(_T("size") );
	//
	atom::attributes< TCHAR > padding( this->getPref().get< std::string >( po_ui_padding ), d1, d2 );
	this->paint_param.padding_size			= padding.as<unsigned int>(_T("size") );
	//
	atom::attributes< TCHAR > scroll( this->getPref().get< std::string >( po_ui_scroll ), d1, d2 );
	this->paint_param.scroll_brush			= CreateSolidBrush( scroll.as_color( _T("color") ) );
	this->paint_param.scroll_size			= scroll.as<unsigned int>(_T("size") );
	//
	this->invalidate();
}

void window::processAutostart() {
	//Registry.SetValue(@"HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run",
	//          "MyStartUp",
	//          @"C:\StartUpApp.exe");
}

void
window::parseConf( TCHAR const* conf ) {
	std::string s( conf );
	//
	for( size_t i = 0; i < conf_cmds_cnt; ++i ) {
		if ( ( s == conf_cmds[i].cmd ) && ( conf_cmds[i].id ) ) {
			SendMessage( this->get_hwnd(), WM_COMMAND, MAKEWPARAM( conf_cmds[i].id, 0 ), 0 );
			return;
		}
	}
	// check command line options
	try {
		this->getPref().parse( std::string( conf ) );
	}
	catch ( std::exception& e ) {
		//send exception description back to the console process
	}
}


void
window::closeFrame( frame_id_t const id ) {
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
		}
	}
	this->invalidate();
}

*/