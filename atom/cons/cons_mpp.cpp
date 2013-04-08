#include "./pch.hpp"
#include "./cmds.hpp"
#include "./cons_mpp.hpp"

cons_mpp::cons_mpp() :
		pipe_name()
	,	pipe()
	,	shmem_name()
	,	shmem()
	,	shmem_region()
	,	shared_block( NULL )
	,	wnd( NULL )
	,	input( INVALID_HANDLE_VALUE )
	,	output( INVALID_HANDLE_VALUE ) {
}

cons_mpp::~cons_mpp(){
}

bool cons_mpp::init( HWND hWnd, unsigned int const width, unsigned int const height ) {
	this->wnd = hWnd;
	this->pipe_name = this->gen_guid();
	this->shmem_name = this->gen_guid();
	//
	if ( this->pipe.create( this->pipe_name ) ) {
		return ( this->build_shmem( true, width, height ) );
	}
	return false;
}

bool cons_mpp::bind() {
	// ???? create boost::bind for server only
	return ( this->pipe.connect() );
}

bool cons_mpp::init(string_t const& pname, string_t const& shmname, HANDLE hin, HANDLE hout, unsigned int const width, unsigned int const height) {
	this->pipe_name = pname;
	this->shmem_name = shmname;
	this->input = input;
	this->output = output;
	//
	if ( this->pipe.open( this->pipe_name ) ) {
		return ( this->build_shmem( false, width, height ) );
	}
	return false;
}

void cons_mpp::onkey( KEY_EVENT_RECORD const& key ) {
	command c;
	c.type = command::cmdKbrd;
	c.key = key;
	this->pipe.write( &c, sizeof( c ) );
}

void cons_mpp::ctrl_break() {
	command c;
	c.type = command::cmdCtrlBreak;
	this->pipe.write( &c, sizeof( c ) );
}

void cons_mpp::ctrl_c() {
	command c;
	c.type = command::cmdCtrlC;
	this->pipe.write( &c, sizeof( c ) );
}

void cons_mpp::onexit() {
	command c;
	c.type = command::cmdExit;
	this->pipe.write( &c, sizeof( c ) );
}

cons_mpp::string_t cons_mpp::gen_guid() {
	GUID guid;
	CoCreateGuid( &guid );
	std::stringstream gss;
	gss << guid;
	return ( gss.str() );
}

bool cons_mpp::build_shmem( bool const create, unsigned int const width, unsigned int const height ) {
	size_t const shm_size = sizeof( shared_block_t ) + width * height * sizeof( CHAR_INFO );
	if ( create ) {
		this->shmem = boost::shared_ptr< boost::interprocess::windows_shared_memory >( 
			new boost::interprocess::windows_shared_memory( 
					boost::interprocess::create_only,
					"MySharedMemory",
					boost::interprocess::read_write,
					shm_size )
			);
	} else {
		this->shmem = boost::shared_ptr< boost::interprocess::windows_shared_memory >( 
			new boost::interprocess::windows_shared_memory(
					boost::interprocess::open_only,
					"MySharedMemory",
					boost::interprocess::read_write )
			);
	}
	this->shmem_region = boost::shared_ptr< boost::interprocess::mapped_region >(
		new boost::interprocess::mapped_region(
				*(this->shmem.get()),
				boost::interprocess::read_write
				)
		);
	this->shared_block = static_cast< shared_block_t* >( this->shmem_region->get_address() );
	return true;
}

bool cons_mpp::handle_kbrd( command const& c ) {
	INPUT_RECORD ir;
	ir.EventType = KEY_EVENT;
	ir.Event.KeyEvent = c.key;
	DWORD wr = 0;
	WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), &ir, sizeof( ir ), &wr );
	//if ( ir.Event.KeyEvent.wVirtualKeyCode == VK_RETURN ) {
	//	//std::cout << "!!!!!!!!!!!!!!!!!!!!";
	//	write_vk2cons( 'G' );
	//	write_vk2cons( 'I' );
	//	write_vk2cons( 'T' );
	//	write_vk2cons( VK_SPACE );
	//}
	return true;
}

bool cons_mpp::handle_ctrl_break( command const& c ) {
	GenerateConsoleCtrlEvent( CTRL_BREAK_EVENT, 0 );
	return true;
}

bool cons_mpp::handle_ctrl_c( command const& c ) {
	GenerateConsoleCtrlEvent( CTRL_C_EVENT, 0 );
	return true;
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


bool cons_mpp::handle_exit( command const& c ) {
	write_vk2cons( VK_RETURN );
	write_vk2cons( 'E' );
	write_vk2cons( 'X' );
	write_vk2cons( 'I' );
	write_vk2cons( 'T' );
	write_vk2cons( VK_RETURN );
	return false;
}


#if 0
	// start child console process
	this->si.cb				= sizeof( this->si );
	this->si.dwFlags		= STARTF_USECOUNTCHARS | STARTF_USESHOWWINDOW;
	this->si.dwXCountChars	= 80;
	this->si.dwYCountChars	= this->get_pref().get<unsigned int>( po_ui_lines_count );
	this->si.wShowWindow	= SW_SHOW;
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
		<< " --pipe=" << this->pipe.get_name()
		<< " --width=" << this->si.dwXCountChars
		<< " --height=" << this->si.dwYCountChars
		;
	this->get_logger() << pipe.get_name();
	TCHAR cmd_line[MAX_PATH] = { 0 };
	strcpy_s( cmd_line, ss.str().c_str() );
	if ( CreateProcess( NULL, cmd_line, NULL, NULL, TRUE, CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi ) ) {
		// ????? check if process finished immediately
		//
		struct ep_t {
			HWND	cons_wnd;
			DWORD	pid;
		} ep = { NULL, this->pi.dwProcessId };
		struct _ {
			static BOOL CALLBACK __( HWND hwnd, LPARAM lParam ) {
				ep_t& p = *(reinterpret_cast<ep_t*>( lParam ));
				DWORD pid = 0;
				GetWindowThreadProcessId( hwnd, &pid ); 
				if ( pid == p.pid ) {
					p.cons_wnd = hwnd;
					return FALSE;
				}
				return TRUE;
			}
		};
		EnumWindows( _::__, reinterpret_cast<LPARAM>( &ep ) );
		TCHAR caption[ MAX_PATH ] = { 0 };
		GetWindowText( ep.cons_wnd, caption, MAX_PATH );
		this->process_caption = uni_string( caption );
	}
#endif
