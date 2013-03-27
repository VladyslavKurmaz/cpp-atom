#include <windows.h>
#include <iostream>
#include <string>
#include <assert.h>

//#include <boost/smart_ptr.hpp>

//#include <atom/util/log.hpp>
//#include <atom/node/tldefs.hpp>
//#include <atom/node/node.hpp>
#include <atom/util/dbg.hpp>
//#include <atom/util/po.hpp>
//#include <atom/util/wwindow.hpp>
//#include <atom/util/waccel.hpp>
//#include <atom/util/ptr.hpp>
#include <boost/foreach.hpp>

#include "./cmds.hpp"
#include <atom/util/pipe.hpp>


void on_size( COORD const& sz ) {
	std::cout << "size:" << sz.X << "," << sz.Y << std::endl; 
}

void on_run( LPCTSTR cmd ) {
	std::cout << "run:" << cmd << std::endl; 
}

void on_kbrd( KEY_EVENT_RECORD const& kbrd ) {
	std::cout << "kbrd:" << std::endl; 
}

void on_exit() {
	PostQuitMessage( 0 ); 
}

int main( int argc, char *argv[] )
{
	ATOM_DBG_MARK_BEGIN( p1, -1 ); {
		std::cout << "consd stared" << std::endl;
		//
		atom::pipe pipe;
		if ( pipe.open( "PIPE TEST NAME" ) ) {
			bool cont = true;
			std::cout << "start listing" << std::endl;
			while ( cont ) {
				command c;
				if ( pipe.read( &c, sizeof( c )  ) ) {
					switch( c.type ) {
					case command::cmdSize:
						break;
					case command::cmdRun:
						break;
					case command::cmdRunas:
						break;
					case command::cmdKbrd:
						break;
					case command::cmdBreak:
						break;
					case command::cmdExit:
						std::cout << "exit" << std::endl;
						cont = false;
						break;
					}
				} else {
					cont = false;
				}
			}
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
	}
	ATOM_DBG_MARK_END( p1, p2, p1p2diff, true );
	return 0;

}


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
	ir[0].Event.KeyEvent.uChar.UnicodeChar	=	c;
	//ir[0].Event.KeyEvent.uChar.AsciiChar	=	MapVirtualKey( vk, MAPVK_VK_TO_CHAR );
	ir[0].Event.KeyEvent.dwControlKeyState	=
		( ( GetKeyState( VK_CAPITAL ) & 0x01 ) ? ( CAPSLOCK_ON ) : ( 0 ) ) |
		//ENHANCED_KEY
		( ( GetKeyState( VK_LMENU ) & 0x80 ) ? ( LEFT_ALT_PRESSED ) : ( 0 ) ) |
		( ( GetKeyState( VK_LCONTROL ) & 0x80 ) ? ( LEFT_CTRL_PRESSED ) : ( 0 ) ) |
		( ( GetKeyState( VK_NUMLOCK ) & 0x01 ) ? ( NUMLOCK_ON ) : ( 0 ) ) |
		( ( GetKeyState( VK_RMENU ) & 0x80 ) ? ( RIGHT_ALT_PRESSED ) : ( 0 ) ) |
		( ( GetKeyState( VK_RCONTROL ) & 0x80 ) ? ( RIGHT_CTRL_PRESSED ) : ( 0 ) ) |
		( ( GetKeyState( VK_SCROLL ) & 0x01 ) ? ( SCROLLLOCK_ON ) : ( 0 ) ) |
		( ( GetKeyState( VK_SHIFT ) & 0x80 ) ? ( SHIFT_PRESSED ) : ( 0 ) ) ;

	//
	ir[1] = ir[0];
	ir[1].Event.KeyEvent.bKeyDown			=	FALSE;

//	HKL WINAPI GetKeyboardLayout(
//  _In_  DWORD idThread
//);

	//
	DWORD wr = 0;
	WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), ir, sizeof( ir ) / sizeof( ir[0] ), &wr );
}

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

