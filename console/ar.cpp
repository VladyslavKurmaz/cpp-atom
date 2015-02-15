#include "./pch.hpp"
#include "./classes.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./ar.hpp"

ar::ar( logger_ptr l, pref_ptr p ):
		mode( l, p ) {
}

ar::~ar() {
}

void ar::activate( bool const state ) {
}

void ar::show( bool const state ) {
}

bool ar::command( int const id ) {
	return false;
}

void ar::key( KEY_EVENT_RECORD const& k ) {
}

void ar::paint( paint_param_t& paintParam, RECT const& rect ) {
}

void ar::clear() {
	base_t::clear();
}


