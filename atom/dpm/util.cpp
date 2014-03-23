#include "./pch.hpp"
#include "./logger.hpp"
#include "./util.hpp"

logger&
print_title( logger& log, string_t const& t, unsigned int const l ) {
	print_offset( log, l ) << "[" << t << "]";
	return log;
}

logger&
print_offset( logger& log, unsigned int const l ) {
	assert( l < 512 );
	char_t offset[512] = { 0 };
	std::fill( offset, offset + 512, ' ' );
	offset[ l ] = '\0';
	log << offset;
	return log;
}
