#include <windows.h>
#include <iostream>
#include <string>
#include <assert.h>

//#include <boost/smart_ptr.hpp>

//#include <atom/util/log.hpp>
//#include <atom/node/tldefs.hpp>
//#include <atom/node/node.hpp>
#include <atom/util/dbg.hpp>
#include <atom/util/po.hpp>
//#include <atom/util/wwindow.hpp>
//#include <atom/util/waccel.hpp>
//#include <atom/util/ptr.hpp>
#include <boost/foreach.hpp>

#include "./cmds.hpp"
#include <atom/util/pipe.hpp>


void write_vk2cons( WORD const vk ) {
	//SHORT vk1 = VkKeyScanEx( 'a', GetKeyboardLayout( GetCurrentThreadId() ) );
	//SHORT vk2 = VkKeyScanEx( 'A', GetKeyboardLayout( GetCurrentThreadId() ) );
	//
	INPUT_RECORD ir[2] = { 0 };
	//
	ir[0].EventType = KEY_EVENT;
	ir[0].Event.KeyEvent.bKeyDown			=	TRUE;
	ir[0].Event.KeyEvent.wRepeatCount		=	1;
	ir[0].Event.KeyEvent.wVirtualKeyCode	=	vk;
	ir[0].Event.KeyEvent.wVirtualScanCode	=	MapVirtualKey( vk, MAPVK_VK_TO_VSC );
	BYTE kbrd[256] = { 0 };
	WORD c = 0;
	GetKeyboardState( kbrd );
	ToAscii(
		ir[0].Event.KeyEvent.wVirtualKeyCode,
		ir[0].Event.KeyEvent.wVirtualScanCode,
		kbrd,
		&c,
		0
		);
	//ir[0].Event.KeyEvent.uChar.UnicodeChar	=	c;
	ir[0].Event.KeyEvent.uChar.AsciiChar	=	MapVirtualKey( vk, MAPVK_VK_TO_CHAR );
	//ir[0].Event.KeyEvent.dwControlKeyState	=
	//	( ( GetKeyState( VK_CAPITAL ) & 0x01 ) ? ( CAPSLOCK_ON ) : ( 0 ) ) |
	//	//ENHANCED_KEY
	//	( ( GetKeyState( VK_LMENU ) & 0x80 ) ? ( LEFT_ALT_PRESSED ) : ( 0 ) ) |
	//	( ( GetKeyState( VK_LCONTROL ) & 0x80 ) ? ( LEFT_CTRL_PRESSED ) : ( 0 ) ) |
	//	( ( GetKeyState( VK_NUMLOCK ) & 0x01 ) ? ( NUMLOCK_ON ) : ( 0 ) ) |
	//	( ( GetKeyState( VK_RMENU ) & 0x80 ) ? ( RIGHT_ALT_PRESSED ) : ( 0 ) ) |
	//	( ( GetKeyState( VK_RCONTROL ) & 0x80 ) ? ( RIGHT_CTRL_PRESSED ) : ( 0 ) ) |
	//	( ( GetKeyState( VK_SCROLL ) & 0x01 ) ? ( SCROLLLOCK_ON ) : ( 0 ) ) |
	//	( ( GetKeyState( VK_SHIFT ) & 0x80 ) ? ( SHIFT_PRESSED ) : ( 0 ) ) ;

	//
	ir[1] = ir[0];
	ir[1].Event.KeyEvent.bKeyDown			=	FALSE;

	//	HKL WINAPI GetKeyboardLayout(
	//  _In_  DWORD idThread
	//);

	//
	DWORD wr = 0;
	WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), ir, 2/*sizeof( ir ) / sizeof( ir[0] )*/, &wr );
}

void wait_please() {
	std::string s;
	std::getline( std::cin, s );
}

int main( int argc, char *argv[] )
{
	//std::cout << "consd stared" << std::endl;
	//for ( int i = 0; i < argc; ++i ) {
	//	std::cout << "[" << i << "]:" << argv[i] << std::endl;
	//}
	ATOM_DBG_MARK_BEGIN( p1, -1 ); {
		//atom::po po;
		//atom::po::options_description_t& desc = po.add_desc( 0, "program options" );
		////
		//po.
		//add_option( 1, "pipe", boost::program_options::value<std::string>(), "pipe name", desc );
		//try {
		//	po.parse_arg( argc, argv, desc, true );
		//	//
		//	if ( !po.count( 1 ) )
		//		throw std::exception( "[ERROR] Pipe's name wasn't defined" );
		//	//
		//} catch( std::exception& excpt ) {
		//	std::stringstream ss;
		//	desc.print( ss );
		//	std::cout << excpt.what() << std::endl;
		//	std::cout << ss.str() << std::endl;
		//	wait_please();
		//	return -1;
		//}
		//
		atom::pipe pipe;
		if ( pipe.open( argv[1]/*po.as<std::string>( 1 )*/ ) ) {
			//
			PROCESS_INFORMATION pi;
			STARTUPINFO si;
			//
			ZeroMemory( &si, sizeof( si ) );
			si.cb				= sizeof(STARTUPINFO);
			//si.dwFlags			= STARTF_USESHOWWINDOW;
			//si.wShowWindow		= SW_SHOW;
			//
			bool cont = true;
			while ( cont ) {
				command c;
				if ( pipe.read( &c, sizeof( c )  ) ) {
					switch( c.type ) {
					case command::cmdSize:
						break;
					case command::cmdRun:
						{
							if ( CreateProcess( NULL, c.process.cmd_line, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi ) ) {
								//Sleep( 1000 );
								//write_vk2cons( 'A' );
								//write_vk2cons( 'B' );
								//write_vk2cons( 'C' );
								//write_vk2cons( 'D' );
								//write_vk2cons( 'E' );
								//write_vk2cons( 'F' );
								//write_vk2cons( 'G' );
								//write_vk2cons( 'H' );
								//write_vk2cons( 'I' );
								//write_vk2cons( 'J' );
								//write_vk2cons( 'K' );
								//write_vk2cons( 'L' );
								//write_vk2cons( 'M' );
								//write_vk2cons( 'N' );
								//write_vk2cons( 'O' );
								//write_vk2cons( 'P' );
								//write_vk2cons( 'Q' );
								//write_vk2cons( 'R' );
								//write_vk2cons( 'S' );
								//write_vk2cons( 'T' );
								//write_vk2cons( 'U' );
								//write_vk2cons( 'V' );
								//write_vk2cons( 'W' );
								//write_vk2cons( 'X' );
								//write_vk2cons( 'Y' );
								//write_vk2cons( 'Z' );
								//write_vk2cons( VK_RETURN );
							} else {
							}
							break;
						}
					case command::cmdRunas:
						break;
					case command::cmdKbrd:
						{
							INPUT_RECORD ir;
							ir.EventType = KEY_EVENT;
							ir.Event.KeyEvent = c.key;
							DWORD wr = 0;
							//WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), &ir, sizeof( ir ), &wr );
							break;
						}
					case command::cmdBreak:
						GenerateConsoleCtrlEvent( CTRL_BREAK_EVENT, GetCurrentProcessId() );
						break;
					case command::cmdExit:
						write_vk2cons( VK_RETURN );
						write_vk2cons( 'E' );
						write_vk2cons( 'X' );
						write_vk2cons( 'I' );
						write_vk2cons( 'T' );
						write_vk2cons( VK_RETURN );
						WaitForSingleObject( pi.hProcess, INFINITE );
						cont = false;
						break;
					}
				} else {
					cont = false;
				}
			}
			// close all child processes
		} else {
			std::cout << "[ERROR] Pipe creation error" << std::endl;
		}
	}
	ATOM_DBG_MARK_END( p1, p2, p1p2diff, true );
	return 0;

}


		//CONSOLE_SCREEN_BUFFER_INFOEX csbiex = { 0 };
		//csbiex.cbSize = sizeof( csbiex );

		//GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
		//csbiex.dwSize.X = 100;
		//csbiex.dwSize.Y = 40;
		//csbiex.dwMaximumWindowSize.X = 100;
		//csbiex.dwMaximumWindowSize.Y = 40;
		//csbiex.srWindow.Right = 97;
		//csbiex.srWindow.Bottom = 33;


		//if ( SetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex ) ) {
		//	GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
		//	SMALL_RECT crt;
		//	crt.Left = 0;
		//	crt.Top = 0;
		//	crt.Right	= 99;
		//	crt.Bottom	= 39;
		//	SetConsoleWindowInfo( GetStdHandle( STD_OUTPUT_HANDLE ), FALSE, &crt );
		//	GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
		//}

		//CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
		//GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );
		//csbi.dwSize.X = 100;
		//csbi.dwSize.Y = 40;
		//SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), csbi.dwSize );
		//GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );

		//SMALL_RECT crt;
		//crt.Left	= csbi.srWindow.Left;
		//crt.Top		= csbi.srWindow.Top;
		//crt.Right	= csbi.dwSize.X - 1;
		//crt.Bottom	= csbi.dwSize.Y - 1;
		//SetConsoleWindowInfo( GetStdHandle( STD_OUTPUT_HANDLE ), FALSE, &crt );
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

