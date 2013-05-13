
namespace atom { namespace zoom {

	dx9mesh::dx9mesh( logger_ptr l, stream_ptr s ) : mesh( l, s ) {
	}

	dx9mesh::~dx9mesh() {
	}

	void
	dx9mesh::clear() {
		mesh::clear();
	}

} }
