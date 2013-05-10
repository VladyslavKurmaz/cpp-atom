#include "./dx9render.hpp"

namespace atom { namespace zoom {

	dx9render::dx9render( logger_ptr l ) :
			render< dx9render, atom::lock_t >( l ) {

	}

	dx9render::~dx9render() {
		guard_t l( this->lock );
	}

} }
