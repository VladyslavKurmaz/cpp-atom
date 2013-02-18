#include "./pch.hpp"
#include "./process.hpp"

process::process( logger::shared_ptr l, frame::shared_ptr f ) {
	atom::mount<process2logger>( this, l );
	atom::mount<process2frame>( this, f );
}

process::~process() {
}
