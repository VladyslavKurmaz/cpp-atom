#pragma once
#include <windows.h>

#include <iostream>

#include <boost/smart_ptr.hpp>

#include <atom/node/tldefs.hpp>
#include <atom/node/node.hpp>
#include <atom/util/dbg.hpp>
#include <atom/util/po.hpp>

///
static atom::po::id_t const po_help					=	0;
//[hk.*]
static atom::po::id_t const po_hk_appear			=	po_help + 1;
static atom::po::id_t const po_hk_split				=	po_hk_appear + 1;
static atom::po::id_t const po_hk_minmax			=	po_hk_split + 1;
static atom::po::id_t const po_hk_rotate			=	po_hk_minmax + 1;
static atom::po::id_t const po_hk_next				=	po_hk_rotate + 1;
static atom::po::id_t const po_hk_prev				=	po_hk_next + 1;
static atom::po::id_t const po_hk_close				=	po_hk_prev + 1;
//[ui.*]
static atom::po::id_t const po_ui_timeout			=	po_hk_close + 1;
static atom::po::id_t const po_ui_alignment			=	po_ui_timeout + 1;
static atom::po::id_t const po_ui_width				=	po_ui_alignment + 1;
static atom::po::id_t const po_ui_height			=	po_ui_width + 1;
static atom::po::id_t const po_ui_clip				=	po_ui_height + 1;
static atom::po::id_t const po_ui_alpha				=	po_ui_clip + 1;
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
static WORD	const CMDID_MINMAX	= CMDID_SPLIT + 1;
static WORD	const CMDID_ROTATE	= CMDID_MINMAX + 1;
static WORD	const CMDID_NEXT	= CMDID_ROTATE + 1;
static WORD	const CMDID_PREV	= CMDID_NEXT + 1;
static WORD	const CMDID_CLOSE	= CMDID_PREV + 1;
