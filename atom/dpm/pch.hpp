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
// help, interactive, home, env, dpmdir, dldir, envdir
static atom::po::id_t const po_desc1				=	po_none + 1;
// recursive
static atom::po::id_t const po_desc2				=	po_desc1 + 1;
// pos1, pos2, pos3, pos4
static atom::po::id_t const po_desc3				=	po_desc2 + 1;

// positional desc stages|switch|tree|exit, components|env, debug|release, x86|x64
static atom::po::id_t const po_pdesc3				=	po_desc3 + 1;
// desc1 + desc2 + desc3 + pdesc3
static atom::po::id_t const po_desc_cmdline			=	po_pdesc3 + 1;
// desc2 + desc3 + pdesc3
static atom::po::id_t const po_desc_interactive		=	po_desc_cmdline + 1;

static atom::po::id_t const po_help					=	po_desc_interactive + 1;
static atom::po::id_t const po_interactive			=	po_help + 1;
static atom::po::id_t const po_home					=	po_interactive + 1;
static atom::po::id_t const po_env					=	po_home + 1;
static atom::po::id_t const po_dpmdir				=	po_env + 1;
static atom::po::id_t const po_dldir				=	po_dpmdir + 1;
static atom::po::id_t const po_envdir				=	po_dldir + 1;
static atom::po::id_t const po_recursive			=	po_envdir + 1;

static atom::po::id_t const po_pos1					=	po_recursive + 1;
static atom::po::id_t const po_pos2					=	po_pos1 + 1;
static atom::po::id_t const po_pos3					=	po_pos2 + 1;
static atom::po::id_t const po_pos4					=	po_pos3 + 1;
static atom::po::id_t const po_pos5					=	po_pos4 + 1;

static char const* fslash				= "/";
static char const* bslash				= "\\";

static char const* cmd_help				= "help";
static char const* cmd_tree				= "tree";
static char const* cmd_switch			= "switch";
static char const* cmd_exit				= "exit";

