#include "./pch.hpp"
#include "./cmds.hpp"

#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./frame.hpp"
#include "./window.hpp"

frame::frame( logger_ptr l, pref_ptr p, window_ptr w ) :
		buffer()
	,	brdg()
	,	process_caption() {
	atom::mount<frame2logger>( this, l );
	atom::mount<frame2pref>( this, p );
	atom::mount<frame2window>( this, w );
	//
	try {
		this->brdg.init().bind().run();
	}
	catch( std::exception& e ){
		*(this->get_logger()) << e.what() << std::endl;
	}
}

frame::~frame() {
}

frame_ptr frame::split(){
#ifndef STANDALONE
	frame_ptr f = create( get_slot<frame2logger>().item(), get_slot<frame2pref>().item(), get_slot<frame2window>().item() );
	return f;
#else
	return ( this->shared_from_this() );
#endif
}

void frame::close() {
	this->brdg.close();

	// ??? move code below to the async close notification from console
	this->get_window()->frame_close( this->shared_from_this() );
	base_node_t::clear();
}

void
frame::process( bridge_msg::type const id, void const* param ) {
	this->brdg.write_to_pipe( id, param );
}

void frame::draw( HDC dc, RECT const& rt ) {
	RECT rect;
	SetRectEmpty( &rect );
	DrawText( dc, "A", 1, &rect, DT_LEFT | DT_TOP | DT_CALCRECT );
	//this->cmpp.draw( dc, rt, rect.right, rect.bottom );
}
