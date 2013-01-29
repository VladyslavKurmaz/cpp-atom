#include "./pch.hpp"
#include <atom/util/log.hpp>

int main( int argc, char *argv[] )
{
	atom::log l;
	l.add_std_cout() << "hi there" << std::endl;
	return 0;
}
