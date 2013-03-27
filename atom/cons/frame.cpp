#include "./pch.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./window.hpp"
#include "./frame.hpp"

frame::frame( logger_ptr l, pref_ptr p, window_ptr w, frame_coord const & fc ) :
		index( 0 )
	,	buffer()
	,	bf()
	,	coord( fc )
	,	next()
	,	prev()
	,	process()
	,	process_caption() {
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

void frame::run( uni_string const& cmd ) {
	cleanup_process();
	this->process = process::create( get_value( boost::mpl::identity< frame2logger >() ).item(), this->shared_from_this() );
	this->process_caption = this->process->run( cmd );
}

void  frame::onchar( TCHAR ch ) {
	if ( ch == VK_RETURN ) {
		//process->write( "\x0D\x0A" );
		//process->write( "\x0A" );
	//} else if ( ch == VK_SPACE ) {
	//	process->write( "^C\x0A" );
	} else {
		//process->write( ch );
	}
}

void frame::clear() {
	cleanup_process();
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

uni_string frame::get_caption() const {
	std::stringstream ss;
	ss << this->process_caption << " #" << this->index + 1;
	return ( ss.str() );
}


void frame::cleanup_process() {
	if ( this->process ) {
		process->close().terminate().clear();
	}
}
