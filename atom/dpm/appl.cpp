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
	atom::po::options_description_t& desc1 = this->po.add_desc( po_desc1, "" );
	this->po.
		add_option( po_help,					"help,h",											"show this (h)elp", desc1 );
	//
	atom::po::options_description_t& desc2 = this->po.add_desc( po_desc2, "" );
	this->po.
		add_option( po_interactive,				"interactive,i",									"(i)nteractive mode", desc2 ).
		add_option( po_home,					"home,o",
			boost::program_options::value<std::string>()->default_value( h ),						"define dpm h(o)me directory,override %DPM_HOME% env var", desc2 ).
		add_option( po_dpmdir,					"dir,d",
			boost::program_options::value<std::string>()->default_value( ".dpm" ),					"define dpm system (d)ir name", desc2 );
	
	atom::po::options_description_t& desc3 = this->po.add_desc( po_desc3, "" );
	this->po.
		add_option( po_stages,					"stages,s",
			boost::program_options::value<std::string>()->default_value( "" ),						"(s)tages \"stat;sync;conf;build\"", desc3 ).
		add_option( po_components,				"components,c",
			boost::program_options::value<std::string>()->default_value( "" ),						"(c)omponents \"boost\"", desc3 ).
		add_option( po_recursive,				"recursive,r",										"(r)ecursive", desc3 ).
		add_option( po_tree,					"tree,t",											"show components' (t)ree", desc3 );
	//
	atom::po::options_description_t& desc4 = this->po.add_desc( po_desc4, "" );
	this->po.
		add_option( po_switch,					"switch,w",
			boost::program_options::value<std::string>()->default_value( "" ),						"s(w)itch to new environment", desc4 ).
		add_option( po_exit,					"exit,e",											"(e)xit from interactive mode", desc4 );
	//
	atom::po::options_description_t& desc_cmdline = this->po.add_desc( po_desc_cmdline, "" );
	desc_cmdline.add( desc1 ).add( desc2 ).add( desc3 );

	atom::po::options_description_t& desc_console = this->po.add_desc( po_desc_console, "" );
	desc_console.add( desc1 ).add( desc3 ).add( desc4 );
}

appl::~appl() {
}

bool
appl::init( int argc, char const * const argv[] ) {
	atom::po::options_description_t& desc = this->po.get_desc( po_desc_cmdline );
	try {
		this->po.parse_arg( argc, argv, desc, true );
		this->home = this->po.as< string_t >( po_home );
		// rescan dpm structure
		this->scan();
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
	atom::po::options_description_t const& desc = this->po.get_desc( po_desc_console );
	bool l = true;
	bool const i = ( this->po.count( po_interactive ) > 0 );
	while( l && i ) {
		std::string s;
		os << "dpm [" << this->cenv->get_caption() << "] > ";
		std::getline( is, s );
		//
		try {
			this->po.parse_cmd_line( s, desc, true );
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
	n.dl	=	"dl";
	n.env	=	"env";
	this->cenv = _::find_env( this->get_logger(), this->shared_from_this(), env_ptr(), n );
	atom::mount<appl2env>( this, this->cenv );
}

bool
appl::process_command() {
	//
	if ( po.count( po_help ) )
		throw std::exception( "dpm command line parameters:" );
	//
	//
	string_t s = this->po.as< string_t >( po_stages );
	string_t c = this->po.as< string_t >( po_components );
	if ( s.length() && c.length() ) {
	} else if ( po.count( po_tree ) ) {
		std::cout << std::endl << "environments' structure:" << std::endl << std::endl;
		this->get_env()->print( std::cout, string_t() );
		std::cout << std::endl << std::endl;
	} else if ( po.count( po_switch ) ) {
		this->get_env()->find( this->po.as< string_t >( po_switch ), this->cenv );
	}
	//
	return ( this->po.count( po_exit ) == 0 );
}

void
appl::print_error( atom::po::options_description_t const& desc, std::exception& exc ) {
	std::stringstream ss;
	desc.print( ss );
	*(this->get_logger()) << exc.what() << std::endl;
	*(this->get_logger()) << ss.str() << std::endl;
}
