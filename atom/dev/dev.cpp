#include "./pch.hpp"
#include "./logger.hpp"
#include "./appl.hpp"

int main( int ac, char *av[] ) {
	ATOM_DBG_MARK_BEGIN( p1, -1 )
		logger_ptr l = logger::create();
		l->add_std_cout();
		appl_ptr a = appl::create( l );
		if ( a->init( ac, av ) ) {
			a->run( std::cout, std::cin );
		}
		a->clear();
	ATOM_DBG_MARK_END( p1, p2, p1p2diff, true )
	return 0;
}