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

void bridge::run( on_exit_t oe ) {
	this->on_exit = oe;
	this->guard_thread = boost::thread( boost::bind( &bridge::guard, this ) );
}

void bridge::run( on_exit_t oe, string_t const& mn, string_t const& wpn, string_t const& rpn ) {
	this->server = false;
	this->mutex_name = mn;
	this->wpipe_name = wpn;
	this->rpipe_name = rpn;
	this->run( oe );
}

void bridge::join() {
	this->on_exit.clear();
	this->guard_thread.join();
}

void bridge::write( bridge_msg::type const id, void const* param ){
	bridge_msg m;
	m.id = id;
	//
	if ( id  == bridge_msg::bmKbrd ) {
		m.key = *( static_cast< KEY_EVENT_RECORD const* >( param ) );
	}
	//
	this->wpipe.write( &m, sizeof( m ) );
}

void bridge::guard(){
	if ( this->mutex.create( this->mutex_name, false ) ) {
		//
		if ( this->wpipe.create( this->wpipe_name ) ) {
			struct ntf {
				bridge* b;
				ntf() : b( NULL ){}
				~ntf(){ if ( b ) b->on_exit(); }
			} n;
			//
			boost::thread read_thread;
			//
			if ( this->server ){
				n.b = this;
#ifdef STANDALONE
				struct _{
					static void __() {
					}
				};
				bridge e;
				e.run( boost::bind( _::__ ), this->mutex_name, this->rpipe_name, this->wpipe_name );
#else
				// start child console process
				atom::proc<TCHAR> p;
				//
				TCHAR path[MAX_PATH] = { 0 };
				TCHAR drive[MAX_PATH] = { 0 };
				TCHAR dir[MAX_PATH] = { 0 };
				TCHAR filename[MAX_PATH] = { 0 };
				TCHAR ext[MAX_PATH] = { 0 };
				GetModuleFileName( NULL, path, MAX_PATH );
				_tsplitpath_s( path, drive, dir, filename, ext );
				_tmakepath_s( path, drive, dir, _T("consd"), ext );
				std::stringstream ss;
				ss 
					<< path
					<< " -m " << this->mutex_name
					<< " -w " << this->rpipe_name
					<< " -r " << this->wpipe_name
					;
				//
				p.run( ss.str(), 80, 25, true );
#endif
				this->wpipe.connect();
				read_thread = boost::thread( boost::bind( &bridge::read, this ) );
#ifdef STANDALONE
				e.join();
#else
				// wait for process
				p.join();
#endif
			} else {
				//
				// start read
				read_thread = boost::thread( boost::bind( &bridge::read, this ) );
				this->wpipe.connect();
				//
				atom::tty< TCHAR > tty1( std::cin, std::cout );
				// add console command: help, conf, exit
				struct _ {
					static void exit( atom::tty< TCHAR >& t, atom::tty< TCHAR >::string_t const& param ) {
						t.exit();
					}
				};
				tty1.add( "exit", _::exit );
				tty1.run();
			}
			//
			this->wpipe.close();
			this->rpipe.close();
			read_thread.join();
		}
	}
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
	if ( this->rpipe.open( this->rpipe_name ) ) {
		bool cont = true;
		while ( cont ) {
			bridge_msg bm;
			memset( &bm, 0, sizeof( bm ) );
			if ( cont = this->rpipe.read( &bm, sizeof( bm )  ) ) {
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
}


