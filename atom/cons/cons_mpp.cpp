#include "./pch.hpp"
#include "./consd_tty.hpp"
#include "./cmds.hpp"
#include "./cons_mpp.hpp"

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

cons_mpp::cons_mpp() :
		pipe_name()
	,	pipe()
	,	mutex_name()
	,	mutex()
	,	header_name()
	,	header()
	,	header_region()
	,	header_ptr( NULL )
	,	buffer()
	,	buffer_region()
	,	buffer_ptr( NULL )
	,	wnd( NULL )
	,	child( 0 )
	,	pi()
	,	si()
{
}

cons_mpp::~cons_mpp() {
}

bool cons_mpp::server_init( HWND hWnd, unsigned int const width, unsigned int const height ) {
	// init server side
	this->wnd = hWnd;
	this->pipe_name = this->gen_guid();
	this->mutex_name = this->gen_guid();
	this->header_name = this->gen_guid();
	//
	if ( this->pipe.create( this->pipe_name ) ) {
		if ( this->mutex.create( this->mutex_name, false ) ) {
			if ( this->build_shmem( this->header_name, width * height, this->header, this->header_region, this->header_ptr ) ) {
#ifdef STANDALONE
				struct _ {
					static DWORD WINAPI __( LPVOID lpParameter ) {
						cons_mpp* s = reinterpret_cast<cons_mpp*>( lpParameter );
						cons_mpp cmpp;
						cmpp.client_run( s->get_pipe_name(), s->get_mutex_name(), s->get_header_name() );
						return 0;
					}
				};
				COORD size = { width, height };
				SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), size );
				this->child = CreateThread( NULL, 0, _::__, reinterpret_cast<LPVOID>( this ), 0, NULL );
				return true;
#else
				// start child console process
				this->si.cb				= sizeof( this->si );
				this->si.dwFlags		= STARTF_USECOUNTCHARS | STARTF_USESHOWWINDOW;
				this->si.dwXCountChars	= width;
				this->si.dwYCountChars	= height;
				this->si.wShowWindow	= SW_HIDE/*SW_SHOW*/;
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
				ss << path << this->get_cmdline();
				//
				TCHAR cmd_line[MAX_PATH] = { 0 };
				strcpy_s( cmd_line, ss.str().c_str() );
				if ( CreateProcess( NULL, cmd_line, NULL, NULL, TRUE, CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi ) ) {
					// ????? check if process finished immediately
					//
					//struct ep_t {
					//	HWND	cons_wnd;
					//	DWORD	pid;
					//} ep = { NULL, this->pi.dwProcessId };
					//struct _ {
					//	static BOOL CALLBACK __( HWND hwnd, LPARAM lParam ) {
					//		ep_t& p = *(reinterpret_cast<ep_t*>( lParam ));
					//		DWORD pid = 0;
					//		GetWindowThreadProcessId( hwnd, &pid ); 
					//		if ( pid == p.pid ) {
					//			p.cons_wnd = hwnd;
					//			return FALSE;
					//		}
					//		return TRUE;
					//	}
					//};
					//EnumWindows( _::__, reinterpret_cast<LPARAM>( &ep ) );
					//TCHAR caption[ MAX_PATH ] = { 0 };
					//GetWindowText( ep.cons_wnd, caption, MAX_PATH );
					//this->process_caption = uni_string( caption );
					this->child = this->pi.hProcess;
					return true;
				}
#endif
			}
		}
	}
	return false;
}

bool cons_mpp::server_bind() {
	return ( this->pipe.connect() );
}

void cons_mpp::server_close() {
	WaitForSingleObject( this->child, INFINITE );
}

void cons_mpp::client_run( string_t const& pname, string_t const& mname, string_t const& hname ) {
	this->pipe_name = pname;
	this->mutex_name = mname;
	this->header_name = hname;
	//
	bool run = false;
	//
	if ( this->pipe.open( this->pipe_name ) ) {
		if ( this->mutex.create( this->mutex_name, false ) ) {
			if ( this->build_shmem( this->header_name, 0, this->header, this->header_region, this->header_ptr ) ) {
				boost::interprocess::scoped_lock< atom::mutex > l( this->mutex );
				//
				this->header_ptr->csbiex.cbSize = sizeof( this->header_ptr->csbiex ); 
				GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &( this->header_ptr->csbiex ) );
				//
				string_t buffer_name = this->gen_guid();
				strcpy_s( this->header_ptr->buffer_name, buffer_name.c_str() );
				this->header_ptr->invalid = true;
				//
				if ( this->build_shmem( buffer_name, this->header_ptr->csbiex.dwSize.X * this->header_ptr->csbiex.dwSize.X * sizeof( CHAR_INFO ), this->buffer, this->buffer_region, this->buffer_ptr ) ) {
					run = true;
				}
			}
		}
	}
	//
	if ( run ) {
		struct _ {
			static DWORD WINAPI __( LPVOID lpParameter ) {
				cons_mpp* cmpp = reinterpret_cast<cons_mpp*>( lpParameter );
				//
				bool cont = true;
				while ( cont ) {
					command c;
					memset( &c, 0, sizeof( c ) );
					if ( cont = cmpp->pipe.read( &c, sizeof( c )  ) ) {
						switch( c.type ) {
						case command::cmdSize:
							{
								break;
							}
						case command::cmdKbrd:
							{
								INPUT_RECORD ir;
								ir.EventType = KEY_EVENT;
								ir.Event.KeyEvent = c.key;
								DWORD wr = 0;
								WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), &ir, sizeof( ir ), &wr );
								cmpp->copy_csb();
								//if ( ir.Event.KeyEvent.wVirtualKeyCode == VK_RETURN ) {
								//	//std::cout << "!!!!!!!!!!!!!!!!!!!!";
								//	write_vk2cons( 'G' );
								//	write_vk2cons( 'I' );
								//	write_vk2cons( 'T' );
								//	write_vk2cons( VK_SPACE );
								//}
								break;
							}
						case command::cmdCtrlBreak:
							{
								GenerateConsoleCtrlEvent( CTRL_BREAK_EVENT, 0 );
								break;
							}
						case command::cmdCtrlC:
							{
								GenerateConsoleCtrlEvent( CTRL_C_EVENT, 0 );
								break;
							}
						case command::cmdExit:
							{
								write_vk2cons( VK_RETURN );
								write_vk2cons( 'E' );
								write_vk2cons( 'X' );
								write_vk2cons( 'I' );
								write_vk2cons( 'T' );
								write_vk2cons( VK_RETURN );
								cont = false;
								break;
							}
						}
					}
				}
				//close all child processes
				return 0;
			}
		};
		//, GetStdHandle( STD_INPUT_HANDLE ), GetStdHandle( STD_OUTPUT_HANDLE ) )
		HANDLE thread = CreateThread( NULL, 0, _::__, reinterpret_cast<LPVOID>( this ), 0, NULL );
		//
		tty tty1( std::cout );
		tty1.run();
		//
		WaitForSingleObject( thread, INFINITE );
	}
}

void cons_mpp::onkey( KEY_EVENT_RECORD const& key ) {
	command c;
	c.type = command::cmdKbrd;
	c.key = key;
	this->pipe.write( &c, sizeof( c ) );
}

void cons_mpp::onctrl_break() {
	command c;
	c.type = command::cmdCtrlBreak;
	this->pipe.write( &c, sizeof( c ) );
}

void cons_mpp::onctrl_c() {
	command c;
	c.type = command::cmdCtrlC;
	this->pipe.write( &c, sizeof( c ) );
}

void cons_mpp::onexit() {
	command c;
	c.type = command::cmdExit;
	this->pipe.write( &c, sizeof( c ) );
}

void cons_mpp::draw( HDC dc, RECT const& rt, LONG const cw, LONG const ch ) {
	boost::interprocess::scoped_lock< atom::mutex > l( this->mutex );
	if ( this->header_ptr->invalid ) {
		if ( this->build_shmem( this->header_ptr->buffer_name, 0, this->buffer, this->buffer_region, this->buffer_ptr ) ) {
			this->header_ptr->invalid = false;
		}
	}
	//
	if ( !this->header_ptr->invalid && this->buffer_ptr ) {

		LONG const cols = ( rt.right - rt.left ) / cw;
		LONG rows = ( rt.bottom - rt.top ) / ch;
		//
		CONSOLE_SCREEN_BUFFER_INFOEX const& csbiex = this->header_ptr->csbiex;
		size_t const q = csbiex.dwSize.X / cols;
		size_t const m = csbiex.dwSize.X % cols;

		size_t const columns		= ( q )?( ( m )?( q + 1 ):( q ) ):( 1 );
		size_t const first_count	= ( q )?( ( m )?( m ):( cols ) ):( csbiex.dwSize.X );
		size_t const next_count		= cols;
		//
		RECT rect;
		SetRect( &rect, rt.left, rt.bottom - ch, rt.right, rt.bottom );
		//
		TCHAR* chars = static_cast< TCHAR* >( _alloca( sizeof( TCHAR ) * cols ) );
		CHAR_INFO *csb_char = this->buffer_ptr;
		//
		SHORT csb_row = min( csbiex.srWindow.Bottom, csbiex.dwCursorPosition.Y );
		size_t c_count = first_count;
		size_t c_column = columns;
		while( rows && csb_row ) {
			bool found = false;
			for( size_t i = 0; i < c_count; ++i ) {
				chars[i] = csb_char[ csb_row * csbiex.dwSize.X + ( c_column - 1 ) * next_count + i ].Char.AsciiChar;
				found |= ( chars[i] != _T(' ') );
			}
			if ( found ) {
				DrawText( dc, chars, c_count, &rect, DT_LEFT | DT_TOP );
				//DrawText( dc, "TEST", -1, &rect, DT_LEFT | DT_TOP );
				OffsetRect( &rect, 0, -ch ); 
				rows--;
			}
			//
			if ( --c_column ) {
				c_count = next_count;
			} else {
				c_column = columns;
				c_count = first_count;
				csb_row--;
			}
		}
	}
}


cons_mpp::string_t cons_mpp::gen_guid() {
	GUID guid;
	CoCreateGuid( &guid );
	std::stringstream gss;
	gss << guid;
	return ( gss.str() );
}

void
cons_mpp::copy_csb() {
	GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &( this->header_ptr->csbiex ) );
	CONSOLE_SCREEN_BUFFER_INFOEX const& csbiex = this->header_ptr->csbiex;
	//
	COORD size = { csbiex.dwSize.X, csbiex.dwSize.Y };
	COORD left_top = { 0, csbiex.srWindow.Top };
	SMALL_RECT reg = { 0, csbiex.srWindow.Top, csbiex.dwSize.X - 1, csbiex.srWindow.Bottom };

	if ( !ReadConsoleOutput(
		GetStdHandle( STD_OUTPUT_HANDLE ),
		this->buffer_ptr,
		size,
		left_top,
		&reg ) ) {
			std::cerr << " copy console screen buffer error" << std::endl;
	}
}

