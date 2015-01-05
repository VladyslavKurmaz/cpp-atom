#include "./pch.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./frame.hpp"
#include "./window.hpp"

frame_ptr frame::create( logger_ptr l ) {
	static frame_id_t gid = 0;
	frame_ptr f = frame_ptr( new frame( ++gid, l ) );
	return ( f );
}

frame::frame( frame_id_t const i, logger_ptr l ) :
		id( i )
	,	index( 0 )
	,	console2Proxy()
	,	process_caption() {
	atom::mount<frame2logger>( this, l );
	this->console2Proxy.console();
}

frame::~frame() {
}

void frame::clear(){
	this->console2Proxy.join();
	base_node_t::clear();
}

void  frame::key( KEY_EVENT_RECORD const& k ) {
	this->console2Proxy.write( k );
}


void frame::paint( HDC dc, RECT const& rt ) {
	RECT rect;
	SetRectEmpty( &rect );
	DrawText( dc, "A", 1, &rect, DT_LEFT | DT_TOP | DT_CALCRECT );
}
