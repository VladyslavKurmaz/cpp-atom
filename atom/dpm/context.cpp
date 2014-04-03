#include "./pch.hpp"
#include "./logger.hpp"
#include "./context.hpp"


context::context( logger_ptr l ) {
	atom::mount<context2logger>( this, l );
}

context::~context() {
}

void
context::clear() {
	base_node_t::clear();
}