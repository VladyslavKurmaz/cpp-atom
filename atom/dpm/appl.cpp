#include "./pch.hpp"
#include "./logger.hpp"
#include "./env.hpp"
#include "./appl.hpp"

appl::appl( logger_ptr l ) : 
		po()
	,	home()
	,	cenv() {
	atom::mount<appl2logger>( this, l );
	//
	char const* root = getenv( "DPM_HOME" ); 
	string_t h( ( root !=NULL )?( root ):( "" ) );
	//
	atom::po::options_description_t& desc1 = this->po.add_desc( po_desc1, "Init param" );
	this->po.
		add_option( po_help,					"help,h",											"show this (h)elp", desc1 ).
		add_option( po_interactive,				"interactive,i",									"(i)nteractive mode", desc1 ).
		add_option( po_home,					"home,o",
			boost::program_options::value<std::string>()->default_value( h ),						"define dpm h(o)me directory, override %DPM_HOME% env var", desc1 ).
		add_option( po_env,						"env,e",
			boost::program_options::value<std::string>()->default_value( fslash ),					"define current (e)nvironment", desc1 ).
		add_option( po_dpmdir,					"dpmdir",
			boost::program_options::value<std::string>()->default_value( ".dpm" ),					"define dpm system dir name", desc1 ).
		add_option( po_dldir,					"dldir",
			boost::program_options::value<std::string>()->default_value( "dl" ),					"define download dir name", desc1 ).
		add_option( po_envdir,					"envdir",
			boost::program_options::value<std::string>()->default_value( "env" ),					"define env dir name", desc1 );

	atom::po::options_description_t& desc2 = this->po.add_desc( po_desc2, "" );
	this->po.
		add_option( po_recursive,				"recursive,r",										"(r)ecursive", desc2 );

	atom::po::options_description_t& desc3 = this->po.add_desc( po_desc3, "Subcommands" );
	this->po.
		add_option( po_pos1,					"pos1",
			boost::program_options::value<std::string>()->default_value( "" ),						"tree|switch|exit|<stages>", desc3 ).
		add_option( po_pos2,					"pos2",
			boost::program_options::value<std::string>()->default_value( "" ),						"    | <env>|    |<components>", desc3 ).
		add_option( po_pos3,					"pos3",
			boost::program_options::value<std::string>()->default_value( "" ),						"    |      |    |<toolset>", desc3 ).
		add_option( po_pos4,					"pos4",
			boost::program_options::value<std::string>()->default_value( "" ),						"    |      |    |<configurations>", desc3 ).
		add_option( po_pos5,					"pos5",
			boost::program_options::value<std::string>()->default_value( "" ),						"    |      |    |<architecture>", desc3 );
	//
	atom::po::options_description_t& desc_cmdline = this->po.add_desc( po_desc_cmdline, "" );
	desc_cmdline.add( desc1 ).add( desc2 ).add( desc3 );
	//
	atom::po::options_description_t& desc_interactive = this->po.add_desc( po_desc_interactive, "" );
	desc_interactive.add( desc2 ).add( desc3 );
	//
	atom::po::positional_options_description_t& pdesc3 = this->po.add_pdesc( po_pdesc3, "" );
	this->po.
		add_option( po_pos1, "pos1", 1, pdesc3 ).
		add_option( po_pos2, "pos2", 1, pdesc3 ).
		add_option( po_pos3, "pos3", 1, pdesc3 ).
		add_option( po_pos4, "pos4", 1, pdesc3 ).
		add_option( po_pos5, "pos5", 1, pdesc3 );
}

appl::~appl() {
}

bool
appl::init( int argc, char const * const argv[] ) {
	atom::po::options_description_t& desc = this->po.get_desc( po_desc_cmdline );
	atom::po::positional_options_description_t& pdesc = this->po.get_pdesc( po_pdesc3 );
	try {
		this->po.parse_arg( argc, argv, desc, pdesc, true );
		this->home = this->po.as< string_t >( po_home );
		// rescan dpm structure
		this->scan();
		//set currrent environment
		this->get_env()->find( this->po.as< string_t >( po_env ), this->cenv );
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
	atom::po::options_description_t& desc = this->po.get_desc( po_desc_interactive );
	atom::po::positional_options_description_t& pdesc = this->po.get_pdesc( po_pdesc3 );
	bool l = true;
	bool const i = ( this->po.count( po_interactive ) > 0 );
	while( l && i ) {
		std::string s;
		os << "dpm [" << this->cenv->get_caption() << "] > ";
		std::getline( is, s );
		//
		try {
			this->po.parse_cmd_line( s, desc, pdesc, true );
			l = this->process_command();
		} catch( std::exception& exc ) {
			this->print_error( desc, exc );
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
	//
	struct _ {
		static env_ptr find_env( logger_ptr l, appl_ptr a, env_ptr e, env::names const& n ) {
			env_ptr result;
			//
			WIN32_FIND_DATA fdt;
			string_t s = n.root + n.dpm;
			HANDLE hf = FindFirstFile( s.c_str(), &fdt );
			if ( hf != INVALID_HANDLE_VALUE ) {
				if ( fdt.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
					result = env::create( l, a, e, n );
				}
				FindClose( hf );
			}
			// scan subfolders
			if ( result ) {
				WIN32_FIND_DATA fdt;
				string_t s = n.root + n.env + string_t( bslash ) + string_t( "*" );
				HANDLE hf = FindFirstFile( s.c_str(), &fdt );
				if ( hf != INVALID_HANDLE_VALUE ) {
					do {
						if ( ( fdt.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && strcmp( fdt.cFileName, "." ) && strcmp( fdt.cFileName, ".." ) ) {
							env::names nn = n;
							nn.root += n.env + string_t( bslash ) + string_t( fdt.cFileName ) + string_t( bslash );
							nn.name += ( ( nn.name == string_t( fslash ) )?( string_t() ):( string_t( fslash ) ) ) + string_t( fdt.cFileName );
							find_env( l, a, result, nn );
						}
					} while( FindNextFile( hf, &fdt ) );
					FindClose( hf );
				}
			}

			return ( result );
		}
	};
	env::names n;
	n.name	=	string_t( fslash );
	n.root	=	this->home;
	n.dpm	=	this->po.as< string_t >( po_dpmdir );
	n.dl	=	this->po.as< string_t >( po_dldir );
	n.env	=	this->po.as< string_t >( po_envdir );
	this->cenv = _::find_env( this->get_logger(), this->shared_from_this(), env_ptr(), n );
	atom::mount<appl2env>( this, this->cenv );
}

bool
appl::process_command() {
	//
	string_t pos1 = this->po.as< string_t >( po_pos1 );
	string_t pos2 = this->po.as< string_t >( po_pos2 );
	string_t pos3 = this->po.as< string_t >( po_pos3 );
	string_t pos4 = this->po.as< string_t >( po_pos4 );
	//
	if ( ( pos1 == string_t( cmd_help ) ) || this->po.count( po_help ) ) {
		throw std::exception( "dpm command line parameters:" );
	} else if ( pos1 == string_t( cmd_exit ) ) {
		return false;
	} else if ( pos1 == string_t( cmd_tree ) ) {
		//
		*(this->get_logger()) << std::endl << "structure of environments:" << std::endl << std::endl;
		this->get_env()->print( this->get_logger(), string_t() );
		*(this->get_logger()) << std::endl;
	} else if ( pos1 == string_t( cmd_switch ) ) {
		//
		this->get_env()->find( pos2, this->cenv );
	} else if ( pos1.length() && pos2.length() ) {
		// run msbuild
		*(this->get_logger())
			<< "msbuild /p:stages=\"" << pos1 << "\""
			<< " /p:components=\"" << pos2 << "\""
			<< " /p:recursive=" << (( this->po.count( po_recursive ) )?( "true" ):( "false" )) << std::endl;
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
