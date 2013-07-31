#include "./pch.hpp"
#include "./logger.hpp"
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
env::print( logger_ptr l, env_ptr ce, string_t const& offs ) {
	*l << (( ce.get() == this )?("> "):("  ")) << offs << "[" << this->pname << "] " << this->proot << "" << std::endl;
	//
	struct _ {
		static bool __( env_ptr e, logger_ptr l, env_ptr ce, string_t const& offs ) {
			e->print( l, ce, offs );
			return false;
		};
	};
	string_t s = offs + string_t( " " );
	this->get_slot<env2envs>().for_each( boost::bind( _::__, _1, l, ce, boost::ref( s ) ) );
}

bool
env::find( string_t const& n, env_ptr& ce ) {
	if ( this->pname == n ) {
		ce = this->shared_from_this();
		return true;
	}
	struct _ {
		static bool __( env_ptr e, string_t const& n, env_ptr& ce ) {
			return ( e->find( n, ce ) );
		};
	};
	return ( this->get_slot<env2envs>().for_each( boost::bind( _::__, _1, boost::cref( n ), boost::ref( ce ) ) ) );
}
