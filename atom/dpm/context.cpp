#include "./pch.hpp"
#include "./logger.hpp"
#include "./context.hpp"


context::context( logger_ptr l ) :
		env_vars() {
	atom::mount<context2logger>( this, l );
}

context::~context() {
}

void
context::clear() {
	base_node_t::clear();
}

void
context::add_env_var( string_t const& n, string_t const& v ) {
	if ( n.length() && v.length() ) {
		string_t k = n;
		char_t const* c = CONST_PREFIX_REPLACE;
		while( *c ){
			std::replace( k.begin(), k.end(), *c, CONST_PREFIX_DELIM[0] );
			c++;
		}
		boost::to_upper( k );
		this->env_vars.push_back( std::make_pair( k, v ) );
	} else {
		throw std::runtime_error( "Invalid environment variable name or value" );
	}
}
