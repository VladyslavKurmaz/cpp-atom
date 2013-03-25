#include <windows.h>
#include <iostream>
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

int main( int argc, char *argv[] )
{
	ATOM_DBG_MARK_BEGIN( p1, -1 ); {
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
		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		//
		ZeroMemory( &si, sizeof( si ) );
		si.cb				= sizeof(STARTUPINFO);
		si.dwFlags			= STARTF_USESHOWWINDOW/* | STARTF_USESTDHANDLES;
		si.hStdInput		= GetStdHandle( STD_INPUT_HANDLE );
		si.hStdOutput		= GetStdHandle( STD_OUTPUT_HANDLE );
		si.hStdError		= GetStdHandle( STD_ERROR_HANDLE )*/;
		si.wShowWindow		= SW_SHOW;
		//
		TCHAR command[MAX_PATH] = 
			//"c:\\work\\env\\cygwin\\cygwin.bat"
			//"powershell.exe"
			"cmd.exe"
			;
		if ( CreateProcess( NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi ) ) {
			//write2cons( "apa" );
			//write2cons( "ping 127.0.0.1" );
			//write2cons( "\n\r" );
			write_vk2cons( 'D' );
			write_vk2cons( 'I' );
			write_vk2cons( 'R' );
			write_vk2cons( VK_RETURN );
			//DWORD wr = 0;
			////WriteConsole( GetStdHandle( STD_INPUT_HANDLE ), ping, strlen( ping ), &wr, NULL );
			//INPUT_RECORD ic = { 0 };
			//ic.EventType = KEY_EVENT;
			//ic.Event.KeyEvent.bKeyDown			=	TRUE;
			//ic.Event.KeyEvent.wRepeatCount		=	1;
			//ic.Event.KeyEvent.wVirtualKeyCode	=	'a';
			//ic.Event.KeyEvent.wVirtualScanCode	=	'a';
			//ic.Event.KeyEvent.uChar.UnicodeChar	=	L'a';
			//ic.Event.KeyEvent.uChar.AsciiChar	=	'a';
			//ic.Event.KeyEvent.dwControlKeyState	=	0;

			//if ( !WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), &ic, 1, &wr ) ) {
			//	Sleep( 0 );
			//}
			WaitForSingleObject( pi.hProcess, INFINITE );
		}
	}
	ATOM_DBG_MARK_END( p1, p2, p1p2diff, true );
	return 0;

}
