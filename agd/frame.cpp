#include "./pch.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./frame.hpp"
#include "./window.hpp"

frame_ptr frame::create( logger_ptr l, COORD const& size ) {
	static frame_id_t gid = 0;
	frame_ptr f = frame_ptr( new frame( ++gid, l, size ) );
	return ( f );
}

frame::frame( frame_id_t const i, logger_ptr l, COORD const& size ) :
		id( i )
	,	index( 0 )
	,	console2Proxy( size )
	,	process_caption() {
	atom::mount<frame2logger>( this, l );
	this->console2Proxy.console();
}

frame::~frame() {
}

void frame::clear(){
	this->console2Proxy.writeExit();
	this->console2Proxy.join();
	base_node_t::clear();
}

void  frame::key( KEY_EVENT_RECORD const& k ) {
	this->console2Proxy.write( k );
}

void frame::paint( HDC dc, RECT const& rt, unsigned int const lineHeight ) {
	bridge::lines_t lines;
	this->console2Proxy.getLines( lines );
	//
	size_t view_lines = RECT_HEIGHT(rt) / lineHeight;
	size_t total_lines = lines.size();
	//
	int y = 0;
	for ( size_t i = ( view_lines < total_lines )?( total_lines - view_lines ):( 0 ); i < total_lines; i++ ) {
		TextOut( dc, rt.left, rt.top + y, lines[i].c_str(), lines[i].length() );
		y += lineHeight;
	}
}