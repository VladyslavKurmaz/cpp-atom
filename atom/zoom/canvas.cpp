#include "./canvas.hpp"

namespace atom { namespace zoom {

	canvas::canvas( logger_ptr l ) : base_entity_t( l ) {
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
