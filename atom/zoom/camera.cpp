
namespace atom { namespace zoom {

	camera::camera( logger_ptr l, stream_ptr s ) : entity< camera >( l, s ) {
	}

	camera::~camera() {
	}

	void
	camera::clear(){
		entity< camera >::clear();
	}

	void
	camera::input( input_ptr i ) {
		*( this->get_logger() ) << "input" << std::endl;
	}

} }
