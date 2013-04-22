#include "./pch.hpp"
#include "./cmds.hpp"

#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./window.hpp"
#include "./frame.hpp"



frame::frame( logger_ptr l, pref_ptr p, window_ptr w, frame_coord const & fc ) :
		index( 0 )
	,	buffer()
	,	coord( fc )
	,	next()
	,	prev()
	,	cmpp()
	,	process_caption() {
	atom::mount<frame2logger>( this, l );
	atom::mount<frame2pref>( this, p );
	atom::mount<frame2window>( this, w );
	//
	if ( this->cmpp.server_init( w->get_hwnd(), 80, 500 ) ) {
		if ( this->cmpp.server_bind() ) {
			return;
		}
	}
	//
	throw std::runtime_error( "Console mapping initialisation error" );
}

frame::~frame() {
}

frame_ptr frame::split( bool const pref_h ){
#ifndef STANDALONE
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
	frame_ptr f = frame_ptr( new frame( 
										get_value( boost::mpl::identity< frame2logger >() ).item(),
										get_value( boost::mpl::identity< frame2pref >() ).item(),
										get_slot<frame2window>().item(),
										fc ) );
	f->next = f->prev = f;
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

frame_ptr frame::close() {
	this->cmpp.send_exit();
	this->cmpp.close();
	frame_ptr f = frame_ptr();
	if ( ( this->get_next() != this->shared_from_this() ) && ( this->get_prev() != this->shared_from_this() ) ) {
		f = this->get_next();
		this->next->prev = this->get_prev();
		this->prev->next = this->get_next();
	}
	//
	this->next = this->prev = frame_ptr();
	base_node_t::clear();
	return ( f );
}

frame_ptr frame::get_by_index( unsigned int const i ) {
	frame_ptr f = shared_from_this();
	do {
		if ( f->index == i ) {
			return ( f );
		}
		f = f->get_next();
	} while( f.get() != this );
	return ( shared_from_this() );
}

void frame::reorder() {
	frame_ptr f = shared_from_this();
	unsigned int i = 0;
	do {
		f->index = i++;
		f = f->get_next();
	} while( f.get() != this );
}

void frame::onkey( KEY_EVENT_RECORD const& key ) {
	this->cmpp.send_key( key );
}

void frame::ctrl_break() {
	this->cmpp.send_ctrl_break();
}

void frame::ctrl_c() {
	this->cmpp.send_ctrl_c();
}

void frame::draw( HDC dc, RECT const& rt ) {
	RECT rect;
	SetRectEmpty( &rect );
	DrawText( dc, "A", 1, &rect, DT_LEFT | DT_TOP | DT_CALCRECT );
	this->cmpp.draw( dc, rt, rect.right, rect.bottom );
}

uni_string frame::get_caption() const {
	std::stringstream ss;
	ss << /*this->process_caption << */" #" << this->index + 1;
	return ( ss.str() );
}

