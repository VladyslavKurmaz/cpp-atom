#include "./render.hpp"

namespace atom { namespace zoom {

	render::render( logger_ptr l, stream_ptr s, canvas_ptr c ) :
			base_entity_t( l, s )
		,	lock()
		,	canvas( c ) {
	}

	render::~render() {
	}

	bool
	render::init() {
		return true;
	}

	bool
	render::frame() {
		return true;
	}

	void
	render::clear() {
		this->canvas.reset();
		base_entity_t::clear();
	}

} }
