#include "./pch.hpp"
#include "./classes.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./panel.hpp"
#include "./atlas.hpp"
#include "./window.hpp"
#include <boost/algorithm/string.hpp>

atlas::atlas( logger_ptr l, pref_ptr p, window_ptr w ):
		mode( l, p, w ) {
}

atlas::~atlas() {
}

void atlas::activate( bool const state ) {
}

void atlas::show( bool const state ) {
}

bool atlas::command( int const id ) {
	return false;
}

void atlas::key( KEY_EVENT_RECORD const& k ) {
}

void atlas::mouselbdown( bool dblclick, int x, int y, unsigned int state ) {
}

void atlas::mouselbup( int x, int y, unsigned int state ) {
}

void atlas::mousemove( int x, int y, unsigned int state ){
}

void atlas::paint( paint_param_t& paintParam, RECT const& rect ) {
}

void atlas::clear() {
	base_t::clear();
}


