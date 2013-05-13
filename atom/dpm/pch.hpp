#pragma once
#pragma warning(disable : 4996)
//#pragma warning(disable : 4355)

#include <windows.h>

#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>

#include <atom/util/po.hpp>
#include <atom/util/log.hpp>
#include <atom/node/tldefs.hpp>
#include <atom/node/node.hpp>
#include <atom/util/dbg.hpp>
#include <atom/util/po.hpp>
#include <atom/util/ptr.hpp>
#include <atom/util/cast.hpp>


class logger;
typedef boost::shared_ptr< logger >
	logger_ptr;

class appl;
typedef boost::shared_ptr< appl >
	appl_ptr;

class env;
typedef boost::shared_ptr< env >
	env_ptr;

class comp;
typedef boost::shared_ptr< comp >
	comp_ptr;

typedef std::basic_string< TCHAR >
	string_t;

static atom::po::id_t const po_none					=	0;
// help
static atom::po::id_t const po_desc1				=	po_none + 1;
// interactive, home
static atom::po::id_t const po_desc2				=	po_desc1 + 1;
// stages, components, recursive, tree
static atom::po::id_t const po_desc3				=	po_desc2 + 1;
// switch, exit 
static atom::po::id_t const po_desc4				=	po_desc3 + 1;
// desc1 + desc2 + desc3 
static atom::po::id_t const po_desc_cmdline			=	po_desc4 + 1;
// desc1 + desc3 + desc4 
static atom::po::id_t const po_desc_console			=	po_desc_cmdline + 1;

static atom::po::id_t const po_help					=	po_desc_console + 1;
static atom::po::id_t const po_interactive			=	po_help + 1;
static atom::po::id_t const po_home					=	po_interactive + 1;
static atom::po::id_t const po_stages				=	po_home + 1;
static atom::po::id_t const po_components			=	po_stages + 1;
static atom::po::id_t const po_recursive			=	po_components + 1;
static atom::po::id_t const po_tree					=	po_recursive + 1;
static atom::po::id_t const po_switch				=	po_tree + 1;
static atom::po::id_t const po_exit					=	po_switch + 1;




