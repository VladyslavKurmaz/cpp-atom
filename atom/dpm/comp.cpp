#include "./pch.hpp"
#include "./logger.hpp"
#include "./comp.hpp"
#include "./env.hpp"

comp::comp( logger_ptr l, appl_ptr a, string_t const & id, boost::filesystem::path const & h ) :
		id( id )
	,	home( h )
{
	atom::mount<comp2logger>( this, l );
	atom::mount<comp2appl>( this, a );
}

comp::~comp() {
}

void
comp::clear() {
	base_node_t::clear();
}

void
comp::print( logger_ptr l, string_t const& offs ) {
	*l << offs << this->id << std::endl;
}
