#include "./pch.hpp"
#include "./appl.hpp"

appl::appl( logger::shared_ptr l, pref::shared_ptr p )/* :
		wnd( l, p )*/ {
	atom::mount<appl2logger>( this, l );
	atom::mount<appl2pref>( this, p );
	this->get_logger() << "create appl" << std::endl;
}

appl::~appl() {
	this->get_logger() << "free appl" << std::endl;
}

bool appl::init() {
	return ( true/*this->wnd.init()*/ );
}

void appl::run() {
	//this->wnd.show( true ).run();
}


