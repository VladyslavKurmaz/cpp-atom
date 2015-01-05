#pragma once
#include "./classes.hpp"
#include "./bridge.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > frame2logger;

class frame :
	public atom::node< LOKI_TYPELIST_1( frame2logger ) >,
	public boost::enable_shared_from_this< frame > {
	///
	typedef atom::node< LOKI_TYPELIST_1( frame2logger ) >
		base_node_t;
public:
	///
	static frame_ptr create( logger_ptr l );
	///
	~frame();
	///
	void
	set_index( unsigned int const i ) { this->index = i; }
	///
	unsigned int
	get_index() const { return ( this->index ); }
	///
	frame_id_t
	get_id() const { return ( this->id ); }
	///
	void
	clear();
	//
	void key( KEY_EVENT_RECORD const& k );
	///
	void
	paint( HDC dc, RECT const& rt );

protected:
	//
	logger&
	getLogger() { return ( *( get_slot<frame2logger>().item() ) ); }

private:
	///
	frame_id_t
		id;
	///
	unsigned int
		index;
	//
	bridge
		console2Proxy;
	///
	std::string
		process_caption;
	///
	frame( frame_id_t const i, logger_ptr l );
};