
namespace atom { namespace zoom {

	render::render( logger_ptr l, canvas_ptr c ) :
			base_entity_t( l )
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
		this->canvas->clear();
		base_entity_t::clear();
	}

} }
