#include "./pch.hpp"
#include "./appl.hpp"

appl::appl( logger::shared_ptr l, pref::shared_ptr p ) {
	atom::mount<appl2logger>( this, l );
	atom::mount<appl2pref>( this, p );
	atom::mount<appl2window>( this, window::create( l, p ) );

	this->get_logger() << "create appl" << std::endl;
}

appl::~appl() {
	this->get_logger() << "free appl" << std::endl;
}

bool appl::init() {
	return ( this->get_window().init() );
}

void appl::run() {
	this->get_window().show( true ).run();
}


