#pragma once
#include <windows.h>

#include <iostream>

#include <boost/smart_ptr.hpp>

#include <atom/util/log.hpp>
#include <atom/node/tldefs.hpp>
#include <atom/node/node.hpp>
#include <atom/util/dbg.hpp>
#include <atom/util/po.hpp>
#include <atom/util/wwindow.hpp>
#include <atom/util/waccel.hpp>
#include <atom/util/ptr.hpp>

#ifdef UNICODE
#define TXT( x )  L ## x
#else
#define TXT( x )  x
#endif

typedef std::basic_string<TCHAR>
	uni_string;
///
static atom::po::id_t const po_help					=	0;
//[hk.*]
static atom::po::id_t const po_hk_appear			=	po_help + 1;
static atom::po::id_t const po_hk_split				=	po_hk_appear + 1;
static atom::po::id_t const po_hk_expand			=	po_hk_split + 1;
static atom::po::id_t const po_hk_rotate			=	po_hk_expand + 1;
static atom::po::id_t const po_hk_next				=	po_hk_rotate + 1;
static atom::po::id_t const po_hk_prev				=	po_hk_next + 1;
static atom::po::id_t const po_hk_close				=	po_hk_prev + 1;
static atom::po::id_t const po_hk_tty1				=	po_hk_close + 1;
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
//[ui.font.*]
static atom::po::id_t const po_ui_font_name			=	po_ui_bk_color + 1;
static atom::po::id_t const po_ui_font_height		=	po_ui_font_name + 1;
static atom::po::id_t const po_ui_font_color		=	po_ui_font_height + 1;
//[ui.margin.*]
static atom::po::id_t const po_ui_margin_size		=	po_ui_font_color + 1;
//[ui.border.*]
static atom::po::id_t const po_ui_border_size		=	po_ui_margin_size + 1;
static atom::po::id_t const po_ui_border_color		=	po_ui_border_size + 1;
//[ui.padding.*]
static atom::po::id_t const po_ui_padding_size		=	po_ui_border_color + 1;
		//[ui.scroll.*]
static atom::po::id_t const po_ui_scroll_size		=	po_ui_padding_size + 1;
static atom::po::id_t const po_ui_scroll_color		=	po_ui_scroll_size + 1;

///
struct alignment {
	typedef int
		type;
	enum {
		client	= 0x00,
		left	= 0x01,
		right	= 0x02,
		hcenter	= left | right,
		top		= 0x04,
		bottom	= 0x08,
		vcenter	= top | bottom,  
		center	= vcenter | hcenter
	};
};

///
static WORD	const CMDID_SPLIT	= 100;
static WORD	const CMDID_EXPAND	= CMDID_SPLIT + 1;
static WORD	const CMDID_ROTATE	= CMDID_EXPAND + 1;
static WORD	const CMDID_NEXT	= CMDID_ROTATE + 1;
static WORD	const CMDID_PREV	= CMDID_NEXT + 1;
static WORD	const CMDID_CLOSE	= CMDID_PREV + 1;
