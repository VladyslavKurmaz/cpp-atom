#include "./pch.hpp"
#include "./appl.hpp"

appl::appl( logger::shared_ptr l, pref::shared_ptr p ) {
	atom::mount<appl2logger>( this, l );
	atom::mount<appl2pref>( this, p );
	atom::mount<appl2window>( this, window::create( l, p ) );
}

appl::~appl() {
}

bool appl::init() {
	return ( this->get_window().init() );
}

appl& appl::run() {
	this->get_window().show( true ).activate();
	this->get_window().run();
	return (*this);
}

void appl::clear() {
	this->get_window().clear();
}


