#include "./pch.hpp"
#include "./env.hpp"

env::env( logger_ptr l, appl_ptr a, env_ptr e, names const& n ) :
		pname( n.name )
	,	proot( n.root )
	,	pdpm( n.root + n.dpm + string_t( "\\" ) )
	,	pdl( n.root + n.dl + string_t( "\\" )  )
	,	penv( n.root + n.env + string_t( "\\" )  )
 {
	atom::mount<env2logger>( this, l );
	atom::mount<env2appl>( this, a );
	if ( e ) {
		atom::mount<env2env>( this, e );
		atom::mount<env2envs>( e, this->shared_from_this() );
	}
}

env::~env() {
}
