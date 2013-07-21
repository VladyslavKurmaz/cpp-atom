#include "./pch.hpp"
#include "./cmds.hpp"

#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./frame.hpp"
#include "./window.hpp"



frame::frame( logger_ptr l, pref_ptr p, window_ptr w ) :
		buffer()
	//,	cmpp()
	,	process_caption() {
	atom::mount<frame2logger>( this, l );
	atom::mount<frame2pref>( this, p );
	atom::mount<frame2window>( this, w );
	//
	//if ( this->cmpp.server_init( w->get_hwnd(), 80, 500 ) ) {
	//	if ( this->cmpp.server_bind() ) {
	//		return;
	//	}
	//}
	////
	//throw std::runtime_error( "Console mapping initialisation error" );
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
	//this->cmpp.process( command::cmdExit, NULL );
	//this->cmpp.process( command::cmdQuit, NULL );
	//this->cmpp.close();

	// ??? move code below to the async close notification from console
	get_slot<frame2window>().item()->frame_close( this->shared_from_this() );
	base_node_t::clear();
}

void
frame::process( command::type const id, void const* param ) {
	//this->cmpp.process( id, param );
}

void frame::draw( HDC dc, RECT const& rt ) {
	RECT rect;
	SetRectEmpty( &rect );
	DrawText( dc, "A", 1, &rect, DT_LEFT | DT_TOP | DT_CALCRECT );
	//this->cmpp.draw( dc, rt, rect.right, rect.bottom );
}

uni_string frame::get_caption() const {
	std::stringstream ss;
	ss << /*this->process_caption << */" #";
	return ( ss.str() );
}

