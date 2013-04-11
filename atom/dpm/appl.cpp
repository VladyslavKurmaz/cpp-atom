#include "./pch.hpp"
#include "./env.hpp"
#include "./appl.hpp"

appl::appl( logger_ptr l ) : po() {
	atom::mount<appl2logger>( this, l );
}

appl::~appl() {
}

bool
appl::init( int argc, char const * const argv[] ) {
	return false;
}

appl&
appl::run(){
	return ( *this );
}

void
appl::clear(){
	( this->get_env() )->clear();
	base_node_t::clear();
}
