#include <windows.h>

#include <iostream>

//#include <boost/smart_ptr.hpp>

//#include <atom/util/log.hpp>
//#include <atom/node/tldefs.hpp>
//#include <atom/node/node.hpp>
#include <atom/util/dbg.hpp>
#include <atom/util/po.hpp>
//#include <atom/util/wwindow.hpp>
//#include <atom/util/waccel.hpp>
//#include <atom/util/ptr.hpp>



int main( int argc, char *argv[] )
{
	ATOM_DBG_MARK_BEGIN( p1, -1 ); {
		CONSOLE_SCREEN_BUFFER_INFOEX csbiex = { 0 };
		csbiex.cbSize = sizeof( csbiex );
		GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
		CONSOLE_SCREEN_BUFFER_INFO bi;
		GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &bi );
		std::string s;
		do {
			std::cout << "c:\\work : git ";
			std::getline( std::cin, s);
			std::cout << s << std::endl;
		} while( s.size() );
	}
	ATOM_DBG_MARK_END( p1, p2, p1p2diff, true );
	return 0;

}
