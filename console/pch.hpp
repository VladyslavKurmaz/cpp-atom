#pragma once
#pragma warning(disable : 4996)
#pragma warning(disable : 4355)


#include <windows.h>

#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

#include <boost/smart_ptr.hpp>
#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>

//#include <boost/function.hpp>
//#include <boost/bind.hpp>


#include <atom/util/log.hpp>
#include <atom/node/tldefs.hpp>
#include <atom/node/node.hpp>
#include <atom/util/dbg.hpp>
#include <atom/util/po.hpp>
#include <atom/util/wwindow.hpp>
#include <atom/util/waccel.hpp>
#include <atom/util/ptr.hpp>
#include <atom/util/pipe.hpp>
#include <atom/util/cast.hpp>
#include <atom/util/proc.hpp>

#ifdef UNICODE
#define TXT( x )  L ## x
#else
#define TXT( x )  x
#endif

// what does it mean?
//#define STANDALONE
#define WM_FRAMEEXIT	WM_USER+1

typedef unsigned int
	frame_id_t;

///
static atom::po::id_t const po_none					=	0;
static atom::po::id_t const po_help					=	po_none + 1;
static atom::po::id_t const po_autostart			=	po_help + 1;
//[hk.*]
static atom::po::id_t const po_hk_appear			=	po_autostart + 1;
static atom::po::id_t const po_hk_entire_screen		=	po_hk_appear + 1;
static atom::po::id_t const po_hk_frame_split		=	po_hk_entire_screen + 1;
static atom::po::id_t const po_hk_frame_minmax		=	po_hk_frame_split + 1;
static atom::po::id_t const po_hk_frame_rotate		=	po_hk_frame_minmax + 1;
static atom::po::id_t const po_hk_frame_next		=	po_hk_frame_rotate + 1;
static atom::po::id_t const po_hk_frame_prev		=	po_hk_frame_next + 1;
static atom::po::id_t const po_hk_frame_close		=	po_hk_frame_prev + 1;
static atom::po::id_t const po_hk_ctrl_break		=	po_hk_frame_close + 1;
static atom::po::id_t const po_hk_ctrl_c			=	po_hk_ctrl_break + 1;
static atom::po::id_t const po_hk_tty1				=	po_hk_ctrl_c + 1;
static atom::po::id_t const po_hk_tty2				=	po_hk_tty1 + 1;
static atom::po::id_t const po_hk_tty3				=	po_hk_tty2 + 1;
static atom::po::id_t const po_hk_tty4				=	po_hk_tty3 + 1;
static atom::po::id_t const po_hk_tty5				=	po_hk_tty4 + 1;
static atom::po::id_t const po_hk_tty6				=	po_hk_tty5 + 1;
//[ui.*]
static atom::po::id_t const po_ui_timeout			=	po_hk_tty6 + 1;
static atom::po::id_t const po_ui_alignment			=	po_ui_timeout + 1;
static atom::po::id_t const po_ui_width				=	po_ui_alignment + 1;
static atom::po::id_t const po_ui_height			=	po_ui_width + 1;
static atom::po::id_t const po_ui_clip				=	po_ui_height + 1;
static atom::po::id_t const po_ui_alpha				=	po_ui_clip + 1;
static atom::po::id_t const po_ui_bk_color			=	po_ui_alpha + 1;
static atom::po::id_t const po_ui_lines_count		=	po_ui_bk_color + 1;
//[ui.font.*]
static atom::po::id_t const po_ui_font_text			=	po_ui_lines_count + 1;
static atom::po::id_t const po_ui_font_sys			=	po_ui_font_text + 1;
//[ui.margin.*]
static atom::po::id_t const po_ui_margin			=	po_ui_font_sys + 1;
//[ui.border.*]
static atom::po::id_t const po_ui_border			=	po_ui_margin + 1;
//[ui.padding.*]
static atom::po::id_t const po_ui_padding			=	po_ui_border + 1;
//[ui.scroll.*]
static atom::po::id_t const po_ui_scroll			=	po_ui_padding + 1;

///
struct alignment {
	typedef int
		type;
	enum {
		hcenter = 0x00,
		left	= 0x01,
		right	= 0x02,
		hclient	= left | right,
		hmask	= left | right,
		//
		vcenter	= 0x00,
		top		= 0x04,
		bottom	= 0x08,
		vclient	= top | bottom,
		vmask	= top | bottom,
		//
		client	= hclient | vclient,
		center	= hcenter | vcenter
	};
};
///
struct preferences {
	typedef int
		type;
	enum {
		pre		= 0x01,
		update	= 0x02,
		post	= 0x03
	};
};

struct placement {
	RECT		active;
	RECT		inactive;
	bool		maximized;
	int			sliding;
	DWORD		startTime;
	DWORD		timeout;
	UINT_PTR	timerId;
	placement() {
		SetRectEmpty( &active );
		SetRectEmpty( &inactive );
		maximized = false;
		sliding = 0;
		startTime = 0;
		timerId = 10;
	}
};

///
static WORD	const CMDID_FULLSCREEN	= 1000;
static WORD	const CMDID_SPLIT		= CMDID_FULLSCREEN + 1;
extern const TCHAR CMDID_SPLIT_NAME[];
static WORD	const CMDID_EXPAND		= CMDID_SPLIT + 1;
extern const TCHAR CMDID_EXPAND_NAME[];
static WORD	const CMDID_ROTATE		= CMDID_EXPAND + 1;
extern const TCHAR CMDID_ROTATE_NAME[];
static WORD	const CMDID_NEXT		= CMDID_ROTATE + 1;
extern const TCHAR CMDID_NEXT_NAME[];
static WORD	const CMDID_PREV		= CMDID_NEXT + 1;
extern const TCHAR CMDID_PREV_NAME[];
static WORD	const CMDID_CTRL_BREAK	= CMDID_PREV + 1;
static WORD	const CMDID_CTRL_C		= CMDID_CTRL_BREAK + 1;
static WORD	const CMDID_CLOSE		= CMDID_CTRL_C + 1;
static WORD	const CMDID_TTY1		= CMDID_CLOSE + 1;
static WORD	const CMDID_TTY2		= CMDID_TTY1 + 1;
static WORD	const CMDID_TTY3		= CMDID_TTY2 + 1;
static WORD	const CMDID_TTY4		= CMDID_TTY3 + 1;
static WORD	const CMDID_TTY5		= CMDID_TTY4 + 1;
static WORD	const CMDID_TTY6		= CMDID_TTY5 + 1;
//
extern const struct conf_cmd_t {
	TCHAR const *	cmd;
	WORD			id;
	TCHAR const *	desc;
}  conf_cmds[];
extern const size_t conf_cmds_cnt;
//
extern const TCHAR DELIM1[];
extern const TCHAR DELIM2[];
