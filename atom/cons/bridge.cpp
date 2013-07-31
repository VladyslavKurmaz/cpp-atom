#include "./pch.hpp"
#include "./bridge.hpp"

BOOL WINAPI bridge::handler( DWORD dwCtrlType ) {
	switch ( dwCtrlType ){
	case CTRL_C_EVENT:{
			return TRUE;
		}
	case CTRL_BREAK_EVENT:{
			return TRUE;
		}
	case CTRL_CLOSE_EVENT:{
			return TRUE;
		}
	case CTRL_LOGOFF_EVENT:{
			return TRUE;
		}
	case CTRL_SHUTDOWN_EVENT:{
			return TRUE;
		}
	}
	return FALSE;
}

bridge& bridge::init(){
	SetConsoleCtrlHandler( handler, TRUE );
	//
	if ( this->mutex.create( this->mutex_name, false ) ) {
		// configure pipe
		if ( server ) {
			if ( !this->pipe.create( this->pipe_name ) ) {
				throw std::exception( "Pipe creation failed" );
			}
		} else {
			if ( !this->pipe.open( this->pipe_name ) ) {
				throw std::exception( "Pipe opening failed" );
			}
		}
		// 
		if ( server ) {
			this->guard_thread = boost::thread( boost::bind( &bridge::guard, this ) );
		} else {
		}
	} else {
		throw std::exception( "Mutex initialisation failed" );
	}
	return ( *this );
}

bridge& bridge::bind(){
	if ( server ) {
		this->pipe.connect();
	} else {
		// collect console info
		// build shmem and send shmem notification to the server
	}
	// start read thread
	this->read_thread = boost::thread( boost::bind( &bridge::read, this ) );
	return ( *this );
}

bridge& bridge::run(){
	if ( server ) {
	} else {
		//
		atom::tty< TCHAR > tty1( std::cin, std::cout );
		// add console command: help, conf, exit 
		tty1.run();
	}
	return ( *this );
}

void bridge::write_to_pipe( bridge_msg::type const id, void const* param ) {
	bridge_msg m;
	m.id = id;
	//
	if ( id  == bridge_msg::bmKbrd ) {
		m.key = *( static_cast< KEY_EVENT_RECORD const* >( param ) );
	}
	//
	this->pipe.write( &m, sizeof( m ) );
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
	vk2cons( VK_RETURN );
	vk2cons( 'E' );
	vk2cons( 'X' );
	vk2cons( 'I' );
	vk2cons( 'T' );
	vk2cons( VK_RETURN );
}

void bridge::read() {
	bool cont = true;
	while ( cont ) {
		bridge_msg bm;
		memset( &bm, 0, sizeof( bm ) );
		if ( cont = this->pipe.read( &bm, sizeof( bm )  ) ) {
			switch( bm.id ) {
			case bridge_msg::bmNone:
				{
					break;
				}
			case bridge_msg::bmSize:
				{
					break;
				}
			case bridge_msg::bmKbrd:
				{
					INPUT_RECORD ir;
					ir.EventType = KEY_EVENT;
					ir.Event.KeyEvent = bm.key;
					DWORD wr = 0;
					WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), &ir, sizeof( ir ), &wr );
					break;
				}
			case bridge_msg::bmCtrlBreak:
				{
					break;
				}
			case bridge_msg::bmCtrlC:
				{
					break;
				}
			case bridge_msg::bmExit:
				{
					exit2cons();
					break;
				}
			case bridge_msg::bmTerminate:
				{
					break;
				}
			}
		}
	}
}

void bridge::guard(){
#ifdef STANDALONE
	try {
		bridge e( this->mutex_name, this->pipe_name, this->shmem_name );
		e.init().bind().run();
	}
	catch( std::exception& ){
	}
#else
	// start child console process
	// wait for process
#endif
	// send end notofication
}

void bridge::close(){
	write_to_pipe( bridge_msg::bmExit, NULL );
}

