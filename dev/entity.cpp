#include "./pch.hpp"
#include "./logger.hpp"
#include "./entity.hpp"

boost::filesystem::path entity::dev_home;

entity::entity( logger_ptr l, entity_ptr p, string_t const& sid, boost::property_tree::ptree const& a ):
		id( sid )
	,	attr( a ) {
	atom::mount<entity2logger>( this, l );
	atom::mount<entity2entity>( this, p );
}

entity::~entity() {
}

void
entity::scan( ) {
}

void
entity::clear() {
	base_node_t::clear();
}
