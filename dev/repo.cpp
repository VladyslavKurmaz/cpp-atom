#include "./pch.hpp"
#include "./entity.hpp"
#include "./repo.hpp"

repo::repo( logger_ptr l, entity_ptr p ) : entity( l, p ) {
}

repo::~repo() {
}

void
repo::clear() {
	entity::clear();
}
