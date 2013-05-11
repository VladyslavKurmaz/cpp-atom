#include "./win_canvas.hpp"

namespace atom { namespace zoom {

	win_canvas::win_canvas( stream_ptr s ) : canvas( s ) {
	}

	win_canvas::~win_canvas(){
	}
		
	void
	win_canvas::clear(){
		canvas::clear();
	}

} }
