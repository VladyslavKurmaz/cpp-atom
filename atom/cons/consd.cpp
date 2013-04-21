#include "./pch.hpp"
#include "./consd_tty.hpp"
#include "./cmds.hpp"
#include "./cons_mpp.hpp"

void wait_please() {
	std::string s;
	std::getline( std::cin, s );
}

BOOL WINAPI HandlerRoutine ( DWORD dwCtrlType ) {
	switch ( dwCtrlType ){
	case CTRL_C_EVENT:
		{
			return TRUE;
		}
	case CTRL_BREAK_EVENT:
		{
			return TRUE;
		}
	case CTRL_CLOSE_EVENT:
		{
			return TRUE;
		}
	case CTRL_LOGOFF_EVENT:
		{
			return TRUE;
		}
	case CTRL_SHUTDOWN_EVENT:
		{
			return TRUE;
		}
	}
	return FALSE;
}



int main( int argc, char *argv[] )
{
	SetConsoleCtrlHandler( HandlerRoutine, TRUE );
	//std::cout << "consd stared" << std::endl;
	//for ( int i = 0; i < argc; ++i ) {
	//	std::cout << "[" << i << "]:" << argv[i] << std::endl;
	//}
	ATOM_DBG_MARK_BEGIN( p1, -1 ); {
		atom::po po;
		atom::po::options_description_t& desc = po.add_desc( 0, "program options" );
		std::string pipe_name;
		std::string mutex_name;
		std::string header_name;
		unsigned int csb_width = 0;
		unsigned int csb_height = 0;
		//
		po.add_option( 1, "pipe,p", boost::program_options::value<std::string>( &pipe_name ), "pipe name", desc );
		po.add_option( 2, "mutex,m", boost::program_options::value<std::string>( &mutex_name ), "mutex name", desc );
		po.add_option( 3, "header,h", boost::program_options::value<std::string>( &header_name ), "header name", desc );
		try {
			po.parse_arg( argc, argv, desc, true );
			//
			if ( !( po.count( 1 ) && po.count( 2 ) && po.count( 3 ) ) ) {
				throw std::exception( "[ERROR] Pipe's name and/or Shered memory's name wasn't defined" );
			}
			//
		} catch( std::exception& excpt ) {
			std::stringstream ss;
			desc.print( ss );
			std::cout << excpt.what() << std::endl;
			std::cout << ss.str() << std::endl;
			wait_please();
			return -1;
		}
		cons_mpp cmpp;
		cmpp.client_run( po.as< std::string >( 1 ), po.as< std::string >( 2 ), po.as< std::string >( 3 ) );
		cmpp.close();
	}
	ATOM_DBG_MARK_END( p1, p2, p1p2diff, true );
	return 0;

}


bool HandleSize( command const& c ) {
	//CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
	//GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );
	//csbi.dwSize = c.size;
	//csbi.dwCursorPosition;
	//csbi.wAttributes;
	//csbi.srWindow;
	//csbi.dwMaximumWindowSize;

	//GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );

#if 0
	CONSOLE_SCREEN_BUFFER_INFOEX csbiex = { 0 };
	csbiex.cbSize = sizeof( csbiex );

	GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
	csbiex.dwSize.X = 100;
	csbiex.dwSize.Y = 40;
	csbiex.dwMaximumWindowSize.X = 100;
	csbiex.dwMaximumWindowSize.Y = 40;
	csbiex.srWindow.Right = 97;
	csbiex.srWindow.Bottom = 33;


	if ( SetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex ) ) {
		GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
		SMALL_RECT crt;
		crt.Left = 0;
		crt.Top = 0;
		crt.Right	= 99;
		crt.Bottom	= 39;
		SetConsoleWindowInfo( GetStdHandle( STD_OUTPUT_HANDLE ), FALSE, &crt );
		GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
	}

	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );

	SMALL_RECT crt;
	crt.Left	= csbi.srWindow.Left;
	crt.Top		= csbi.srWindow.Top;
	crt.Right	= csbi.dwSize.X - 1;
	crt.Bottom	= csbi.dwSize.Y - 1;
	SetConsoleWindowInfo( GetStdHandle( STD_OUTPUT_HANDLE ), FALSE, &crt );
#endif
	return true;
}

				//INPUT_RECORD ir;
				//ir.EventType = KEY_EVENT;
				//ir.Event.KeyEvent = c.key;
				//DWORD wr = 0;
				//WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), &ir, sizeof( ir ), &wr );

#if 0

DWORD WINAPI PipeGuard( LPVOID lpParameter ) {
	static bool ( *router[] ) ( command const& ) = {
		NULL,
		HandleSize,
		HandleKbrd,
		HandleCtrlBreak,
		HandleCtrlC,
		HandleExit
	};
	atom::pipe* pipe = reinterpret_cast<atom::pipe*>( lpParameter );
	bool cont = true;
	while ( cont ) {
		command c;
		if ( cont = pipe->read( &c, sizeof( c ) ) ) {
			cont = router[c.type]( c );
		}
	}
	//close all child processes
	return 0;
}


#else

DWORD WINAPI PipeGuard( LPVOID lpParameter ) {
	atom::pipe* pipe = reinterpret_cast<atom::pipe*>( lpParameter );
	bool cont = true;
	while ( cont ) {
		command c;
		memset( &c, 0, sizeof( c ) );
		if ( cont = pipe->read( &c, sizeof( c )  ) ) {
			//switch( c.type ) {
			//case command::cmdSize:
			//	HandleSize( c );
			//	break;
			//case command::cmdKbrd:
			//	HandleKbrd( c );
			//	break;
			//case command::cmdCtrlBreak:
			//	HandleCtrlBreak( c );
			//	break;
			//case command::cmdCtrlC:
			//	GenerateConsoleCtrlEvent( CTRL_C_EVENT, 0 );
			//	break;
			//case command::cmdExit:
			//	cont = HandleExit( c );
			//	break;
			//}
		}
	}
	//close all child processes
	return 0;
}
#endif



		//
		//std::string s;
		//std::getline( std::cin, s);
		//
		//PROCESS_INFORMATION pi;
		//STARTUPINFO si;
		////
		//ZeroMemory( &si, sizeof( si ) );
		//si.cb				= sizeof(STARTUPINFO);
		//si.dwFlags			= STARTF_USESHOWWINDOW;
		//si.wShowWindow		= SW_SHOW;
		////
		//TCHAR command[MAX_PATH] = 
		//	//"C:\\Program Files\\Far2\\Far.exe"
		//	//"c:\\work\\env\\cygwin\\cygwin.bat"
		//	//"powershell.exe"
		//	"cmd.exe"
		//	;
		//if ( CreateProcess( NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi ) ) {
		//	//write2cons( "apa" );
		//	//write2cons( "ping 127.0.0.1" );
		//	//write2cons( "\n\r" );
		//	//write_vk2cons( 'D' );
		//	//write_vk2cons( 'I' );
		//	//write_vk2cons( 'R' );
		//	//write_vk2cons( VK_RETURN );
		//	WaitForSingleObject( pi.hProcess, INFINITE );
		//}


void write2cons( std::string const& s ) {
	size_t const bsz = sizeof( INPUT_RECORD ) * ( 2 * s.length() );
	INPUT_RECORD* ir = NULL;
	INPUT_RECORD* iri = ir = reinterpret_cast<INPUT_RECORD*>( _alloca( bsz ) );
	memset( ir, 0, bsz );
	BOOST_FOREACH( char const c, s ) {
		iri->EventType = KEY_EVENT;
		iri->Event.KeyEvent.bKeyDown			=	TRUE;
		iri->Event.KeyEvent.wRepeatCount		=	1;
		iri->Event.KeyEvent.wVirtualKeyCode		=	c;
		iri->Event.KeyEvent.wVirtualScanCode	=	c;
		//iri->Event.KeyEvent.uChar.UnicodeChar	=	L'a';
		iri->Event.KeyEvent.uChar.AsciiChar		=	c;
		iri->Event.KeyEvent.dwControlKeyState	=	0;
		iri++;
		iri->EventType = KEY_EVENT;
		iri->Event.KeyEvent.bKeyDown			=	FALSE;
		iri->Event.KeyEvent.wRepeatCount		=	1;
		iri->Event.KeyEvent.wVirtualKeyCode		=	c;
		iri->Event.KeyEvent.wVirtualScanCode	=	c;
		//iri->Event.KeyEvent.uChar.UnicodeChar	=	L'a';
		iri->Event.KeyEvent.uChar.AsciiChar		=	c;
		iri->Event.KeyEvent.dwControlKeyState	=	0;
		iri++;
	}
	//
	DWORD wr = 0;
	WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), ir, 2 * s.length(), &wr );
}


//CONSOLE_SCREEN_BUFFER_INFOEX csbiex = { 0 };
//csbiex.cbSize = sizeof( csbiex );
//GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
//CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
//GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );
//csbi.dwSize.X = 100;
//csbi.dwSize.Y = 1000;
//SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), csbi.dwSize );
////
//UINT in_cp = GetConsoleCP();
//UINT out_cp = GetConsoleOutputCP();
//std::string s;
//do {
//	std::cout << "c:\\work : git ";
//	std::getline( std::cin, s);
//	std::cout << s << std::endl;
//} while( s.size() );
//

