#include "./pch.hpp"
#include "./frame.hpp"

frame::frame( logger::shared_ptr l, pref::shared_ptr p, frame_coord const & fc ) :
		coord( fc )
	,	next()
	,	prev() {
	atom::mount<frame2logger>( this, l );
	atom::mount<frame2pref>( this, p );
}

frame::~frame() {
}

frame::shared_ptr frame::split( bool const pref_h ){
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
	shared_ptr f = shared_ptr( new frame( 
										get_value( boost::mpl::identity< frame2logger >() ).item(),
										get_value( boost::mpl::identity< frame2pref >() ).item(),
										fc ) );
	f->next = f->prev = f;
	//
	shared_ptr l = this->shared_from_this();
	shared_ptr r = l->get_next();
	//
	std::swap( l->next, f->next );
	std::swap( r->prev, f->prev );
	//
	return f;
}

void frame::clear() {
	this->next = this->prev = frame::shared_ptr();
}


