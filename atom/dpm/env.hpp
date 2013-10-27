#pragma once

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > env2logger;
typedef atom::nstorage< appl, boost::shared_ptr, atom::narray1 > env2appl;
typedef atom::nstorage< env, boost::shared_ptr, atom::narray1 > env2env;
typedef atom::nstorage< env, boost::shared_ptr, atom::nlist > env2envs;
typedef atom::nstorage< comp, boost::shared_ptr, atom::nlist > env2comps;

class env :
	public atom::node< LOKI_TYPELIST_5( env2logger, env2appl, env2env, env2envs, env2comps ) >,
	public boost::enable_shared_from_this< env > {

	typedef atom::node< LOKI_TYPELIST_5( env2logger, env2appl, env2env, env2envs, env2comps ) >
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
	static env_ptr create( logger_ptr l, appl_ptr a, env_ptr p, string_t const & n, string_t const & h ) {
		env_ptr result = env_ptr( new env( l, a, n, h ) );
		if ( p ) {
			atom::mount<env2env>( result, p );
			atom::mount<env2envs>( p, result );
		}
		return result;
	}
	///
	~env();
	///
	void
	clear();
	///
	void
	scan();
	///
	void
	print( logger_ptr l, env_ptr ce, string_t const& offs, bool const pc );
	///
	void
	print_c( logger_ptr l, string_t const& offs );
	///
	bool
	find( string_t const& n, env_ptr& ce );
	///
	static boost::filesystem::path
	get_dpm_folder( boost::filesystem::path const& h );
	///
	static boost::filesystem::path
	get_dl_folder( boost::filesystem::path const& h );
	///
	static boost::filesystem::path
	get_env_folder( boost::filesystem::path const& h );
	///
	static boost::filesystem::path
	get_config_file( boost::filesystem::path const& h );

protected:
	//
	logger_ptr get_logger() {
		return ( get_slot<env2logger>().item() );
	}
	//
	appl_ptr get_appl() {
		return ( get_slot<env2appl>().item() );
	}

private:
	string_t
		name;
	boost::filesystem::path
		home;
	///
	env( logger_ptr l, appl_ptr a, string_t const & n, string_t const & h );
};