#include "./canvas.hpp"

namespace atom { namespace zoom {

	canvas::canvas( logger_ptr l, stream_ptr s ) : base_entity_t( l, s ) {
	}

	canvas::~canvas(){
	}
		
	bool
	canvas::init() {
		return true;
	}

	void
	canvas::clear(){
		base_entity_t::clear();
	}

} }
