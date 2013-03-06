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
	frame_coord fc = this->get_coord();
	shared_ptr f = shared_ptr( new frame( 
										get_value( boost::mpl::identity< frame2logger >() ).item(),
										get_value( boost::mpl::identity< frame2pref >() ).item(),
										fc ) );
	return f;
}

void frame::clear() {
	this->next = this->prev = frame::shared_ptr();
	base_node_t::clear();
}


