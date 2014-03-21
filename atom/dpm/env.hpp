#pragma once

class env_paths {
public:
	env_paths( boost::filesystem::path const& h ) :
			home( h )
		,	conf_file( boost::filesystem::path( home ).operator/=( boost::filesystem::path( "dpm.conf" ) ) )
		,	dpm( boost::filesystem::path( home ).operator/=( boost::filesystem::path( ".dpm" ) ) )
		{}
	boost::filesystem::path const & get_home() const { return ( this->home ); }
	boost::filesystem::path const & get_conf_file() const { return ( this->conf_file ); }
	boost::filesystem::path const & get_dpm() const { return ( this->dpm ); }
	boost::filesystem::path get_dpm_file( boost::filesystem::path const& f ) const { return ( boost::filesystem::path( this->dpm ).operator/= ( f ) ); }

protected:
private:
	boost::filesystem::path const
		home;
	boost::filesystem::path const
		conf_file;
	boost::filesystem::path const
		dpm;
};

class env :
	public atom::node< LOKI_TYPELIST_5( env2logger, env2appl, env2env, env2envs, env2comps ) >,
	public boost::enable_shared_from_this< env > {

	typedef atom::node< LOKI_TYPELIST_5( env2logger, env2appl, env2env, env2envs, env2comps ) >
		base_node_t;

public:
	///
	static env_ptr create( logger_ptr l, appl_ptr a, env_ptr p, string_t const & n, boost::filesystem::path const & h ) {
		env_ptr result;
		env_paths ps( h );
		//
		if ( boost::filesystem::exists( ps.get_conf_file() ) ) {
			result = env_ptr( new env( l, a, n, ps ) );
			if ( p ) {
				atom::mount<env2env>( result, p );
				atom::mount<env2envs>( p, result );
			}
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
	action( string_t const a, bool const r, bool const v );
	///
	void
	find( string_t const& n, env_ptr& ce );
	///
	comp_ptr
	find_comp( string_t const& id );
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
	execute( string_t const& sids, string_t const& scmds, const bool r );

protected:
	///
	void
	sync( bool const r, bool const v );
	///
	void
	info( string_t const& offs, bool const r, bool const v );
	///
	void
	status( bool const r, bool const v );
	//
	logger_ptr get_logger() {
		return ( get_slot<env2logger>().item() );
	}
	//
	appl_ptr get_appl() {
		return ( get_slot<env2appl>().item() );
	}
	//
	env_ptr get_parent() {
		return ( get_slot<env2env>().item() );
	}

private:
	string_t
		name;
	env_paths
		paths;
	boost::property_tree::ptree
		config;

	///
	env( logger_ptr l, appl_ptr a, string_t const & n, env_paths const & ps );
};