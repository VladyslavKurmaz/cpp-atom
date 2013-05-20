
namespace atom { namespace zoom {

	camera::camera( logger_ptr l ) : entity< camera >( l ) {
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
