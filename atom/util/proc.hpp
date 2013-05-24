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
#ifdef _WINDOWS

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
}

#endif
#endif //ATOM_UTIL_PROC_HPP
