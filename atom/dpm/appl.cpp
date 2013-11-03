#include "./pch.hpp"
#include "./logger.hpp"
#include "./env.hpp"
#include "./appl.hpp"

/*
.dpm folder should contain conf file with system folder names overloads 
conf
download_folder=dl
environment_folder=env

*/

appl::appl( logger_ptr l ) : 
		po()
	,	shell_mode( false )
	,	home()
	,	msbuild()
	,	update_cmd()
	,	cenv() {
	atom::mount<appl2logger>( this, l );
	//
	char const* root = getenv( "DPM_HOME2" ); 
	string_t h( ( root != NULL )?( root ):( "" ) );
	//
	char const* arch = getenv( "PROCESSOR_ARCHITECTURE" ); 
	string_t am( ( arch != NULL )?( ( !strcmp( arch, "x86" ) )?( "32" ):( "64" ) ):( "" ) );
	//
	atom::po::options_description_t& util_desc = this->po.add_desc( po_util_desc, "" );
	this->po.
		add_option( po_help,			"help,h",			"show this (h)elp", util_desc ).
		add_option( po_recursive,		"recursive,r",		"(r)ecursive mode", util_desc ).
		add_option( po_verbose,			"verbose,v",		"(v)erbose", util_desc );
	//
	atom::po::options_description_t& startup_desc = this->po.add_desc( po_startup_desc, "" );
	this->po.
		add_option( po_shell,			"shell,s",			"(s)hell mode", startup_desc ).
		add_option( po_home,			"home,o",			"define dpm h(o)me directory, override %DPM_HOME% env var", startup_desc, boost::program_options::value<std::string>()->default_value( h ) ).
		add_option( po_init_env,		"env,e",			"define current (e)nvironment", startup_desc, boost::program_options::value<std::string>()->default_value( slash ) ).
		add_option( po_msbuild_ver,		"msbuild,m",		"(m)sbuild version to use", startup_desc, boost::program_options::value<std::string>()->default_value( "4.0" ) ).
		add_option( po_update_cmd,		"update-cmd,u",		"(u)pdate command", startup_desc, boost::program_options::value<std::string>()->default_value( "svn update" ) );
	//
	atom::po::options_description_t& conf_desc = this->po.add_desc( po_conf_desc, "" );
	this->po.
		add_option( po_osystem,			"osystem,y",		"operation s(y)stem", conf_desc, boost::program_options::value<std::string>()->default_value( "windows" ) ).
		add_option( po_toolset,			"toolset,t",		"build (t)oolset", conf_desc, boost::program_options::value<std::string>()->default_value( "msvc11" ) ).
		add_option( po_instruction_set,	"instruction_set,n","i(n)struction set", conf_desc, boost::program_options::value<std::string>()->default_value( "i386" ) ).
		add_option( po_address_model,	"address-model,a",	"(a)ddress-model", conf_desc, boost::program_options::value<std::string>()->default_value( am ) ).
		add_option( po_configuration,	"configuration,c",	"(c)onfiguration", conf_desc, boost::program_options::value<std::string>()->default_value( "debug" ) );
	//
	atom::po::options_description_t& subcommands_desc = this->po.add_desc( po_subcommands_desc, "" );
	this->po.
		add_option( po_subcommand1,		"sc1",				"<component>|help|cd   |ls|sync|exit", subcommands_desc, boost::program_options::value<std::string>()->default_value( "" ) ).
		add_option( po_subcommand2,		"sc2",				"<stage>    |    |<env>|  |    |", subcommands_desc, boost::program_options::value<std::string>()->default_value( "" ) );

	atom::po::positional_options_description_t& subcommands_posdesc = this->po.add_pdesc( po_subcommands_posdesc, "" );
	this->po.
		add_option( po_subcommand1,		"sc1",				"", subcommands_posdesc, 1 ).
		add_option( po_subcommand2,		"sc2",				"", subcommands_posdesc, 1 );
	//
	atom::po::options_description_t& cmdline_desc = this->po.add_desc( po_cmdline_desc, "" );
	cmdline_desc.add( util_desc ).add( startup_desc ).add( conf_desc ).add( subcommands_desc );
	//
	atom::po::options_description_t& shell_desc = this->po.add_desc( po_shell_desc, "" );
	shell_desc.add( util_desc ).add( conf_desc ).add( subcommands_desc );
}

appl::~appl() {
}

bool
appl::init( int argc, char const * const argv[] ) {
	atom::po::options_description_t& desc = this->po.get_desc( po_cmdline_desc );
	atom::po::positional_options_description_t& pdesc = this->po.get_pdesc( po_subcommands_posdesc );
	try {
		this->po.parse_arg( argc, argv, desc, pdesc, true );
		//
		shell_mode = ( this->po.count( po_shell ) > 0 );
		this->home = this->po.as< string_t >( po_home );
		if ( !this->home.length() ) {
			throw ( "[err] dpm home wasn't defined, set environment variable DPM_HOME2 or use command line argument --env" );
		}

		// add path to msbuild into proccess env vars
		string_t msbuild_reg = string_t ( "SOFTWARE\\Microsoft\\MSBuild\\ToolsVersions\\" ) + this->po.as< string_t >( po_msbuild_ver );
		HKEY key;
		if ( ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, msbuild_reg.c_str(), 0, KEY_READ, &key ) ) {
		    TCHAR buf[ MAX_PATH ] = { 0 };
		    DWORD bufsz = MAX_PATH;
    		if ( ERROR_SUCCESS == RegQueryValueEx( key, _T( "MSBuildToolsPath" ), 0, NULL, (LPBYTE)buf, &bufsz ) ) {
				this->msbuild = buf;
		    }
			RegCloseKey( key );
		}
		if ( !this->msbuild.length() ) {
			throw std::exception( "[emerg] Couldn't locate msbuild" );
		} else {
			// update path environment variable
			bool update_err = true;
			TCHAR b[4096] = { 0 };
			if ( GetEnvironmentVariable( _T( "path" ), b, 4096 ) ) {
				strcat( b, _T( ";" ) );
				strcat( b, this->msbuild.c_str() );
				if ( SetEnvironmentVariable( _T( "path" ), b ) ) {
					update_err = false;
				}
			}
			if ( update_err ) {
				throw std::exception( "[emerg] Couldn't update process env block" );
			}
		}
		update_cmd = this->po.as< string_t >( po_update_cmd );

		// create root environment
		atom::mount<appl2env>( this, this->cenv = env::create( this->get_logger(), this->shared_from_this(), env_ptr(), slash, this->home ) );
		//
		this->cenv->scan();
		//
		this->get_env()->find( this->po.as< string_t >( po_init_env ), this->cenv );
		//
		this->process_command();
		//
	} catch( std::exception& exc ) {
		this->print_error( desc, exc );
		return false;
	}
	return true;
}

void
appl::run( std::ostream& os, std::istream& is ) {
	atom::po::options_description_t& shell_desc = this->po.get_desc( po_shell_desc );
	atom::po::positional_options_description_t& subcommands_posdesc = this->po.get_pdesc( po_subcommands_posdesc );
	while( shell_mode ) {
		std::string s;
		os << "dpm [" << this->cenv->get_caption() << "] $ ";
		std::getline( is, s );
		//
		try {
			this->po.parse_cmd_line( s, shell_desc, subcommands_posdesc, true );
			if ( !this->process_command() ) {
				break;
			}
		} catch( std::exception& exc ) {
			this->print_error( shell_desc, exc );
		}
	}
}

void
appl::clear(){
	this->cenv.reset();
	atom::clear( this->get_env() );
	base_node_t::clear();
}

void
appl::scan() {
	////
	//struct _ {
	//	static env_ptr find_env( logger_ptr l, appl_ptr a, env_ptr e, env::names const& n ) {
	//		env_ptr result;
	//		//
	//		WIN32_FIND_DATA fdt;
	//		string_t s = n.root + n.dpm;
	//		HANDLE hf = FindFirstFile( s.c_str(), &fdt );
	//		if ( hf != INVALID_HANDLE_VALUE ) {
	//			if ( fdt.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
	//				result = env::create( l, a, e, n );
	//			}
	//			FindClose( hf );
	//		}
	//		// scan subfolders
	//		if ( result ) {
	//			WIN32_FIND_DATA fdt;
	//			string_t s = n.root + n.env + string_t( bslash ) + string_t( "*" );
	//			HANDLE hf = FindFirstFile( s.c_str(), &fdt );
	//			if ( hf != INVALID_HANDLE_VALUE ) {
	//				do {
	//					if ( ( fdt.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && strcmp( fdt.cFileName, "." ) && strcmp( fdt.cFileName, ".." ) ) {
	//						env::names nn = n;
	//						nn.root += n.env + string_t( bslash ) + string_t( fdt.cFileName ) + string_t( bslash );
	//						nn.name += ( ( nn.name == string_t( fslash ) )?( string_t() ):( string_t( fslash ) ) ) + string_t( fdt.cFileName );
	//						find_env( l, a, result, nn );
	//					}
	//				} while( FindNextFile( hf, &fdt ) );
	//				FindClose( hf );
	//			}
	//		}

	//		return ( result );
	//	}
	//};
	//env::names n;
	//n.name	=	string_t( fslash );
	//n.root	=	this->home;
	//n.dpm	=	this->po.as< string_t >( po_dpmdir );
	//n.dl	=	this->po.as< string_t >( po_dldir );
	//n.env	=	this->po.as< string_t >( po_envdir );
	//this->cenv = _::find_env( this->get_logger(), this->shared_from_this(), env_ptr(), n );
	//atom::mount<appl2env>( this, this->cenv );
}

bool
appl::process_command() {
	//
	string_t pos1 = this->po.as< string_t >( po_subcommand1 );
	string_t pos2 = this->po.as< string_t >( po_subcommand2 );
	//
	if ( ( pos1 == string_t( cmd_help ) ) || this->po.count( po_help ) ) {
		//
		//
		throw std::exception( "dpm command line parameters:" );
		//
	} else if ( pos1 == string_t( cmd_change ) ) {
		//
		// change current environment
		this->get_env()->find( pos2, this->cenv );
		//
	} else if ( pos1 == string_t( cmd_list ) ) {
		//
		// show environments structure
		*(this->get_logger()) << std::endl;
		this->get_env()->print( this->get_logger(), this->cenv, string_t(), ( this->po.count( po_verbose ) > 0 ) );
		*(this->get_logger()) << std::endl;
		//
	} else if ( pos1 == string_t( cmd_sync ) ) {
		//
		// sync command
		struct _ {
			static bool __( env_ptr e, bool const r, string_t const& cmd, logger_ptr l ) {
				*l << e->get_paths().get_home() << std::endl;
				atom::exec( cmd, e->get_paths().get_dpm().string() );
				*l << std::endl;
				//
				if ( r ) {
					e->get_slot<env2envs>().for_each( boost::bind( _::__, _1, r, boost::cref( cmd ), l ) );
				}
				return (!r);
			};
		};
		_::__( this->cenv, ( this->po.count( po_recursive ) > 0 ), this->update_cmd, this->get_logger() );
		//
	} else if ( pos1 == string_t( cmd_exit ) ) {
		//
		//
		return false;
		//
	} else if ( pos1.length() && pos2.length() ){
		try {
			this->cenv->execute( pos1, pos2 );
		} catch( std::exception& e ) {
			*(this->get_logger()) << e.what() << std::endl;
		}
	}
	*(this->get_logger()) << std::endl;
	return ( true );
}

void
appl::print_error( atom::po::options_description_t const& desc, std::exception& exc ) {
	std::stringstream ss;
	desc.print( ss );
	*(this->get_logger()) << exc.what() << std::endl;
	*(this->get_logger()) << ss.str() << std::endl;
}
