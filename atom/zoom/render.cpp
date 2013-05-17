
namespace atom { namespace zoom {

	render::render( logger_ptr l, stream_ptr s, canvas_ptr c ) :
			base_entity_t( l, s )
		,	lock()
		,	canvas( c )
		,	camera() {
	}

	render::~render() {
	}

	bool
	render::init( camera_ptr c ) {
		this->camera = c;
		return true;
	}

	bool
	render::frame() {
		return true;
	}

	void
	render::clear() {
		this->camera->clear();
		this->camera.reset();
		this->canvas.reset();
		base_entity_t::clear();
	}

} }
