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

typedef std::basic_string< char_t >
	string_t;

typedef std::basic_stringstream< char_t >
	stringstream_t;

//-----------------------------------------------------------------------------
static atom::po::id_t const po_none					=	0;
//-----------------------------------------------------------------------------
static atom::po::id_t const po_util_desc			=	po_none + 1;
//
static atom::po::id_t const po_help					=	po_util_desc + 1;
static atom::po::id_t const po_recursive			=	po_help + 1;
//-----------------------------------------------------------------------------
static atom::po::id_t const po_startup_desc			=	po_recursive + 1;
//
static atom::po::id_t const po_shell				=	po_startup_desc + 1;
static atom::po::id_t const po_home					=	po_shell + 1;
static atom::po::id_t const po_init_env				=	po_home + 1;
static atom::po::id_t const po_msbuild_ver			=	po_init_env + 1;
//-----------------------------------------------------------------------------
static atom::po::id_t const po_conf_desc			=	po_msbuild_ver + 1;
//
static atom::po::id_t const po_osystem				=	po_conf_desc + 1;
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

static char_t const* slash							= "/";
static char_t const* bslash							= "\\";

static char_t const* cmd_help						= "help";
static char_t const* cmd_change						= "cd";
static char_t const* cmd_list						= "ls";
static char_t const* cmd_sync						= "sync";
static char_t const* cmd_exit						= "exit";

static char_t const* folder_dpm						= ".dpm";
static char_t const* folder_dl						= "dl";
static char_t const* folder_env						= "env";

static char_t const* file_config					= "catalog.json";

