#include "./pch.hpp"

#pragma comment(lib, "winmm.lib")

// https://connect.microsoft.com/PowerShell/feedback/details/572313/powershell-exe-can-hang-if-stdin-is-redirected
// http://en.wikipedia.org/wiki/ANSI_escape_code
// http://support.microsoft.com/default.aspx?scid=kb;EN-US;q190351&wa=wsignin1.0
//SetWinEventHook.
// http://stackoverflow.com/questions/2175665/createprocess-process-do-not-terminate-when-redirecting-std-out-in-err

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
