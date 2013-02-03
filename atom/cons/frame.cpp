#include "./pch.hpp"
#include "./frame.hpp"

frame::frame( logger::shared_ptr l, pref::shared_ptr p ) {
	atom::mount<frame2logger>( this, l );
	atom::mount<frame2pref>( this, p );

	this->get_logger() << "create frame" << std::endl;
}

frame::~frame() {
	this->get_logger() << "free frame" << std::endl;
}
