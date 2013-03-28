#pragma once

struct command {
	enum {
		cmdSize,
		cmdRun,
		cmdRunas,
		cmdKbrd,
		cmdBreak,
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
