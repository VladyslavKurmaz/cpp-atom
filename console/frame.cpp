#include "./pch.hpp"
#include "./cmds.hpp"

#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./frame.hpp"
#include "./window.hpp"

frame_ptr frame::create( logger_ptr l ) {
	static frame_id_t gid = 0;
	frame_ptr f = frame_ptr( new frame( ++gid, l ) );
//	struct notify {
//		frame_id_t id;
//		unsigned long tid;
//		notify( frame_id_t const i ) : id( i ), tid( GetCurrentThreadId() ){}
//		void operator()() { PostThreadMessage( this->tid, WM_FRAMEEXIT, this->id, 0 ); }
//	} n( gid );
////	f->brdg.run( boost::bind<void>( n ), boost::bind( &window::parseConf, w, _1 ) );
	return ( f );
}

frame::frame( frame_id_t const i, logger_ptr l ) :
		id( i )
	,	index( 0 )
	//,	buffer()
	//,	brdg()
	,	process_caption() {
	atom::mount<frame2logger>( this, l );
}

frame::~frame() {
}

void frame::clear(){
	//this->brdg.join();
	base_node_t::clear();
}

void
frame::process( bridge_msg::type const id, void const* param ) {
	//this->brdg.write( id, param );
}

void frame::draw( HDC dc, RECT const& rt ) {
	RECT rect;
	SetRectEmpty( &rect );
	DrawText( dc, "A", 1, &rect, DT_LEFT | DT_TOP | DT_CALCRECT );
	//this->cmpp.draw( dc, rt, rect.right, rect.bottom );
}
