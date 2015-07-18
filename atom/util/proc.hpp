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
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		//
    	si.cb = sizeof(si);
		//
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

	template <typename T>
	struct startupinfo;
	template <>
	struct startupinfo < char > {
		STARTUPINFOA si;
		BOOL(__stdcall*create())(
			LPCSTR lpApplicationName,
			LPSTR lpCommandLine,
			LPSECURITY_ATTRIBUTES lpProcessAttributes,
			LPSECURITY_ATTRIBUTES lpThreadAttributes,
			BOOL bInheritHandles,
			DWORD dwCreationFlags,
			LPVOID lpEnvironment,
			LPCSTR lpCurrentDirectory,
			LPSTARTUPINFOA lpStartupInfo,
			LPPROCESS_INFORMATION lpProcessInformation
			) {
			return CreateProcessA;
		}
	};
	template <>
	struct startupinfo < wchar_t > {
		STARTUPINFOW si;
		BOOL(__stdcall*create())(
			LPCWSTR lpApplicationName,
			LPWSTR lpCommandLine,
			LPSECURITY_ATTRIBUTES lpProcessAttributes,
			LPSECURITY_ATTRIBUTES lpThreadAttributes,
			BOOL bInheritHandles,
			DWORD dwCreationFlags,
			LPVOID lpEnvironment,
			LPCWSTR lpCurrentDirectory,
			LPSTARTUPINFOW lpStartupInfo,
			LPPROCESS_INFORMATION lpProcessInformation
			) {
			return CreateProcessW;
		}
	};

	class proc : public boost::noncopyable {
	public:
		//
		proc() : pi() {
		}
		//
		~proc() {
			CloseHandle( this->pi.hThread );
			CloseHandle( this->pi.hProcess );
		}
		//
		template< typename C >
		bool run( std::basic_string<C> const& name, unsigned int const cw, unsigned int const ch, bool const new_console, bool const show ) {
			startupinfo<C> info;
			memset(&info.si, 0, sizeof(info.si));
			info.si.cb = sizeof(info.si);
			info.si.dwFlags = ((cw || ch) ? (STARTF_USECOUNTCHARS) : (0)) | STARTF_USESHOWWINDOW;
			info.si.dwXCountChars = cw;
			info.si.dwYCountChars = ch;
			info.si.wShowWindow = ((show) ? (SW_SHOW) : (SW_HIDE));
			//
			size_t const cmd_line_len = sizeof(C) * (name.length() + 1);
			C* cmd_line = (C*)_alloca(cmd_line_len);
			if (cmd_line){
				memset(cmd_line, 0, cmd_line_len);
				std::copy(name.begin(), name.end(), cmd_line);
				if (info.create()(NULL, cmd_line, NULL, NULL, TRUE, (new_console) ? (CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP) : (0), NULL, NULL, &info.si, &pi)) {
					return true;
				}
			}
			return false;
		}
		template< typename C >
		bool run(std::basic_string<C> const& name, bool const show ) {
			return this->run( name, 0, 0, false, show );
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
