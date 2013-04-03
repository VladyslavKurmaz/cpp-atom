#pragma once

struct command {
	enum {
		cmdNone			= 0,
		cmdSize			= 1,
		cmdKbrd			= 2,
		cmdCtrlBreak	= 3,
		cmdCtrlC		= 4,
		cmdExit			= 5
	} type;
	union {
		COORD				size;
		KEY_EVENT_RECORD	key;
		struct {
			TCHAR				domain[MAX_PATH];
			TCHAR				user[MAX_PATH];
			TCHAR				password[MAX_PATH];
			TCHAR				cmd_line[MAX_PATH];
		} process;
	};
};
