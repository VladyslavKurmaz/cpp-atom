#include "./pch.hpp"
#include "./env.hpp"

env::env( logger_ptr l, appl_ptr a, names const& n ) :
		pname( n.name )
	,	proot( n.root )
	,	pdpm( n.root + n.dpm + string_t( bslash ) )
	,	pdl( n.root + n.dl + string_t( bslash )  )
	,	penv( n.root + n.env + string_t( bslash )  )
 {
	atom::mount<env2logger>( this, l );
	atom::mount<env2appl>( this, a );
}

env::~env() {
}

void
env::clear() {
	atom::clear_r< env_ptr, env2envs >( this->shared_from_this() );
	base_node_t::clear();
}

void
env::print( std::ostream& os, string_t const& offs ) {
	os << offs << "[" << this->pname << "] " << this->proot << "" << std::endl;
	//
	struct _ {
		static bool __( env_ptr e, std::ostream& os, string_t const& offs ) {
			e->print( os, offs );
			return true;
		};
	};
	string_t s = offs + string_t( " " );
	this->get_slot<env2envs>().for_each( boost::bind( _::__, _1, boost::ref( os ), boost::ref( s ) ) );
}

bool
env::find( string_t const& n, env_ptr& ce ) {
	bool result = false;
	if ( this->pname == n ) {
		ce = this->shared_from_this();
		return true;
	}
	struct _ {
		static bool __( env_ptr e, string_t const& n, env_ptr& ce, bool& r ) {
			r = e->find( n, ce );
			return ( !r );
		};
	};
	this->get_slot<env2envs>().for_each( boost::bind( _::__, _1, boost::cref( n ), boost::ref( ce ), boost::ref( result ) ) );
	return result;
}
