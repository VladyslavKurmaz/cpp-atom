#pragma once

struct command {
	enum type {
		cmdNone			= 0,
		cmdSize			= 1,
		cmdKbrd			= 2,
		cmdCtrlBreak	= 3,
		cmdCtrlC		= 4,
		cmdClose		= 5,
		cmdExit			= 6,
		cmdTerminate	= 7,
		cmdQuit			= 8
	} id;
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
