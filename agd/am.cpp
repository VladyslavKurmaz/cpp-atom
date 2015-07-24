#include "./pch.hpp"
#include "./classes.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./panel.hpp"
#include "./window.hpp"
#include "./am.hpp"

am::am(boost::property_tree::ptree const& c, logger_ptr l, pref_ptr p, window_ptr w) :
		mode( c, l, p, w )
{
}

am::~am() {
}

void am::activate( bool const state ) {
}

void am::show( bool const state ) {
}

bool am::command( int const id ) {
	return false;
}

void am::key( KEY_EVENT_RECORD const& k ) {
}

void am::mouselbdown( bool dblclick, int x, int y, unsigned int state ) {
}

void am::mouselbup( int x, int y, unsigned int state ) {
}

void am::mousemove( int x, int y, unsigned int state ){
}

void am::paint( paint_param_t& paintParam, RECT const& rect ) {
}

void am::clear() {
	base_t::clear();
}

