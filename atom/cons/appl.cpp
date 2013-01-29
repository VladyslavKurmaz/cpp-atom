#include "./pch.hpp"
#include "./appl.hpp"

appl::appl( std::ostream& l, pref& p ) :
		log( l )
	,	prefs( p )
	,	wnd( l, p ) {
}

appl::~appl() {
}

bool appl::init() {
	return ( this->wnd.init() );
}

void appl::run() {
	this->wnd.show( true ).run();
}


