#include "./pch.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./frame.hpp"

frame::frame( logger_ptr l, pref_ptr p, frame_coord const & fc ) :
		coord( fc )
	,	next()
	,	prev()
	,	process() {
	atom::mount<frame2logger>( this, l );
	atom::mount<frame2pref>( this, p );
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
}


