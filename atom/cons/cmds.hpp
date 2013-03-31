#pragma once

struct command {
	enum {
		cmdSize,
		cmdKbrd,
		cmdCtrlBreak,
		cmdCtrlC,
		cmdExit
	} type;
	union {
		KEY_EVENT_RECORD	key;
		struct {
			TCHAR				domain[MAX_PATH];
			TCHAR				user[MAX_PATH];
			TCHAR				password[MAX_PATH];
			TCHAR				cmd_line[MAX_PATH];
		} process;
	};
};
