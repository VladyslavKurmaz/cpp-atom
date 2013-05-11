#include "./render.hpp"

namespace atom { namespace zoom {

	render::render( logger_ptr l, stream_ptr s ) : lock() {
		atom::mount<render2logger>( this, l );
		atom::mount<render2stream>( this, s );
	}

	render::~render() {
	}

	bool
	render::init() {
		return true;
	}

	void
	render::clear() {
		base_node_t::clear();
	}

} }
