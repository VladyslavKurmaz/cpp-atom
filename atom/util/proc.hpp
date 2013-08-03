/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_UTIL_PROC_HPP
#define ATOM_UTIL_PROC_HPP
#if defined(_WIN32)

#include <windows.h>
#include <sstream>

namespace atom {

	template < typename T >
	void exec( std::basic_string< T > const & path, std::basic_string< T > const & cdir ) {
		STARTUPINFO si;
	    PROCESS_INFORMATION pi;

	    ZeroMemory( &si, sizeof(si) );
    	si.cb = sizeof(si);
	    ZeroMemory( &pi, sizeof(pi) );
		TCHAR cmd[ MAX_PATH ] = { 0 };
		strcpy( cmd, path.c_str() );
	    // Start the child process. 
	    if( CreateProcess( NULL,   // No module name (use command line)
    	    cmd,        // Command line
	        NULL,           // Process handle not inheritable
    	    NULL,           // Thread handle not inheritable
	        FALSE,          // Set handle inheritance to FALSE
    	    0,              // No creation flags
	        NULL,           // Use parent's environment block
    	    (( cdir.length() )?( cdir.c_str() ):( NULL )),           // Use parent's starting directory 
	        &si,            // Pointer to STARTUPINFO structure
    	    &pi )           // Pointer to PROCESS_INFORMATION structure
	    ) 
	    {
		    WaitForSingleObject( pi.hProcess, INFINITE );
		    CloseHandle( pi.hProcess );
		    CloseHandle( pi.hThread );
    	}
	}

	template < typename T >
	class proc : public boost::noncopyable {
	public:
		//
		proc() : pi(), si() {
		}
		//
		~proc() {
			CloseHandle( this->pi.hThread );
			CloseHandle( this->pi.hProcess );
		}
		//
		bool run( std::basic_string< T > const& name, unsigned int const cw, unsigned int const ch, bool const show ) {
			this->si.cb				= sizeof( this->si );
			this->si.dwFlags		= STARTF_USECOUNTCHARS | STARTF_USESHOWWINDOW;
			this->si.dwXCountChars	= cw;
			this->si.dwYCountChars	= ch;
			this->si.wShowWindow	= ((show)?(SW_SHOW):(SW_HIDE));
			//
			TCHAR cmd_line[MAX_PATH] = { 0 };
			strcpy_s( cmd_line, name.c_str() );
			if ( CreateProcess( NULL, cmd_line, NULL, NULL, TRUE, CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi ) ) {
				return true;
			}
			return false;
		}
		//
		void join() {
			WaitForSingleObject( this->pi.hProcess, INFINITE );
		}
	protected:
	public:
		//
		PROCESS_INFORMATION
			pi;
		//
		STARTUPINFO
			si;
	};
}

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
	//this->process_caption = string_t( caption );


#endif
#endif //ATOM_UTIL_PROC_HPP
