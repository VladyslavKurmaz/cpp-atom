#include "./render.hpp"

namespace atom { namespace zoom {

	render::render( logger_ptr l ) {
		atom::mount<render2logger>( this, l );
		this->get_logger() << "render has been created" << std::endl;
	}

	render::~render() {
		this->get_logger() << "render has been destoyed" << std::endl;
	}

} }
