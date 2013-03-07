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

frame::shared_ptr frame::split(){
	frame_coord fc = this->coord;
	//
	this->coord.left_n;
	this->coord.left_d;
	this->coord.top_n;
	this->coord.top_d;
	this->coord.width *= 2;
	this->coord.height;
	//
	fc.left_n = 1;
	fc.left_d = 2;
	fc.top_n;
	fc.top_d;
	fc.width *= 2;
	fc.height;
	//
	shared_ptr f = shared_ptr( new frame( 
										get_value( boost::mpl::identity< frame2logger >() ).item(),
										get_value( boost::mpl::identity< frame2pref >() ).item(),
										fc ) );
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
	base_node_t::clear();
}


