#include "./pch.hpp"
#include "./logger.hpp"
#include "./env.hpp"
#include "./appl.hpp"

appl::appl( logger_ptr l ) : 
		po()
	,	interactive( false )
	,	home() {
	atom::mount<appl2logger>( this, l );
	//
	atom::po::options_description_t& desc = this->po.add_desc( 0, "dpm options" );
	//
	string_t h( getenv( "DPM_HOME" ) );
	po.
		add_option( po_help,					"help,h",											"show this (h)elp", desc ).
		add_option( po_interactive,				"interactive,i",									"(i)nteractive mode", desc ).
		add_option( po_exit,					"exit,e",											"(e)xit from interactive mode", desc ).
		add_option( po_home,					"home,o",
		boost::program_options::value<std::string>()->default_value( h ),							"define dpm h(o)me directory,override %DPM_HOME% environment var", desc ).
		add_option( po_stages,					"stages,s",
		boost::program_options::value<std::string>()->default_value( "" ),							"(s)tages", desc ).
		add_option( po_components,				"components,c",
		boost::program_options::value<std::string>()->default_value( "" ),							"(c)omponents", desc ).
		add_option( po_recursive,				"recursive,r",										"(r)ecursive", desc )
		;
}

appl::~appl() {
}

bool
appl::init( int argc, char const * const argv[] ) {
	atom::po::options_description_t& desc = this->po.get_desc( 0 );
	try {
		this->po.parse_arg( argc, argv, desc, true );
		this->interactive = this->po.count( po_interactive ) > 0;
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
	atom::po::options_description_t const& desc = this->po.get_desc( 0 );
	bool l = true;
	while( l && this->interactive ) {
		std::string s;
		os << "dpm [" << this->home << "] > ";
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
	atom::clear( this->get_env() );
	base_node_t::clear();
}

void
appl::scan( ) {
}

bool
appl::process_command() {
	//
	if ( po.count( po_help ) )
		throw std::exception( "dpm command line parameters:" );
	//
	if ( this->po.count( po_home ) ) {
		// rescan dpm structure
		this->home = this->po.as< string_t >( po_home );
		this->scan();
	}
	//
	string_t s = this->po.as< string_t >( po_stages );
	string_t c = this->po.as< string_t >( po_components );
	if ( s.length() && c.length() ) {
		// 
		*(this->get_logger()) << s << std::endl << c << std::endl;
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
