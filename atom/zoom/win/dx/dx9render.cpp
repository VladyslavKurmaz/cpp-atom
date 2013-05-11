#include "./dx9render.hpp"

namespace atom { namespace zoom {

	dx9render::dx9render( logger_ptr l, stream_ptr s, canvas_ptr c ) : render( l, s, c ) {
	}

	dx9render::~dx9render() {
	}

	bool
	dx9render::init() {
		guard_t l( this->lock );
		if ( render::init() ) {
			return true;
		}
		return false;
	}

	void
	dx9render::clear(){
		guard_t l( this->lock );
		render::clear();
	}

} }
