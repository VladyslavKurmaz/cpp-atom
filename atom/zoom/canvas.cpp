#include "./canvas.hpp"

namespace atom { namespace zoom {

	canvas::canvas( stream_ptr s ){
		atom::mount<canvas2stream>( this, s );
	}

	canvas::~canvas(){
	}
		
	bool
	canvas::init() {
		return true;
	}

	void
	canvas::clear(){
		base_node_t::clear();
	}

} }
