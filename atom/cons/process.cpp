#include "./pch.hpp"
#include "./process.hpp"

#include <atom/util/cast.hpp>

// https://connect.microsoft.com/PowerShell/feedback/details/572313/powershell-exe-can-hang-if-stdin-is-redirected


process::process( logger::shared_ptr l ) :
		buffer()
	,	std_in( NULL )
	,	child_process( NULL )
	,	output_read( NULL )
	,	input_write( NULL )
	,	thread( NULL )
	,	threadid( 0 )
	,	run_thread( true ) {
	atom::mount<process2logger>( this, l );
}

process::~process() {
}

std::string get_last_error( std::string const& caption ) {
	return std::string("error");
}

DWORD WINAPI read_from_pipe( LPVOID lpvThreadParam ) {
	boost::scoped_ptr< process::shared_ptr > ptr( (process::shared_ptr*)lpvThreadParam );
	process::shared_ptr self( *ptr );
	//
	CHAR lpBuffer[256];
	DWORD nBytesRead;

	while( self->is_running() ) {
		ZeroMemory( lpBuffer, sizeof(lpBuffer) );
		if ( !ReadFile( self->get_output_read(), lpBuffer, sizeof( lpBuffer ) - 1, &nBytesRead, NULL) || !nBytesRead ) {
			if (GetLastError() == ERROR_BROKEN_PIPE)
				break; // pipe done - normal exit path.
			else
				throw get_last_error( "Read frompipe" );
		}
#if 0
		lpBuffer[nBytesRead] = L'\0';
		std::wstring wstr( (wchar_t*)lpBuffer);
		self->get_logger() << atom::string2string<std::string>( wstr ); 
#else
		lpBuffer[nBytesRead] = '\0';
		self->append( lpBuffer );
#endif
	}
	return 0;
}

void process::run( std::basic_string<TCHAR> const& cmd ){
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
	si.wShowWindow		= SW_SHOW;//SW_HIDE;
	//
	TCHAR command[MAX_PATH] = { 0 };
	strcpy_s( command, cmd.c_str() );
	if ( !CreateProcess( NULL, command, NULL, NULL, TRUE, 0/*CREATE_NEW_CONSOLE*/, NULL, NULL, &si, &pi ) ) {\
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
	shared_ptr* ptr = new shared_ptr( shared_from_this() );
	thread = CreateThread( NULL, 0, read_from_pipe, (LPVOID)ptr, 0, &threadid );
	if ( thread == NULL) {
		delete ptr;
		throw get_last_error( "Read thread" );
	}
}

void process::write( std::string const& str ) {
	DWORD nBytesWrote;
	if ( !WriteFile( input_write, str.c_str(), str.length(), &nBytesWrote, NULL ) )
	{
		if ( GetLastError() == ERROR_NO_DATA )
			get_last_error( "Write pipe was closed" );
		else
			get_last_error( "Write to pipe" );
	}
	this->get_logger() << str << ":" << nBytesWrote << std::endl;
}

void process::write( char const ch ){ 
	DWORD nBytesWrote = 0;
	if ( !WriteFile( input_write, &ch, sizeof(ch), &nBytesWrote, NULL ) )
	{
		if ( GetLastError() == ERROR_NO_DATA )
			get_last_error( "Write pipe was closed" );
		else
			get_last_error( "Write to pipe" );
	}
	this->get_logger() << ch << ":" << nBytesWrote << std::endl;
}

void process::close() {
	run_thread = false;
	this->write( "exit\x0D\x0A" );
	if ( WaitForSingleObject( thread, INFINITE ) == WAIT_FAILED ) {
		throw get_last_error( "Wait for thread" );
	}
	//
	if ( !CloseHandle( output_read ) ) {
		throw get_last_error( "Close output pipe read handle" );
	}
	if ( !CloseHandle( input_write ) ) {
		throw get_last_error( "Close input pipe write handle" );
	}
}
