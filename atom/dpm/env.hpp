#pragma once

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > env2logger;
typedef atom::nstorage< appl, boost::shared_ptr, atom::narray1 > env2appl;
typedef atom::nstorage< env, boost::shared_ptr, atom::narray1 > env2env;
typedef atom::nstorage< env, boost::shared_ptr, atom::nlist > env2envs;

class env :
	public atom::node< LOKI_TYPELIST_4( env2logger, env2appl, env2env, env2envs ) >,
	public boost::enable_shared_from_this< env > {

	typedef atom::node< LOKI_TYPELIST_4( env2logger, env2appl, env2env, env2envs ) >
		base_node_t;

public:
	struct names {
		string_t name;
		string_t root;
		string_t dpm;
		string_t dl;
		string_t env;
	};
	///
	static env_ptr create( logger_ptr l, appl_ptr a, env_ptr e, names const& n ) {
		env_ptr result = env_ptr( new env( l, a, n ) );
		if ( e ) {
			atom::mount<env2env>( result, e );
			atom::mount<env2envs>( e, result );
		}
		return result;
	}
	///
	~env();
	///
	void
	clear();
	///
	string_t
	get_name() const {
		return ( this->pname );
	}
	///
	string_t
	get_caption() const {
		return ( this->get_name() );
	}
	///
	void
	print( logger_ptr l, string_t const& offs );
	///
	bool
	find( string_t const& n, env_ptr& ce );

protected:
	//
	logger_ptr get_logger() {
		return ( get_slot<env2logger>().item() );
	}

private:
	string_t
		pname;
	string_t
		proot;
	string_t
		pdpm;
	string_t
		pdl;
	string_t
		penv;
	///
	env( logger_ptr l, appl_ptr a, names const& n );
};