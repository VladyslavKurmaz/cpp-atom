#pragma once
#pragma warning(disable : 4996)
#pragma warning(disable : 4355)


#include <windows.h>
#include <commctrl.h>

#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <limits>
#include <cstddef>

#include <boost/smart_ptr.hpp>
#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
#include "boost/tuple/tuple.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/locale.hpp>

//#include <boost/function.hpp>
//#include <boost/bind.hpp>


#include <atom/util/cast.hpp>
#include <atom/strs.hpp>
#include <atom/util/log.hpp>
#include <atom/node/tldefs.hpp>
#include <atom/node/node.hpp>
#include <atom/util/dbg.hpp>
#include <atom/util/po.hpp>
#include <atom/util/wwindow.hpp>
#include <atom/util/wctrls.hpp>
#include <atom/util/waccel.hpp>
#include <atom/util/ptr.hpp>
#include <atom/util/pipe.hpp>
#include <atom/util/proc.hpp>


#include "resource.h"

#undef max




//#define STANDALONE

//#define CONSOLE_STATE
#define AUGMENTED_DESKTOP_STATE




#ifdef STANDALONE
extern const atom::string_t TEST_PIPE_NAME;
extern const atom::string_t TEST_SHAREDMEM_NAME;
#endif

#define WM_FRAMEEXIT	WM_USER+1

///////////////////////////////////////////////////////////////////////////////
/// 

static WORD	const CMDID_TRANSPARENT_FIRST_ID = 1000;
static WORD	const CMDID_ALIGNMENT_FIRST_ID = 1100;
static WORD	const CMDID_VRATION_FIRST_ID = 1200;
static WORD	const CMDID_HRATION_FIRST_ID = 1300;
static WORD	const CMDID_SLIDE_FIRST_ID = 1400;

static WORD	const CMDID_FULLSCREEN = 1500;
static WORD	const CMDID_EXIT = CMDID_FULLSCREEN + 1;
static WORD	const CMDID_HIDE = CMDID_EXIT + 1;
static WORD	const CMDID_MAXIMIZE = CMDID_HIDE + 1;
static WORD	const CMDID_RESTORE = CMDID_MAXIMIZE + 1;
static WORD	const CMDID_BACKGROUND_COLOR = CMDID_RESTORE + 1;
static WORD	const CMDID_WORK_AREA = CMDID_BACKGROUND_COLOR + 1;
static WORD	const CMDID_SPLIT = CMDID_WORK_AREA + 1;
static WORD	const CMDID_EXPAND = CMDID_SPLIT + 1;
static WORD	const CMDID_ROTATE = CMDID_EXPAND + 1;
static WORD	const CMDID_NEXT = CMDID_ROTATE + 1;
static WORD	const CMDID_PREV = CMDID_NEXT + 1;
static WORD	const CMDID_CTRL_BREAK = CMDID_PREV + 1;
static WORD	const CMDID_CTRL_C = CMDID_CTRL_BREAK + 1;
static WORD	const CMDID_CLOSE = CMDID_CTRL_C + 1;

static WORD	const CMDID_TTY1 = CMDID_CLOSE + 1;
static WORD	const CMDID_TTY2 = CMDID_TTY1 + 1;
static WORD	const CMDID_TTY3 = CMDID_TTY2 + 1;
static WORD	const CMDID_TTY4 = CMDID_TTY3 + 1;
static WORD	const CMDID_TTY5 = CMDID_TTY4 + 1;
static WORD	const CMDID_TTY6 = CMDID_TTY5 + 1;

///////////////////////////////////////////////////////////////////////////////
// Augmented desktop panel
// ad-panel controls
static unsigned int const	AD_PANEL_TOOLBAR			=	10000;
static unsigned int const	AD_PANEL_LISTVIEW			=	AD_PANEL_TOOLBAR + 1;

// ad-panel commands
static unsigned int const	AD_PANEL_TB_PIN				=	0x0800;
static unsigned int const	AD_PANEL_TB_LANG_FROM		=	AD_PANEL_TB_PIN + 1;
static unsigned int const	AD_PANEL_TB_LANG_SWAP		=	AD_PANEL_TB_LANG_FROM + 1;
static unsigned int const	AD_PANEL_TB_LANG_TO			=	AD_PANEL_TB_LANG_SWAP + 1;
static unsigned int const	AD_PANEL_TB_DELETE			=	AD_PANEL_TB_LANG_TO + 1;
static unsigned int const	AD_PANEL_TB_LANGUAGE		=	AD_PANEL_TB_DELETE + 1;

// ad-panel images
static int const			AD_PANEL_IMAGE_PIN			=	0;
static int const			AD_PANEL_IMAGE_SWAP			=	AD_PANEL_IMAGE_PIN + 1;
static int const			AD_PANEL_IMAGE_DELETE		=	AD_PANEL_IMAGE_SWAP + 1;
static int const			AD_PANEL_IMAGE_LANG_FIRST	=	AD_PANEL_IMAGE_DELETE + 1;
static int const			AD_PANEL_IMAGE_LANG_EN		=	AD_PANEL_IMAGE_LANG_FIRST;
static int const			AD_PANEL_IMAGE_LANG_UA		=	AD_PANEL_IMAGE_LANG_EN + 1;
static int const			AD_PANEL_IMAGE_LANG_RU		=	AD_PANEL_IMAGE_LANG_UA + 1;
static int const			AD_PANEL_IMAGE_LANG_LAST	=	AD_PANEL_IMAGE_LANG_RU;
static int const			AD_PANEL_IMAGE_LANG_COUNT	=	AD_PANEL_IMAGE_LANG_LAST - AD_PANEL_IMAGE_LANG_FIRST + 1;


typedef unsigned int
	frame_id_t;


static std::string CONFIG_TIMEOUT = "timeout";
static std::string CONFIG_ALIGNMENT = "alignment";
static std::string CONFIG_WIDTH = "width";
static std::string CONFIG_HEIGHT = "height";
static std::string CONFIG_MAXIMIZED = "maximized";
static std::string CONFIG_CLIP = "clip";

static std::string CONFIG_MODE = "mode.";
static std::string CONFIG_BK_COLOR = "background-color";


static struct command_t {
	std::string id;
	WORD command;
} const commands[] = {
	{ "hk.full-screen", CMDID_FULLSCREEN },
	{ "hk.split", CMDID_SPLIT },
	{ "hk.expand", CMDID_EXPAND },
	{ "hk.rotate", CMDID_ROTATE },
	{ "hk.next", CMDID_NEXT },
	{ "hk.prev", CMDID_PREV },
	{ "hk.close", CMDID_CLOSE },
	{ "hk.ctrl-break", CMDID_CTRL_BREAK },
	{ "hk.ctrl-c", CMDID_CTRL_C }
};
static std::string CONFIG_HK_APPEAR = "hk.appear";

///
struct alignment_t {
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
struct placement_t {
	RECT			destination;
	unsigned int	alpha;
	bool			visible;
	bool			fullScreen;
	bool			clip;
	unsigned int	width;
	unsigned int	height;
	alignment_t::type alignment;
	bool			sliding;
	DWORD			startTime;
	DWORD			lastTime;
	DWORD			timeout;
	UINT_PTR		timerId;
	placement_t() {
		SetRectEmpty( &destination );
		alpha		= 0;
		visible		= false;
		fullScreen	= false;
		sliding		= false;
		startTime	= 0;
		lastTime	= 0;
		timeout		= 0;
		timerId		= 10;
	}
};

struct hotkey {
	int id;
	UINT mods;
	UINT vk;
	hotkey() : id(0), mods(), vk() {}   
	int operator==( hotkey const& r ) const { return ( ( this->mods == r.mods ) && ( this->vk == r.vk ) ); }
	int operator!=( hotkey const& r ) const { return ( !( *this == r ) ); }
};

//

atom::string_t gen_uuid();

bool getConsoleSize( COORD& size, SMALL_RECT& view );

bool bitmapSave( atom::string_t const& file, HDC hDC, HBITMAP hBmp );
