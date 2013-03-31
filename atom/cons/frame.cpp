#include "./pch.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./window.hpp"
#include "./frame.hpp"

frame::frame( logger_ptr l, pref_ptr p, window_ptr w, frame_coord const & fc ) :
		index( 0 )
	,	buffer()
	,	coord( fc )
	,	next()
	,	prev()
	,	pi()
	,	si()
	,	pipe()
	,	process_caption() {
	atom::mount<frame2logger>( this, l );
	atom::mount<frame2pref>( this, p );
	atom::mount<frame2window>( this, w );
	//
	GUID guid;
	CoCreateGuid( &guid );
	std::stringstream gss;
	gss << guid;
	this->pipe.create( gss.str() );
	//
	// start child console process
	this->si.cb				= sizeof( this->si );
	this->si.dwFlags		= STARTF_USESHOWWINDOW;
	this->si.wShowWindow	= SW_SHOW;
	//
	std::stringstream ss;
	ss << _T( "D:\\work\\env\\cpp-atom\\tmp\\msvc10_x86_Debug\\atom\\cons\\Debug\\consd.exe " ) << "--pipe=" << this->pipe.get_name();
	this->get_logger() << pipe.get_name();
	TCHAR cmd_line[MAX_PATH] = { 0 };
	strcpy_s( cmd_line, ss.str().c_str() );
	if ( CreateProcess( NULL, cmd_line, NULL, NULL, TRUE, CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi ) ) {
		// ????? check if process finished immediately
		this->pipe.connect();
		//
		struct ep_t {
			HWND	cons_wnd;
			DWORD	pid;
		} ep = { NULL, this->pi.dwProcessId };
		struct _ {
			static BOOL CALLBACK __( HWND hwnd, LPARAM lParam ) {
				ep_t& p = *(reinterpret_cast<ep_t*>( lParam ));
				DWORD pid = 0;
				GetWindowThreadProcessId( hwnd, &pid ); 
				if ( pid == p.pid ) {
					p.cons_wnd = hwnd;
					return FALSE;
				}
				return TRUE;
			}
		};
		EnumWindows( _::__, reinterpret_cast<LPARAM>( &ep ) );
		TCHAR caption[ MAX_PATH ] = { 0 };
		GetWindowText( ep.cons_wnd, caption, MAX_PATH );
		this->process_caption = uni_string( caption );
	}
}

frame::~frame() {
}

frame_ptr frame::split( bool const pref_h ){
	frame_coord fc = this->coord;
	if ( ( this->coord.width < this->coord.height) || ( ( this->coord.width == this->coord.height ) && pref_h ) ) {
		// split vertical
		this->coord.width *= 2;
		fc.width *= 2;
		fc.left = fc.left + fraction( 1, fc.width );
	} else if ( ( this->coord.width > this->coord.height) || ( ( this->coord.width == this->coord.height ) && !pref_h ) ) {
		// split horizontal
		this->coord.height *= 2;
		fc.height *= 2;
		fc.top = fc.top + fraction( 1, fc.height );
	}
	//
	frame_ptr f = frame_ptr( new frame( 
										get_value( boost::mpl::identity< frame2logger >() ).item(),
										get_value( boost::mpl::identity< frame2pref >() ).item(),
										get_slot<frame2window>().item(),
										fc ) );
	f->next = f->prev = f;
	//
	frame_ptr l = this->shared_from_this();
	frame_ptr r = l->get_next();
	//
	std::swap( l->next, f->next );
	std::swap( r->prev, f->prev );
	//
	return f;
}

frame_ptr frame::get_by_index( unsigned int const i ) {
	frame_ptr f = shared_from_this();
	do {
		if ( f->index == i ) {
			return ( f );
		}
		f = f->get_next();
	} while( f.get() != this );
	return ( shared_from_this() );
}

void frame::reorder() {
	frame_ptr f = shared_from_this();
	unsigned int i = 0;
	do {
		f->index = i++;
		f = f->get_next();
	} while( f.get() != this );
}

void frame::onkey( KEY_EVENT_RECORD const& key ) {
	command c;
	c.type = command::cmdKbrd;
	c.key = key;
	this->pipe.write( &c, sizeof( c ) );
}

void frame::onchar( TCHAR ch ) {
}

void frame::ctrl_break() {
	command c;
	c.type = command::cmdCtrlBreak;
	this->pipe.write( &c, sizeof( c ) );
}

void frame::ctrl_c() {
	command c;
	c.type = command::cmdCtrlC;
	this->pipe.write( &c, sizeof( c ) );
}

void frame::clear() {
	command c;
	c.type = command::cmdExit;
	this->pipe.write( &c, sizeof( c ) );
	//
	this->next = this->prev = frame_ptr();
	base_node_t::clear();
}

void frame::draw( HDC dc, RECT const& rt ) {
	RECT rect = rt;
	struct _{
		static bool __( TCHAR const* str, size_t const count, HDC dc, RECT& rect ) {
			RECT rt;
			SetRectEmpty( &rt );
			DrawText( dc, str, count, &rt, DT_LEFT | DT_TOP | DT_CALCRECT );
			//SIZE sz;
			//GetTextExtentPoint32( dc, str, count, &sz );
			if ( ( rect.bottom -= rt.bottom ) > rect.top ) {
				OffsetRect( &rt, rect.left, rect.bottom );
				DrawText( dc, str, count, &rt, DT_LEFT | DT_TOP );
				return true;
			}
			return false;
		}
	};
	//this->bf.for_each( 0, boost::bind( &_::__, _1, _2, dc, boost::ref( rect ) ) );
}

uni_string frame::get_caption() const {
	std::stringstream ss;
	ss << this->process_caption << " #" << this->index + 1;
	return ( ss.str() );
}
