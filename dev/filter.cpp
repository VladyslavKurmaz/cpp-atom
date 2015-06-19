#include "./pch.hpp"
#include "./logger.hpp"
#include "./filter.hpp"


filter::filter( logger_ptr l ) {
	atom::mount<filter2logger>( this, l );
}

filter::~filter() {
}

void
filter::clear() {
	base_node_t::clear();
}
