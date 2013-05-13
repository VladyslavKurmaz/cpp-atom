
namespace atom { namespace zoom {

	mesh::mesh( logger_ptr l, stream_ptr s ) : wo( l, s ) {
	}

	mesh::~mesh() {
	}

	void
	mesh::clear() {
		wo::clear();
	}

} }
