#include "./pch.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./frame.hpp"
#include "./process.hpp"

process::process( logger_ptr l, frame_ptr f ) :
		pi()
	,	si()
	,	pipe() {
	atom::mount<process2logger>( this, l );
	atom::mount<process2frame>( this, f );
	this->pipe.create();
}

process::~process() {
}

uni_string process::run( uni_string const& cmd ){
	uni_string result;
	//
	this->si.cb				= sizeof( this->si );
	this->si.dwFlags		= STARTF_USESHOWWINDOW;
	this->si.wShowWindow	= SW_SHOW;
	//
	TCHAR command[MAX_PATH] = _T( "D:\\work\\env\\cpp-atom\\tmp\\msvc10_x86_Debug\\atom\\cons\\Debug\\consd.exe" ) 
	//{ 0 }
	;
	//strcpy_s( command, cmd.c_str() );
	if ( CreateProcess( NULL, command, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi ) ) {
		this->pipe.connect();
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
		result = uni_string( caption );
	}
	return ( result );
}

process& process::kbrd( KEY_EVENT_RECORD const& key ){
	command c;
	c.type = command::cmdKbrd;
	c.key = key;
	this->pipe.write( &c, sizeof( c ) );
	return (*this);
}


// node: add close timeout before terminate
process& process::close() {
	command c;
	c.type = command::cmdExit;
	this->pipe.write( &c, sizeof( c ) );
	return (*this);
}

process& process::terminate() {
	return (*this);
}

process& process::clear() {
	base_node_t::clear();
	return (*this);
}

#if 0
uni_string process::run( uni_string const& cmd ){
	SECURITY_ATTRIBUTES sa;
	HANDLE output_read_tmp, output_write;
	HANDLE input_write_tmp, input_read;
	HANDLE error_write;
	//
	sa.nLength				= sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle		= TRUE;
	//
	// Create the child output pipe.
	if ( !CreatePipe( &output_read_tmp, &output_write, &sa, 0 ) ) {
		throw get_last_error("Create output pipe" );
	}
	//
	if ( !DuplicateHandle(GetCurrentProcess(), output_write, GetCurrentProcess(), &error_write, 0, TRUE, DUPLICATE_SAME_ACCESS ) ) {
		throw get_last_error( "Duplicate handle for errors' stream" );
	}
	//
	// Create the child input pipe.
	if ( !CreatePipe( &input_read, &input_write_tmp, &sa, 0 ) ) {
		throw get_last_error( "Create input pipe" );
	}
	//
	if ( !DuplicateHandle( GetCurrentProcess(), output_read_tmp, GetCurrentProcess(), &output_read, 0, FALSE, DUPLICATE_SAME_ACCESS ) ) {
		throw get_last_error( "Duplicate handle for output pipe read operation" );
	}
	//
	if ( !DuplicateHandle( GetCurrentProcess(), input_write_tmp, GetCurrentProcess(), &input_write, 0, FALSE, DUPLICATE_SAME_ACCESS ) ) {
		throw get_last_error( "Duplicate handle for input pipe write operation" );
	}
	//
	if ( !CloseHandle( output_read_tmp ) ){
		throw get_last_error( "Close output pipe read temp handle" );
	}
	//
	if ( !CloseHandle( input_write_tmp ) ) {
		throw get_last_error( "Close input pipe write temp handle" );
	}
	// Get std input handle so you can close it and force the ReadFile to
	// fail when you want the input thread to exit.
	if ( ( std_in = GetStdHandle( STD_INPUT_HANDLE ) ) == INVALID_HANDLE_VALUE ) {
		throw get_last_error( "Get std input handle" );
	}
	//
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	//
	ZeroMemory( &si, sizeof( si ) );
	si.cb				= sizeof(STARTUPINFO);
	si.dwFlags			= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdOutput		= output_write;
	si.hStdInput		= input_read;
	si.hStdError		= error_write;
	si.wShowWindow		= SW_HIDE;
	//
	TCHAR command[MAX_PATH] = { 0 };
	strcpy_s( command, cmd.c_str() );
	if ( !CreateProcess( NULL, command, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi ) ) {
		throw get_last_error( "Get std input handle" );
	}
	//
	child_process = pi.hProcess;
	if ( !CloseHandle( pi.hThread ) ) {
		throw get_last_error( "Close child process main thread" );
	}
	//
	if ( !CloseHandle( output_write ) ) {
		throw get_last_error( "Close output pipe write handle" );
	}
	if ( !CloseHandle( input_read ) ) {
		throw get_last_error( "Close input pipe read handle" );
	}
	if ( !CloseHandle( error_write ) ) {
		throw get_last_error( "Close error pipe write handle" );
	}
	//
	process_ptr* ptr = new process_ptr( shared_from_this() );
	thread = CreateThread( NULL, 0, read_from_pipe, (LPVOID)ptr, 0, &threadid );
	if ( thread == NULL) {
		delete ptr;
		throw get_last_error( "Read thread" );
	}
	Sleep( 100 );
	//
	struct ep_t {
		HWND	cons_wnd;
		DWORD	pid;
	} ep = { NULL, pi.dwProcessId };
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
	//
	SendMessage( ep.cons_wnd, WM_CHAR, 'A', 0 );
	return ( uni_string( caption) );
}

DWORD WINAPI process::read_from_pipe( LPVOID lpvThreadParam ) {
	boost::scoped_ptr< process_ptr > ptr( (process_ptr*)lpvThreadParam );
	process_ptr self( *ptr );
	frame_ptr f = self->get_slot<process2frame>().item();
	//
	CHAR lpBuffer[256];
	DWORD nBytesRead;

	while( self->is_running() ) {
		ZeroMemory( lpBuffer, sizeof(lpBuffer) );
		if ( !ReadFile( self->output_read, lpBuffer, sizeof( lpBuffer ) - 1, &nBytesRead, NULL) || !nBytesRead ) {
			if (GetLastError() == ERROR_BROKEN_PIPE)
				break; // pipe done - normal exit path.
			else
				throw get_last_error( "Read from pipe" );
		}
		lpBuffer[nBytesRead] = '\0';
		f->append( lpBuffer, nBytesRead );
	}
	return 0;
}

#endif
