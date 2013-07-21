#pragma once
#include "./classes.hpp"
#include "./cons_mpp.hpp"


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
	static frame_ptr create( logger_ptr l, pref_ptr p, window_ptr w ) {
		return ( frame_ptr( new frame( l, p, w ) ) );
	}
	///
	~frame();
	///
 	frame_ptr
	split();
	///
	void
	close();
	///
	void
	process( command::type const id, void const* param );
	///
	void
	draw( HDC dc, RECT const& rt );

protected:
	//
	logger&
	get_logger() { return ( *( get_slot<frame2logger>().item() ) ); }
	//
	pref&
	get_pref() { return ( *( get_slot<frame2pref>().item() ) ); }

private:
	//
	uni_string
		buffer;
	///
	//cons_mpp
	//	cmpp;
	///
	uni_string
		process_caption;
	///
	frame( logger_ptr l, pref_ptr p, window_ptr w );
};