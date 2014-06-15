#include "./pch.hpp"
#include "./logger.hpp"
#include "./entity.hpp"

entity::entity( logger_ptr l, entity_ptr p ) {
	atom::mount<entity2logger>( this, l );
	atom::mount<entity2entity>( this, p );
}

entity::~entity() {
}

void
entity::clear() {
	base_node_t::clear();
}
