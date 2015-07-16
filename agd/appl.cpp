#include "./pch.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"
#include "./frame.hpp"
#include "./window.hpp"
#include "./appl.hpp"

appl::appl( logger_ptr l, pref_ptr p ) {
	atom::mount<appl2logger>( this, l );
	atom::mount<appl2pref>( this, p );
	atom::mount<appl2window>( this, window::create( l, p ) );
}

appl::~appl() {
}

bool appl::init() {
	return ( this->getWindow()->init() );
}

appl& appl::run() {
	this->getWindow()->run();
	return (*this);
}

void appl::clear() {
	this->getWindow()->clear();
}


