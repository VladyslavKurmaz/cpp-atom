#pragma once


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
			result->init();
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
	action( context_ptr cont, string_t const& c, unsigned int const l, bool const r, bool const v );
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
	void
	get_prefix( ostream_t& os ){
		env_ptr p = this->get_parent();
		if ( p ) {
			p->get_prefix( os );

		} else {
			os << CONST_PREFIX_HEAD;
		}
		os << get_caption();
	}
	///
	env_paths const&
	get_paths() const { return ( this->paths ); }
	///
	void
	execute( context_ptr cont, string_t const& sids, string_t const& scmds, const bool r );

protected:
	///
	void
	init();
	///
	void
	sync( string_t const& a, unsigned int const l );
	///
	void
	info( string_t const& a, unsigned int const l );
	///
	void
	status( string_t const& a, unsigned int const l );
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
	//
	typedef std::map< string_t, boost::function< void( string_t const&, unsigned int const ) > >
		actions_t;
	actions_t
		actions;
	///
	env( logger_ptr l, appl_ptr a, string_t const & n, env_paths const & ps );
};