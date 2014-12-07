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

void bridge::run( on_exit_t oe, on_parse_t op ) {
	this->on_exit = oe;
	this->on_parse = op;
	this->guard_thread = boost::thread( boost::bind( &bridge::guard, this ) );
}

void bridge::run( on_exit_t oe, on_parse_t op, std::string const& mn, std::string const& wpn, std::string const& rpn ) {
	this->server = false;
	this->mutex_name = mn;
	this->wpipe_name = wpn;
	this->rpipe_name = rpn;
	this->run( oe, op );
}

void bridge::join() {
	this->on_exit.clear();
	this->guard_thread.join();
}

void bridge::write( bridge_msg::type const id, void const* param ){
	bridge_msg m;
	m.id = id;
	//
	if ( id == bridge_msg::bmSize ) {
	} else if ( id == bridge_msg::bmConf ) {
		strcpy_s( m.text, static_cast< TCHAR const* >( param ) );
	} else if ( id == bridge_msg::bmKbrd ) {
		m.key = *( static_cast< KEY_EVENT_RECORD const* >( param ) );
	} else if ( id == bridge_msg::bmCtrlBreak ) {
	} else if ( id == bridge_msg::bmCtrlC ) {
	} else if ( id == bridge_msg::bmText ) {
		strcpy_s( m.text, static_cast< TCHAR const* >( param ) );
	} else if ( id == bridge_msg::bmExit ) {
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
				struct _{ static void _1() { } static void _2( TCHAR const* ) {} };
				bridge e;
				e.run( boost::bind( _::_1 ), boost::bind( _::_2, _1 ), this->mutex_name, this->rpipe_name, this->wpipe_name );
#else
				// start child console process
				TCHAR path[MAX_PATH] = { 0 };
				TCHAR drive[MAX_PATH] = { 0 };
				TCHAR dir[MAX_PATH] = { 0 };
				TCHAR filename[MAX_PATH] = { 0 };
				TCHAR ext[MAX_PATH] = { 0 };
				GetModuleFileName( NULL, path, MAX_PATH );
				_tsplitpath_s( path, drive, dir, filename, ext );
				_tmakepath_s( path, drive, dir, _T("consd"), ext );
				std::stringstream ss;
				ss << "\"" << path << "\" -m " << this->mutex_name << " -w " << this->rpipe_name << " -r " << this->wpipe_name;
				//
				atom::proc<TCHAR> p;
				//???????? w & h
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
				this->tty();
			}
			//
			this->wpipe.close();
			this->rpipe.close();
			read_thread.join();

			//if ( this->server ) {
			//	OutputDebugString( "Server guard thread exit\n" );
			//} else {
			//	OutputDebugString( "Client guard thread exit\n" );
			//}

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
				if ( bm.id == bridge_msg::bmSize ) {
				} else if ( bm.id == bridge_msg::bmConf ) {
					this->on_parse( bm.text );
				} else if ( bm.id == bridge_msg::bmKbrd ) {
					INPUT_RECORD ir;
					ir.EventType = KEY_EVENT;
					ir.Event.KeyEvent = bm.key;
					DWORD wr = 0;
					WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), &ir, sizeof( ir ), &wr );
				} else if ( bm.id == bridge_msg::bmCtrlBreak ) {
					GenerateConsoleCtrlEvent( CTRL_BREAK_EVENT, 0 );
				} else if ( bm.id == bridge_msg::bmCtrlC ) {
					GenerateConsoleCtrlEvent( CTRL_C_EVENT, 0 );
				} else if ( bm.id == bridge_msg::bmText ) {
					std::cout << std::endl << bm.text << std::endl;
				} else if ( bm.id == bridge_msg::bmExit ) {
					exit2cons();
				}
			}
		}
	}
	//if ( this->server ) {
	//	OutputDebugString( "Server read thread exit\n" );
	//} else {
	//	OutputDebugString( "Client read thread exit\n" );
	//}
}

#include <strsafe.h>
void ErrorExit(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}


// split expand rotate next prev tt1 - tt6
void bridge::tty() {

	atom::tty< TCHAR > tty1( std::cin, std::cout );
	struct _ {
		//
		static void help( atom::tty< TCHAR >& t, atom::tty< TCHAR >::string_t const& param ) {
		}
		//
		static void conf( atom::tty< TCHAR >& t, atom::tty< TCHAR >::string_t const& param, bridge& b ) {
			b.write( bridge_msg::bmConf, param.c_str() );
		}
		//
		static void exec( atom::tty< TCHAR >& t, atom::tty< TCHAR >::string_t const& param ) {
			atom::exec( param, atom::tty< TCHAR >::string_t() );
		}
		//
		static void exit( atom::tty< TCHAR >& t, atom::tty< TCHAR >::string_t const& param ) {
			t.exit();
		}
	};
	tty1.add_cmd( "help", _::help );
	tty1.add_cmd( "conf", boost::bind( _::conf, _1, _2, boost::ref( *this ) ) );
	tty1.add_cmd( "exec", boost::bind( _::exec, _1, _2 ) );
	tty1.add_cmd( "exit", _::exit );
	tty1.run( "cons v0.9.0 [/github/cpp-atom/cons]", "cons : " );
}


