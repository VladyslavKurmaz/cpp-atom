#include "./pch.hpp"

#pragma comment(lib, "winmm.lib")

// https://connect.microsoft.com/PowerShell/feedback/details/572313/powershell-exe-can-hang-if-stdin-is-redirected
// http://en.wikipedia.org/wiki/ANSI_escape_code
// http://support.microsoft.com/default.aspx?scid=kb;EN-US;q190351&wa=wsignin1.0
//SetWinEventHook.
// http://stackoverflow.com/questions/2175665/createprocess-process-do-not-terminate-when-redirecting-std-out-in-err

#ifdef STANDALONE
const std::string TEST_PIPE_NAME		= "TEST_PIPE";
const std::string TEST_SHAREDMEM_NAME	= "TEST_SHAREDMEM";
#endif

const TCHAR CMDID_SPLIT_NAME[]		= _T( "split" );
const TCHAR CMDID_EXPAND_NAME[]		= _T( "expand" );
const TCHAR CMDID_ROTATE_NAME[]		= _T( "rotate" );
const TCHAR CMDID_NEXT_NAME[]		= _T( "next" );
const TCHAR CMDID_PREV_NAME[]		= _T( "prev" );

const conf_cmd_t conf_cmds[] = {
	{ CMDID_SPLIT_NAME,		CMDID_SPLIT,	_T("split current frame") },
	{ CMDID_EXPAND_NAME,	CMDID_EXPAND,	_T("maximize/restore frame") },
	{ CMDID_ROTATE_NAME,	CMDID_ROTATE,	_T("rotate frames") },
	{ CMDID_NEXT_NAME,		CMDID_NEXT,		_T("focus next frame") },
	{ CMDID_PREV_NAME,		CMDID_PREV,		_T("focus prev frame") },
};
const size_t conf_cmds_cnt = sizeof( conf_cmds ) / sizeof( conf_cmds[0] );

const TCHAR DELIM1[] = _T( ";" );
const TCHAR DELIM2[] = _T( ":" );

int atom::node_cnt = 0;


std::string gen_uuid() {
	GUID guid;
	CoCreateGuid( &guid );
	std::stringstream gss;
	gss << guid;
	return ( gss.str() );
}

bool getConsoleSize( COORD& size, SMALL_RECT& view ) {
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo = { 0 };
	if ( GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiInfo ) ) {
		size = csbiInfo.dwSize;
		view = csbiInfo.srWindow;
		return true;
	}
	return false;
}

/*
void window::processAutostart() {
	//Registry.SetValue(@"HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run",
	//          "MyStartUp",
	//          @"C:\StartUpApp.exe");
}

*/

#if 0
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
#endif