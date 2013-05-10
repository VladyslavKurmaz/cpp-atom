#include "./dx9render.hpp"

namespace atom { namespace zoom {

	dx9render::dx9render( logger_ptr l ) :
			render( l ) {
			this->get_logger() << "render has been created" << std::endl;

	}

	dx9render::~dx9render() {
		guard_t l( this->lock );
		this->get_logger() << "render has been destoyed" << std::endl;
	}

} }
