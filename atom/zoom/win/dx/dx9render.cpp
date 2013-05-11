#include "./dx9render.hpp"

namespace atom { namespace zoom {

	dx9render::dx9render( logger_ptr l, stream_ptr s ) : render( l, s ) {
	}

	dx9render::~dx9render() {
		guard_t l( this->lock );
	}

	bool
	dx9render::init() {
		if ( render::init() ) {
			return true;
		}
		return false;
	}

	void
	dx9render::clear(){
		render::clear();
	}

} }
