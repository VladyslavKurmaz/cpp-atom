#include "./pch.hpp"
#include "./env.hpp"
#include "./appl.hpp"

appl::appl( logger_ptr l ) : po() {
	atom::mount<appl2logger>( this, l );
	//
	atom::po::options_description_t& desc = this->po.add_desc( 0, "dpm options" );
	//
	string_t home( getenv( "DPM_HOME" ) );
	po.
		add_option( po_help,					"help,h",											"show this (h)elp", desc ).
		add_option( po_interactive,				"interactive,i",									"(i)nteractive mode", desc ).
		add_option( po_home,					"home,o",											"define dpm h(o)me directory,override %DPM_HOME% environment var", desc ).

		add_option( po_home,					"home,o",
		boost::program_options::value<std::string>()->default_value( home ),						"define dpm h(o)me directory,override %DPM_HOME% environment var", desc ).
		add_option( po_stages,					"stages,s",											"(s)tages", desc ).
		add_option( po_components,				"components,c",										"(c)omponents", desc ).
		add_option( po_recursive,				"recursive,r",										"(r)ecursive", desc )
		;
}

appl::~appl() {
}

bool
appl::init( int argc, char const * const argv[] ) {
	return false;
}

appl&
appl::run(){
	return ( *this );
}

void
appl::clear(){
	env_ptr e = this->get_env();
	if ( e ) {
		e->clear();
	}
	base_node_t::clear();
}
