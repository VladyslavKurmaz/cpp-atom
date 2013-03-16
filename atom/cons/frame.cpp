#include "./pch.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./window.hpp"
#include "./frame.hpp"

frame::frame( logger_ptr l, pref_ptr p, window_ptr w, frame_coord const & fc ) :
		buffer()
	,	bf()
	,	coord( fc )
	,	next()
	,	prev()
	,	process() {
	atom::mount<frame2logger>( this, l );
	atom::mount<frame2pref>( this, p );
	atom::mount<frame2window>( this, w );
	//
	bf.init( 162, 500 );
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

void frame::run( uni_string const& cmd ) {
	this->process = process::create( get_value( boost::mpl::identity< frame2logger >() ).item(), this->shared_from_this() );
	this->process->run( cmd );
	//child->run( "cmd.exe" );
	//child->run( "msbuild.exe" );
	//child->run( "c:\\work\\env\\cygwin\\bin\\bash.exe --login -i" );
	//child->run( "powershell.exe" );
	//child->run( "cmd /c \"powershell.exe\"" );
}

void  frame::onchar( TCHAR ch ) {
	if ( ch == VK_RETURN ) {
		process->write( "\x0D\x0A" );
	} else {
		process->write( ch );
	}
}

void frame::clear() {
	if ( this->process ) {
		process->close().clear();
	}
	this->next = this->prev = frame_ptr();
	base_node_t::clear();
}

void frame::append( void const* b, size_t const sz ) {
	this->bf.write( static_cast< char const* >( b ), sz );
	this->buffer += std::string( (char const*)b );
	get_slot<frame2window>().item()->invalidate();
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
	this->bf.for_each( 0, boost::bind( &_::__, _1, _2, dc, boost::ref( rect ) ) );
}
