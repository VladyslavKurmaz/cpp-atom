#pragma once

class comp :
	public atom::node< LOKI_TYPELIST_3( comp2logger, comp2appl, comp2env ) >,
	public boost::enable_shared_from_this< comp > {

	typedef atom::node< LOKI_TYPELIST_3( comp2logger, comp2appl, comp2env ) >
		base_node_t;

public:
	///
	static comp_ptr create( logger_ptr l, appl_ptr a, env_ptr e, boost::property_tree::ptree const& pt ) {
		comp_ptr result = comp_ptr( new comp( l, a, pt ) );
		atom::mount<env2comps>( e, result );
		atom::mount<comp2env>( result, e );
		return result;
	}
	///
	~comp();
	///
	void
	clear();
	///
	string_t
	get_id() const {
		return ( this->props.get<string_t>("id") );
	}
	///
	void
	build_env_block( context_ptr cont );
	///
	//void
	//action( string_t const& a, unsigned int const l, bool const r );
	///
	void
	update();
	///
	void
	info( string_t const& offs );
	///
	void
	execute( context_ptr cont, string_t const& c );
	///
	static void
	parse_depends( context_ptr cont, string_t const& sids, env_ptr e, comp_deq_t& cs, const bool r );
	///
	static void
	parse_inherits( context_ptr cont, string_t const& sids, env_ptr e, comp_deq_t& cs, const bool r );

protected:
	//
	logger_ptr get_logger() {
		return ( get_slot<comp2logger>().item() );
	}
	env_ptr get_env() {
		return ( get_slot<comp2env>().item() );
	}
	///
	void
	build_env_vars( context_ptr cont, env_paths const& ep, string_t const& id, string_t const& prefix );
	///
	static void
	parse_hierarchy( context_ptr cont, string_t const& key, string_t const& sids, env_ptr e, comp_deq_t& cs, const bool r );

private:
	boost::property_tree::ptree
		props;
	///
	comp( logger_ptr l, appl_ptr a, boost::property_tree::ptree const& pt );
};


/*
var
 name		value
 name		gen_value
 gen_name	value
 gen_name	gen_value

 mod: override | extend

file
* archive
* executable
* file to download
*/
class prop {
public:
protected:
private:
};