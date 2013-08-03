#include "./pch.hpp"
#include "./cmds.hpp"

#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./frame.hpp"
#include "./window.hpp"

frame_ptr frame::create( logger_ptr l, pref_ptr p, window_ptr w ) {
	static frame_id_t id = 0;
	frame_ptr f = frame_ptr( new frame( ++id, l, p, w ) );
	struct notify {
		unsigned long tid;
		notify() : tid( GetCurrentThreadId() ){}
		void operator()() { PostThreadMessage( this->tid, WM_FRAMEEXIT, id, 0 ); }
	} n;
	f->brdg.run( boost::bind<void>( n ) );
	return ( f );
}

frame::frame( frame_id_t const i, logger_ptr l, pref_ptr p, window_ptr w ) :
		id( i )
	,	index( 0 )
	,	buffer()
	,	brdg()
	,	process_caption() {
	atom::mount<frame2logger>( this, l );
	atom::mount<frame2pref>( this, p );
	atom::mount<frame2window>( this, w );
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

void frame::clear(){
	this->brdg.join();
	base_node_t::clear();
}

void
frame::process( bridge_msg::type const id, void const* param ) {
	this->brdg.write( id, param );
}

void frame::draw( HDC dc, RECT const& rt ) {
	RECT rect;
	SetRectEmpty( &rect );
	DrawText( dc, "A", 1, &rect, DT_LEFT | DT_TOP | DT_CALCRECT );
	//this->cmpp.draw( dc, rt, rect.right, rect.bottom );
}
