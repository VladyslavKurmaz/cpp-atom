
#pragma once
#pragma warning(disable : 4996)
//#pragma warning(disable : 4355)

#include <iostream>

#include <windows.h>
#include <tchar.h>

#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <atom/util/po.hpp>
#include <atom/util/log.hpp>
#include <atom/node/tldefs.hpp>
#include <atom/node/node.hpp>
#include <atom/util/dbg.hpp>
#include <atom/util/po.hpp>
#include <atom/util/ptr.hpp>
#include <atom/util/cast.hpp>
#include <atom/util/proc.hpp>


typedef TCHAR
	char_t;

typedef std::basic_string< char_t >
	string_t;

typedef std::basic_stringstream< char_t >
	stringstream_t;


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
typedef std::deque< comp_ptr >
	comp_deq_t;

// appl
typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > appl2logger;
typedef atom::nstorage< env, boost::shared_ptr, atom::narray1 > appl2env;

// env
typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > env2logger;
typedef atom::nstorage< appl, boost::shared_ptr, atom::narray1 > env2appl;
typedef atom::nstorage< env, boost::shared_ptr, atom::narray1 > env2env;
typedef atom::nstorage< env, boost::shared_ptr, atom::nlist > env2envs;
typedef atom::nstorage< comp, boost::shared_ptr, atom::nlist > env2comps;

//comp
typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > comp2logger;
typedef atom::nstorage< appl, boost::shared_ptr, atom::narray1 > comp2appl;
typedef atom::nstorage< env, boost::shared_ptr, atom::narray1 > comp2env;



//-----------------------------------------------------------------------------
static atom::po::id_t const po_none					=	0;
//-----------------------------------------------------------------------------
static atom::po::id_t const po_util_desc			=	po_none + 1;
//
static atom::po::id_t const po_help					=	po_util_desc + 1;
static atom::po::id_t const po_recursive			=	po_help + 1;
static atom::po::id_t const po_verbose				=	po_recursive + 1;
//-----------------------------------------------------------------------------
static atom::po::id_t const po_startup_desc			=	po_verbose + 1;
//
static atom::po::id_t const po_shell				=	po_startup_desc + 1;
static atom::po::id_t const po_home					=	po_shell + 1;
static atom::po::id_t const po_init_env				=	po_home + 1;
static atom::po::id_t const po_msbuild_ver			=	po_init_env + 1;
//-----------------------------------------------------------------------------
static atom::po::id_t const po_conf_desc			=	po_msbuild_ver + 1;
//
static atom::po::id_t const po_user					=	po_conf_desc + 1;
static atom::po::id_t const po_password				=	po_user + 1;
static atom::po::id_t const po_email				=	po_password + 1;
static atom::po::id_t const po_osystem				=	po_email + 1;
static atom::po::id_t const po_toolset				=	po_osystem + 1;
static atom::po::id_t const po_instruction_set		=	po_toolset + 1;
static atom::po::id_t const po_address_model		=	po_instruction_set + 1;
static atom::po::id_t const po_configuration		=	po_address_model + 1;
//-----------------------------------------------------------------------------
static atom::po::id_t const po_subcommands_desc		=	po_configuration + 1;
static atom::po::id_t const po_subcommands_posdesc	=	po_subcommands_desc + 1;
//
static atom::po::id_t const po_subcommand1			=	po_subcommands_posdesc + 1;
static atom::po::id_t const po_subcommand2			=	po_subcommand1 + 1;
//-----------------------------------------------------------------------------
static atom::po::id_t const po_cmdline_desc			=	po_subcommand2 + 1;
//-----------------------------------------------------------------------------
static atom::po::id_t const po_shell_desc			=	po_cmdline_desc + 1;

static char_t const slash								= '/';
static char_t const bslash								= '\\';

static char_t const* CONST_CMD_HELP						= "help";
static char_t const* CONST_CMD_CHANGE_ENV				= "cd";
static char_t const* CONST_CMD_LIST						= "ls";
static char_t const* CONST_CMD_SYNC						= "sync";
static char_t const* CONST_CMD_EXIT						= "exit";

static char_t const* CONST_PT_COMP_DEPENDS				= "depends";
static char_t const* CONST_PT_COMP_INHERITS				= "inherits";

// dpm.conf
static char_t const* CONST_DPM_CONF_REPO_GIT			= "repo:git";
static char_t const* CONST_DPM_CONF_CATALOG_FILE		= "repo:catalog";

// catalog_conf
static char_t const* CONST_CATALOG_CONF_COMPONENT		= "component";
