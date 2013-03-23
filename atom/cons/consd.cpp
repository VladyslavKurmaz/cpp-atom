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
		//CONSOLE_SCREEN_BUFFER_INFOEX csbiex = { 0 };
		//csbiex.cbSize = sizeof( csbiex );
		//GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
		CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
		GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );
		csbi.dwSize.X = 100;
		csbi.dwSize.Y = 1000;
		SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), csbi.dwSize );
		//
		UINT in_cp = GetConsoleCP();
		UINT out_cp = GetConsoleOutputCP();
		//std::string s;
		//do {
		//	std::cout << "c:\\work : git ";
		//	std::getline( std::cin, s);
		//	std::cout << s << std::endl;
		//} while( s.size() );
		//
		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		//
		ZeroMemory( &si, sizeof( si ) );
		si.cb				= sizeof(STARTUPINFO);
		si.dwFlags			= STARTF_USESHOWWINDOW;
		si.wShowWindow		= SW_SHOW;
		//
		TCHAR command[MAX_PATH] = 
			//"powershell.exe"
			"cmd.exe"
			;
		if ( CreateProcess( NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi ) ) {
			Sleep( 5000 );
			char ping[] = "ping 127.0.0.1\x0D\x0A";
			DWORD wr = 0;
			WriteConsole( GetStdHandle( STD_OUTPUT_HANDLE ), ping, strlen( ping ), &wr, NULL );
			//std::cout << ping;
			WaitForSingleObject( pi.hProcess, INFINITE );
		}

	}
	ATOM_DBG_MARK_END( p1, p2, p1p2diff, true );
	return 0;

}
