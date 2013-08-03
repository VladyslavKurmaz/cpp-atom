#pragma once
#include "./classes.hpp"
#include "./bridge.hpp"


typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > frame2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > frame2pref;
typedef atom::nstorage< window, boost::shared_ptr, atom::narray1 > frame2window;


class frame :
	public atom::node< LOKI_TYPELIST_3( frame2logger, frame2pref, frame2window ) >,
	public boost::enable_shared_from_this< frame > {
	///
	typedef atom::node< LOKI_TYPELIST_3( frame2logger, frame2pref, frame2window ) >
		base_node_t;
public:
	///
	static frame_ptr create( logger_ptr l, pref_ptr p, window_ptr w );
	///
	~frame();
	///
	void
	set_index( unsigned int const i ) { this->index = i; }
	///
	unsigned int
	get_index() const { return ( this->index ); }
	///
 	frame_ptr
	split();
	///
	void
	clear();
	///
	void
	process( bridge_msg::type const id, void const* param );
	///
	void
	draw( HDC dc, RECT const& rt );

protected:
	//
	logger_ptr
	get_logger() { return ( get_slot<frame2logger>().item() ); }
	//
	pref_ptr
	get_pref() { return ( get_slot<frame2pref>().item() ); }
	///
	window_ptr
	get_window() { return ( get_slot<frame2window>().item() ); }

private:
	///
	frame_id_t
		id;
	///
	unsigned int
		index;
	///
	string_t
		buffer;
	///
	bridge
		brdg;
	///
	string_t
		process_caption;
	///
	frame( frame_id_t const i, logger_ptr l, pref_ptr p, window_ptr w );
};