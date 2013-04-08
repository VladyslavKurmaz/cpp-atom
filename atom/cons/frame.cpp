#include "./pch.hpp"
#include "./cmds.hpp"
#include "./consd_tty.hpp"

#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./window.hpp"
#include "./frame.hpp"


HANDLE ht;
CONSOLE_SCREEN_BUFFER_INFOEX csbiex = { 0 };
std::string pipe_name;
std::string shmem_name;
COORD size;

DWORD WINAPI ConsEmul( LPVOID lpParameter ) {
	cons_mpp cmpp;
	if ( cmpp.init( pipe_name, shmem_name, GetStdHandle( STD_INPUT_HANDLE ), GetStdHandle( STD_OUTPUT_HANDLE ), csbiex.dwSize.X, csbiex.dwSize.Y ) ) {
		tty tty1( std::cout );
		tty1.run( csbiex.dwSize.X, csbiex.dwSize.Y );
	}
	return 0;
}

frame::frame( logger_ptr l, pref_ptr p, window_ptr w, frame_coord const & fc ) :
		index( 0 )
	,	buffer()
	,	coord( fc )
	,	next()
	,	prev()
	,	pi()
	,	si()
	,	cmpp()
	,	process_caption() {
	atom::mount<frame2logger>( this, l );
	atom::mount<frame2pref>( this, p );
	atom::mount<frame2window>( this, w );
	//
	csbiex.cbSize = sizeof( csbiex );
	GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
	this->cmpp.init( w->get_hwnd(), csbiex.dwSize.X, csbiex.dwSize.Y );
	//
	HANDLE ht = CreateThread( NULL, 0, ConsEmul, reinterpret_cast<LPVOID>( NULL ), 0, NULL );
	///
	pipe_name = this->cmpp.get_pipe_name();
	shmem_name = this->cmpp.get_shmem_name();
	this->cmpp.bind();
}

frame::~frame() {
	WaitForSingleObject( ht, INFINITE );
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
	this->cmpp.onkey( key );
}

void frame::ctrl_break() {
	this->cmpp.ctrl_break();
}

void frame::ctrl_c() {
	this->cmpp.ctrl_c();
}

void frame::clear() {
	this->cmpp.onexit();
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
	//CHAR_INFO const* ci = static_cast< CHAR_INFO const* >( this->shmem_region->get_address() );
	//std::string s;
	//for( unsigned int i = 0; i < this->si.dwXCountChars; ++i ) {
	//	s += ci[i].Char.AsciiChar;
	//}
	//DrawText( dc, s.c_str(), -1, &rect, DT_LEFT | DT_TOP );
}

uni_string frame::get_caption() const {
	std::stringstream ss;
	ss << /*this->process_caption << */" #" << this->index + 1;
	return ( ss.str() );
}

