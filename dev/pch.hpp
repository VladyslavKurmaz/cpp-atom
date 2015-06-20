
#pragma once
//#pragma warning(disable : 4996)
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

namespace dev {

	typedef TCHAR char_t;
	typedef std::basic_string< char_t > string_t;
	typedef std::basic_stringstream< char_t > stringstream_t;
	typedef std::ostream ostream_t;

	class logger;
	typedef boost::shared_ptr< logger > logger_ptr;
#define LOGGER_ACCESSOR( t )	logger_ptr getLogger() { return ( ( get_value( boost::mpl::identity< t >() ).item() ) ); }

	class appl;
	typedef boost::shared_ptr< appl > appl_ptr;

	class entity;
	typedef boost::shared_ptr< entity > entity_ptr;


	// appl
	typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > appl2logger;
	typedef atom::nstorage< entity, boost::shared_ptr, atom::narray1 > appl2entity;


#if 0

	class comp;
	typedef boost::shared_ptr< comp > comp_ptr;
	typedef std::deque< comp_ptr > comp_deq_t;

	class context;
	typedef boost::shared_ptr< context > context_ptr;

	class filter;
	typedef boost::shared_ptr< filter > filter_ptr;


	// entity
	typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > entity2logger;
	typedef atom::nstorage< entity, boost::shared_ptr, atom::narray1 > entity2entity;
	typedef atom::nstorage< entity, boost::shared_ptr, atom::nlist > entity2entities;

	//context
	typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > context2logger;

	//filter
	typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > filter2logger;


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
	static atom::po::id_t const po_home					=	po_startup_desc + 1;
	static atom::po::id_t const po_init_env				=	po_home + 1;
	static atom::po::id_t const po_msbuild_ver			=	po_init_env + 1;
	//-----------------------------------------------------------------------------
	static atom::po::id_t const po_conf_desc			=	po_msbuild_ver + 1;
	//
	static atom::po::id_t const po_user					=	po_conf_desc + 1;
	static atom::po::id_t const po_password				=	po_user + 1;
	static atom::po::id_t const po_email				=	po_password + 1;
	static atom::po::id_t const po_origin				=	po_email + 1;
	static atom::po::id_t const po_branch				=	po_origin + 1;
	static atom::po::id_t const po_osystem				=	po_branch + 1;
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

	static char_t const* CONST_DEV_HOME_ENV_VAR				= "DEV_HOME";
	static char_t const* CONST_IS_X86						= "x86";
	static char_t const* CONST_IS_X64						= "x64";



	//env.json
	static char const* const CONST_ENV_JSON_CONFIG_FILE		= "env.json";

	static char_t const* CONST_CMD_DELIM					= ";";
	static char_t const* CONST_CMD_DELIM1					= "|";
	static char_t const* CONST_PREFIX_DELIM					= "_";
	static char_t const* CONST_PREFIX_HEAD					= "DPM";
	static char_t const* CONST_PREFIX_REPLACE				= "/\\-.";

	static char_t const* CONST_CMD_HELP						= "help";
	static char_t const* CONST_CMD_CHANGE_ENV				= "cd";
	static char_t const* CONST_CMD_EXIT						= "exit";
	static char_t const* CONST_CMD_ENV_ACTION				= ".";

	static char_t const* CONST_CMD_INFO						= "info";
	static char_t const* CONST_CMD_SYNC						= "sync";
	static char_t const* CONST_CMD_STATUS					= "status";

	static char_t const* CONST_PT_COMP_DEPENDS				= "depends";
	static char_t const* CONST_PT_COMP_INHERITS				= "inherits";
	static char_t const* CONST_PT_COMP_PACKAGE				= "package";
	static char_t const* CONST_PT_COMP_VAR					= "var";


	// dpm.conf
	static char_t const* CONST_DPM_CONF_CATALOG_FILE		= "catalog";
	static char_t const* CONST_DPM_CONF_REPO_GIT			= "repo.git";
	static char_t const* CONST_DPM_CONF_REPO_SVN			= "repo.svn";
	static char_t const* CONST_DPM_CONF_REPO_REMOTE			= "repo.remote";
	static char_t const* CONST_DPM_CONF_REPO_REMOTE_DEF		= "origin";
	static char_t const* CONST_DPM_CONF_REPO_BRANCH			= "repo.branch";
	static char_t const* CONST_DPM_CONF_REPO_BRANCH_DEF		= "master";
	static char_t const* CONST_DPM_CONF_REPO_USER_NAME		= "repo.user.name";
	static char_t const* CONST_DPM_CONF_REPO_USER_EMAIL		= "repo.user.email";

	// catalog_conf
	static char_t const* CONST_CATALOG_CONF_COMPONENT		= "component";


#include "./util.hpp"

	//
	class env_paths {
	public:
		env_paths( boost::filesystem::path const& h ) :
		  home( h )
			  ,	conf_file( boost::filesystem::path( home ).operator/=( boost::filesystem::path( "dpm.conf" ) ) )
			  ,	dpm( boost::filesystem::path( home ).operator/=( boost::filesystem::path( ".dpm" ) ) )
		  {}
		  boost::filesystem::path const & get_home() const { return ( this->home ); }
		  boost::filesystem::path const & get_conf_file() const { return ( this->conf_file ); }
		  boost::filesystem::path const & get_dpm() const { return ( this->dpm ); }
		  boost::filesystem::path get_dpm_file( boost::filesystem::path const& f ) const { return ( boost::filesystem::path( this->dpm ).operator/= ( f ) ); }

	protected:
	private:
		boost::filesystem::path const
			home;
		boost::filesystem::path const
			conf_file;
		boost::filesystem::path const
			dpm;
	};


#define make_msg( s, t ) error( s, t ).str().c_str()
#define make_error_msg( t ) make_msg( error::err, t )
#define make_crit_msg( t ) make_msg( error::crit, t )

	class error {
	public:
		//
		enum severity {
			emerg,
			alert,
			crit,
			err,
			warn,
			notice,
			info,
			debug
		};
		//
		enum facility {
			user
		};
		//
		error( severity const s, std::string const& t ) : sev( s ), text( t ) {
		}
		//
		std::string str() {
			std::stringstream ss;
			ss << "[";
			switch ( sev ){
			case emerg:
				ss << "emerg";
				break;
			case alert:
				ss << "alert";
				break;
			case crit:
				ss << "crit";
				break;
			case err:
				ss << "err";
				break;
			case warn:
				ss << "warn";
				break;
			case notice:
				ss << "notice";
				break;
			case info:
				ss << "info";
				break;
			case debug:
				ss << "debug";
				break;
			}
			ss << "] " << text;
			return ss.str();
		}
	protected:
	private:
		//
		severity sev;
		//
		std::string text;
	};
#endif
}
