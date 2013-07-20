#include "./pch.hpp"
#include "./cmds.hpp"

#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./frame.hpp"
#include "./window.hpp"



frame::frame( logger_ptr l, pref_ptr p, window_ptr w ) :
		buffer()
	,	next()
	,	prev()
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

	//frame_coord fc = this->coord;
	//if ( ( this->coord.width < this->coord.height) || ( ( this->coord.width == this->coord.height ) && pref_h ) ) {
	//	// split vertical
	//	this->coord.width *= 2;
	//	fc.width *= 2;
	//	fc.left = fc.left + fraction( 1, fc.width );
	//} else if ( ( this->coord.width > this->coord.height) || ( ( this->coord.width == this->coord.height ) && !pref_h ) ) {
	//	// split horizontal
	//	this->coord.height *= 2;
	//	fc.height *= 2;
	//	fc.top = fc.top + fraction( 1, fc.height );
	//}

frame_ptr frame::split(){
#ifndef STANDALONE
	frame_ptr f = create( get_slot<frame2logger>().item(), get_slot<frame2pref>().item(), get_slot<frame2window>().item() );
	//
	frame_ptr l = this->shared_from_this();
	frame_ptr r = l->get_next();
	//
	std::swap( l->next, f->next );
	std::swap( r->prev, f->prev );
	//
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
	frame_ptr f = frame_ptr();
	if ( ( this->get_next() != this->shared_from_this() ) && ( this->get_prev() != this->shared_from_this() ) ) {
		f = this->get_next();
		this->next->prev = this->get_prev();
		this->prev->next = this->get_next();
	}
	this->next = this->prev = frame_ptr();
	get_slot<frame2window>().item()->frame_close( this->shared_from_this(), f );
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

