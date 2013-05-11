#include "./canvas.hpp"

namespace atom { namespace zoom {

	canvas::canvas( stream_ptr s ){
		atom::mount<canvas2stream>( this, s );
	}

	canvas::~canvas(){
	}
		
	void
	canvas::clear(){
		base_node_t::clear();
	}

} }
