#include "./pch.hpp"
#include "./bridge.hpp"


bridge::bridge() :
		proc()
#ifdef STANDALONE
	,	pipeName( TEST_PIPE_NAME )
#else
	,	pipeName( gen_uuid() )
#endif
	,	pipe() {
}

bridge::bridge( std::string const& name ) :
		proc()
	,	pipeName( name )
	,	pipe() {
}

bridge::~bridge() {
}

void bridge::join() {
	this->pipe.close();
	this->proc.join();
}

void bridge::console() {
	//
	this->pipe.create( this->pipeName );
#ifndef STANDALONE
	// start child console process
	TCHAR path[MAX_PATH] = { 0 };
	TCHAR drive[MAX_PATH] = { 0 };
	TCHAR dir[MAX_PATH] = { 0 };
	TCHAR filename[MAX_PATH] = { 0 };
	TCHAR ext[MAX_PATH] = { 0 };
	GetModuleFileName( NULL, path, MAX_PATH );
	_tsplitpath_s( path, drive, dir, filename, ext );
	_tmakepath_s( path, drive, dir, _T("proxy"), ext );
	std::stringstream ss;
	ss << "\"" << path << "\" --pipe-name " << this->pipeName;
	this->proc.run( ss.str(), 120, 25, true, true );
#endif
	this->pipe.connect();
}

void vk2cons( WORD const vk ) {
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
	//GetKeyboardState( kbrd );
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
		( ( kbrd[ VK_CAPITAL ] & 0x01 ) ? ( CAPSLOCK_ON ) : ( 0 ) ) |
		//ENHANCED_KEY
		( ( kbrd[ VK_LMENU ] & 0x80 ) ? ( LEFT_ALT_PRESSED ) : ( 0 ) ) |
		( ( kbrd[ VK_LCONTROL ] & 0x80 ) ? ( LEFT_CTRL_PRESSED ) : ( 0 ) ) |
		( ( kbrd[ VK_NUMLOCK ] & 0x01 ) ? ( NUMLOCK_ON ) : ( 0 ) ) |
		( ( kbrd[ VK_RMENU ] & 0x80 ) ? ( RIGHT_ALT_PRESSED ) : ( 0 ) ) |
		( ( kbrd[ VK_RCONTROL ] & 0x80 ) ? ( RIGHT_CTRL_PRESSED ) : ( 0 ) ) |
		( ( kbrd[ VK_SCROLL ] & 0x01 ) ? ( SCROLLLOCK_ON ) : ( 0 ) ) |
		( ( kbrd[ VK_SHIFT ] & 0x80 ) ? ( SHIFT_PRESSED ) : ( 0 ) ) ;

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
void exit2cons() {
	vk2cons( '\n' );
	vk2cons( 'E' );
	vk2cons( 'X' );
	vk2cons( 'I' );
	vk2cons( 'T' );
	vk2cons( '\n' );
}

void bridge::proxy() {
	this->proc.run( "cmd.exe", 0, 0, false, true );
	// read loop from pipe
	if ( this->pipe.open( this->pipeName ) ) {
		bool cont = true;
		bridgeMsg m;
		INPUT_RECORD ir;
		DWORD wr = 0;
		while ( cont ) {
			memset( &m, 0, sizeof( m ) );
			if ( cont = this->pipe.read( &m, sizeof( m )  ) ) {
				switch ( m.id ) {
				case bridgeMsg::bmSize:
					break;
				case bridgeMsg::bmKbrd:
					ir.EventType = KEY_EVENT;
					ir.Event.KeyEvent = m.key;
					WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), &ir, 1, &wr );
					break;
				case bridgeMsg::bmCtrlBreak:
					//GenerateConsoleCtrlEvent( CTRL_BREAK_EVENT, 0 );
					break;
				case bridgeMsg::bmCtrlC:
					//GenerateConsoleCtrlEvent( CTRL_C_EVENT, 0 );
					break;
				case bridgeMsg::bmExit:
					//exit2cons();
					break;
				}
			}
		}
	}
}

void bridge::write( KEY_EVENT_RECORD const& k ) {
	bridgeMsg m;
	m.id = bridgeMsg::bmKbrd;
	m.key = k;
	this->pipe.write( &m, sizeof( m ) );
}


