#pragma once

class env_paths {
public:
	env_paths( boost::filesystem::path const& h ) :
			home( h )
		,	dpm( boost::filesystem::path( home ).operator/=( boost::filesystem::path( ".dpm" ) ) )
		,	config_file( boost::filesystem::path( dpm ).operator/=( boost::filesystem::path( "catalog.json" ) ) )
		,	dl( boost::filesystem::path( home ).operator/=( boost::filesystem::path( "dl" ) ) )
		,	env( boost::filesystem::path( home ).operator/=( boost::filesystem::path( "env" ) ) )
		{}
	boost::filesystem::path const & get_home() const { return ( this->home ); }
	boost::filesystem::path const & get_dpm() const { return ( this->dpm ); }
	boost::filesystem::path const & get_config_file() const { return ( this->config_file ); }
	boost::filesystem::path const & get_dl() const { return ( this->dl ); }
	boost::filesystem::path const & get_env() const { return ( this->env ); }

protected:
private:
	boost::filesystem::path const
		home;
	boost::filesystem::path const
		dpm;
	boost::filesystem::path const
		config_file;
	boost::filesystem::path const
		dl;
	boost::filesystem::path const
		env;
};

class env :
	public atom::node< LOKI_TYPELIST_5( env2logger, env2appl, env2env, env2envs, env2comps ) >,
	public boost::enable_shared_from_this< env > {

	typedef atom::node< LOKI_TYPELIST_5( env2logger, env2appl, env2env, env2envs, env2comps ) >
		base_node_t;

public:
	///
	static env_ptr create( logger_ptr l, appl_ptr a, env_ptr p, string_t const & n, boost::filesystem::path const & h ) {
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
	print( logger_ptr l, env_ptr ce, string_t const& offs, bool const v );
	///
	void
	print_c( logger_ptr l, string_t const& offs );
	///
	void
	find( string_t const& n, env_ptr& ce );
	///
	string_t
	get_caption() const {
		return ( this->name );
	}
	///
	env_paths const&
	get_paths() const { return ( this->paths ); }
	///
	void
	execute( string_t const& id, string_t const& cmd );

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
	env_paths
		paths;
	///
	env( logger_ptr l, appl_ptr a, string_t const & n, boost::filesystem::path const & h );
};